/** @file

hippodraw::PyDataRep class implementation.
 
Copyright (C) 2002-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: PyDataRep.cxx,v 1.82 2007/07/20 19:29:56 pfkeb Exp $

*/

// for dll interface warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

// include first to avoid _POSIX_C_SOURCE warning.
#include <boost/python.hpp>

#include "PyDataRep.h"

#include "PyApp.h"
#include "PyDataSource.h"
#include "QtCut.h"

#include "controllers/DisplayController.h"
#include "controllers/DataRepController.h"
#include "controllers/CutController.h"
#include "projectors/ProjectorBase.h"
#include "reps/RepBase.h"
#include "datareps/DyHistogram.h"
#include "datasrcs/NTuple.h"
#include "plotters/Cut1DPlotter.h"

#include <stdexcept>
#include <sstream>

using std::runtime_error;
using std::string;

using namespace boost::python;

namespace hippodraw {
namespace Python {

void
export_DataRep()
{
  class_ < PyDataRep >
    ( "DataRep",
      "A base class for representing data in various ways.   For example, a\n"
      "a histogram is a way to represent a column of data." )

     .def ( init < const std::string &,
            const DataSource *,
            const std::vector< std::string > & >
	    (
	     "DataRep ( string ) -> DataRep\n"
	     "DataRep ( string, DataSource, sequence ) -> DataRep\n"
	     "DataRep ( DataRep ) -> DataRep\n"
	     "\n"
	     "Constructors for DataRep objects.   The first form creates a\n"
	     "static version.   The second form creates a dynamic version\n"
	     "bound to a DataSource with bindings taken from the strings in\n"
	     "the sequence.  The final version makes a copy of existing\n"
	     "DataRep.\n\n"
	     "For the names of the types of DataRep classes available\n"
	     "see names().") )

     .def ( init < const std::string &,
            const PyDataSource *,
            const std::vector< std::string > & > () )

     .def ( init < PyDataRep * > () )

     .def ( "names", &PyDataRep::names,
	    return_value_policy < copy_const_reference > (),
	    "names () -> tuple\n"
	    "\n"
	    "Returns a tuple containing the names of the types of DataRep\n"
	    "that are available." )

    .staticmethod ( "names" )

     .def ( "setPointRep", &PyDataRep::setPointRep,
	    "setPointRep ( RepBase ) -> None\n"
	    "\n"
	    "Sets the point representation." )

     .def ( "setAxisBinding", &PyDataRep::setAxisBinding,
	    "setAxisBinding ( string, string ) -> None\n"
	    "\n"
	    "Sets an axis binding.  The first argument is the axis and the\n"
	    "second is a label of a column in the DataSource." )

     .def ( "setAxisBindings", &PyDataRep::setAxisBindings,
	    "setAxisBindings ( string, sequence ) -> None\n"
	    "\n"
	    "Sets all the axes bindings.  The first argument is the axis and\n"
	    "the sequence contains labels of columns in the DataSource." )

     .def ( "setWeight", &PyDataRep::setWeight,
	    "setWeight ( string ) -> None\n"
	    "\n"
	    "Sets the weight option if DataRep is a type of histogram.\n"
	    "The string is a label of a column in the DataSource." )

     .def ( "name", &PyDataRep::name,
            return_value_policy < copy_const_reference > (),
	    "name ( ) -> string\n"
	    "\n"
	    "Returns the type of DataRep." )

     .def ( "getBinWidth", &PyDataRep::getBinWidth,
	    "getBinWidth ( string ) -> value\n"
	    ""
	    "Returns the bin width on specified axis. Only meaningful if\n"
	    "object is histogram type." )

     .def ( "getMean", &PyDataRep::getMean,
	    "getMean ( string ) -> value\n"
	    "\n"
	   "Returns the mean of the data along specified axis"  )

     .def ( "getRMS", &PyDataRep::getRMS,
	    "getRMS ( string ) -> value\n"
	    "\n"
	    "Returns root mean squared of data along specified axis" )
   
     .def ( "numberOfEntries", &PyDataRep::numberOfEntries,
	    "numberOfEntries ( ) -> value\n"
	    "\n"
	    "Returns the number of entries." )

     .def ( "applyCut", &PyDataRep::applyCut,
	    "applyCut ( Cut ) -> None\n"
	    "\n"
	    "Applies a Cut to limit the rows of the DataSource used by\n"
	    "the Display." )

     .def ( "applyCuts", &PyDataRep::applyCuts,
	    "applyCuts ( sequence ) -> None\n"
	    "\n"
	    "Applies a sequence of Cut object to limit the rows of\n"
	    "the DataSource used by the Display." )

     .def ( "colorNames", &PyDataRep::colorNames,
	    return_value_policy < copy_const_reference > (),
	    "colorNames () -> tuple\n"
	    "\n"
	    "Returns a list of available color names." )

    .def ( "set", 
	   ( void ( PyDataRep:: * ) // function pointer cast
	     ( Color::Value ) )     // function signature
	   &PyDataRep::set,         // member function
	   "set ( ColorValue ) -> None\n"
	   "set ( Line ) -> None\n"
	   "set ( Symbol ) -> None\n"
	   "\n"
	   "Sets the color, line style or Symbol of the RepBase object.\n"
	   "Use ColorValue.values, Line.values, or Symbol.values to see\n"
	   "available values." )

    .def ( "set", 
	   ( void ( PyDataRep:: * ) // function pointer cast
	     ( Line::Style ) )      // signature
	   &PyDataRep::set )     // member function

    .def ( "set",
	   ( void ( PyDataRep:: * ) // function pointer cast
	     ( Symbol::Type ) )     // signature
	   &PyDataRep::set )     // member function

     .def ( "setColor", &PyDataRep::setColor,
	    "setColor ( string ) -> None\n"
	    "\n"
	    "Sets the color of the point representation.\n"
	    "See DataRep.colorNames() for list of available colors." )

     .def ( "setErrorDisplay", &PyDataRep::setErrorDisplay,
	    "setErrorDisplay ( string, boolean ) -> None\n"
	    "\n"
	    "Turns on or off display of error bars, if available, on\n"
	    "specified axis." )

     .def ( "createNTuple", &PyDataRep::createNTuple,
            return_value_policy < manage_new_object > (),
	    "createNTuple () -> NTuple\n"
	    "\n"
	    "Creates and returns an NTuple representation of the object." )

     .def ( "getNTupleWithCuts", &PyDataRep::getNTupleWithCuts,
            return_value_policy < manage_new_object > (),
	    "getNTupleWithCuts () -> NTuple\n"
	    "\n"
	    "Creates and returns to NTuple being used, with cuts applied." )

     .def ( "getColumnWithCuts", 
            ( const std::vector<double> & (PyDataRep:: *)
              ( const std::string & ) const )
            &PyDataRep::getColumnWithCuts,
            return_value_policy < copy_const_reference > (),
	    "getColumnWithCuts ( column ) -> tuple\n"
	    "\n"
	    "Creates and returns the named column, with cuts applied." )

    .def ( "createNTupleUnderRegion", &PyDataRep::createNTupleUnderRegion,
	   return_value_policy < manage_new_object > (),
	   "createNTupleUnderRegion () -> NTuple\n"
	   "\n"
	   "Creates and returns a NTuple contain only rows that pass the\n"
	   "region cuts." )

    .def ( "addCut", &PyDataRep::addCut,
	   "addCut ( ) -> None\n"
	   "\n"
	   "Adds a cut to limit the region given to a fitter." )

    .def ( "setCutRange", &PyDataRep::setCutRange,
	   "setCutRange ( low, high ) -> None\n"
	   "\n"
	   "Sets the low and high end of the region highlight" )

    .def ( "setSize", & PyDataRep::setSize,
	   "setSize ( value ) -> None\n"
	   "\n"
	   "Sets the size of the point representation." )

    .def ( "setSymbol", &PyDataRep::setSymbol,
	   "setSymbol ( string, value ) -> None\n"
	   "\n"
	   "Sets the point symbol and size. This method is deprecated, use\n"
	   "DataRep.set(Symbol) and DataRep.setSize() instead." )

    .def ( "setLineStyle", &PyDataRep::setLineStyle,
	   "setLineStyle ( string ) -> None\n"
	   "\n"
	   "Sets the line style.\n"
	   "This method is deprecated, use DataRep.set(Line) instead." )

    .def ( "normalizeTo", &PyDataRep::normalizeTo,
	   "normalizeTo ( DataRep ) -> None\n"
	   "\n"
	   "Sets the object to normalize itself to a target one" )

    .def ( "setBinWidth", &PyDataRep::setBinWidth,
	   "setBinWidth ( string, value ) -> None\n"
	   "\n"
	   "Sets the width of the bins, if data representation is binned.\n" )
    ;
}

} // namespace Python
} // namespace hippodraw

