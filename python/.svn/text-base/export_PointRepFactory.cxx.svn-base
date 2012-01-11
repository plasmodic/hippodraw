/** @file

Part of  Python extension module for hippodraw::PointRepFactory class.

Copyright (C) 2003, 2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_PointRepFactory.cxx,v 1.7 2006/03/31 22:33:20 pfkeb Exp $

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

#include "pattern/Factory.h"
#include "reps/PointRepFactory.h"
#include "reps/RepBase.h"

using std::vector;
using namespace boost::python;

namespace hippodraw {
namespace Python {

void
export_Factory_PointRep ()
{
  class_ < Factory < RepBase >,
    bases <>,
    Factory < RepBase >, 
    boost::noncopyable >
    ( "FactoryPointRep",
      "A private base class for PointRepFactory.",
      no_init )

    ;
}

void 
export_PointRepFactory ()
{
  class_ < PointRepFactory, 
    bases < Factory < RepBase > >,
    PointRepFactory, boost::noncopyable >
    ( "PointRepFactory",
      "A factory for creating object whose class derive from PointRep.",
      no_init )

    .def ( "instance",  &PointRepFactory::instance,
	   return_value_policy < reference_existing_object >  (),
	   "instance () -> PointRepFactory\n"
	   "\n"
	   "Returns the instance of the PointRepFactory." )

    .staticmethod ( "instance" )

    .def ( "names", &PointRepFactory::names,
	   return_value_policy < copy_const_reference > (),
	   "names () -> sequence\n"
	   "\n"
	   "Returns the names of available PointRep classes." )

    .def ( "create", &PointRepFactory::create,
	   return_value_policy < reference_existing_object > (),
	   "create ( string ) -> PointRep\n"
	   "\n"
	   "Returns a PointRep object of type named in string." )
    ;

}

} // namespace Python
} // namespace hippodraw
