/** @file

Python extension for hippodraw::FunctionFactory class.

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_FunctionFactory.cxx,v 1.5 2007/01/31 23:06:13 pfkeb Exp $

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

#include "functions/FunctionBase.h"
#include "functions/FunctionFactory.h"

using std::vector;
using namespace boost::python;

namespace hippodraw {
namespace Python {

void 
export_FunctionFactory()
{
  class_ < FunctionFactory, bases<>,
    FunctionFactory, boost::noncopyable >
    ( "FunctionFactory",
      "The factory for creating derived classes of FunctionBase.",
      no_init )

    .def ( "instance",  &FunctionFactory::instance,
	   return_value_policy < reference_existing_object >  (),
	   "instance () -> FunctionFactory\n"
	   "\n"
	   "Returns the single instance of the FunctionFactory." )

    .staticmethod( "instance" )

    .def ( "names", &FunctionFactory::names,
	   return_value_policy < copy_const_reference > (),
	   "names () -> sequence\n"
	   "\n"
	   "Returns the names of the available FunctinBase derived classes." )

    .def ( "create", &FunctionFactory::create,
	   return_value_policy < reference_existing_object > (),
	   "create ( string ) -> FunctionBase\n"
	   "\n"
	   "Returns the newly created object derived from FunctionBase." )

    .def ( "add", &FunctionFactory::add,
	   with_custodian_and_ward_postcall < 1, 2 > (),
	   "add ( FunctionBase ) -> None\n"
	   "\n"
	   "Adds the function to the factory." )

    ;

}

} // namespace Python
} // namespace hippodraw
