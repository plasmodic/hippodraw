/** @file

hippodraw::CanvasWindow class implementation

Copyright (C) 2001-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

Original from QTDIR/examples/canvas/canvas.cpp version 3.0

$Id: CanvasWindow.cxx,v 1.563 2007/09/14 22:41:07 pfkeb Exp $

*/

#warning TODO: Replace all occurances of std::string with QString

#ifdef HAVE_CONFIG_H
// for have root and cfitsio
#include "config.h"
#endif

#include "CanvasWindow.h"

#include "CanvasView.h"
#include "Inspector.h"
#include "QtFileDialog.h"
#include "PlotTable.h"
#include "PlotTableEvent.h"
#include "PlotterEvent.h"
#include "QtGroupView.h"
#include "SaveAsImageEvent.h"
#include "WindowController.h"

#include "datasrcs/NTupleController.h"
#include "datasrcs/DataSourceController.h"
#include "plotters/PlotterBase.h"
#include "qtxml/QtXMLController.h"

#include <QApplication>
#include <QCloseEvent>
#include <QEvent>
#include <QFileDialog>
#include <QHideEvent>
#include <QResizeEvent>
#include <QShowEvent>

#include <QMessageBox>
#include <QFontDialog>

#include <cassert>
#include <algorithm>

using std::exception;
using std::list;
using std::string;
using std::vector;

using namespace hippodraw;

PlotTable * CanvasWindow::s_plot_table = 0;

/** @todo This is the only place where know we are using Qt XML
    support. It would be abstracted to a factory.  Should also be in
    the CanvasView and not this class so it could be used by other
    windows.
*/
CanvasWindow::CanvasWindow ( QWidget * parent )
  : QMainWindow(), 
    m_file_dialog ( 0 ),
    m_prefix ( tr("HippoDraw - Canvas ") ),
    m_filename ( tr("UNTITLED") ),
    m_changed ( tr(" - Not Saved ") ),
    m_hasChanged ( false ),
    m_inhibit_close ( false ),
    m_allow_close ( false ),
    m_filenameExists ( false )
{
  setupUi(this);

  QGraphicsScene * canvas = new QGraphicsScene ();

  m_xml_controller = QtXMLController::instance ();
  m_canvas_view = new CanvasView ( canvas, this );

  setCentralWidget ( m_canvas_view );

  resize ( sizeHint () ) ;

  setCaption ();
  m_file_dialog = new QtFileDialog ();

  m_canvas_view -> initFitterSettings ( m_set_fitter );  // adds the actions.
  m_canvas_view -> initRecentFiles ( m_recent_files ); // add the actions.
  m_canvas_view -> initDockWindows ( this );  // set the dock windows.

  // fix bug in uic3, it didn't add these menu items.
  QMenu * fitter_menu = menuSettings -> addMenu ( tr("Fitter") );
  QList < QAction * > actions = m_set_fitter -> actions ();
  QList < QAction * >::iterator first = actions.begin();
  while ( first != actions.end() ) {
    QAction * action = *first++;
    fitter_menu -> addAction ( action );
  }

  WindowController * controller = WindowController::instance ();
  controller->newWindow ( this );
}


/** @todo Probably should remove this method or make it private.
 */
CanvasWindow::CanvasWindow ( const CanvasWindow & )
  : CanvasWindowBase ( ) // to keep a waring away
{
  assert ( false );
}

CanvasWindow::~CanvasWindow()
{
  QGraphicsScene * canvas = m_canvas_view->scene();

  delete canvas;
}

