/** @file
 
hippodraw::PyCanvas class implementation.
 
Copyright (C) 2002, 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.


$Id: PyCanvas.cxx,v 1.100 2007/08/30 22:10:02 pfkeb Exp $

*/

// for dll interface warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include <iostream>
#include "PyApp.h"
#include "QtCut.h"

#include "PyCanvas.h"

#include "controllers/DisplayController.h"
#include "controllers/FunctionController.h"
#include "datasrcs/NTuple.h"
#include "pattern/FactoryException.h"
#include "plotters/Cut1DPlotter.h"

#include "qt/QtView.h"
#include "qt/CanvasView.h"
#include "qt/CanvasViewProxy.h"
#include "qt/CanvasWindow.h"
#include "transforms/BinaryTransform.h"

#include "axes/Range.h"

#include <stdexcept>

using std::runtime_error;
using std::string;

using namespace hippodraw;

PyCanvas::PyCanvas ( CanvasWindow * window ) 
  : m_has_gui ( true )
{
  m_canvas = window;
  CanvasView * view = m_canvas -> getCanvasView ();

  m_canvas_proxy = new CanvasViewProxy ( view );
}

/** @bug @@@@@@ Can not use this method from Python, needs to be added
    so one can run in batch.  Try running batchmode.py
 */
PyCanvas::PyCanvas ( )
  : m_canvas (0),
    m_canvas_proxy (0),
    m_has_gui ( true )
{
  PyApp::lock();

  m_canvas = new CanvasWindow ();
  CanvasView * view = m_canvas -> getCanvasView ();
  m_canvas_proxy = new CanvasViewProxy ( view );

  PyApp::unlock ();
}

void
PyCanvas::
check () const
{
  if ( m_canvas == 0 ) {
    string what ( "Can not use this method as there is no window\n"
		  "              associated with the canvas." );
    throw  std::runtime_error ( what );
  }
}

void
PyCanvas::
show ()
{
  check ();
  PyApp::lock();
  m_canvas -> show ();
  PyApp::unlock ();
}

void
PyCanvas::
close ()
{
  PyApp::lock();
  m_canvas -> closeNoPrompt ();
  m_canvas = 0;
  PyApp::unlock ();
}

/** @bug @@ why is variable yes ignored?
 */
void
PyCanvas::
addDisplay ( QtDisplay * display_wrap )
{
  if ( m_has_gui ) {
    check();
    PlotterBase * plotter = display_wrap->display();
    m_canvas_proxy -> addDisplay ( plotter );
  }
  else {
    m_displays.push_back ( display_wrap );
  }
  //  bool yes = 
  PyApp::hasPendingEvents ();
 }

void PyCanvas::saveAs ( const std::string & filename )
{
  if ( m_has_gui ) {
    check();
    PyApp::lock();
    m_canvas->saveAs ( filename.c_str() );
    PyApp::unlock ();
  } else {
    vector < PlotterBase * > plotters;
    unsigned int size = m_displays.size ();

    for ( unsigned int i = 0; i < size; i++ ) {
      QtDisplay * display = m_displays [i];
      PlotterBase * plotter = display -> display ();
      plotters.push_back ( plotter );
    }
    CanvasView::saveAs ( plotters, filename.c_str() );
  }

}

QtDisplay * PyCanvas::getDisplay ()
{
  check();

  PyApp::lock();
  QtDisplay * display = 0;
   PlotterBase * plotter = m_canvas->selectedPlotter();
   if (plotter != 0) {
      display = new QtDisplay( m_canvas->selectedPlotter() );
   }
   PyApp::unlock ();

   return display;
}

/** @bug @@@@ The displays do not seem to come out in same order that
    they were inserted.
 */
const std::vector<QtDisplay *> & PyCanvas::getDisplays() const {

  check();
  PyApp::lock();
   m_displays.clear();

   // Replicate some of the logic in PlotTable::initialize() to create
   // a vector of QtDisplay pointers.
 
   const std::vector< const ViewBase * > & views = m_canvas->views();
   std::vector< const ViewBase * > :: const_iterator viewIt = views.begin();
   while ( viewIt != views.end() ) {
      const ViewBase * view = *viewIt++;
      PlotterBase * plotter = view->getPlotter();
      m_displays.push_back( new QtDisplay(plotter) );
   }
   PyApp::unlock ();
   return m_displays;
}

QtCut * 
PyCanvas::
getCut ()
{
  check();
  QtCut * qtcut = 0;
  PlotterBase * plotter = m_canvas->selectedPlotter();

  if ( plotter != 0 ) {
     CutPlotter * cut_plotter = dynamic_cast < CutPlotter * > ( plotter );

     if ( cut_plotter != 0 ) {
       qtcut = new QtCut ( cut_plotter );
     }
  }

  return qtcut;
}

