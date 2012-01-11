/** @file

hippodraw::QtDisplay class implementation.

Copyright (C) 2001-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtDisplay.cxx,v 1.204 2007/07/02 18:17:16 pfkeb Exp $

@bug Why do we have HAVE_ROOT but not HAVE_CFITSIO?
*/

#ifdef HAVE_CONFIG_H
#include "config.h" // for have_root and have_numarray
#endif

// include first to avoid _POSIX_C_SOURCE warning.
#include <boost/python.hpp>

// For truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "QtDisplay.h"

#include "ListTuple.h"
#include "PyFunctionRep.h"
#include "PyApp.h"
#include "PyDataRep.h"
#include "PyDataSource.h"
#include "PyNTuple.h"
#include "FunctionWrap.h"

#include "controllers/CutController.h"
#include "controllers/DisplayController.h"
#include "controllers/FunctionController.h"

#include "pattern/FactoryException.h"
#include "plotters/PlotterBase.h"
#include "plotters/PlotterException.h"
#include "projectors/ProjectorBase.h"

#include "datareps/DataRepException.h"
#include "datareps/DataRep.h"

#include "datasrcs/DataSourceController.h"

#include "reps/RepBase.h"
#include "reps/ContourPointRep.h"
#include "colorreps/BinToColorFactory.h"
#include "colorreps/BinToColor.h"

#ifdef HAVE_ROOT
#include "root/QtRootNTuple.h"
#endif

#include <algorithm>
#include <sstream>
#include <utility>


using std::runtime_error;
using std::string;
using std::vector;

using namespace boost::python;