void
CanvasWindow::
customEvent ( QEvent * event )
{
  SaveAsImageEvent * image_event
    = dynamic_cast < SaveAsImageEvent * > ( event );

  if ( image_event != 0 ) {
    const PlotterBase * plotter = image_event -> plotter ();
    const string & filename = image_event -> filename ();
    m_canvas_view -> savePlotAsImage ( plotter, filename );
    return;
  }

  PlotterEvent * pe = dynamic_cast < PlotterEvent * > ( event );
  if ( pe != 0 ) {
      PlotterBase * plotter = pe -> plotter ();
      if ( plotter == 0 ) {
	setChanged ( true );
	setCaption ();
      }
      else {
          addDisplay ( plotter );
      }
  }

  PlotTableEvent * ptevent = dynamic_cast < PlotTableEvent * > ( event );
  if ( ptevent != 0 ) {
    int type = ptevent -> type();

    if ( type == PlotTableEvent::Copy ) {
      m_canvas_view -> addFromPasteboard ();
      return;
    }

    if ( type == PlotTableEvent::Close ) {
      if ( m_browsed_canvas != 0 ) {
	delete m_browsed_canvas;
      }
      s_plot_table -> setBrowserMode ( false );
      return;
    }
  }
}

void CanvasWindow::windowActivationChange ( bool oldActive )
{
  QWidget::windowActivationChange ( oldActive );

  if ( isActiveWindow () == true ) {
    WindowController * wc = WindowController::instance ();
    if ( wc -> currentCanvas ( ) != this ) {
      wc -> setCurrentCanvas ( this );
      updateActions ();
      m_canvas_view -> notifyObservers ();
    }
  }
}

void
CanvasWindow::
setAllowClose ( bool yes )
{
  m_allow_close = yes;
}

/** @todo bring the window up front before showing message box.  Just
    doing a setActiveWindow() does work.
*/
bool CanvasWindow::allowClose ( )
{
  if ( m_allow_close ||
       m_hasChanged == false ) return true;

  bool _allowClose = true;
  const string & app_name = m_canvas_view -> applicationName ();

  QString message = tr( "The document,\n"
			"%1\n"
			"contains unsaved changes.\n\n"
			"Do you want to save the document before closing it?"
			).arg(m_filename);

  int retval
    = QMessageBox::information ( this,
				 app_name.c_str(),
				 message,
				 tr("&Save"), tr("&Discard"), tr("Cancel"));

  switch (retval)
    {
    case 0: // save
      fileSave ();
      _allowClose = true;
      break;

    case 1:
      _allowClose = true;
      break;

    case 2:
      _allowClose = false;
      break;

    default:
      _allowClose = false;
      break;
    }

  return _allowClose;
}

bool
CanvasWindow::closeNoPrompt ( )
{
  m_allow_close = true;

  return QWidget::close();
}

void CanvasWindow::closeEvent ( QCloseEvent * e )
{
  if ( m_allow_close ) {
    e -> accept ();
    WindowController::instance() -> aboutToClose ( this );
    return;
  }

  if ( allowClose () == false ) {
    e->ignore ();
    return;
  }

  if ( m_inhibit_close == true ) {
    const string & app_name = m_canvas_view -> applicationName ();

    QString message = tr( "Closing the only document window will also\n"
			  "terminate the application.\n\n"
			  "Do you want to quit?" );
    int retval = QMessageBox::information ( this,
					    app_name.c_str(),
					    message,
					    QMessageBox::Yes,
					    QMessageBox::No |
					    QMessageBox::Default |
					    QMessageBox::Escape,
					    Qt::NoButton );

    switch (retval)
      {
      case QMessageBox::Yes: // save
	break;

      case QMessageBox::No:
	e->ignore();
	return;
	break;

      default:
	e->ignore();
	return;
	break;
      }
  }

  e->accept ();
  WindowController::instance() -> aboutToClose ( this );
}

void CanvasWindow::hideEvent ( QHideEvent * )
{
  WindowController::instance () -> hasBeenHidden ();
}

void CanvasWindow::showEvent ( QShowEvent * )
{
  WindowController::instance () -> unHide ( this );
}

void CanvasWindow::inhibitClose ( bool flag )
{
  m_inhibit_close = flag;
}

void CanvasWindow::setChanged ( bool flag )
{
  WindowController * controller = WindowController::instance ();
  controller -> updateActions ();
 if ( m_hasChanged == flag ) return;

  if ( flag == true ) {
    m_changed = tr("   not saved");
  }
  else {
    m_changed = tr("   saved");
  }
  m_hasChanged = flag;
  setCaption ();
}

