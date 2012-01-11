 /** @file

hippodraw::CanvasView class implementation

Original taken from $QTDIR/examples/canvas/canvas.cpp version 3.0

Copyright (C)  2001-2006  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: CanvasView.cxx,v 1.371 2007/07/02 18:17:16 pfkeb Exp $

*/
// for MEMFUN1 and others
#ifdef HAVE_CONFIG_H
// for wcslib also
#include "config.h"
#endif

#include <QMouseEvent>

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "CanvasView.h"

#include "CanvasSelectionEvent.h"
#include "CanvasSettings.h"
#include "CanvasWindow.h"
#include "Inspector.h"
#include "PlotterEvent.h"
#include "QtViewFactory.h"
#include "PickTable.h"
#include "WindowController.h"
#include "CreateNTuple.h"
#include "QtGroupView.h"

#include "controllers/DisplayController.h"
#include "controllers/FunctionController.h"
#include "datareps/ScatterPlot.h"
#include "datasrcs/DataSourceController.h"
#include "datasrcs/NTupleController.h"
#include "graphics/EpsView.h"
#include "plotters/PlotterBase.h"
#include "plotters/Cut1DPlotter.h"
#include "plotters/Cut2DPlotter.h"
#include "qtxml/QtXMLController.h"
#include "transforms/BinaryTransform.h"

#ifdef HAVE_CFITSIO
#include "fits/FitsController.h"
#endif

#ifdef HAVE_ROOT
#include "root/RootController.h"
#endif

#ifdef HAVE_WCSLIB
#include "wcslib/wcsconfig.h"
#endif

#include <QApplication>
#include <QAction>
#include <QFileDialog>
#include <QImageWriter>
#include <QCursor>
//#include <qiconset.h>
#include <QMessageBox>
#include <QPainter>
#include <QSettings>
#include <QStatusBar>
#include <QWaitCondition>
#include <QFont>
#include <QTextStream>

#include <algorithm>
#include <exception>

#include <cctype>
#include <cmath>

#include <cassert>

using std::bind2nd;
using std::for_each;
using std::map;
using std::mem_fun;
using std::list;
using std::string;
using std::tolower;
using std::vector;

namespace {
   QWaitCondition mouseEvent;
}

using namespace hippodraw;

/* Use `=' syntax so that Doxygen picks it up. */
const string CanvasView::s_app_name = tr("HippoDraw").toAscii().data();
string CanvasView::s_image_filters;

XmlController * CanvasView::s_xml_controller = 0;

CanvasView::CanvasView ( QGraphicsScene * c, QWidget * parent,
                         const char * name, Qt::WFlags f )
  :  FigureEditor ( c, parent, name, f ),
     m_inspector ( 0 ),
     m_image_dialog ( 0 ),
     m_num_width ( 2 ),
     m_num_height ( 3 ),
     m_cut_mode ( 1 )
{
  s_xml_controller = QtXMLController::instance ();
  m_table_map.clear();
  m_drag_rect = 0;

  initSettings ();

  connect (scene(), SIGNAL(selectionChanged()), this, SLOT(notifyObservers()));
}

void
CanvasView::
initSettings ()
{
  QSettings settings;

  m_num_width
    = settings.value ( s_app_key + "Canvas/Number/Width", 2 ) . toInt();
  m_num_height
    = settings.value ( s_app_key + "Canvas/Number/Height", 3 ) . toInt();
  m_add_selected
    = settings.value ( s_app_key + "Canvas/AddSelected" ) . toInt();
  m_ignore_margin
    = settings.value ( s_app_key + "Canvas/IgnoreMargin" ) . toInt();
}

void
CanvasView::
initDockWindows ( QMainWindow * mw )
{
  QSettings settings;
  QByteArray dockSetting = settings.value(s_app_key +
				"DockWindow/Position").toByteArray();
  mw->restoreState(dockSetting);
}

void
CanvasView::
initRecentFiles ( QActionGroup * ag )
{
  QObject * parent = ag -> parent ();
  QSettings settings;
  bool ok = false;

  // Read every entries from the settings.
  for (unsigned int i = 0; i<5; i++){
    QString number=QString::number(i);
    ok = settings.contains ( s_app_key + "Files/File"+number );
    QString entry = settings.value ( s_app_key + "Files/File"+number,
				     QVariant ( QString::null ) ) . toString();
    
    // No entry or empty entry.
    if ((ok==false) || ( entry == "" )) return;
    
    
    QAction * action = new QAction ( entry, parent );
    ag -> addAction ( action );
    m_recent_list.push_back( action );
    action -> setCheckable ( true );
  }
}


void
CanvasView::
initFitterDefault ()
{
  QSettings settings;
  bool ok = settings.contains ( s_app_key + "Fitter/Name" );
  QString entry = settings.value ( s_app_key + "Fitter/Name",
				   QVariant ( QString::null ) ) . toString();
  if ( ok == true ) {
    const string name ( entry.toLatin1().data() );
    try { // might not be available
      setFitterDefault ( name );
      if ( m_inspector != 0 ) {
        m_inspector -> setSelectedFitter ( name );
      }
    }
    catch ( ... ) { // not available in this build
      // could raise error dialog, but not nice on initialization
    }
  }
}

void
CanvasView::
setFitterDefault ( const QString & name )
{
  QSettings settings;
  settings.setValue ( s_app_key + "Fitter/Name", name );

  const string fitter ( name.toLatin1().data() );
  setFitterDefault ( fitter );
}
void
CanvasView::
setFitterDefault ( const std::string & name )
{
  FunctionController * controller = FunctionController::instance ();
  controller -> setDefaultFitter ( name );
}

void
CanvasView::
initFitterSettings ( QActionGroup * ag )
{
  initFitterDefault ();
  initFitterMenu ( ag );
}

void
CanvasView::
initFitterMenu ( QActionGroup * ag )
{
  FunctionController * controller = FunctionController::instance();
  const vector < string > & fitters = controller -> getFitterNames ();
  const string & def_fitter = controller -> getDefaultFitter ();
  QObject * parent = ag -> parent ();
  for ( unsigned int i = 0; i < fitters.size(); i++ ) {
    const QString name ( fitters[i].c_str() );
    QAction * action = new QAction ( name, parent );
    ag -> addAction ( action );
    action -> setCheckable ( true );
    if ( def_fitter == fitters[i] ) {
      action -> setChecked ( true );
    }
  }
}


void
CanvasView::
saveSettings () const
{
  QSettings settings;

  int orientation;
  if( s_printer -> orientation() == QPrinter::Portrait )
    orientation = 0;
  else
    orientation = 1;

  settings.setValue ( s_app_key + "Printer/Orientation", orientation );
  settings.setValue ( s_app_key + "Canvas/Number/Width",  m_num_width );
  settings.setValue ( s_app_key + "Canvas/Number/Height", m_num_height );
  settings.setValue ( s_app_key + "Canvas/AddSelected", m_add_selected );
  settings.setValue ( s_app_key + "Canvas/IgnoreMargin", m_ignore_margin );

  if( m_printer_bounds == false )
    settings.setValue ( s_app_key + "Canvas/Margin", 0 );
  else
    settings.setValue ( s_app_key + "Canvas/Margin", 1 );
}