namespace hippodraw {
namespace Python {

void
export_QtDisplay()
{
  class_< QtDisplay > 
    ( "Display",
      "A wrapper for the HippoDraw PlotterBase C++ class.\n "
      "  See HippoDraw's QtDisplay documentation for more details.",
      init < const std::string & >
      (
#if __GNUC__ < 3
       "Display ( string, DataSource, tuple ) -> Display\n"
       "\n"
       "Constructor for a Display."
#else // gcc 2.95.3 crashes on the following...
       "Display ( string ) -> Display\n"
       "Display ( string, DataSource, tuple ) -> Display\n"
       "Display ( string, DataArray, tuple ) - > Display\n"
       "Display ( string, tuple, tuple ) -> Display\n"
       "Display ( string, list, tuple ) -> Display\n"
       "Display ( string, RootNTuple, tuple ) -> Display\n"
       "Display ( string, RootNTuple, tuple, tuple ) -> Display\n"
       "\n"
       "This method is used to create a Display object.\n"
       "The first method is used for creating static version of\n"
       "Histogram, etc., where the string is the type of DataRep.\n"
       "The remaining methods are for dynamic versions.\n"
       "The string argument is the type.   The second argument\n"
       "is the DataSource and the third argument is tuple of\n"
       "string for the binding to the DataSource.\n"
       "For the last method, the fourth argument is a tuple\n"
       "of integers to access a RootNTuple array variable.\n\n"
       "For the names of the types of DataRep classes available\n"
       "call DataRep.names()."
#endif
	) )

    .def ( init < PyFunctionRep * > () )

    .def ( init < const std::string &,
	   const DataSource &,
	   const std::vector< std::string > & > () )

    .def ( init < const std::string &,
	   const PyDataSource &,
	   const std::vector< std::string > & > () )

    .def ( init < const std::string &,
	   boost::python::tuple,
	   const std::vector< std::string > & > () )

    .def ( init < const std::string &,
            boost::python::list,
	   const std::vector< std::string > & > () )

#ifdef HAVE_ROOT
    .def ( init < const std::string &,
            const QtRootNTuple &,
	   const std::vector< std::string > & > () )

    .def ( init < const std::string &,
	   const QtRootNTuple &,
	   const std::vector< std::string > &,
	   boost::python::list > () )
#endif

    .def ( "applyCut", &QtDisplay::applyCut,
	   "applyCut ( Cut ) -> None\n"
	   "\n"
	   "Apply a Cut to the Display" )

    .def ( "applyCuts", &QtDisplay::applyCuts,
	   "applyCuts ( sequence ) -> None\n"
	   "\n"
	   "Apply each Cut in the sequence to the Display" )

    .def ( "createNTuple", &QtDisplay::createNTuple,
	   return_value_policy < manage_new_object > (),
	   "createNTuple ( ) -> NTuple\n"
	   "\n"
	   "Returns a NTuple representation of the Display's contents." )

    .def ( "createDataArray", &QtDisplay::createDataArray,
	   return_value_policy < manage_new_object > (),
	   "createDataArray ( ) -> DataArray\n"
	   "\n"
	   "Returns a DataArray representation of the Display's contents\n"
	   "(This method available if configured with numarray)" )

    .def ( "setNumberOfBins", &QtDisplay::setNumberOfBins,
	   "setNumberOfBins ( string, value ) -> None\n"
	   "\n"
	   "Sets the number of bins on named axis, e.g. 'x' to the given \n"
	   "value." )
	  
    .def ( "setBinWidth", 
           ( void (QtDisplay::*) ( const std::string &,
                                   double, 
                                   bool ) )
            &QtDisplay::setBinWidth,
            "Set the bin width, explicitly saving the value or not." )

    .def ( "getBinWidth", 
           ( double (QtDisplay::*) ( const std::string & ) )
            &QtDisplay::getBinWidth,
	   "getBinWidth ( string ) -> value\n"
	   "\n"
	   "Returns the bin width on axis specified as a string,\n"
	   "e.g., 'x'." )

    .def ( "setBinWidth", 
           ( void (QtDisplay::*) ( const std::string &,
                                   double ) )
            &QtDisplay::setBinWidth,
	   "setBinWidth ( string, value ) -> None\n"
	   "\n"
            "Set the bin width to value  on axis specified as a string,\n"
	   "e.g., 'x'." )

    .def ( "reset", &QtDisplay::reset,
	   "reset () -> None\n"
	   "\n"
	   "Resets the contents of all bins.   Only applicable to static\n"
	   "histograms." )

    .def ( "setOffset", &QtDisplay::setOffset,
	   "setOffset ( string, value ) -> None\n"
	   "\n"
	   "Sets the offset of the bins relative to their current width on\n"
	   "specified axis." )
	  
    .def ( "setRange", 
           ( void (QtDisplay::*) (const std::string &, 
                                  double, double) )
           &QtDisplay::setRange )

    .def ( "setRange", 
           ( void (QtDisplay::*) (const std::string &, 
                                  double, double, bool) )
           &QtDisplay::setRange,
	   "setRange ( string, value, value ) -> None\n"
	   "setRange ( string, value, value, Boolean ) -> None\n"
	   "\n"
           "Set the upper and lower bounds for the specified axis.  For the\n"
	   "second form, also save them if the Boolean argument is true." )


    .def ( "getRange", &QtDisplay::getRange,
	   "getRange ( string ) -> tuple\n"
	   "\n"
	   "Returns the tuple representing the range for the axis specified\n"
	   "as a string, e.g., 'x'." )

     .def ( "saveView", &QtDisplay::saveView,
            "saveView ( ) -> int\n"
            "\n"
            "Saves the current set of x and y ranges and "
            "returns the index of the saved view." )

     .def ( "setView", &QtDisplay::setView,
            "setView ( int ) -> None\n"
            "\n"
            "Set the view by its index." )

     .def ( "nextView", &QtDisplay::nextView,
            "nextView ( bool ) -> int\n"
            "\n"
            "Cycle to the next view in the view buffer. "
            "Set the argument to True to cycle for wards, "
            "False to cycle back wards.\n"
            "Returns the index of the new view." )

     .def ( "numViews", &QtDisplay::numViews,
            "numViews ( ) -> int\n"
            "\n"
            "Return the number of stored views." )

     .def ( "deleteView", &QtDisplay::deleteView,
            "deleteView ( int ) -> None\n"
            "\n"
            "Delete a view by index." )

     .def ( "currentView", &QtDisplay::currentView,
            "currentView ( ) -> int\n"
            "\n"
            "Index of the current view." )

    .def ( "setTitle", &QtDisplay::setTitle,
	   "setTitle ( string ) -> None\n"
	   "\n"
	   "Sets the title of the display." )

    .def ( "getTitle", &QtDisplay::getTitle,
            return_value_policy < copy_const_reference > (),
	   "getTitle () -> string\n"
	   "\n"
	   "Returns the current title.   The title will be the title of\n"
	   "the DataSource unless it has been explicitly changed." )

    .def ( "setLabel", &QtDisplay::setLabel,
	   "setLabel ( string, string ) -> None\n"
	   "\n"
	   "Sets the label for the axis specified by first argument to value\n"
	   "of the second argument." )

    .def ( "getLabel", &QtDisplay::getLabel,
            return_value_policy< copy_const_reference > (),
	   "getLabel ( string ) -> string\n"
	   "\n"
            "Returns the label of the axis specified as a string,\n"
	   "e.g., 'x'." )

    .def ( "getDataRep", &QtDisplay::getDataRep,
	   return_value_policy < manage_new_object > (),
	   "getDataRep ( ) -> DataRep\n"
	   "\n"
	   "Returns a reference to the active DataRep or if all DataRep objects are\n"
	   "active, returns a reference to the first one." )

    .def ( "getDataReps", &QtDisplay::getDataReps,
            return_value_policy < copy_const_reference > (),
	   "getDataReps ( ) -> list\n"
	   "\n"
	   "Returns a list of DataRep objects contained by the Display.." )
    
    .def ( "addDataRep", 
	   ( void (QtDisplay::*) (PyDataRep *) )
	   &QtDisplay::addDataRep,
	   "addDataRep ( DataRep ) -> Display\n"
	   "addDataRep ( Function ) -> Display\n"
	   "addDataRep ( string, DataSource, tuple ) -> Display\n"
	   "\n"
	   "Adds a DataRep to the display sharing the same Y axis range\n"
	   "The first two methods adds existing DataRep or Function to the\n"
	   "Display.  The third method creates and adds DataRep to the\n"
	   "Display.  Arguments are same as Display constructor." )

    .def ( "addDataRep", 
	   ( void (QtDisplay::*) (const std::string &, 
				  const DataSource *,
				  const std::vector <std::string > &) )
	   &QtDisplay::addDataRep )

    .def ( "addDataRep", 
	   ( void (QtDisplay::*) (PyFunctionRep *) )
	   &QtDisplay::addDataRep )

    .def ( "addDataRepStacked", 
	   ( void (QtDisplay::*) (const std::string &, 
				  const DataSource *,
				  const std::vector <std::string > &) )
	   &QtDisplay::addDataRepStacked,
	   "addDataRepStacked ( string, DataSource, tuple ) -> Display\n"
	   "\n"
	   "Creates and adds a DataRep with independent Y axis ranges.\n"
	   "The arguments are the same as Display constructor." ) 

#ifndef BOOST_DEFECT
    .def ( "addFunction", 
	   ( void (QtDisplay::*) (FunctionBase *) )
	   &QtDisplay::addFunction,
	   "addFunction ( FunctionBase ) -> None\n"
	   "\n"
	   "Adds a FunctionBase object to the display by appropriately\n"
	   "wrapping it with a Function." )
#endif // BOOST_DEFECT

    .def ( "setAutoRanging",
            ( void (QtDisplay::*) (const std::string &,
                                   bool flag ) )
            &QtDisplay::setAutoRanging,
	   "setAutoRanging ( string, Boolean ) -> None\n"
	   "\n"
	   "Sets auto-ranging on axis specified as a string, e.g. 'x',n"
	   "on or off." )

    .def ( "setLog", &QtDisplay::setLog,
	   "setLog ( string, Boolean ) -> None\n"
	   "\n"
	   "Sets the axis specified by the first argument on log scale." )

    .def ( "getLog", &QtDisplay::getLog,
	   "getLog ( string ) -> value\n"
	   "\n"
	   "Returns True if axis specified as a string, e.g. 'x', is being\n"
	   "displayed on a logarithmic scale." )

    .def ( "setTransform", &QtDisplay::setTransform,
	   "setTransform ( string ) -> None\n"
	   "\n"
	   "Sets the transform object." )

    .def ( "addValues", &QtDisplay::addValues,
	   "addValue ( tuple ) -> None\n"
	   "\n"
	   "For static histograms, adds a value to the accumulation.\n"
	   "For 1D histogram, the tuple should contain one or two values,\n"
	   "the second used as a weight.  For 2D histogram, the tuple should\n"
	   "contain two or three elements, the third being the weight.\n"
	   "non static Displays do nothing." )

    .def ( "setPointRep", &QtDisplay::setPointRep,
	   "setPointRep ( RepBase ) -> None\n"
	   "\n"
	   "Sets the point representation to be used." )

    .def ( "setContourLevels", &QtDisplay::setContourLevels,
	   "setContourLevels ( sequence ) -> None\n"
	   "\n"
	   "Sets the contour levels if the Display is using contour point\n"
	   "representation from the values in the sequence." )

    .def ( "setAspectRatio", &QtDisplay::setAspectRatio,
	   "setAspectRatio ( value ) -> None\n"
	   "\n"
	   "Sets the required aspect ratio of the Display to value, the\n"
	   "ratio of the width to the height." )

     .def ( "numberOfEntries", &QtDisplay::numberOfEntries,
	    "numberOfEntries ( ) -> value\n"
	    "\n"
	    "Returns the number of entries in the Display." )

     .def ( "resize", &QtDisplay::resize,
	    "resize () -> None\n"
	    "\n"
	    "Resizes the Display to its saved values." )

     .def ( "plotterId", &QtDisplay::plotterId,
	    "plotterId () -> value\n"
	    "\n"
	    "Returns a unique identifier for the Display." )

     .def ( "setColorMap", &QtDisplay::setColorMap,
	    "setColorMap ( string ) -> None\n"
	    "\n"
            "Set the value-to-color map to one named by the argument.")

    .def ( "update", &QtDisplay::update,
	   "update () -> None\n"
	   "\n"
	   "Updates the display." )

    .def ( "addObserver", &QtDisplay::addObserver,
	   "addObserver ( Observer ) -> None\n"
	   "\n"
	   "Adds an Observer to the Display object." )

    .def ( "setAutoTicks", &QtDisplay::setAutoTicks,
	   "setAutoTicks ( Boolean ) -> None\n"
	   "\n"
	   "Set the ticks generation to be automatic (the default) or\n"
	   "manually." )

    .def ( "setTicks", &QtDisplay::setTicks,
	   "setTicks ( string, sequence, sequence ) -> None\n"
	   "\n"
	   "Sets the tick locations and labels.   The first argument is the\n"
	   " axis, the second argument is a sequence containing the\n"
	   "locations, and the third argument is a sequence of tick labels." )

    .def ( "unlock", &QtDisplay::unlock,
	   "unlock () -> None\n"
	   "\n"
	   "Unlock the application thread." )

    ;
}

} // namespace Python
} // namespace hippodraw