void CanvasWindow::updateActions ()
{
  XmlController * controller = QtXMLController::instance ();
  bool no = controller -> isPasteboardEmpty ();
  action_Paste -> setEnabled ( ! no );

  const std::vector < const ViewBase * > & views
    = m_canvas_view->selectedViews ();
  bool yes = ! views.empty ();

  actionSave_selected_as_image->setEnabled ( yes );
  actionCu_t->setEnabled ( yes );
  actionC_opy->setEnabled ( yes );
  actionDelete -> setEnabled ( yes );
  actionUndo->setEnabled ( yes );
  actionLock->setEnabled ( yes );
  actionUnlock->setEnabled ( yes );
  actionGroup_views->setEnabled(yes);

  /* Default for ungroup is false. */
  actionUngroup_views->setEnabled(false);
  if ( yes == false ) return;

  /* By default, group is enabled, ungroup is disabled. Disable group if only one view 
     is selected. Enable group if the only one view is a group view. */
  if (views.size() == 1) {
    actionGroup_views->setEnabled(false);
    const QtGroupView * gv = dynamic_cast <const QtGroupView *> (*views.begin());
    if (gv) {
      actionUngroup_views->setEnabled(true);
    }
  }


  bool one_locked = false;
  bool one_unlocked = false;
  std::vector < const ViewBase * >:: const_iterator first = views.begin ();
  while ( first != views.end () ) {
    const QtView * view = dynamic_cast < const QtView * > ( *first++ );
    /* Disable group if any of the selected view is a groupview. */
    if (!view) {
      actionGroup_views->setEnabled(false);
      return;
    }
      
    bool locked = !(view->flags() & QGraphicsItem::ItemIsMovable);
    one_locked |= locked;
    one_unlocked |= ! locked;
  }
  actionLock->setEnabled ( one_unlocked );
  actionUnlock->setEnabled ( one_locked );
}

/** Prints the canvas to a PostScript file.
    @todo Add the `.ps' subscript to the file name if missing.
*/
void
CanvasWindow::
filePrint()
{
  m_canvas_view->print ();
}

void CanvasWindow::editCopy ()
{
  m_canvas_view->copySelectedToPasteboard ();

  WindowController * controller = WindowController::instance ();
  controller -> updateActions();
}

void
CanvasWindow::
editClear ()
{
  QString message = tr( "The clear operation can not be undone.\n\n"
			"Are you sure you want to remove all canvas items?" );
  int retval
    = QMessageBox::warning ( this,
			     tr("Warning"),
			     message,
			     QMessageBox::Yes,
			     QMessageBox::No );
  if ( retval != QMessageBox::Yes ) return;

  clear ();

  WindowController * controller = WindowController::instance ();
  controller -> updateActions();
}

void CanvasWindow::editCut ()
{
  m_canvas_view->deleteSelected ( true );  // copy to pasteboard and delete

  WindowController * controller = WindowController::instance ();
  controller -> updateActions();
}

void CanvasWindow::editDelete ()
{
  m_canvas_view->deleteSelected ( false );  // just delete

  WindowController * controller = WindowController::instance ();
  controller -> updateActions();
}

void
CanvasWindow::
editReTile ()
{
  m_canvas_view->reTile ();
}

void
CanvasWindow::
editReTilePage ()
{
  m_canvas_view->reTilePage ();
}

void
CanvasWindow::
editUndo ()
{
  m_canvas_view->restoreFromSelectCopy ();

  m_canvas_view->notifyObservers();
}

void CanvasWindow::editPaste ()
{
  m_canvas_view->copyFromPasteboard ();
  WindowController * controller = WindowController::instance ();
  controller -> updateActions();
}

void
CanvasWindow::
editSelectAll ()
{
  m_canvas_view->setAllSelected ( true );

  WindowController * controller = WindowController::instance ();
  controller -> updateActions();
}

void
CanvasWindow::
fileNew ()
{
  WindowController * controller = WindowController::instance ();

  // Will fix the size and position of the new Canvas
  controller->createInspector();
  
  CanvasWindow * window = new CanvasWindow ();
  //controller->newWindow ( window );
  window->setCaption ();
  window->show ();
}