using namespace hippodraw;

std::map< std::string, hippodraw::Symbol::Type > PyDataRep::s_symbols;
std::map< std::string, hippodraw::Line::Style > PyDataRep::s_lineStyles;
bool PyDataRep::s_have_static_members(false);

PyDataRep::PyDataRep ( DataRep * rep ) 
{
   m_datarep = rep;
   init();
}

PyDataRep::PyDataRep ( const std::string & type,
                       const DataSource * ntuple,
                       const std::vector< std::string > & bindings )
{
   DataRepController * controller = DataRepController::instance ();
   m_datarep = controller->createDataRep ( type, ntuple, bindings );
   init();
}

PyDataRep::PyDataRep ( const std::string & type,
                       const PyDataSource * nt,
                       const std::vector< std::string > & bindings )
{
   DataRepController * controller = DataRepController::instance ();
   m_datarep = controller->createDataRep ( type, &(nt->dataSource()),
                                           bindings );
   init();
}

PyDataRep::PyDataRep ()
{
  m_datarep = 0;
  init();
}

PyDataRep::PyDataRep( PyDataRep * pyRep )
{
   m_datarep = pyRep->getDataRep()->clone();
}   

const vector < string > &
PyDataRep::
names ()
{
   DataRepController * controller = DataRepController::instance ();

   return controller -> names ();
}