void
CanvasView::
setAddedSelected ( bool yes )
{
  m_add_selected = yes;
}

void CanvasView::initFrom ( const std::list < ViewBase * > & views )
{
  std::list < ViewBase * > ::const_iterator first = views.begin();
  for ( ; first != views.end(); ++first ) {
    QtView * view = dynamic_cast < QtView * > ( *first );

    // Find parent view
    if (view->getPlotter()->getParentPlotter() != NULL) {
	std::list < ViewBase * > ::const_iterator subit = views.begin();
	PlotterBase *pplot = view->getPlotter()->getParentPlotter();
	while (subit != views.end()) {
		if ((*subit)->getPlotter() == pplot) {
			Rect saverect = view->getDrawRect();
			view->setParentItem(dynamic_cast<QtView *>(*subit));
			view->setDrawRect(saverect);
			break;
		}
		subit++;
	}
    }

    QRectF rect = view->rect();
    if ( rect.height () < 0 || rect.width () < 0 )
	sizeAndPlace ( view );

    add ( view ); // ignore the rest
  }

 scene()->setSceneRect(QRectF());
 setSceneRect(scene()->sceneRect());
 QGraphicsView::centerOn ( QPointF(0, 0) );
}

void
CanvasView::
initFromFile ( const QString & filename )
{
  XmlController::Status status = s_xml_controller->openFile ( filename.toLatin1().data() );

  // If file not found, change the directory and try again.
  if ( status == XmlController::NTupleError ) {

    std::list < std::string > & missing_tuples = 
      s_xml_controller -> getMissingTuples ();
    std::list< std::string>::iterator it;

    for ( it=missing_tuples.begin(); it!=missing_tuples.end(); ++it ) {
	string shortFileName;
	string::size_type pos1 = (*it).find_last_of ("/");
	string::size_type pos2 = (*it).find_last_of (":");
	shortFileName = (*it).substr(pos1+1); // get the short filename
	
	QString cap = tr( "Select the DataSource:  " );
	cap += shortFileName.c_str(); // Caption of the open file dialog
	QString filter = tr( "All files(*.*);;Text NTuple(*.tnt);;FITS file(*.fits);;Compressed FITS file(*.fits.gz);;ROOT file(*.root)");
	
       	QString filename = 
	  QFileDialog::getOpenFileName ( this, cap, 
					 QString::null, // starting directory
					 filter // filter
					 );

	string newFileName = filename.toLatin1().data();

	if ( ( pos2!= string::npos)  && (pos2 != 1) )
	  ( *it ).replace (0, pos2, newFileName); // keep the name after ":"
						  // unchanged
	else 
	  ( *it )=newFileName; // with no name
    }

    status = s_xml_controller->openUpdatedNTuples (  );    
  }


  // Second try failed, show failed message.
  if ( status != XmlController::Success ) {
    const string & app_name = applicationName ();

    QString fn = filename;
    QString message;
    message = tr("Unable to open document file:\n%1\n").arg ( fn );
    if ( status == XmlController::OpenError ) {
      message += tr("\nFile could not be read\n");
    } else
      if ( status == XmlController::ParseError ) {
        message += tr("\nThere was a XML parsing error\n");
      } else {
        message += tr("\nThere were one or more referenced\n"
		      "NTuple files that could not be found.");
      }
    QMessageBox::critical ( this, // parent
                            app_name.c_str(), // caption
                            message,
                            QMessageBox::Ok,
                            Qt::NoButton,
                            Qt::NoButton );
    show ();
    return;
  }

  ViewFactory * factory = QtViewFactory::instance ();
  const list < ViewBase * > & view_list
    = s_xml_controller->getViews ( factory );

  initFrom ( view_list );

  notifyObservers ();
}

void
CanvasView::
invalidImageFormatError ( const std::string & filename )
{
  if ( s_image_filters.empty () ) createImageFilters ();

  string types;
  string::size_type size = s_image_filters.size ();
  for ( string::size_type i = 0; i < size; i++ ) {
    string::value_type c = s_image_filters[i];
    if ( c != '(' &&
         c != ')' &&
         c != ';' ) types += c;
  }

  QString message = tr( "Attempt to save image to file:\n"
			"%1\n"
			"whose suffix indicates unsupported image format.\n\n"
			"Supported image formats are:\n"
			"%2"
			).arg(filename.c_str()).arg(types.c_str());

  QMessageBox::critical ( this, // parent
                          tr("Image format error"), // caption
                          message );

}

/** @bug If plot being copied is target of a cut, then cut should also
    be copied.
*/
void
CanvasView::
copySelectedToPasteboard ()
{
  const vector < const ViewBase * > & selViews = selectedViews();
  if ( selViews.empty () ) return;

  vector < const ViewBase * > view_list;
  fillSelectedWithObservers ( view_list );
  s_xml_controller->saveToPasteboard ( view_list );

  copySelectedToClipboard ();
}

void
CanvasView::
copyFromPasteboard ()
{
  const vector < const ViewBase * > & targets = views ();
  ViewFactory * factory = QtViewFactory::instance ();

  const list < ViewBase * > & pb_views
    = s_xml_controller->getFromPasteboard ( factory, targets );
  pasteViews ( pb_views );
}

void
CanvasView::
addFromPasteboard ()
{
  const vector < const ViewBase * > & targets = views ();
  ViewFactory * factory = QtViewFactory::instance ();

  const list < ViewBase * > & pb_views
    = s_xml_controller->getFromPasteboard ( factory, targets );
  list < ViewBase * >::const_iterator first = pb_views.begin ();
  while ( first != pb_views.end () ) {
    ViewBase * view = *first++;
    QtView * qtview = dynamic_cast < QtView * > ( view );
    placeGraph ( qtview );
    addView ( qtview, true );
  }
}

void
CanvasView::
deleteSelected ( bool pasteboard )
{
  const vector < const ViewBase * > & selViews = selectedViews();
  if ( selViews.empty () ) return;

  vector < const ViewBase * > view_list;
  fillSelectedWithObservers ( view_list );
  if ( pasteboard ) {
    s_xml_controller->saveToPasteboard ( view_list );
  }

  DisplayController * controller = DisplayController::instance ();
  controller->removeTextObservers ( view_list );

  // Remove views from the canvas.

#ifdef ITERATOR_MEMBER_DEFECT
  std::
#endif
  vector < const ViewBase * > ::const_iterator iter = view_list.begin();

  // Remove views or group views.
  while ( iter != view_list.end() ) {
    const ViewBase * curview = ( * iter++ );
    const QtView * view = dynamic_cast < const QtView * > ( curview );
    QtView * v = 0;
    if (view) v = const_cast < QtView * > ( view );
    const QtGroupView * groupview = 
      dynamic_cast < const QtGroupView * > ( curview );
    QtGroupView * gv =  0;
    if (groupview) gv = const_cast < QtGroupView * > (groupview);
    if (v) remove ( v );
    if (gv) remove (gv);
  }

  notifyObservers ();
}