void
CanvasWindow::
fileBrowse ()
{
#warning TODO: Rewrite without std::string
  const QString & filter = QtFileDialog::createBrowseFilter ();

  QString filename 
    = QFileDialog::getOpenFileName ( this,  // parent
				     QString::null,   // default caption
				     QString::null, // starting dir
				     filter);
  if ( filename != QString::null ) {
    QString fn = filename.trimmed ();

    int pos = fn.lastIndexOf ( '.' );
    QString suffix = fn.mid ( pos );

    if ( QtFileDialog::isDocSuffix ( suffix ) ) {
      QGraphicsScene * canvas = new QGraphicsScene ();
      CanvasView * m_browsed_canvas = new CanvasView ( canvas, this );
      m_browsed_canvas -> initFromFile ( filename );
      if ( s_plot_table == 0 ) {
	s_plot_table = new PlotTable ();
      }
      s_plot_table -> setBrowserMode ( true, this );
      s_plot_table -> setCanvas ( m_browsed_canvas );
      s_plot_table -> show ();
    }
  }
}

void
CanvasWindow::
fileOpen ()
{
#warning TODO: Rewrite using QString
  const QString &filter = QtFileDialog::createOpenFilter ();

  QString filename 
    = QFileDialog::getOpenFileName ( this,
				     QString::null,
				     QString::null, // starting dir
				     filter);
  if ( filename != QString::null ) {
    QString fn = filename.trimmed ();

    QString suffix;
    int pos = fn.lastIndexOf ( '.' );
    if ( pos >= 0 ) {
      suffix = fn.mid ( pos );
    }

    if ( QtFileDialog::isDocSuffix ( suffix ) ) {

      WindowController * controller = WindowController::instance ();
      
      // Will fix the size and position of the new Canvas.
      controller->createInspector();
  
      CanvasWindow * window = new CanvasWindow ();
      try {
	window -> initFromFile ( fn );
      }
      catch ( exception & e ) {
	QString message = tr("Attempt to read file\n"
			     "`%1' led to error:\n"
			     "%2").arg(filename).arg(e.what());
	QMessageBox::critical ( this, // parent
				tr("File error"),
				message,
				QMessageBox::Ok,
				0 );
	window -> setChanged ( false );
	window -> close ();
      }
      return;
    }

    else if ( QtFileDialog::isTextSuffix ( suffix ) ) {
      m_file_dialog -> openTextTuple ( fn );
      m_canvas_view -> notifyObservers ();
      m_canvas_view -> addRecentFile (fn, m_recent_files);
      return;
    }
#ifdef HAVE_ROOT
    else if ( QtFileDialog::isRootSuffix ( suffix ) ) {
      m_file_dialog -> openRootTuple ( fn, this );
      m_canvas_view -> notifyObservers ();
      m_canvas_view -> addRecentFile (fn, m_recent_files);
      return;
    }
#endif

#ifdef HAVE_CFITSIO
    else if ( QtFileDialog::isFitsSuffix ( suffix ) ) {
      m_file_dialog -> openFitsTuple ( fn, this );
      m_canvas_view -> notifyObservers ();
      m_canvas_view -> addRecentFile (fn, m_recent_files);
      return;
    }
    else {
      bool yes = QtFileDialog::isFitsFile ( fn );
      if ( yes ) {
	m_file_dialog -> openFitsTuple ( fn, this );
	m_canvas_view -> notifyObservers ();
	m_canvas_view -> addRecentFile (fn, m_recent_files);
	return;
      }
    }
#endif
    // if all else fails, try text file
    m_file_dialog -> openTextTuple ( fn );

    m_canvas_view -> notifyObservers ();
    m_canvas_view -> addRecentFile ( fn, m_recent_files );
  }
}

  /** @bug @@@@@@ After initializing from file, the Window appears and
      responds like it has been changed.
   */