using namespace hippodraw;

/** @request Expose to Python the names of the available displays.
 */
void QtDisplay::createDisplay ( const std::string & type, 
				const DataSource & nt, 
				const std::vector < std::string > & bindings )
{
  PyApp::lock();
  DisplayController * controller = DisplayController::instance ();
  try {
    m_plotter = controller->createDisplay ( type, nt, bindings );
    PyApp::unlock ();
  }
  catch ( const FactoryException & e ) {
    PyApp::unlock ();
    throw e;
  }
  catch ( const DataRepException & e ) {
    PyApp::unlock ();
    throw e;
  }
  catch ( const runtime_error & e ) {
    PyApp::unlock ();
    throw e;
  }
}

QtDisplay::
QtDisplay ( const std::string & type,
	    boost::python::tuple seq,
	    const std::vector < std::string > & labels )
{
  PyApp::lock();

  object obj = seq.attr ( "__len__" ) ();

  ListTuple * ntuple = new ListTuple ( );

  try {
    unsigned int size = extract < unsigned int > ( obj );

    if ( size > labels.size () ) {
      string what ( "Display: Too few labels" );
      throw runtime_error ( what );
    }
      
    for ( unsigned int i = 0, j = 0; i < size; i++, j++ ) {
      boost::python::list l = extract < boost::python::list > ( seq[i] );

      while ( labels[j] == "nil" ) {
	j++; // skip such labels
	if ( ! ( j < labels.size () ) ) {
	  string what ( "Display: Too few non 'nil' labels" );
	  throw runtime_error ( what );
	}

      }
      ntuple -> addColumn ( labels[j], l );
    }
  }
  catch ( const runtime_error & e ) {
    delete ntuple;
    PyApp::unlock ();
    throw e;
  }

  // Do not call createDisplay() as exceptions wouldn't print nicely
  try { 
    DisplayController * dc = DisplayController::instance ();
    m_plotter = dc -> createDisplay ( type, *ntuple, labels );
  }
  catch ( const FactoryException & e ) {
    delete ntuple;
    PyApp::unlock ();
    throw e;
  }
  catch ( const DataRepException & e ) {
    delete ntuple;
    PyApp::unlock ();
    throw e;
  }
  catch ( const runtime_error & e ) {
    delete ntuple;
    PyApp::unlock ();
    throw e;
  }

  // No use telling controller about it until we are sure to accept it.
  DataSourceController * dsc = DataSourceController::instance ();
  dsc -> registerNTuple ( ntuple );

  PyApp::unlock ();
}