void
CanvasView::
clear ()
{
  FigureEditor::clear ();

  notifyObservers ();
}

/** @bug Need to remove any text displays that are observing this view
    as cutSelectedToPasteboard does. ( To remove: Text displays are removed
    together with plot display. But not through this function. )
*/
void CanvasView::removeSelected ( )
{
  const vector < const ViewBase * > & views = selectedViews ();
#ifdef ITERATOR_MEMBER_DEFECT
  std::
#endif
  vector < const ViewBase * > ::const_iterator first = views.begin();
  while ( first != views.end() ) {
    const QtView * view = dynamic_cast < const QtView * > ( *first++ );
    QtView * v = const_cast < QtView * > ( view );
    remove ( v );
  }
}

/** @bug When pasting from selectionboard, should not displace like
    when pasting from pasteboard.
*/
void CanvasView::pasteViews ( const std::list < ViewBase * > & views )
{
  scene()->clearSelection();
  std::list < ViewBase * > ::const_iterator first = views.begin();
  for ( ; first != views.end(); ++first ) {
    QtView * view = dynamic_cast < QtView * > ( *first );
    paste ( view );
    view->setSelected(true);
  }

  setChanged ( true );
}

void CanvasView::setInspector ( Inspector * inspector )
{
  m_inspector = inspector;
  m_inspector->enableNewPlotBox ( );
}

Inspector * CanvasView::getInspector ()
{
  return m_inspector;
}

void CanvasView::setChanged ( bool flag )
{
  QWidget * parent = parentWidget ();
  if ( parent != 0 ) {
    PlotterEvent * event = new PlotterEvent ( 0 );
    QApplication::postEvent ( parent, event );
  }
}

void CanvasView::notifyObservers ()
{
  setChanged ( true );
  if ( m_inspector != 0 ) {
    vector < PlotterBase * > plotters = getSelectedPlotters ();

    CanvasSelectionEvent * event = new CanvasSelectionEvent ( plotters );
    QApplication::postEvent ( m_inspector, event );
  }

  WindowController * controller = WindowController::instance ();
  controller -> updateActions ();
}

vector < PlotterBase * >
CanvasView::
getSelectedPlotters () const
{
  vector < PlotterBase * > plotters;

  const vector < const ViewBase * > & viewList = selectedViews ();
  unsigned int size = viewList.size ();
  for ( unsigned int i = 0; i < size; i++ ) {
    const ViewBase * view = viewList [ i ];

    const QtGroupView * groupView = dynamic_cast <const QtGroupView *> (view);

    if (groupView) {
      const vector <PlotterBase *> groupPlotters = groupView -> getPlotters ();
      plotters.insert ( plotters.end(), 
			groupPlotters.begin(), groupPlotters.end() );
      break;
    } 

    //else
    PlotterBase * plotter = view -> getPlotter ();
    if ( plotter != 0 ) {
      plotters.push_back ( plotter );
    }
  }

  return plotters;
}

PlotterBase * CanvasView::selectedPlotter () const
{
  const vector < const ViewBase * > & viewList = selectedViews ();

  if ( viewList.size() != 1 )
    {
      return 0;
    }

  PlotterBase * plotter = viewList[0]->getPlotter ();
  //assert ( plotter );      // Can return 0, handle by caller.

  return plotter;
}

void
CanvasView::
addView ( QtView * view, bool select )
{
  add ( view ); // in FigureEditor
  if ( select ) {
    ensureVisible ( view );
    setSelectedItem ( view );
    vector < const ViewBase * > view_list;
    fillSelectedWithObservers ( view_list );
    s_xml_controller -> saveToSelectionCopy ( view_list );
  }
  notifyObservers ();
}

void
CanvasView::
setPlotMatrix ( unsigned int columns, unsigned int rows )
{
  m_num_width = columns;
  m_num_height = rows;
}

void
CanvasView::
swapOrientation ()
{
  QGraphicsScene * can = scene();
  int numpages = static_cast<int> ( can -> height()
				   / (double) m_apage_h );
  std::swap( m_upage_w, m_upage_h );
  std::swap( m_apage_w, m_apage_h );

  can -> setSceneRect(0, 0, 1.2 * m_upage_w, numpages * m_apage_h );

  showPrinterMargins ( false ); // Delete old ones ( if any    )
  showPrinterMargins ( true );  // Redraw new ones ( if needed )
}

void
CanvasView::
setup ()
{
  CanvasSettings * cs = new CanvasSettings ( this );
  cs -> setOrientation ( s_printer -> orientation() );
  cs -> setWidthNumber ( m_num_width );
  cs -> setHeightNumber ( m_num_height );
  cs -> setAddSelected ( m_add_selected );
  cs -> setPrinterBounds ( m_printer_bounds );
  cs -> setIgnorePrinterMargin ( m_ignore_margin );

  int retval = cs -> exec ();

  if (  retval == CanvasSettings::AcceptedAndRetile ||
        retval == CanvasSettings::Retile )
    {
      m_add_selected = cs -> getAddSelected ();


      if ( cs -> orientation() != s_printer -> orientation() ) {
	s_printer -> setOrientation( cs -> orientation() );
	swapOrientation ();
      }

      if( cs -> printerBounds() == false )
        showPrinterMargins ( false ); // Delete old ones ( if any    )
      else
        {
          showPrinterMargins ( false ); // Delete old ones ( if any    )
          showPrinterMargins ( true );  // Redraw new ones ( if needed )
        }

      m_ignore_margin = cs -> ignorePrinterMargin ();
    }

  if ( retval == CanvasSettings::AcceptedAndRetile ||
       retval == CanvasSettings::Retile ) {
    unsigned int columns = cs -> getWidthNumber ();
    unsigned int rows    = cs -> getHeightNumber ();
    setPlotMatrix ( columns, rows );
    reTile ();
  }

  if ( retval == CanvasSettings::AcceptedAndRetile ) {
    saveSettings ();
  }

  delete cs;
}


void
CanvasView::
setFonts( const QFont & font )
{
  const vector < const ViewBase * > & allviews = views();

  for( unsigned int i = 0; i < allviews.size(); i++ )
    {
      const ViewBase * curview = allviews[ i ];
      const QtView * view = dynamic_cast < const QtView * > ( curview );
      QtView * v = const_cast < QtView * > ( view );
      v -> setDefaultFont( font );
      v -> getPlotter() -> update();
    }
}