void
CanvasWindow::
initFromFile ( const QString & filename )
{
  m_canvas_view -> initFromFile ( filename );
  setTitleFileName ( filename );
  setChanged ( false );

  show();
}

void
CanvasWindow::
clear()
{
  m_canvas_view->clear();
}

void CanvasWindow::resizeEvent ( QResizeEvent * e )
{
  m_canvas_view->resizeEvent ( e );

  QWidget::resizeEvent ( e );
}

void CanvasWindow::viewAddPage()
{
  m_canvas_view->addPage ();
}

bool CanvasWindow::areDataSourcesSaved ()
{
  const vector < const ViewBase * > & views = m_canvas_view->views ();
  bool yes = m_xml_controller->areDataSourcesSaved ( views );

  if ( yes == false ) {
    const string & app_name = m_canvas_view -> applicationName ();

    QString message = tr( "Document can not be saved.\n\n"
			  "It uses data sources that were neither\n"
			  "read from nor saved to a file.\n\n"
			  "Use File menu: \"%1\"\n"
			  "to save the data sources first, or\n"
			  "use the File menu: \"%2\".")
      .arg(actionSave_selected_as_image->text())
      .arg(actionSave_All->text());

    QMessageBox::critical ( this, // parent
			    app_name.c_str(), // caption
			    message,
			    QMessageBox::Ok,
			    Qt::NoButton );
  }

  return yes;
}

void CanvasWindow::fileSave()
{
  bool yes = areDataSourcesSaved ();
  if ( ! yes ) return;

  if ( m_filenameExists )
    {
      if ( m_hasChanged )
	{
	  QString message = tr( "Document file exists\n\n"
				"Over-write existing file?" );
	  int retval
	    = QMessageBox::warning ( this,
				     tr("Warning"),
				     message,
				     QMessageBox::Yes,
				     QMessageBox::No );
	  if ( retval != QMessageBox::Yes ) return;
	  saveAs ( m_filename );
	}
    }
  else
    {
      fileSaveAs ();
    }
}

std::string
CanvasWindow::
getSaveDocFilename ()
{
  const QString & filter = QtFileDialog:: createDocumentFilter ();

  QString filename;
  filename = QFileDialog::getSaveFileName ( this,
				       tr("Save canvas to ..."),
				       QString::null, // starting dir
				       filter);
      
  if ( filename == QString::null ) return string();

  const QString & suffix = QtFileDialog::getDocSuffix();
  if (!filename.endsWith(suffix))
      filename += suffix;

  QFileInfo info ( filename );
  filename = info.absoluteFilePath();
  string name = filename.toLatin1().data();

  return name;
}

bool
CanvasWindow::
setFilenameFromDialog ()
{
  m_filename = getSaveDocFilename ().c_str();

  m_filenameExists = m_filename.isEmpty () ? false : true;

  return m_filenameExists;
}

void CanvasWindow::fileSaveAs()
{
  bool yes = areDataSourcesSaved ();
  if ( ! yes ) return;

bool ok = setFilenameFromDialog ();
  if ( ! ok ) return;

  saveAs ( m_filename );
}


void
CanvasWindow::
fileSaveAll()
{
  if ( m_filenameExists == false ) {
    fileSaveAllAs ();
    return;
  }
  saveAllAs ( m_filename );
}

void
CanvasWindow::
fileSaveAllAs()
{
  bool ok = setFilenameFromDialog ();
  if ( ! ok ) return;
  saveAllAs ( m_filename );
}

/** @request Raise error dialog if saving doesn't work.
 */
void
CanvasWindow::
saveAllAs ( const QString & filename )
{
  QString suffix = QtFileDialog:: getDocSuffix ();
  string::size_type pos = filename.indexOf ( suffix );
  QString prefix = filename.left ( pos );
  NTupleController * controller = NTupleController::instance ();

  QString data_suffix = QtFileDialog::getTextSuffix ();
  controller -> saveNTuples ( prefix.toLatin1().data(), data_suffix.toLatin1().data() );
  saveAs ( filename );
}

void CanvasWindow::fileSaveSelectedImages ()
{
  m_canvas_view->saveSelectedImages ();
}