QtDisplay::
QtDisplay ( const std::string & type,
	    boost::python::list seq,
	    const std::vector < std::string > & labels )
{
  PyApp::lock();

  object obj = seq.attr ( "__len__" ) ();

  ListTuple * ntuple = new ListTuple ( );

  try {
    unsigned int size = extract < unsigned int > ( obj );

    if ( size > labels.size () ) {
      string what ( "Display: Too few labels" );
      throw runtime_error ( what );
    }
      
    // gcc 2.95.3 needs full scoping
    for ( unsigned int i = 0, j = 0; i < size; i++, j++ ) {
      boost::python::list l = extract < boost::python::list > ( seq[i] );

      while ( labels[j] == "nil" ) {
	j++; // skip such labels
	if ( ! ( j < labels.size () ) ) {
	  string what ( "Display: Too few non 'nil' labels" );
	  throw runtime_error ( what );
	}

      }
      ntuple -> addColumn ( labels[j], l );
    }
  }
  catch ( const runtime_error & e ) {
    delete ntuple;
    PyApp::unlock ();
    throw e;
  }

  // Do not call createDisplay() as exceptions wouldn't print nicely
  try { 
    DisplayController * dc = DisplayController::instance ();
    m_plotter = dc -> createDisplay ( type, *ntuple, labels );
  }
  catch ( const FactoryException & e ) {
    delete ntuple;
    PyApp::unlock ();
    throw e;
  }
  catch ( const DataRepException & e ) {
    delete ntuple;
    PyApp::unlock ();
    throw e;
  }
  catch ( const runtime_error & e ) {
    delete ntuple;
    PyApp::unlock ();
    throw e;
  }

  // No use telling controller about it until we are sure to accept it.
  DataSourceController * dsc = DataSourceController::instance ();
  dsc -> registerNTuple ( ntuple );

  PyApp::unlock ();
}