void
CanvasView::
calcDefaultSize ( QtView * view, double transform_aspect_ratio )
{
  double width = ( 0.95 * m_upage_w ) / m_num_width;
  double height = ( 0.95 * m_upage_h ) / m_num_height;

  if ( transform_aspect_ratio > 0.0 )
    {
      width = height * transform_aspect_ratio;
    }

  view -> setDrawRect ( 0.0, 0.0, width, height );
}


void
CanvasView::
addPlotDisplay ( PlotterBase * plotter )
{
  addPlotDisplay ( plotter, m_add_selected );
}

QtView *
CanvasView::
wrapPlotter ( PlotterBase * plotter )
{
  ViewFactory * factory = QtViewFactory::instance ();
  ViewBase * view = factory -> createView ( plotter );
  QtView * qtview = dynamic_cast< QtView * > ( view );
  qtview -> setRect ( 0, 0,  -1, -1 );

  return qtview;
}

void CanvasView::addPlotDisplay ( PlotterBase * plotter, bool select )
{
  QtView * qtview = wrapPlotter ( plotter );
  qtview -> setPageWidth( m_upage_w );
  qtview -> setInspector( m_inspector );
  sizeAndPlace ( qtview  );

  addView ( qtview, select );
}

void
CanvasView::
sizeAndPlace ( QtView * view )
{
  PlotterBase * plotter = view -> getPlotter ();
  double transform_aspect_ratio = plotter -> getAspectRatio();
  calcDefaultSize ( view, transform_aspect_ratio );
  placeGraph ( view );
}

static bool
sortViews ( QGraphicsItem * first, QGraphicsItem * second )
{
  QRectF frect = first->sceneBoundingRect();
  QRectF srect = second->sceneBoundingRect();

  QPointF diff = frect.topLeft() - srect.topLeft();

  if (fabs(diff.y()) < 100.)	// Same-ish row
	return (diff.x() < 0);	// Furthest left

  return (diff.y() < 0);	// Otherwise, closest to top
}

void
CanvasView::
reTile ()
{
  QList<QGraphicsItem *> items = scene()->items();
  QList<QGraphicsItem *>::const_iterator it;

  for (it = items.begin(); it != items.end(); it++) {
	if ((*it)->scene() && (*it)->parentItem() == NULL)
	    (*it)->scene()->removeItem(*it);
  }

  qSort(items.begin(), items.end(), sortViews);

  placeItems(items);
}

void
CanvasView::
placeItems ( QList<QGraphicsItem *> & items )
{
  QList<QGraphicsItem *>::const_iterator first = items.begin();

  while ( first != items.end() ) {
      QGraphicsItem *item = *first++;

      QtView *view = dynamic_cast<QtView *>(item);
      if (view == NULL) {
	if (item->scene() == NULL)
	  m_canvas->addItem(item);
	continue;
      }

      if (view->getPlotter()->isTextPlotter())
	continue;

      calcDefaultSize ( view, view -> getAspectRatio() );
      placeGraph ( view );
      add ( view );
  }
}

void
CanvasView::
reTilePage ()
{
  double y = mapToScene(QPoint(0,0)).y();
  double page = y / m_apage_h;
  QRectF rect(0, page * m_apage_h, m_apage_w, m_apage_h);
  QList<QGraphicsItem *> items = scene()->items(rect);
  QList<QGraphicsItem *>::const_iterator it;

  for (it = items.begin(); it != items.end(); it++) {
	if ((*it)->scene() && (*it)->parentItem() == NULL)
	    (*it)->scene()->removeItem(*it);
  }

  qSort(items.begin(), items.end(), sortViews);

  placeItems(items);
}

void CanvasView::moveToPlotter ( const PlotterBase * plotter, QtView * view )
{
  QtView * target = getViewFor ( plotter );
  QRectF rect = target -> boundingRect ();
  double x = 0.25 * rect.width();
  double y = 0.25 * rect.height();

  if (view->scene() != NULL)
	view->scene()->removeItem(view);

  view->setParentItem(target);
  view->moveBy ( x - view->x(), y - view->y() );
}

std::pair<double, double>
CanvasView::
moveToPlotterAt ( const PlotterBase * plotter,
                  QtView * view,
                  double xrel, double yrel )
{
   QtView * sel_item = getViewFor ( plotter );
   QRectF rect = sel_item->boundingRect();
   double x = xrel * rect.width();
   double y = xrel * rect.height();

   view->moveBy ( x, y );

// Find the lower left corner of the view.
   QRectF viewRect = view->boundingRect();
   double xLowerLeft = view->x();
   double yLowerLeft = view->y() + viewRect.height();

// Normalize to selectedItem coordinate system.
   xLowerLeft = ( xLowerLeft - sel_item->x() ) / rect.width();
   yLowerLeft = ( yLowerLeft - sel_item->y() ) / rect.height();

   return std::make_pair<double, double>(xLowerLeft, yLowerLeft);
}

void
CanvasView::
addTextDisplay ( PlotterBase * plotter,
                 const std::string & s,
                 const std::string & text )
{
  DisplayController * display_controller = DisplayController::instance ();
  ViewFactory * factory = QtViewFactory::instance ();

  ViewBase * view
    = display_controller->createTextView ( factory, plotter, s, text );
  assert ( view != 0 );

  QtView * qtview = dynamic_cast < QtView * > ( view );
  assert ( qtview != 0 );

  moveToPlotter(plotter, qtview);
  scene()->clearSelection();
  qtview->setSelected(true);
  notifyObservers();
}

std::pair<double, double>
CanvasView::
addTextDisplayAt ( PlotterBase * plotter,
                   const std::string & type,
                   const std::string & text,
                   double x, double y )
{
  DisplayController * display_controller = DisplayController::instance ();
  ViewFactory * factory = QtViewFactory::instance ();

  ViewBase * view
    = display_controller->createTextView ( factory, plotter, type, text );

  QtView * qtview = dynamic_cast < QtView * > ( view );

  std::pair<double, double> lowerLeftCorner
     = moveToPlotterAt ( plotter, qtview, x, y );
  scene()->clearSelection();
  qtview->setSelected(true);
  notifyObservers();
 
  return lowerLeftCorner;
}

void
CanvasView::
addFuncDisplay ( PlotterBase * plotter, const std::string & s)
{
  FunctionController * controller = FunctionController::instance ();
  const ViewFactory * factory = QtViewFactory::instance ();

  ViewBase * view
    = controller->createFuncView ( factory, plotter, s );

  QtView * qtview = dynamic_cast < QtView * > ( view );
  moveToPlotter ( plotter, qtview );

  addView (qtview, false );

  // Make sure new view on top of others
  double z = maximumZ ();
  
  // Make sure the new text plotter on top all other text plotters
  qtview->setZValue(1.0);
}

/** @bug Need to also remove any views observing this plotter as
    cutSelectedToPasteboard does. */
void
CanvasView::
removeDisplay ( PlotterBase * plotter )
{
 QtView * view = getViewFor ( plotter );

 if ( view != 0 ) remove ( view );
}