void CanvasWindow::fileSaveSelectedImages ( const std::string & filename )
{
   m_canvas_view->saveSelectedImages(filename);
}

void
CanvasWindow::
saveAs ( const QString & filename )
{
  m_canvas_view -> saveAs ( filename );

  setTitleFileName ( filename );
  setChanged ( false );
  setCaption ();
}

CanvasView *
CanvasWindow::
getCanvasView ()
{
  return m_canvas_view;
}

void
CanvasWindow::
fileExportTextTuple ()
{

  Inspector * inspector = m_canvas_view -> getInspector ();
  const string tuple_name = inspector -> getSelectedDataSourceName ();

  QString fn = QtFileDialog:: getExportTupleFilename ( this );
  if ( fn.isEmpty() ) return;

  
  int pos = fn.lastIndexOf ( '.' );
  QString suffix = fn.mid ( pos );
  
  if ( QtFileDialog::isTextSuffix ( suffix ) ) {
    m_file_dialog -> saveTextTuple ( fn, this );
    m_canvas_view -> notifyObservers ();
    return;
  }
 
  else if ( QtFileDialog::isFitsSuffix ( suffix ) ||
	    QtFileDialog::isZippedFitsSuffix (suffix) ) {
#ifdef HAVE_CFITSIO
    m_file_dialog -> saveFitsTuple ( fn, this );
    m_canvas_view -> notifyObservers ();
#else
    m_canvas_view -> showFitsError ();
#endif
    return;
  }
  
  else {
    bool yes = QtFileDialog:: isFitsFile ( fn );
    if ( yes ) {
#ifdef HAVE_CFITSIO
      m_file_dialog -> saveFitsTuple ( fn, this );
      m_canvas_view -> notifyObservers ();
#else
      m_canvas_view -> showFitsError ();
#endif
      return;
    }
  }
  
  // if all else fails, show error message.
  
  QString message = tr("The file type: %1\n"
		       "is not supported.\n\n"
		       "Please use Text ntuple (.tnt)\n"
		       "or FITS file (.fits).").arg ( suffix );  
  QMessageBox::critical ( this, // parent
			  tr("Unsupported file type"), // caption
			  message,
			  QMessageBox::Ok,
			  Qt::NoButton );
}

void
CanvasWindow::
fileCreateNTuple ()
{
  m_canvas_view -> createNTuple();
}

void
CanvasWindow::
fileExit ()
{
  bool ok = WindowController::instance () -> okToQuit();

  if ( ok ) {
    // save dock window and recent files
    autosaveSettings(); 
    qApp->quit ();
  }
}

void CanvasWindow::viewZoomIn()
{
  m_canvas_view->viewZoomIn ();
}

void CanvasWindow::viewZoomOut()
{
  m_canvas_view->viewZoomOut ();
}

void
CanvasWindow::
viewZoomReset()
{
  m_canvas_view -> viewZoomReset ();
}

void
CanvasWindow::
viewLock ()
{
  m_canvas_view->setLocked ( true );
  updateActions ();
}

void
CanvasWindow::
viewUnlock ()
{
  m_canvas_view->setLocked ( false );
  updateActions ();
}

/** @bug Mac only.  About menu when launch with .app does not have the
    same case as the application claims to be.
 */
void CanvasWindow::helpAbout()
{
  m_canvas_view -> helpAbout ();
}

void
CanvasWindow::
helpHelp ()
{
#ifndef HAVE_HELP
  const string & app_name = m_canvas_view -> applicationName ();
  QString message = tr( "The application was built without\n"
			"built-in help support" );
  QMessageBox::information ( this,
			     app_name.c_str(),
			     message,
			     QMessageBox::Ok );
  return;
#endif

  WindowController * controller = WindowController::instance ();
  controller -> openAssistant ();
}

void
CanvasWindow::
aboutQt ()
{
  QMessageBox::aboutQt ( this );
}

void CanvasWindow::setCaption ()
{
  QWidget::setWindowTitle ( m_prefix + m_filename + m_changed );
}