QtDisplay::QtDisplay ()
  : m_plotter ( 0 )
{
}

QtDisplay::
QtDisplay ( PyFunctionRep * rep )
{
  PyApp::lock();
  DisplayController * controller = DisplayController::instance ();
  try {
    DataRep * dr = rep -> getRep ();
    m_plotter = controller -> createDisplay ( dr );
    PyApp::unlock ();
  }
  catch ( const FactoryException & e ) {
    PyApp::unlock ();
    throw e;
  }
  catch ( const DataRepException & e ) {
    PyApp::unlock ();
    throw e;
  }
}

QtDisplay::
QtDisplay ( const std::string & type )
{
//   PyApp::lock();
  DisplayController * controller = DisplayController::instance ();
  try {
    m_plotter = controller -> createDisplay ( type );
    PyApp::unlock ();
  }
  catch ( const FactoryException & e ) {
    PyApp::unlock ();
    throw e;
  }
  catch ( const DataRepException & e ) {
    PyApp::unlock ();
    throw e;
  }
//   PyApp::unlock ();
}


QtDisplay::QtDisplay( const std::string & type,
                      const DataSource & nt,
                      const std::vector< std::string > & bindings )
{
   createDisplay ( type, nt, bindings );
}

QtDisplay::QtDisplay( const std::string & type,
                      const PyDataSource & nt,
                      const std::vector< std::string > & bindings )
{
   createDisplay ( type, nt.dataSource(), bindings );
}

#ifdef HAVE_ROOT
QtDisplay::
QtDisplay ( const std::string & type,
	    const QtRootNTuple & nt,
	    const std::vector < std::string > & bindings )
{
  createDisplay ( type, nt, bindings );
}

QtDisplay::
QtDisplay ( const std::string & type,
	    const QtRootNTuple & nt,
	    const std::vector < std::string > & variables,
	    boost::python::list indices )
{
  object obj = indices.attr ( "__len__" ) ();
  unsigned int size = extract < unsigned int > ( obj );

  if ( size != variables.size() ) {
    const string what ( "Display: bindings and indexes not the same size." );
    PyApp::unlock ();
    throw runtime_error ( what );
  }

  vector < vector < int > > vec ( size );
  for ( unsigned int i = 0; i < size; i++ ) {
    boost::python::list l = extract < boost::python::list > ( indices[i] );
    object o = l.attr ( "__len__" ) ();
    unsigned int len = extract < unsigned int > ( o );
    for ( unsigned int j = 0; j < len; j++ ) {
      unsigned int k = extract < unsigned int > ( l[j] );
      vec[i].push_back ( k );
    }
  }

  vector < string > bindings ( size );

  for ( unsigned int i = 0; i < size; i++ ) {
    const string & label = variables [ i ];
    const vector < int > & indexes = vec [ i ];
    const string name = nt.createBinding ( label, indexes );
    bindings [ i ] = name;
  }

  createDisplay ( type, nt, bindings );
}

#endif // have_root

QtDisplay::
QtDisplay ( PlotterBase * plotter) : m_plotter(plotter) {}

QtDisplay::~QtDisplay () 
{ 
//     delete m_plotter; 
}

PlotterBase * QtDisplay::display()
{
  return m_plotter;
}

/** @request @@@@@@ Also need to have removeDataRep in interface.
*/
void QtDisplay::addDataRep ( const std::string & type, 
			     const DataSource * ntuple,
			     const std::vector < std::string > & bindings )
{
  PyApp::lock();

  DisplayController * controller = DisplayController::instance ();
  controller->addDataRep ( m_plotter, type, ntuple, bindings );

  PyApp::unlock ();
}

void
QtDisplay::
addDataRepStacked ( const std::string & type, 
		  const DataSource * ntuple,
		  const std::vector < std::string > & bindings )
{
  PyApp::lock();

  DisplayController * controller = DisplayController::instance ();
  controller->addDataRepStacked ( m_plotter, type, ntuple, bindings );

  PyApp::unlock ();
}

