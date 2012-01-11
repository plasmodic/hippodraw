/** @file

Python extension module for hippodraw::QtRootController class.

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_CutController.cxx,v 1.1 2007/06/07 18:16:05 pfkeb Exp $

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

#include "controllers/CutController.h"
#include "datasrcs/DataSource.h"

using std::vector;
using namespace boost::python;

namespace hippodraw {
namespace Python {

void 
export_CutController()
{
  class_ < CutController, bases<>,
    CutController, boost::noncopyable >
    ( "CutController", 
      "The CutController class provides interface to Cut functions.",
      no_init )

    .def ( "instance",  &CutController::instance,
	   return_value_policy < reference_existing_object >  (),
	   "instance () -> CutController\n"
	   "\n"
	   "Returns the singleton instance of the controller." )

    .staticmethod( "instance" )

    .def ( "createIdCut",
	   &CutController::createIdCut,
	   "createIdCut (source, dest ) -> None\n"
	   "\n"
	   "Creates a column in dest that can be used as Cut on it, \n"
	   "for values in source" )
    ;
}

} // namespace Python
} // namespace hippodraw
