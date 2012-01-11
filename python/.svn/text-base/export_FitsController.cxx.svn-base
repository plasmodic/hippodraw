/** @file

Python extension module for PyFitsController class

Copyright (C) 2005, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_FitsController.cxx,v 1.4 2006/09/28 23:24:28 pfkeb Exp $

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

#include "PyFitsController.h"
#include "PyDataSource.h"

#include "fits/FitsNTuple.h"

using namespace boost::python;

namespace hippodraw {
namespace Python {

void 
export_FitsController()
{
  class_ < PyFitsController, bases<>,
    PyFitsController, boost::noncopyable >
    ( "FitsController",
      "A class for creation of DataSource objects from a FITS file.",
      no_init )

    .def ( "instance",  &PyFitsController::instance,
	   return_value_policy < reference_existing_object >  (),
	   "instance () -> FitsController\n"
	   "\n"
	   "Returns the single instance of the controller." )

    .staticmethod( "instance" )

    .def ( "getNTupleNames",
	   &PyFitsController::getNTupleNames,
	   return_value_policy < copy_const_reference > (),
	   "getNTupleNames ( string ) -> sequence\n"
	   "\n"
	   "Returns the names of the HDU data source objects in the file." )

    .def ( "createNTuple",
	   &PyFitsController::createNTuple,
	   return_value_policy < manage_new_object >  (),
	   "createNTuple ( string, string ) -> FitsNTuple\n"
	   "\n"
	   "Creates FitsNTuple from the named file with key name.  Python \n"
	   "take possession of it." )

    .def ( "createDataArray",
	   &PyFitsController::createDataArray,
	   return_value_policy < reference_existing_object >  (),
	   "createDataArray ( string, string ) -> DataArray\n"
	   "\n"
	   "Creates DataArray from a file with key name" )

    .def ( "writeToFile",
	   ( void ( PyFitsController::* ) // function pointer cast
	     ( const PyDataSource *, // function signature
	       const std::string & ) )
	   &PyFitsController::writeToFile,
	   "writeToFile ( DataArray, filename ) -> None.\n"
	   "writeToFile ( DataSource, filename ) -> None,\n"
	   "writeToFile ( DataArray, filename, tuple, tuple ) -> None.\n"
	   "writeToFile ( DataSource, filename, tuple, tuple ) -> None,\n"
	   "\n"
	   "Writes a DataSource or DataArray to FITS file as binary table.\n"
	   "In the latter two forms, the first tuple is list of Cuts and\n"
	   "and the second is tuple of column labels" )

    .def ( "writeToFile",
	   ( void ( PyFitsController::* ) // function pointer cast
	     ( const DataSource *, // function signature
	       const std::string & ) )
	   &PyFitsController::writeToFile )

    .def ( "writeToFile",
	   ( void ( PyFitsController::* ) // function pointer
	     ( const DataSource *, // function signature
	       const std::string &,
	       const std::vector < QtCut * > &,
	       const std::vector < std::string > & ) )
	     &PyFitsController::writeToFile )

    .def ( "writeToFile",
	   ( void ( PyFitsController::* ) // function pointer
	     ( const PyDataSource *, // function signature
	       const std::string &,
	       const std::vector < QtCut * > &,
	       const std::vector < std::string > & ) )
	     &PyFitsController::writeToFile )
    ;

}

} // namespace Python
} // namespace hippodraw