void QtDisplay::addDataRep ( PyDataRep * pyRep )
{
  PyApp::lock();

   DisplayController * controller = DisplayController::instance ();
   controller->addDataRep ( m_plotter, pyRep->getDataRep() );

  PyApp::unlock ();
}

void QtDisplay::addDataRep ( PyFunctionRep * pyFuncRep )
{
  PyApp::lock();

   DisplayController * controller = DisplayController::instance ();
   
   controller->addDataRep ( m_plotter, pyFuncRep->getRep() );

  PyApp::unlock ();
}

#ifndef BOOST_DEFECT
void
QtDisplay::
addFunction ( FunctionBase * function )
{
   PyApp::lock();
   FunctionController * funcController = FunctionController::instance();
   DataRep * dataRep = m_plotter->getDataRep( 0 );

// The following datarep will be deleted by the CompositePlotter.
   FunctionRep * funcRep = 
      funcController->createFunctionRep(function, dataRep);

   DisplayController * controller = DisplayController::instance();
   dataRep = reinterpret_cast<DataRep *>(funcRep);
   controller->addDataRep(m_plotter, dataRep);

   PyApp::unlock ();
}
#endif // BOOST_DEFECT

void
QtDisplay::
setRange ( const std::string & axis, double low, double high,
	   bool save)
{
  PyApp::lock();

  if (save) {
     if (axis == "x" || axis == "X") 
        m_ranges["x"] = std::make_pair(low, high);
     if (axis == "y" || axis == "Y") 
        m_ranges["y"] = std::make_pair(low, high);
  }
     
  m_plotter->setRange ( axis, low, high );

  PyApp::unlock ();
}

void
QtDisplay::
setRange ( const std::string & axis, double low, double high )
{
  PyApp::lock();
  setRange( axis, low, high, false );
  PyApp::unlock();
}

std::vector<double>
QtDisplay::
getRange ( const std::string & axis )
{
  PyApp::lock ();

  std::vector<double> myRange;
  try { 
    Axes::Type type = Axes::convert ( axis );
    const Range & axisRange = m_plotter->getRange( type, true );
    myRange.push_back(axisRange.low());
    myRange.push_back(axisRange.high());
    
    PyApp::unlock ();
  }
  catch ( const PlotterException & e ) {
    PyApp::unlock ();
    throw e;
  }

  return myRange;
}

int QtDisplay::saveView ()
{
   std::vector<double> range_values;
   std::vector<double> range = getRange("x");
   range_values.push_back(range[0]);
   range_values.push_back(range[1]);
   m_ranges["x"] = std::make_pair(range[0], range[1]);

   range = getRange("y");
   range_values.push_back(range[0]);
   range_values.push_back(range[1]);
   m_ranges["y"] = std::make_pair(range[0], range[1]);

   return m_plotter->saveView(range_values);
}

void QtDisplay::setView ( int index )
{
  PyApp::lock();
  try {
     m_plotter->setView( index );
  } catch (...) {
    PyApp::unlock ();
     throw;
  }
  PyApp::unlock ();
}

int QtDisplay::nextView( bool stepForward )
{
  int index(-1);
  PyApp::lock();
   try {
      index = m_plotter->nextView(stepForward);
   } catch (...) {
     PyApp::unlock ();
      throw;
   }
   PyApp::unlock ();
   return index;
}

int QtDisplay::numViews () {
   return m_plotter->numViews();
}

void QtDisplay::deleteView (int index) {
  PyApp::lock();
   try {
      m_plotter->deleteView(index);
   } catch (...) {
     PyApp::unlock ();
      throw;
   }
   PyApp::unlock ();
}

int QtDisplay::currentView () {
   return m_plotter->currentView();
}

void QtDisplay::setTitle ( const std::string & title )
{
  PyApp::lock();
  m_plotter->setTitle ( title );
  PyApp::unlock ();
}

const std::string & QtDisplay::getTitle() const {
   return m_plotter->getTitle();
}

void
QtDisplay::
setPointRep ( RepBase * rep )
{
  PyApp::lock();
  try {
     m_plotter->setRepresentation ( rep );
  } catch ( const std::runtime_error & e ) {
    PyApp::unlock ();
    throw e;
  }
  PyApp::unlock ();
}

void
QtDisplay::
setContourLevels ( const std::vector<double> &levels ) 
{
   PyApp::lock();
   RepBase * rep = m_plotter->representation();
   
   if ( rep->name() == std::string("Contour") ) {
      DataRep * datarep 
         = m_plotter->getDataRep ( m_plotter->activePlotIndex() );

      dynamic_cast<ContourPointRep *>(rep)
         ->setContourValues( const_cast<std::vector<double>&>(levels), 
                             datarep->getProjector() );

      datarep->notifyObservers();

   } else {
      // do nothing
   }
   PyApp::unlock ();
}

