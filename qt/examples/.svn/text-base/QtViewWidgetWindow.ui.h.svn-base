/* -*- mode: c++ -*- */
/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

using std::string;
using std::vector;

using namespace hippodraw;

/** @request This class needs to be converted to Qt 4
 */
void QtViewWidgetWindow::init()
{

}

void QtViewWidgetWindow::fileOpen()
{
  QString file_name 
    = QFileDialog::getOpenFileName ( QString::null, // starting directory
				     "ntuple (*.*)", // filter
				     this, // parent
				     "open ntuple file", // internal name
				     "Open ntuple" ); // actual window caption
  if ( file_name.isEmpty () ) return;

 const string filename( file_name.latin1() );
 NTupleController * nt_controller = NTupleController::instance ();
 DataSource * nt 
    = nt_controller->createNTuple ( filename  );

  const string histo ( "Histogram" );
  vector < string > bindings;
  bindings.push_back ( "Cost" );

  DisplayController * dc_controller = DisplayController::instance ();
  PlotterBase * plotter 
      = dc_controller->createDisplay ( histo, *nt, bindings );

  m_view->setPlotter ( plotter );

  m_inspector = new Inspector ();
  m_view -> setInspector ( m_inspector );

  m_inspector->show();
  m_view->update ( plotter );
  PlotterEvent * event = new PlotterEvent ( plotter );
  QApplication::postEvent ( m_inspector, event );
}

void QtViewWidgetWindow::filePrint()
{

}

void QtViewWidgetWindow::fileExit()
{
qApp->quit();
}

void QtViewWidgetWindow::helpAbout()
{

}