void
CanvasView::
setVisibleNonViews ( bool yes )
{
  QList<QGraphicsItem *> items = m_canvas->items ();
  QList<QGraphicsItem *>::iterator first = items.begin();
  while ( first != items.end() ) {
    QGraphicsItem * item = *first++;
    QtView * view = dynamic_cast < QtView * > ( item );
      if ( view == 0 ) {
        item -> setVisible ( yes );
      }
  }
}

void
CanvasView::
print ()
{
  setVisibleNonViews ( false );
  FigureEditor::print ();
  setVisibleNonViews ( true );
}

void
CanvasView::
print ( const std::string & filename )
{
  setVisibleNonViews ( false );
  FigureEditor::print ( filename );
  setVisibleNonViews ( true );
}


const vector < const ViewBase * >
CanvasView::
selectedViews () const
{
  vector<const ViewBase *> sel_views;
  const vector<QGraphicsItem *> selected = getSelectedItems();

  vector<QGraphicsItem *>::const_iterator first = selected.begin();
  while ( first != selected.end() ) {
    QGraphicsItem *item = *first++;
    QtView *view = dynamic_cast<QtView *>(item);
    if ( view != NULL ) // may be printer page border
      sel_views.push_back ( view );
    QtGroupView * groupview = dynamic_cast<QtGroupView *>(item);
    if ( groupview != NULL )
      sel_views.push_back ( groupview );
  }

  return sel_views;
}

const vector < const ViewBase * > CanvasView::views () const
{
  vector<const ViewBase *> views;

  QList<QGraphicsItem *> item_list = m_canvas->items();
  QList<QGraphicsItem *>::iterator it = item_list.begin();
  for ( ; it != item_list.end(); ++ it ) {
    const QtView * view = dynamic_cast < const QtView * > ( *it );
    if ( view != NULL )
      views.push_back ( view );
  }

  return views;
}

void
CanvasView::
fillPlotterList ( std::vector < PlotterBase * > & plotterlist )
{
  plotterlist.clear();

  const vector < const ViewBase * > & view_list = views ();
#ifdef ITERATOR_MEMBER_DEFECT
  std::
#endif
  vector < const ViewBase * >::const_iterator first = view_list.begin ();
  for ( ; first != view_list.end(); ++first ) {
    const QtView * qtview = dynamic_cast < const QtView * > ( *first );
    plotterlist.push_back ( qtview->getPlotter () );
  }
}


QtView *
CanvasView::
getViewFor ( const PlotterBase * target ) const
{
  QtView * view = 0;
  const vector < const ViewBase * > & all_views = views ();
  vector < const ViewBase * > :: const_iterator  first = all_views.begin();

  while ( first != all_views.end() ) {
    const ViewBase * vb = *first++;
    const PlotterBase * plotter = vb->getPlotter();
    if ( plotter == target ) {
      const QtView * v = dynamic_cast < const QtView * > ( vb );
      view = const_cast < QtView * > ( v );
      break;
    }
  }

  return view;
}

void
CanvasView::
createImageFilters ()
{
#ifdef STRING_CLEAR_DEFECT
  s_image_filters.erase();
#else
  s_image_filters.clear();
#endif

  const string eps ( "eps" );
  bool eps_inserted = false;

  QList < QByteArray >  slist = QImageWriter::supportedImageFormats ();
  QList < QByteArray > ::const_iterator it = slist.begin();

  while ( it != slist.end() ) {
    QString upper = *it++;
    QString lower = upper.toLower();
    string low = lower.toLatin1().data();
#ifdef Q_OS_MACX
#else // not mac os x
  if ( low == "jpeg" ) low = "jpg";
#endif
    if ( eps >= low ) {

      s_image_filters += " (*." + low + ");;";
    }
    else {
      if ( eps_inserted == false ) {
        s_image_filters += "(*." + eps + ");;";
        eps_inserted = true;
      }
      s_image_filters += "(*." + low + ");;";
    }
  }
  // Add FITS file.
  s_image_filters += "(*.fits);;";
  s_image_filters += "(*.fits.gz)";
}

QFileDialog *
CanvasView::
createImageDialog ()
{
  if ( s_image_filters.empty () ) createImageFilters ();

  QStringList filters = QStringList(s_image_filters.c_str());

  QFileDialog * dialog = new QFileDialog ( this );
  dialog->setFilters ( filters );
  dialog->setFileMode ( QFileDialog::AnyFile );

  return dialog;
}

/* The menu item calling this method is disabled when no plot is
   selected so there's no need to see if any plots are selected
   here.
*/
void
CanvasView::
saveSelectedImages ()
{
  setVisibleNonViews ( false );

  const vector < QGraphicsItem * > views = getSelectedItems (); // make copy
  assert ( views.empty () == false );

  for ( unsigned int i = 0; i < views.size (); i++ ) {
    setCollidingSelected ( views[i] );
  }

  if ( s_image_filters.empty () ) createImageFilters ();
  QString sel_filt;
  QString filename 
    = QFileDialog::getSaveFileName ( this, // parent
				     tr("Save image as"), // caption
				     QString::null, // working dir
				     s_image_filters.c_str(),
				     & sel_filt ); // selected filter

  if ( filename.isEmpty () ) return;

  string fn = filename.toLatin1().data();
  int pos = filename.lastIndexOf ( '.' );
  if ( pos < 0 ) {
    string sf = sel_filt.toLatin1().data();
    string::size_type pos1 = sf.find ( '(' );
    string::size_type pos2 = sf.find_last_of ( ')' );
    string::size_type len = ( pos2 ) - ( pos1 + 2 );
    string suffix = sf.substr ( pos1 + 2, len );
    fn += suffix;
  }

  saveSelectedImages ( fn );

  // Restore original selection flags.
  scene()->clearSelection();
  for ( unsigned int i = 0; i < views.size(); i++ )
    views[i]->setSelected(true);

  setVisibleNonViews ( true );
}

/** @todo Move this method and others that use PlotterBase to a
    controller.  This logic is independent of the GUI so should not be
    here. */
void CanvasView::setIntervalEnabled ( bool yes )
{
  DisplayController * controller = DisplayController::instance ();
  const vector < const ViewBase * > & view_list = views ();
  vector < const ViewBase * > ::const_iterator first = view_list.begin ();

  while ( first != view_list.end() ) {
    const ViewBase * view = *first++;
    PlotterBase * plotter = view->getPlotter();
    controller -> setAllIntervalEnabled ( plotter, yes );
  }
}

void
CanvasView::
restoreFromSelectCopy ()
{

  ViewFactory * factory = QtViewFactory::instance ();
  const vector < const ViewBase * > & targets = views ();

  const list < ViewBase * > & views
    = s_xml_controller->getFromSelectionCopy ( factory, targets );

  if ( views.empty() == false ) {
    removeSelected ();
    pasteViews ( views );

    notifyObservers ();
  }
}