void QtDisplay::setLabel ( const std::string & axis,
			   const std::string & label )
{
  PyApp::lock();

  Axes::Type at = Axes::convert ( axis );
  m_plotter->setLabel ( at, label );

  PyApp::unlock ();
}

const std::string &
QtDisplay::
getLabel ( const std::string & axis ) const
{
  PyApp::lock ();
  Axes::Type at = Axes::convert ( axis );
  const std::string & label = m_plotter -> getLabel( at );
  PyApp::unlock ();

  return label;
}

void
QtDisplay::
setNumberOfBins ( const std::string & axis, unsigned int number )
{
  PyApp::lock();
  m_plotter->setNumberOfBins ( axis, number );
  PyApp::unlock();
}

void 
QtDisplay::
reset()
{
  PyApp::lock();
  m_plotter -> reset ();
  PyApp::unlock ();
}

void QtDisplay::setBinWidth ( const std::string & axis, double width,
                              bool save )
{
  PyApp::lock();

  if (save) {
     if (axis == "x" || axis == "X") 
        m_binWidths["x"] = width;
     if (axis == "y" || axis == "Y") 
        m_binWidths["y"] = width;
  }     

  m_plotter->setBinWidth ( axis, width );

  PyApp::unlock ();
}

double
QtDisplay::
getBinWidth ( const std::string & axis ) const
{
  return m_plotter->getBinWidth ( axis);
}

void
QtDisplay::
setBinWidth ( const std::string & axis, double width )
{
    PyApp::lock();
   setBinWidth( axis, width, false );
   PyApp::unlock ();
}

void QtDisplay::setOffset ( const std::string & axis, double offset )
{
  PyApp::lock();
  DisplayController * controller = DisplayController::instance ();
  Axes::Type type = Axes::convert ( axis );

  controller->setOffset ( m_plotter, type, offset );
  PyApp::unlock();
}

/** @request Expose to Python the list of available transforms.
 */
void QtDisplay::setTransform ( const std::string & name )
{
  PyApp::lock();

  DisplayController * controller = DisplayController::instance ();
  try {
     controller->setTransform ( m_plotter, name );
     PyApp::unlock ();
  } catch (PlotterException & eObj) {
    PyApp::unlock ();
     throw eObj;
  } catch (FactoryException & eObj) {
    PyApp::unlock ();
     throw eObj;
  }
}

void QtDisplay::unlock() {
  PyApp::unlock ();
}

void QtDisplay::setLog ( const std::string & axis, int flag )
{
  PyApp::lock();
  Axes::Type type = Axes::convert ( axis );
  DisplayController * controller = DisplayController::instance ();
  bool yes = flag != 0;

  controller->setLog ( m_plotter, type, yes );
  PyApp::unlock ();
}

int QtDisplay::getLog ( const std::string & axis )
{
  DisplayController * controller = DisplayController::instance ();
  if ( controller->getLog ( m_plotter, axis) ) {
     return 1;
  } else {
     return 0;
  }
}

void QtDisplay::setAutoRanging ( const std::string & axis, bool flag )
{
  PyApp::lock();
  Axes::Type type = Axes::convert ( axis );
  m_plotter->setAutoRanging ( type, flag );
  PyApp::unlock ();
}

PyDataRep *
QtDisplay::
getDataRep ()
{
  PyApp::lock ();

  int index = m_plotter->activePlotIndex();
  if ( index < 0 ) index = 0;
  DataRep * rep = m_plotter->getDataRep ( index );
  PyDataRep * pyrep = new PyDataRep ( rep );

  PyApp::unlock ();

  return pyrep;
}

const std::vector<PyDataRep *> &
QtDisplay::
getDataReps () const
{
  PyApp::lock ();

   m_pyDataReps.clear();
   int nReps = m_plotter->getNumDataReps();
   for (int i = 0; i < nReps; i++) {
      m_pyDataReps.push_back( new PyDataRep ( m_plotter->getDataRep( i ) ) );
   }
   PyApp::unlock ();

   return m_pyDataReps;
}

void
QtDisplay::
setAspectRatio ( double ratio )
{
  PyApp::lock();
  m_plotter->setAspectRatio ( ratio );
  PyApp::unlock ();
}

void
QtDisplay::
addValues ( const std::vector < double > & v )
{
  PyApp::lock();
  m_plotter -> addValues ( v );
  PyApp::unlock ();
}

PyNTuple *
QtDisplay::
createNTuple () const
{
  // Need to lock else could create NTuple not yet displayed
  PyApp::lock();
  FunctionController * controller = FunctionController::instance();
  const NTuple * ntuple = controller -> createNTuple ( m_plotter, 0 );

  PyNTuple * pytuple = new PyNTuple ( *ntuple );
  delete ntuple;

  PyApp::unlock ();

  return pytuple;
}