DataRep * PyDataRep::getDataRep()
{
  return m_datarep;
}

/** @bug @@@@ A particular DataRep can only accept a subset of all
    RepBase objects.  Thus, this class should return the names of that
    subset which is already available to the Inspector and this
    argument should be replaced by a string.
 */
void PyDataRep::setPointRep ( RepBase * pointRep )
{
   PyApp::lock();
   m_datarep->setPointRep ( pointRep );
   PyApp::unlock ();
}

void PyDataRep::setAxisBinding ( const std::string & axis,
                                 const std::string & label )
{
   PyApp::lock();
   try {
     m_datarep->setAxisBinding ( axis, label );
   }
   catch ( runtime_error & e ) {
     PyApp::unlock ();
     throw e;
   }
   PyApp::unlock ();
}

void PyDataRep::setAxisBindings ( const std::vector< std::string > & bindings)
{
   PyApp::lock();
   try {
     m_datarep->setAxisBindings ( bindings );
   }
   catch ( runtime_error & e ) {
     PyApp::unlock ();
     throw e;
   }
   PyApp::unlock ();
}

void PyDataRep::setWeight ( const std::string &label )
{
   PyApp::lock();
   if ( m_datarep->name() == "Histogram" 
        || m_datarep->name() == "Color Plot"
        || m_datarep->name() == "Contour Plot") {
      m_datarep->setAxisBinding( std::string("Weight (optional)"),
                                 label );
   } else {
      // do nothing
   }
   PyApp::unlock ();
}

const std::string & PyDataRep::name () const 
{
   return m_datarep->name();
}

double PyDataRep::getBinWidth ( const std::string &axis )
{
   PyApp::lock();
   Axes::Type at = Axes::convert ( axis );
   ProjectorBase * projector = m_datarep->getProjector ();
   PyApp::unlock ();

   return projector->getBinWidth ( at );
}

double
PyDataRep::
getMean ( const std::string & axis )
{
  PyApp::lock();
  double mean = m_datarep -> getMean ( axis );
  PyApp::unlock ();

  return mean;
}