void
CanvasView::
fillSelectedWithObservers ( std::vector < const ViewBase * > & view_list )
{
  const vector < const ViewBase * > & sel_views = selectedViews ();

  view_list.resize ( sel_views.size() );
  copy ( sel_views.begin(), sel_views.end(), view_list.begin() );

  DisplayController * controller = DisplayController::instance ();
  const vector < const ViewBase * > & all_views = views ();

  controller->addTextViewToList ( view_list, all_views );
}




void
CanvasView::
savePlotAsImage ( const PlotterBase * plotter,
                  const std::string & filename )
{
  if ( plotter == 0 ) return;

  string::size_type i = filename.find_last_of ( '.');
  if ( i != string::npos ) {
    string suffix ( filename.substr ( i + 1 ) );
    if ( suffix == "eps" ||
         suffix == "EPS" ) {
      savePlotAsEps ( plotter, filename );
    }
    else {
      QtView * view = getViewFor ( plotter );
      assert ( view != 0 );

      QRectF rect = view -> rect ();
      saveAreaAsPixmap ( rect, filename );
    }
  }
}

void
CanvasView::
toLower ( std::string & upper )
{
  string::size_type size = upper.size();
  for ( string::size_type i = 0; i < size; i++ ) {
    upper[i] = std::tolower ( upper[i] );
  }
}

bool
CanvasView::
isValidImageFormat ( const std::string & suffix )
{
  // Handle gzipped suffix
  if (suffix=="gz" || suffix=="GZ") return true;

  if ( s_image_filters.empty () ) createImageFilters ();

  string star ( "*." + suffix );
  toLower ( star );

  // following could be 64 bit unsigned int
  string::size_type i = s_image_filters.find ( star );

  return i != string::npos;
}

void
CanvasView::
showFitsError () const
{
  QString message = tr( "Sorry, this installation of %1 was not\n"
			"built with the optional FITS support."
			).arg(s_app_name.c_str());
  QMessageBox::information ( const_cast < CanvasView * > ( this ),
			     s_app_name.c_str(),
			     message );
}

void
CanvasView::
saveSelectedImages ( const std::string & fn )
{
  string filename = fn;
  string::size_type i = filename.find_last_of ( '.' );

  if ( i == string::npos ) { // no suffix given
    if ( m_image_dialog != 0 ) {
      QString s = m_image_dialog->selectedFilter ();
      string filt = s.toLatin1().data();
      string::size_type ipos = filt.find ( "*." ) + 1;
      string::size_type epos = filt.find ( ")" );
      string suffix ( filt.substr ( ipos, epos - ipos ) );
      filename += suffix;
    }
    else {
      filename += ".jpg";
    }
    i = filename.find_last_of ( '.' );
  }

  string suffix ( filename.substr ( i + 1 ) );
  if ( isValidImageFormat ( suffix ) ) {
    string base ( filename.substr ( 0, i - 1 ) ); //basename
    if ( base.empty () ) filename = tr("image.").toAscii().data() + suffix;

    setVisibleNonViews ( false );
    if ( suffix == "eps" ||
         suffix == "EPS" ) 
      {
	saveSelectedAsEps ( filename );
      } 
    else if (suffix == "fits" ||
	     suffix == "FITS" ||
	     suffix == "gz"   ||
	     suffix == "GZ")
      {
#ifdef HAVE_CFITSIO
	saveSelectedAsFits ( filename );
#else
	showFitsError ();
#endif	
      }
    else
      {
	setUnselectedVisible ( false );
	saveSelectedAsPixmap ( filename );
	setUnselectedVisible ( true );
      }
    setVisibleNonViews ( true );
  }
  else {
    invalidImageFormatError ( filename );
  }
}

void
CanvasView::
savePlotAsFits ( const PlotterBase * plotter,
		 const std::string & filename ) const
{
#ifdef HAVE_CFITSIO
  if ( plotter -> isImageConvertable () == false ) {
    QString message = tr( "The contents of this plotter is not\n"
			  "onvertable to a FITS image." );
    QMessageBox::information ( const_cast < CanvasView * > ( this ),
			       s_app_name.c_str(),
			       message );
    return;
  }

  PlotterBase * p = const_cast < PlotterBase * > ( plotter );

  const std::vector <unsigned int> & fitsshape = p->getShape();
  const std::vector <double> & fitsdata = p->getZValues();
  
  FitsController::instance ()
    ->writeImageToFile ( fitsshape[0], fitsshape[1], fitsdata, filename );

#else
  showFitsError ();
#endif
}

#ifdef HAVE_CFITSIO
void
CanvasView::
saveSelectedAsFits ( const std::string & filename )
{
  PlotterBase * plotter = selectedPlotter ();

  if ( !plotter ) {
    QString message = tr( "No plot or more than one plot selected." );
    QMessageBox::information ( this,
                               s_app_name.c_str(),
                               message );
    return;
  }

  else {
    savePlotAsFits ( plotter, filename );
  }
}
#endif
  

void
CanvasView::
savePlotAsEps ( const PlotterBase * plotter,
                const std::string & filename ) const
{
  if ( plotter != 0 ) {
    QtView * view = getViewFor ( plotter );
    assert ( view != 0 );

    QRectF rect = view -> sceneBoundingRect ();
    EpsView * eps = new EpsView ( filename,
                                  rect.x(), rect.y (),
                                  rect.width(), rect.height() );
    const Rect & marg_rect = view -> getMarginRect ();
    eps -> setMarginRect ( marg_rect );

    PlotterBase * p = const_cast < PlotterBase * > ( plotter );
    eps -> setPlotter ( p );
    eps -> setDrawRect ( rect.x(), rect.y(),
                         rect.width(), rect.height() );
    eps -> drawSelf ();
    eps -> closeFile ();
    delete eps;
  }
}

void
CanvasView::
saveSelectedAsEps ( const std::string & filename )
{
  QRectF rect = getSelectedBounds ();

  EpsView * eps_view
    = new EpsView ( filename,
                    rect.x(), rect.y(), rect.width(), rect.height() );

  const vector < const ViewBase * > & viewList = selectedViews ();
#ifdef ITERATOR_MEMBER_DEFECT
  std::
#endif
  vector < const ViewBase * > :: const_iterator first = viewList.begin();
  while ( first != viewList.end() ) {
    const ViewBase * vb = *first++;
    const QtView * view = dynamic_cast < const QtView * > ( vb );
    PlotterBase * plotter = view->getPlotter ();
    QRectF rect = view->sceneBoundingRect ();

    eps_view -> setPlotter ( plotter );
    eps_view -> setDrawRect ( rect.x(), rect.y(),
                              rect.width(), rect.height() );
    const Rect & marg_rect = view -> getMarginRect ();
    eps_view -> setMarginRect ( marg_rect );

    eps_view -> drawSelf ();
  }

  eps_view -> closeFile ();

  delete eps_view;
}

