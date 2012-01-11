/** @file

Python extension module for hippodraw::PyNTupleController class.

Copyright (C) 2003-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_NTupleController.cxx,v 1.31 2006/05/13 21:49:38 pfkeb Exp $

*/

#ifdef _MSC_VER
// nonstandard extension used 'extern' before...
# pragma warning(disable:4231)  

// needs to have dll-interface used by client
# pragma warning(disable:4251)  

// non dll-interface struct
# pragma warning(disable:4275)  

// 'int' : forcing value to bool 'true' or 'false' (performance warning)
# pragma warning(disable:4800)  
#endif

// include first to avoid _POSIX_C_SOURCE warning.
#include <boost/python.hpp>

#include "PyDataSource.h"
#include "PyNTupleController.h"

#include "datasrcs/CircularBuffer.h"

using std::vector;
using namespace boost::python;

namespace hippodraw {
namespace Python {

void 
export_NTupleController()
{
  class_ < PyNTupleController, bases<>,
    PyNTupleController, boost::noncopyable >
    ( "NTupleController",
      "A singleton object to manage NTuple objects.",
      no_init )

    .def ( "instance",  &PyNTupleController::instance,
	   return_value_policy < reference_existing_object >  (),
	   "instance () -> NTupleController\n"
	   "\n"
	   "Returns the single instance of the controller" )

    .staticmethod( "instance" )

    .def ( "createDataArray",
	   &PyNTupleController::createDataArray,
	   return_value_policy < manage_new_object > (),
	   "createDataArray ( filename ) -> DataArray\n"
	   "\n"
	   "Creates a DataArray from file." )

    .def ( "createNTuple",
	   ( NTuple * ( PyNTupleController::* ) // function point cast
	     ( ) ) // function signature
	   &PyNTupleController::createNTuple,
	   return_value_policy < manage_new_object >  (),
	   "Creates an empty NTuple" )

    .def ( "createNTuple",
	   ( DataSource * ( PyNTupleController::* ) // function point cast
	     ( const std::string & ) ) // function signature
	   &PyNTupleController::createNTuple,
	   return_value_policy < manage_new_object >  (),
	   "createNTuple () -> NTuple\n"
	   "createNTuple ( string ) -> NTuple\n"
	   "\n"
	   "Creates and registers an NTuple.   The first method creates an\n"
	   "empty NTuple object.   The second method creates one from a\n"
	   "file.  Both methods return ownership of the created object to\n"
	   "Python." )

    .def ( "createCircularBuffer",
	   ( CircularBuffer * ( PyNTupleController::* ) // function point cast
	     ( unsigned int ) ) // function signature
	   & PyNTupleController::createCircularBuffer,
	   return_value_policy < manage_new_object >  (),
	   "createCircularBuffer ( value ) -> CircularBuffer\n"
	   "\n"
	   "Creates and registers a CircularBuffer object with set number of\n"
	   "columns." )

    .def ( "registerNTuple",
	   ( std::string ( PyNTupleController:: * ) // function pointer cast
	     ( DataSource * ) ) // function signature
	   & PyNTupleController::registerNTuple,
	   "Register an NTuple to make it visible to the application" )

    .def ( "registerNTuple",
	   ( void ( PyNTupleController:: * ) // function pointer cast
	     ( const std::string &, DataSource * ) ) // function signature
	   & PyNTupleController::registerNTuple,
	   "registerNTuple ( DataSource ) -> None\n"
	   "registerNTuple ( string, DataSource ) -> None\n"
	   "\n"
	   "Register an NTuple to make it visible to the application.   The\n"
	   "first form takes the name from the DataSource object, if it has\n"
	   "one, otherwise generates a unique name.   The second form takes\n"
	   "string argument for the name." )

    .def ( "writeToFile",
	   ( int ( PyNTupleController:: * ) // function pointer cast
	     ( DataSource *, const std::string & ) ) // function signature
	   & PyNTupleController::writeNTupleToFile,
	   "writeToFile ( DataSource, string ) -> value\n"
	   "\n"
	   "Writes the NTuple to a file as ASCII text.   Returns 0 if\n"
	   "successful." )

    .def ( "findNTuple",
	   ( DataSource * ( PyNTupleController:: * ) // function pointer cast
	     ( const std::string & ) const ) // function signature
	   & PyNTupleController::findDataSource,
	   return_value_policy < reference_existing_object > (),
	   "findNTuple ( string ) -> DataSource\n"
	   "\n"
	   "Find an registered NTuple by its name and returns a reference to\n"
	   "it.  If the DataSource can not be found, an exception is thrown.\n"
	   "\n"
	   "Warning: if more than one DataSource has the same name,\n"
	   "the first one found is returned." )

     .def ( "changeName",
            ( void ( PyNTupleController:: * ) // function pointer cast
              ( DataSource *, const std::string & ) ) // function signature
            & PyNTupleController::changeName,
	    "changeName ( DataSource, string ) -> None\n"
	    "\n"
            "Change the name of an NTuple.\n"
	    "This method is deprecated, there's not need to do this" )

    ;

}

} // namespace Python
} // namespace hippodraw