double
PyDataRep::
getRMS ( const std::string & axis )
{
  PyApp::lock();
  double rms = m_datarep -> getRMS ( axis );
  PyApp::unlock ();

  return rms;
}

double PyDataRep::numberOfEntries() const 
{
   ProjectorBase * projector = m_datarep->getProjector();
   return projector -> getNumberOfEntries();
}

void PyDataRep::applyCut ( QtCut * cut )
{
   PyApp::lock();
   CutController * controller = CutController::instance();
   CutPlotter * cp = dynamic_cast < CutPlotter * > ( cut -> display() );
   controller -> linkCutAndRep( cp, m_datarep );
   PyApp::unlock ();
}

void PyDataRep::applyCuts ( const std::vector < QtCut * > & cuts )
{
   PyApp::lock();
   CutController * controller = CutController::instance();
   unsigned int size = cuts.size();
   for ( unsigned int i = 0; i < size; i++ ) {
     CutPlotter * cp = dynamic_cast < CutPlotter * > ( cuts[i] -> display() );
     controller -> linkCutAndRep( cp, m_datarep );
   }

   PyApp::unlock ();
}

const vector < string > &
PyDataRep::
colorNames ()
{
  return Color::colorNames ();
}

void 
PyDataRep::
set ( Color::Value value )
{
  PyApp::lock();
  const RepBase * rep = m_datarep -> getRepresentation ();
  if ( rep -> uses ( value ) ) {
    m_datarep -> set ( value );
  }
  else {
    PyApp::unlock ();
    const string what ( "This DataRep does not use ColorValue." );
    throw std::runtime_error ( what );
  }
  PyApp::unlock ();
}

void
PyDataRep::
setColor ( const std::string & color )
{
   PyApp::lock();

   if ( Color::isValid ( color ) ) {
     Color colorObj ( color );
      m_datarep -> setRepColor ( colorObj );
   } else {
     PyApp::unlock ();
      std::string what ( "DataRep.setColor: color" );
      what += " `";
      what += color;
      what += "' not available.";
      throw runtime_error( what );
   }
   PyApp::unlock ();
}

void
PyDataRep::
setErrorDisplay ( const std::string &axis, bool flag )
{
   PyApp::lock();

   Axes::Type at = Axes::convert ( axis );   
   m_datarep->setErrorDisplay( at, flag );

   PyApp::unlock ();
}

const NTuple * 
PyDataRep::
createNTuple () const
{
   PyApp::lock();

   const NTuple * ntuple = m_datarep -> createNTuple ();

   PyApp::unlock ();

   return ntuple;
}

NTuple * PyDataRep::getNTupleWithCuts () const {
   PyApp::lock();
   NTuple * ntuple = m_datarep->getNTupleAfterCuts();
   PyApp::unlock ();

   return ntuple;
}

const std::vector<double> & 
PyDataRep::
getColumnWithCuts( const std::string & column ) {
   PyApp::unlock ();

   m_columnData.clear();
   m_datarep->fillColumnAfterCuts(column, m_columnData);

   PyApp::unlock ();

   return m_columnData;
}

NTuple *
PyDataRep::
createNTupleUnderRegion () const
{
  PyApp::lock ();

  NTuple * nt = m_datarep -> createNTupleUnderRegion ();

  PyApp::unlock ();

  return nt;
}

void
PyDataRep::
addCut ( )
{
  PyApp::lock();
  m_datarep -> addCut ( );
  PyApp::unlock ();
}

void
PyDataRep::
setCutRange ( double low, double high )
{
  PyApp::lock();

  const Range range ( low, high );
  m_datarep ->  setCutRangeAt ( range, 0 );

  PyApp::unlock ();
}

void PyDataRep::makeSymbolMap() {
   s_symbols["square"] = hippodraw::Symbol::SQUARE;
   s_symbols["filled_square"] = hippodraw::Symbol::SOLIDSQUARE;
   s_symbols["plus"] = hippodraw::Symbol::PLUS;
   s_symbols["times"] = hippodraw::Symbol::TIMES;
   s_symbols["triangle"] = hippodraw::Symbol::TRIANGLE;
   s_symbols["filled_triangle"] = hippodraw::Symbol::FILLED_TRIANGLE;
   s_symbols["circle"] = hippodraw::Symbol::CIRCLE;
   s_symbols["filled_circle"] = hippodraw::Symbol::FILLED_CIRCLE;
}