void
CanvasWindow::
setTitleFileName ( const QString & name )
{
  m_filename = name;
  m_filenameExists = true;
}

void
CanvasWindow::
addTextDisplay ( PlotterBase * plotter,
		 const std::string & s )
{
  m_canvas_view->addTextDisplay ( plotter, s );
}

void
CanvasWindow::
addTextDisplay ( PlotterBase * plotter,
		 const std::string & type,
		 const std::string & text)
{
  m_canvas_view->addTextDisplay ( plotter, type, text );
}

std::pair<double, double>
CanvasWindow::
addTextDisplayAt ( PlotterBase * plotter,
                   const std::string & type,
                   const std::string & text,
                   double xrel, double yrel )
{
   return m_canvas_view->addTextDisplayAt( plotter, type, text, xrel, yrel );
}

void
CanvasWindow::
addFuncDisplay ( PlotterBase * plotter, const std::string & s )
{
  m_canvas_view->addFuncDisplay ( plotter, s );
}

void CanvasWindow::addPlotDisplay ( PlotterBase * plotter, bool sel )
{
  m_canvas_view->addPlotDisplay ( plotter, sel );
}

void CanvasWindow::addDisplay ( PlotterBase * plotter )
{
  m_canvas_view->addPlotDisplay ( plotter );
}

/** @todo This method should only be in the CanvasView.
 */
void
CanvasWindow::
removeDisplay ( PlotterBase * plotter )
{
  m_canvas_view->removeDisplay ( plotter );
}

/** @todo This method should go away or return null pointer if multiple
    plots are selected. */
PlotterBase * CanvasWindow::selectedPlotter ()
{
  return m_canvas_view->selectedPlotter ();
}

const vector<const ViewBase *> CanvasWindow::views ()
{
  return m_canvas_view->views ();
}

void
CanvasWindow::
fillPlotterList ( std::vector < PlotterBase * > & plotters )
{
  m_canvas_view -> fillPlotterList ( plotters );
}

QtView *
CanvasWindow::
getViewFor ( const PlotterBase * plotter ) const
{
  return m_canvas_view->getViewFor ( plotter );
}

void CanvasWindow::setIntervalEnabled ( bool yes )
{
  m_canvas_view->setIntervalEnabled ( yes );
}

void
CanvasWindow::
viewShowInspector ()
{
  m_canvas_view -> showInspector ();
}

void
CanvasWindow::
viewShowPlotTable ()
{
  if ( s_plot_table == 0 ) {
    s_plot_table = new PlotTable ();
  }
  s_plot_table->setCanvas ( m_canvas_view );
  s_plot_table->show();
}

void
CanvasWindow::
viewShowPickTable ()
{
  m_canvas_view->viewShowPickTable();

  actionPick_points -> toggle (); // need to get in this mode
}

void
CanvasWindow::
savePlotAsImage ( const PlotterBase * plotter,
		  const std::string & filename )
{
  SaveAsImageEvent * event = new SaveAsImageEvent ( plotter, filename );
  QApplication::postEvent ( this, event );
}

void
CanvasWindow::
saveAsImage ( const PlotterBase * plotter,
	      const std::string & filename )
{
#warning FK: I assume those locks are no longer needed, but I might be wrong. \
         In case of problems I might need to dig deeper into the threading code here.
  //  qApp -> lock ();
  m_canvas_view -> savePlotAsImage ( plotter, filename );
  //  qApp -> unlock ();
}

void
CanvasWindow::
setAllSelected ( bool flag )
{
   m_canvas_view->setAllSelected( flag );
}

void
CanvasWindow::
setSelected ( QtView * view )
{
   m_canvas_view->setSelectedItem( view );
}

void 
CanvasWindow::
previousView ( )
{
   PlotterBase * currentPlotter = selectedPlotter();
   if (currentPlotter) {
      if ( ! ( currentPlotter->getCurrentRangeSaved () ) ) {
         m_canvas_view -> setCurrentRange();
      }
      
      selectedPlotter() -> nextView (false );
      m_canvas_view -> scene() -> update();
   }
}