/** @request Raise error dialog if saving to file was not successful.
 */
void
CanvasView::
saveAs ( const QString & filename ) const
{
  const vector < const ViewBase * > & view_list = views ();
  saveAs ( view_list, filename );
}

void
CanvasView::
saveAs ( const std::vector < PlotterBase * > & plotters,
	 const QString & filename )
{
  vector < const ViewBase * > views;

  unsigned int size = plotters.size ();
  for ( unsigned int i = 0; i < size; i++ ) {
    PlotterBase * plotter = plotters [i];
    QtView * view = wrapPlotter ( plotter );
    views.push_back ( view );
  }
  saveAs ( views, filename );
}

void
CanvasView::
saveAs ( const std::vector < const ViewBase * > & views,
	 const QString & filename )
{
  if ( s_xml_controller == 0 ) {
    s_xml_controller = QtXMLController::instance ();
  }

  XmlController::Status status
    =  s_xml_controller->saveToFile ( views, filename.toLatin1().data() );
  if ( status != XmlController::Success ) {
    return;
  }
}

/** @todo Report any error conditions.
    @todo One error condition is saving on top of existing file.
*/
void
CanvasView::
exportTextTuple ( const std::string & data_suffix )
{
 DataSourceController * controller = DataSourceController::instance ();

  const vector < string > & tuples = controller->getNTupleNames();

  if ( tuples.empty () ) {
    QString message = tr( "There are no NTuples to be exported" );
    QMessageBox::information ( this,
                               s_app_name.c_str(),
                               message );
    return;
  }

  QString filename =
    QFileDialog::getSaveFileName ( this, // parent
				   tr("save file dialog") );

  if ( filename == QString::null ) return;

  QString suffix ( data_suffix.c_str() );
  if ( filename.endsWith ( suffix, Qt::CaseInsensitive ) == false ) {
    filename += data_suffix.c_str();
  }

  const string tuple_name = m_inspector -> getSelectedDataSourceName ();
  string tuple_file ( filename.toLatin1().data() );

  try {
    NTupleController::instance ()
      ->writeNTupleToFile ( tuple_name, tuple_file );
    controller -> changeName ( tuple_name, tuple_file );
  }
  catch ( const std::exception & e ) {
    QString message = tr( "An error occurred in writing file.\n\n" );
    message += e.what();
    QMessageBox::critical ( this, // parent
			    tr ( "Write error" ), // caption
			    message,
			    QMessageBox::Ok,
			    QMessageBox::NoButton,
			    QMessageBox::NoButton );
  }

  notifyObservers ();
}
 
bool
CanvasView::
fillPickedPoint(QMouseEvent * e, std::vector<double> & picked) {
   QPointF p = mapToScene(e->pos());

   QGraphicsItem * selItem = FigureEditor::selectedItem();
   QGraphicsRectItem * rect = dynamic_cast<QGraphicsRectItem *>(selItem);
   if (selItem) {
      QtView * qtview = dynamic_cast<QtView *>(selItem);
      if (!qtview) return false;
      picked.clear();
      if ( rect->rect().contains(p) ) {
         qtview -> fillPickedPoint ( p.x(), p.y(), picked );
         if (!picked.empty()) {
            return true;
         }
      }
   }
   return false;
}

void
CanvasView::
updatePickTable(QtView * qtview, std::vector<double> & picked) {
  if (!picked.empty()){
    PickTable * table = 0;
    map < const QtView *, PickTable * > ::iterator first
      = m_table_map.find ( qtview );
    if ( first != m_table_map.end () ) {
      table = m_table_map [ qtview ];
    }
    if ( m_cut_mode == 0 ) {
      table -> addItem ( picked );
    }
  }
}

/* Note: indexing into the picked vector offset by one because the
   first entry is the type.   See XyPlotter::fillPickedPointFrom().
 */
void
CanvasView::
setPlotZoom(const std::vector<double> & start_picked,
            const std::vector<double> & picked) {
   if (getZoomMode()) {
      if ( !(selectedPlotter()->getCurrentRangeSaved()) ) {
         setCurrentRange();
      }
      PlotterBase * plotter = selectedPlotter();
      plotter->setRange( "x",
                         std::min(picked[1], start_picked[1]),
                         std::max(picked[1], start_picked[1]) );
      if (plotter->hasAxis(Axes::Z) || plotter->hasZoomY()) {
         plotter->setRange( "y",
                            std::min(picked[2], start_picked[2]),
                            std::max(picked[2], start_picked[2]) );
      }
      plotter -> setCurrentRangeSaved ( false );
   }
}

void
CanvasView::
setCurrentRange()
{
  if ( ! m_current_range.empty () ){
    m_current_range.clear ();
  }
  PlotterBase * plotter = selectedPlotter ();

    m_current_range.push_back( plotter -> getRange(Axes::X,true).low());
    m_current_range.push_back( plotter -> getRange(Axes::X,true).high());
    m_current_range.push_back( plotter -> getRange(Axes::Y,true).low());
    m_current_range.push_back( plotter -> getRange(Axes::Y,true).high());

    int i = plotter  -> currentView();
    plotter -> saveViewAtIndex( m_current_range , i + 1 );
    plotter -> setCurrentRangeSaved( true );
}

std::vector<double>&
CanvasView::
getCurrentRange()
{
  return m_current_range;
}

void
CanvasView::
showInspector ()
{
  m_inspector -> show ();
}

void
CanvasView::
viewShowPickTable()
{
//   if ( !isSingleItemSelected() ) return;
  if ( isSingleItemSelected () == false ) {
    QString message = tr( "One must select a single plot before showing\n"
			  "the pick table." );
    QMessageBox::information ( this,
			       s_app_name.c_str(),
			       message );
    return;
  }

  QGraphicsItem * selItem = selectedItem();
  QtView * qtview = dynamic_cast < QtView * > ( selItem );
  if ( qtview == 0 ) {
    QString message = tr( "Can not show pick table of a group view.\n"
			  "Please select a single plotter view.");
    QMessageBox::information ( this,
                               s_app_name.c_str(),
                               message );
    return;
  }
  map < const QtView *, PickTable * > ::iterator first
    = m_table_map.find ( qtview );

  if ( first == m_table_map.end() ) {
    PlotterBase * plotter = qtview->getPlotter ();

    PickTable * table = new PickTable ( plotter );
    m_table_map [ qtview ] = table;
    table->show();
  }
  else {
    // Show existing pick table.
    m_table_map[qtview]->show();
  }
}