void PyCanvas::selectAllDisplays ( bool flag )
{
  check();
   PyApp::lock();
   m_canvas -> setAllSelected ( flag );
   PyApp::unlock ();
}

void PyCanvas::selectDisplay ( QtDisplay * display ) {
  check();
   PyApp::lock();
   QtView * selectedView = findSelectedView ( display );
   if ( selectedView ) {
      m_canvas->setSelected( selectedView );
   }
   PyApp::unlock ();
}

void
PyCanvas::
print ( const std::string & filename )
{
  check();
  PyApp::lock();
  m_canvas -> print ( filename );
  PyApp::unlock ();
}

/** @todo The exception should be thrown from the CanvasView which
    knows about required suffixes.
*/
void PyCanvas::saveAsImage( QtDisplay * display, const std::string &filename )
{
//   check();
   // Ensure that a suffix is provided...
   std::string::size_type i = filename.find_last_of( '.' );
   if ( i == std::string::npos ) {
      const std::string 
         what ( "PyCanvas::saveAsImage: filename suffix missing." );
      PyApp::unlock ();
      throw runtime_error( what );
   }

   QtView * selectedView = findSelectedView( display );
   if ( selectedView ) {
      std::string file = filename;
      PlotterBase * plotter = selectedView->getPlotter();
      m_canvas_proxy -> saveAsImage ( plotter, filename );
   }
}

void PyCanvas::saveSelectedImages( const std::string &filename )
{
  check();
   PyApp::lock();
   // Ensure that a suffix is provided...
   std::string::size_type i = filename.find_last_of( '.' );
   if ( i == std::string::npos ) {
      const std::string 
         what ( "PyCanvas::saveSelectedImages: filename suffix missing." );
      PyApp::unlock ();
      throw runtime_error( what );
   }
   m_canvas->fileSaveSelectedImages ( filename );
   PyApp::unlock ();
}

QtView * PyCanvas::findSelectedView ( QtDisplay * display )
{
  check();
   PlotterBase * myPlotter = display->display();

   return m_canvas -> getViewFor ( myPlotter );
}   

void PyCanvas::removeDisplay ( QtDisplay * display )
{
  check();
   PlotterBase * plotter = display->display();
   m_canvas->removeDisplay ( plotter );
}

void
PyCanvas::
addTextRep ( QtDisplay * display, const std::string & type )
{
  check();
  PyApp::lock();

  try {
    PlotterBase * plotter = display -> display ();
    if ( type == "Function Parameters" ||
	 type == "Chi-squared" ) {
      FunctionController * controller = FunctionController::instance ();
      if ( controller -> hasFunction ( plotter, 0 ) ) {
	m_canvas -> addFuncDisplay ( plotter, type );
      }
    }
    else {
      plotter -> setActivePlot ( 0, false );
      const std::string null ("");
      m_canvas -> addTextDisplay ( plotter, type, null );
      plotter -> setActivePlot ( -1, true );
    }

  }
  catch ( const FactoryException & e ) {
    PyApp::unlock ();
    throw e;
  }

  PyApp::unlock ();
}

const std::vector < std::string > &
PyCanvas::getTextRepTypes () const
{
  check();
  DisplayController * controller = DisplayController:: instance ();

  return controller -> getTextTypes ();
}

void PyCanvas::addText( QtDisplay * display, 
                        const std::string &text )
{
  check();
   PyApp::lock();

   PlotterBase * plotter = display->display();

   // Only one DataRep can be active in order to add a "Text From Box"
   // textrep.
   plotter->setActivePlot(0, false);

   m_canvas->addTextDisplay( plotter, "Text From Box", text );

   // Reset all plotters active.
   plotter->setActivePlot(-1, true);

   PyApp::unlock ();
}

void PyCanvas::addTextAt ( QtDisplay * display, const std::string &text,
                           double xrel, double yrel )
{
  check();

   PyApp::lock();
   PlotterBase * plotter = display->display();

   // Only one DataRep can be active in order to add a "Text From Box"
   // textrep.
   plotter->setActivePlot(0, false);
   m_canvas->addTextDisplayAt ( plotter, "Text From Box", 
				text, xrel, yrel );

   // Reset all plotters active.
   plotter->setActivePlot(-1, true);
   plotter -> update ();
   PyApp::unlock ();
}