void 
CanvasWindow::
nextView ( )
{
   PlotterBase * currentPlotter = selectedPlotter();
   if (currentPlotter) {
      if ( !(currentPlotter->getCurrentRangeSaved()) ) {
         m_canvas_view->setCurrentRange();
      }

      selectedPlotter() -> nextView ( true );
      m_canvas_view->scene()->update();
   }
}


void
CanvasWindow::
setZoomMode ( bool on )
{
  m_canvas_view -> setZoomMode ( on );
}

void
CanvasWindow::
settingPrinter()
{
  m_canvas_view -> setPrinterSettings ();
}

void
CanvasWindow::
settingCanvas()
{
  m_canvas_view -> setup ();
}

void
CanvasWindow::
settingFonts()
{
  bool ok;

  // From the Qt documentation - "The usual way to use QFontDialog class is
  // to call one of the static convenience functions"
  QFont font
    = QFontDialog::getFont ( &ok, QFont( "Helvetica [Cronyx]", 10), this);

  m_canvas_view -> setFonts( font );
}

void
CanvasWindow::
print ( const std::string & filename )
{
  m_canvas_view -> print ( filename );
}

void
CanvasWindow::
setPlotMatrix ( unsigned int columns, unsigned int rows )
{
  m_canvas_view -> setPlotMatrix ( columns, rows );
  m_canvas_view -> reTile ();
}

const QString &
CanvasWindow::
getAppKey ( ) const
{
  return m_canvas_view -> getAppKey ();
}

void
CanvasWindow::
setFitter ( QAction * action )
{
  QString name = action -> text ();
  m_canvas_view -> setFitterDefault ( name );
}

void
CanvasWindow::
setAddedSelected ( bool yes )
{
  m_canvas_view -> setAddedSelected ( yes );
}

void 
CanvasWindow::
setCutMode ( QAction * action )
{
  if ( action == actionDrag_to_set_cut )
    m_canvas_view -> setCutMode (1);
  else if ( action == actionClick_to_set_cut )
    m_canvas_view -> setCutMode (2);
  else if ( action == actionDrag_cut_positions )
    m_canvas_view -> setCutMode (3);
  else
    m_canvas_view -> setCutMode (0);  // pick mode
}

NTuple *
CanvasWindow::
getPickTable () const
{
  return m_canvas_view -> getPickTable();
}

NTuple *
CanvasWindow::
getPickTable ( const PlotterBase * plotter ) const
{
  return m_canvas_view -> getPickTable ( plotter );
}

void
CanvasWindow::
groupView ()
{
  m_canvas_view -> groupView ();
  updateActions();
}

void
CanvasWindow::
ungroupView ()
{
  m_canvas_view -> ungroupView ();
  updateActions();
}

void
CanvasWindow::
openRecentFile(QAction * action )
{
  QString filename = action -> text ();
  int pos = filename.lastIndexOf ( '.' );
  QString suffix = filename.mid ( pos );


  if ( QtFileDialog::isTextSuffix ( suffix ) ) {
    QtFileDialog::openTextTuple ( filename );
    action -> setCheckable ( false );
    action -> setCheckable ( true );
    m_canvas_view -> notifyObservers ();
    return;
  }

  if ( QtFileDialog::isFitsSuffix ( suffix ) ) {
    m_file_dialog ->openFitsTuple ( filename, this );
    action -> setCheckable ( false );
    action -> setCheckable ( true );
    m_canvas_view -> notifyObservers ();
    return;
  }

  if ( QtFileDialog::isRootSuffix ( suffix ) ) {
    m_file_dialog ->openRootTuple ( filename, this );
    action -> setCheckable ( false );
    action -> setCheckable ( true );
    m_canvas_view -> notifyObservers ();
    return;
  }

  m_file_dialog -> openTextTuple ( filename );

  // Should use normal menu item.
  action -> setCheckable ( false );
  action -> setCheckable ( true );
  m_canvas_view -> notifyObservers ();
}

void
CanvasWindow::autosaveSettings()
{
  m_canvas_view -> autosaveSettings ( this );
}
