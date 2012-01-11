/** @file

Python extension module for hippodraw::QtRootController class.

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_RootController.cxx,v 1.10 2007/03/27 18:53:06 pfkeb Exp $

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

#include "root/QtRootController.h"
#include "root/RootController.h"
#include "root/QtRootNTuple.h"

#include "python/PyDataSource.h"

using std::vector;
using namespace boost::python;

namespace hippodraw {
namespace Python {

void 
export_RootController()
{
  class_ < QtRootController, bases<>,
    QtRootController, boost::noncopyable >
    ( "RootController", 
      "The RootController class provides interface to ROOT ntuple files.",
      no_init )

    .def ( "instance",  &QtRootController::instance,
	   return_value_policy < reference_existing_object >  (),
	   "instance () -> RootController\n"
	   "\n"
	   "Returns the singleton instance of the controller." )

    .staticmethod( "instance" )

    .def ( "getNTupleNames",
	   &QtRootController::getNTupleNames,
	   return_value_policy < copy_const_reference > (),
	   "getNTupleNames ( filename ) -> list\n"
	   "\n"
	   "Returns a list of names of the possible DataSource objects in\n"
	   "the ROOT file.   That is, the top level TTree names." )

    .def ( "createNTuple",
	   &QtRootController::createNTuple,
	   return_value_policy < manage_new_object >  (),
	   "createNTuple ( filename, treename ) -> RootNTuple\n"
	   "\n"
	   "Creates a RootNTuple from the named file and named tree.  Python\n"
	   "take possession of the object.\n"
	   "Consider createDataArray() instead, if the hippo module was \n"
	   "configured with numerical array support." )

    .def ( "createDataArray",
	   &QtRootController::createDataArray,
	   return_value_policy < manage_new_object >  (),
	   "createDataArray ( filename, treename ) -> DataArray\n"
	   "\n"
	   "Creates a DataArray object containing a RootNTuple from the\n"
	   "named ROOT file and named tree. Returns a reference to it." )
    ;

}

} // namespace Python
} // namespace hippodraw