void PyCanvas::addTextAtAbs ( QtDisplay * display, const std::string &text,
                              double xabs, double yabs )
{
  check();

   PyApp::lock();
   PlotterBase * plotter = display->display();
   TransformBase * transform = plotter->getTransform();
   BinaryTransform * tf
          = dynamic_cast < BinaryTransform * > ( transform );
   
   tf->transform(xabs,yabs);

   QtView * view = m_canvas -> getViewFor ( plotter );
   //xabs = (view -> toViewX(xabs))/1000.;
   //yabs = (view -> toViewY(yabs))/1000.;
   //xabs = view -> userToDrawX(xabs);
   //yabs = view -> userToDrawY(yabs);
   const Range & rx = plotter -> getDataRange(Axes::X);
   const Range & ry = plotter -> getDataRange(Axes::Y);
   double xmax = rx.high();
   double xmin = rx.low();
   double ymax = ry.high();
   double ymin = ry.low();
   tf->transform(xmax,xmin);
   tf->transform(ymax,ymin);
   double xref = (xabs-xmin)/(xmax-xmin);
   double yref = 1.-(yabs-ymin)/(ymax-ymin);
   plotter->setActivePlot(0, false);
   m_canvas->addTextDisplayAt ( plotter, "Text From Box", 
				text, xref, yref );

   // Reset all plotters active.
   plotter->setActivePlot(-1, true);
   plotter -> update ();
   PyApp::unlock ();
}

void
PyCanvas::
setPlotMatrix ( unsigned int columns, unsigned int rows )
{
  PyApp::lock();
  check();

  m_canvas -> setPlotMatrix ( columns, rows );
  PyApp::unlock ();
}

void
PyCanvas::
swapOrientation ()
{
  check ();
  m_canvas_proxy -> swapOrientation ();
}

/** @bug Even thou clear() passes, it is possible that the window has
    already been closed, like when the application has terminated.
 */
void
PyCanvas::
clear ()
{
  check();

  m_canvas_proxy -> clear ();
}

double
PyCanvas::
getHeight ( QtDisplay * display ) const
{
  check();

  double height = 0;
  const PlotterBase * plotter = display->display();
  const QtView * view = m_canvas->getViewFor(plotter);
  if ( view != NULL ) 
    height = view->rect().height();

  return height;
}

double
PyCanvas::
getWidth ( QtDisplay * display ) const
{
  check();

  double width = 0;
  const PlotterBase * plotter = display->display();
  const QtView * view = m_canvas->getViewFor(plotter);
  if ( view != NULL ) 
    width = view->rect().width();

  return width;
}

void
PyCanvas::
setHeight ( QtDisplay * display, double h )
{
  check();
  PyApp::lock();
  const PlotterBase * plotter = display -> display ();
  QtView * view = m_canvas -> getViewFor ( plotter );
  if ( view != 0 ) {
    Rect rect = view -> getDrawRect ();
    view -> setDrawRect ( rect.getX(), rect.getY(),
			  rect.getWidth(), h );
  }
  PyApp::unlock ();
}

void
PyCanvas::
setWidth ( QtDisplay * display, double w )
{
  check();
  PyApp::lock();
  const PlotterBase * plotter = display -> display ();
  QtView * view = m_canvas -> getViewFor ( plotter );
  if ( view != 0 ) {
    Rect rect = view -> getDrawRect ();
    view -> setDrawRect ( rect.getX(), rect.getY(),
			  w, rect.getHeight () );

  }
  PyApp::unlock ();
}

double
PyCanvas::
getX ( QtDisplay * display ) const
{
  check();
  double x = 0;
  const PlotterBase * plotter = display -> display ();
  QtView * view = m_canvas -> getViewFor ( plotter );
  if (view != NULL)
    x = view->x();
  return x;
}

double
PyCanvas::
getY ( QtDisplay * display ) const
{
  check();
  double y = 0;
  const PlotterBase * plotter = display -> display ();
  QtView * view = m_canvas -> getViewFor ( plotter );
  if (view != NULL)
    y = view->y();
  return y;
}

void
PyCanvas::
setX ( QtDisplay * display, double value )
{
  check();
  PyApp::lock();
  const PlotterBase * plotter = display -> display ();
  QtView * view = m_canvas -> getViewFor ( plotter );
  if ( view != NULL )
    view->moveBy(value - view->x(),0.);

  PyApp::unlock ();
}

void
PyCanvas::
setY ( QtDisplay * display, double value )
{
  check();
  PyApp::lock();
  const PlotterBase * plotter = display -> display ();
  QtView * view = m_canvas -> getViewFor ( plotter );
  if ( view != NULL )
    view->moveBy(0.,value - view->y());

  PyApp::unlock ();
}

NTuple *
PyCanvas::
getSelPickTable ()
{
  check();
  PyApp::lock();
  NTuple * nt = m_canvas->getPickTable();
  PyApp::unlock();
  return nt;
}

NTuple *
PyCanvas::
getPickTable ( QtDisplay * display )
{
  check();
  PyApp::lock();
  const PlotterBase * plotter = display->display ();
  NTuple * nt = m_canvas->getPickTable( plotter );
  PyApp::unlock();

  return nt;
}