double
QtDisplay::
numberOfEntries() const
{
   ProjectorBase * projector = m_plotter->activeProjector();
   return projector->getNumberOfEntries();
}

void
QtDisplay::
resize() 
{
   PyApp::lock();

   if (m_binWidths.count("x")) {
      m_plotter->setBinWidth("x", m_binWidths["x"]);
   } else {
// set and unset log-scale to get decent bin sizes
      DisplayController * controller = DisplayController::instance();
      if ( controller->getLog( m_plotter, "x" ) ) {
         setLog( "x", 0 );
         setLog( "x", 1 );
      } else { 
         setLog( "x", 1 );
         setLog( "x", 0 );
      }
   }

   if (m_binWidths.count("y")) {
      m_plotter->setBinWidth("y", m_binWidths["y"]);
   } else {
     // set and unset log-scale to get decent bin sizes
      DisplayController * controller = DisplayController::instance();
      if ( controller->getLog( m_plotter, "y" ) ) {
         setLog( "y", 0 );
         setLog( "y", 1 );
      } else { 
         setLog( "y", 1 );
         setLog( "y", 0 );
      }
   }

   if (m_ranges.count("x")) {
      m_plotter->setRange("x", m_ranges["x"].first, m_ranges["x"].second);
   } else {
      m_plotter->setAutoRanging("x", true);
   }

   if (m_ranges.count("y")) {
      m_plotter->setRange("y", m_ranges["y"].first, m_ranges["y"].second);
   } else {
      m_plotter->setAutoRanging("y", true);
   }

   PyApp::unlock ();
}

int QtDisplay::plotterId() const
{
   return m_plotter->plotterId();
}

/** @request Expose the BinToColorFactory to Python so user can see
    the choice of names.
 */
void QtDisplay::
setColorMap ( const std::string & name ) {
   PyApp::lock();
   BinToColorFactory * factory = BinToColorFactory::instance();
   const vector< std::string > & names = factory -> names();
   if ( std::find(names.begin(), names.end(), name) != names.end() ) {
      BinToColor * rep = factory -> create ( name );
      m_plotter -> setValueRep( rep );
      PyApp::unlock ();
   } else {
     PyApp::unlock();
     std::ostringstream message;
      message << "QtDisplay::setColorMap:\n"
              << "BinToColor rep '" << name << "' does not exist.\n"
              << "Valid rep names are \n\n";
      for (unsigned int i = 0; i < names.size() ; i++) {
         message << "'" << names[i] << "'\n";
      }
      throw std::runtime_error(message.str());
   }
}

void
QtDisplay::
update ()
{
  PyApp::lock();

  m_plotter -> update ();

  PyApp::unlock ();
}

void
QtDisplay::
addObserver ( Observer * observer )
{
  m_plotter -> addObserver ( observer );
}

void
QtDisplay::
setTicks ( const std::string & axis,
	   const std::vector < double > & values,
	   const std::vector < std::string > & labels )
{
  PyApp::lock();
  m_plotter -> setTicks ( axis, values, labels );
  PyApp::unlock ();
}

void
QtDisplay::
setAutoTicks ( const std::string & axis, bool yes )
{
  PyApp::lock();
  m_plotter -> setAutoTicks ( axis, yes );
  PyApp::unlock ();
}

PyDataSource *
QtDisplay::
createDataArray () const
{
  // Need to lock else could create NTuple not yet displayed
#ifdef HAVE_NUMARRAY
  PyApp::lock();

  FunctionController * controller = FunctionController::instance();
  NTuple * ntuple = controller -> createNTuple ( m_plotter, 0 );
  PyDataSource * ds = new PyDataSource ( "NTuple", ntuple );

  PyApp::unlock ();

  return ds;
#else
   runtime_error e ( "HippoDraw was not built with numeric Python support" );
   throw e;
#endif
}

void
QtDisplay::
applyCut ( QtDisplay * cut )
{
   PyApp::lock();

   PlotterBase * cut_plotter = cut -> display();
   PlotterBase * target_plotter = display ();

   CutController * controller = CutController::instance();
   controller -> addCut ( cut_plotter, target_plotter );

   PyApp::unlock ();
}

void
QtDisplay::
applyCuts ( const std::vector < QtDisplay * > & cuts )
{
   PyApp::lock();

   vector < PlotterBase * > cut_plotters;
   unsigned int size = cuts.size ();
   for ( unsigned int i = 0; i < size; i++ ) {
     QtDisplay * cut_display = cuts[i];
     PlotterBase * plotter = cut_display -> display();
     cut_plotters.push_back ( plotter );
   }

   PlotterBase * target_plotter = display ();
   CutController * controller = CutController::instance();
   controller -> addCuts ( cut_plotters, target_plotter );

   PyApp::unlock ();
}