void
PyDataRep::
set ( Symbol::Type type )
{
  PyApp::lock();
  const RepBase * rep = m_datarep -> getRepresentation ();
  if ( rep -> uses ( type ) ) {
    m_datarep -> setRepStyle ( type );
  }
  else {
    PyApp::unlock ();
    const string what ( "This DataRep does not use Symbol type." );
    throw std::runtime_error ( what );
  }
  PyApp::unlock ();
}

void
PyDataRep::
setSize ( double size )
{
  PyApp::lock();
  m_datarep -> setRepSize ( size );
  PyApp::unlock ();
}

void PyDataRep::setSymbol( const std::string &symbolName, float size) {
   PyApp::lock();
   if ( s_symbols.count( symbolName ) ) {
      m_datarep -> setRepStyle ( s_symbols[symbolName] );
      m_datarep -> setRepSize ( size );
   } else {
     PyApp::unlock ();
      std::ostringstream what;
      what << "PyDataRep::setSymbol: symbol " 
           << symbolName << " is not available.\n"
           << "Valid symbol names:\n";
      std::map< std::string, hippodraw::Symbol::Type >
         ::const_iterator it = s_symbols.begin();
      for ( ; it != s_symbols.end(); it++) {
         what << "  " << it->first << "\n";
      }
      throw runtime_error( what.str() );
   }
   PyApp::unlock ();
}

void PyDataRep::makeLineStyleMap() {
   s_lineStyles["Solid"] = hippodraw::Line::Solid;
   s_lineStyles["Dash"] = hippodraw::Line::Dash;
   s_lineStyles["Dot"] = hippodraw::Line::Dot;
   s_lineStyles["DashDot"] = hippodraw::Line::DashDot;
   s_lineStyles["DashDotDot"] = hippodraw::Line::DashDotDot;
   s_lineStyles["Invisible"] = hippodraw::Line::Invisible;
}

void
PyDataRep::
set ( Line::Style style )
{
  PyApp::lock();
  const RepBase * rep = m_datarep -> getRepresentation ();
  if ( rep -> uses ( style ) ) {
    m_datarep -> setRepStyle ( style );
  }
  else {
    PyApp::unlock ();
    const string what ( "This DataRep does not use Line style." );
    throw std::runtime_error ( what );
  }

  PyApp::unlock ();
}

void
PyDataRep::
setLineStyle( const std::string &lineStyleName )
{
   PyApp::lock();
   if ( s_lineStyles.count( lineStyleName ) ) {
      m_datarep->setRepStyle( s_lineStyles[lineStyleName] );
   } else {
      PyApp::unlock ();
      std::ostringstream what;
      what << "PyDataRep::setLineStyle: lineStyle " 
           << lineStyleName << " is not available.\n"
           << "Valid lineStyle names:\n";
      std::map< std::string, hippodraw::Line::Style >
         ::const_iterator it = s_lineStyles.begin();
      for ( ; it != s_lineStyles.end(); it++) {
         what << "  " << it->first << "\n";
      }
      throw runtime_error( what.str() );
   }
   PyApp::unlock ();
}

void PyDataRep::init() {
   if (!s_have_static_members) {
      makeSymbolMap();
      makeLineStyleMap();

      s_have_static_members = true;
   }
}

void
PyDataRep::
normalizeTo ( const PyDataRep * rep )
{
  PyApp::lock();

  const DataRep * datarep = rep -> m_datarep;
  m_datarep -> normalizeTo ( datarep );

  PyApp::unlock ();
}

void
PyDataRep::
setBinWidth ( const std::string & axis, double width )
{
  PyApp::lock();

  Axes::Type type = Axes::convert ( axis );
  DisplayController * controller = DisplayController::instance ();
  controller -> setBinWidth ( m_datarep, type, width );
  PyApp::unlock ();
}