void
CanvasView::helpAbout()
{
  const std::string & version = WindowController::version ();
  QString about = tr("<h2>HippoDraw version: ");
  about += version.c_str();
  about += "</h2>";
#ifndef HIPPO_RELEASE
  about += QString("<h4>SVN $Revision: 61655 $</h4>").replace("$","");
#endif
  about += tr("<h3> Built on ");
  about += "<ul>";
  about += tr("<li> Qt ");
  about += QT_VERSION_STR;
#ifdef HAVE_CFITSIO
  about += tr("<li> CFITSIO ");
  const string & fits_version = FitsController::instance () -> version ();
  about += fits_version.c_str();
#endif

#ifdef HAVE_MINUIT
  about += tr("<li> C++ Minuit");
#endif

#ifdef HAVE_MINUIT2
  about += tr("<li> Minuit2");
#endif

#ifdef HAVE_ROOT
  about += tr("<li> ROOT ");
  const string & root_version = RootController::instance() -> version ();
  about += root_version.c_str();
#endif

#ifdef HAVE_WCSLIB
  about += tr("<li> WCSlib ");
  about += WCSLIB_VERSION_STR;
#endif
  about += "</ul>";

  about += tr("<h3>Contributors...");
  about += "<ul>";
  about += "<li> Stephane Bonneaud";
  about += "<li> James Chiang";
  about += "<li> Johann Cohen-Tanugi";
  about += "<li> Xie Fang";
  about += "<li> Kaustuv";
  about += "<li> Fabian Kislat";
  about += "<li> Paul F. Kunz";
  about += "<li> Sanket B. Malde";
  about += "<li> Matthew D. Langston";
  about += "<li> Joy Rajiv";
  about += "<li> Matan Shacham";
  about += "<li> Nathan Whitehorn";
  about += "<li> Oded Wurman";
  about += "</ul>";

  QMessageBox::about ( this, // parent
		       tr("HippoDraw"),  // title
		       about );
}

const std::string &
CanvasView::
applicationName () const
{
  return s_app_name;
}

void
CanvasView::
setCutMode ( int mode )
{
  m_cut_mode=mode;
}

void
CanvasView::
createNTuple ()
{
  DataSourceController * controller = DataSourceController::instance ();

  const vector < string > & tuples = controller->getNTupleNames();

  if ( tuples.empty () ) {
    QString message = tr( "There are no NTuples to be used as source" );
    QMessageBox::information ( this,
                               s_app_name.c_str(),
                               message );
    return;
  }

  CreateNTuple * cnt = new CreateNTuple ();
  cnt-> show ();
}

NTuple *
CanvasView::
getPickTable ()
{
  const QtView * view = dynamic_cast<const QtView *>(selectedItem());
  if (view == NULL)
	return NULL;

  PickTable * table=0;
  map < const QtView *, PickTable * > ::iterator first
    = m_table_map.find ( view );
  if ( first != m_table_map.end () ) {
    table = m_table_map [ view ];
  }
  if ( table ) {
    return table->getPickTable();
  }
  
  return NULL;

}

NTuple *
CanvasView::
getPickTable ( const PlotterBase * plotter )
{

  const QtView * view = getViewFor ( plotter );
  
  PickTable * table = 0;
  map < const QtView *, PickTable * > :: iterator first
    = m_table_map.find (view);
  if ( first !=m_table_map.end() ) {
    table = m_table_map [view];
  }
  if ( table ) {
    return table->getPickTable();
  }
  
  return NULL;
}

void
CanvasView::
groupView ()
{
  const vector < const ViewBase * > & selViews = selectedViews();
  if ( selViews.empty () ) return;

  QRectF groupRect;
  QtGroupView * groupView = new QtGroupView ();

  vector < const ViewBase * >::const_iterator it = selViews.begin();
  while (it != selViews.end()) {
    ViewBase * view = const_cast < ViewBase * > ( *(it++) );

    QtView * qtview = dynamic_cast < QtView * > ( view );
    if (!qtview) continue;  // don't include groupview in groupview

    if (qtview->scene() != NULL)
	    qtview->scene()->removeItem(qtview);
    groupView->addView(view);
    groupRect |= qtview->sceneBoundingRect();
    if (qtview->parentItem() == NULL)
	    qtview->setParentItem(groupView);
  }

  QList<QGraphicsItem *> children = groupView->childItems();
  QList<QGraphicsItem *>::const_iterator childit;

  for (childit = children.begin(); childit != children.end(); childit++)
	if ((*childit)->parentItem() == groupView)
		(*childit)->moveBy(-groupRect.x(),-groupRect.y());
  
  groupView->setDrawRect(groupRect.x(), groupRect.y(), groupRect.width(),
	groupRect.height() );
  groupView->setPositions();  // set relative position
  add(groupView);
  scene()->clearSelection();
  groupView->setSelected(true);
}

void
CanvasView::
ungroupView ()
{
  const vector < const ViewBase * > & selViews = selectedViews();
  if ( selViews.size() != 1 ) return;

  const QtGroupView * groupView = 
    dynamic_cast < const QtGroupView * > ( selViews[0] );
  if (!groupView) return;

  QtGroupView *gv = const_cast<QtGroupView *>(groupView);
  QRectF groupRect = gv->sceneBoundingRect();
  scene()->clearSelection();
  const vector < ViewBase * > & views = gv->getViews();

  vector < ViewBase * >::const_iterator first = views.begin();
  for ( ; first != views.end(); ++first ) {
    QtView * qtview = dynamic_cast < QtView * > ( *first );
    if (qtview->parentItem() == gv) {
	qtview->setParentItem(NULL);
	qtview->moveBy(groupRect.x(), groupRect.y());
        add(qtview);
    }
    
    gv->removeView(*first);
    --first;     // a view is removed, decrease the iterator
    qtview->setSelected(true);
  }
  remove ( gv );
}

void
CanvasView::
addRecentFile (const QString & filename, QActionGroup * ag )
{
  QObject * parent = ag -> parent ();
    
  // Avoid duplicate recently opened files. 
  // Remove old item with same name if needed.
  std::list<QAction *>::iterator it=m_recent_list.begin();

  for ( ; it!=m_recent_list.end(); ++it ) {
    if ( (*it)->text() == filename ) {
      delete *it;    // need removeFrom(ag) ?
      it=m_recent_list.erase(it);
      break;
    }
  }

  // Add new item.
  QAction * action = new QAction ( filename, parent );
  ag -> addAction ( action );
  action -> setCheckable ( true );
  m_recent_list.push_back(action);

  // Remove the first element if size is more than 5.
  if (m_recent_list.size() > 5) {
    it=m_recent_list.begin();
    delete *it;    //need removeFrom(ag) ?
    m_recent_list.pop_front();
  }
}

void
CanvasView::
autosaveSettings (QMainWindow * mw)
{
  QSettings settings;
  QString fn;
  list<QAction *>::iterator it;
  unsigned int i=0;

  // Save recently opened files
  for (it=m_recent_list.begin(); it!=m_recent_list.end(); i++, it++) {
      fn = (*it)->text();
      QString number=QString::number(i);
      settings.setValue ( s_app_key + "Files/File"+number, fn );
  }

  // Save dock window positions.
  settings.setValue(s_app_key+"DockWindow/Position", mw->saveState());
}

