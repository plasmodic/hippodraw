/** @file

Python extension for hippodraw::FitterFactory class.

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_FitterFactory.cxx,v 1.5 2006/03/31 22:33:19 pfkeb Exp $

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

#include "minimizers/Fitter.h"
#include "minimizers/FitterFactory.h"

using std::vector;
using namespace boost::python;

namespace hippodraw {
namespace Python {

void 
export_FitterFactory()
{
  class_ < FitterFactory, bases<>,
    FitterFactory, boost::noncopyable >
    ( "FitterFactory",
      "The factory for returning fitters preloaded with objective functions.",
      no_init )

    .def ( "instance",  &FitterFactory::instance,
	   return_value_policy < reference_existing_object >  (),
	   "instance () -> FitterFactory\n"
	   "\n"
	   "Returns the single instance of the FitterFactory" )

    .staticmethod( "instance" )

    .def ( "names", &FitterFactory::names,
	   return_value_policy < copy_const_reference > (),
	   "names () -> sequence\n"
	   "\n"
	   "Returns the names of the available fitters." )

    .def ( "create", &FitterFactory::create,
	   return_value_policy < manage_new_object > (),
	   "create ( string ) -> Fitter\n"
	   "\n"
	   "Returns the newly created fitter object." )

    .def ( "setDefault", &FitterFactory::setDefault,
	   "setDefault ( string ) -> None\n"
	   "\n"
	   "Sets the default fitter object.   Throws an exception if given\n"
	   "invalid name." )
    ;

}

} // namespace Python
} // namespace hippodraw
