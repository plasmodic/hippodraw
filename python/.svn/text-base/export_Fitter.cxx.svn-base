/** @file 

Part of Python extension module for hippodraw::Fitter class.

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_Fitter.cxx,v 1.9 2006/03/31 22:33:19 pfkeb Exp $

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
#include "minimizers/Fitter.h"
#include "minimizers/StatedFCN.h"

using std::vector;
using namespace boost::python;

namespace hippodraw {
namespace Python {

void
export_Fitter()
{
  class_ < Fitter, bases<>,
    Fitter, boost::noncopyable >
    ( "Fitter",
      "Provides an interface to objective function minimizers.",
      no_init )

    .add_property ( "degreesOfFreedom", 
		    &Fitter::calcDegreesOfFreedom )

    .add_property ( "objectiveValue",
		    &Fitter::objectiveValue )

    .def ( "name", &Fitter::name,
	   return_value_policy < copy_const_reference > (),
	   "name () -> string\n"
	   "\n"
	   "Returns the minimizer name." )

    .def ( "getFCN", &Fitter::getFCN,
	   return_value_policy < reference_existing_object > (),
	   "getFCN () -> FCNBase\n"
	   "\n"
	   "Returns the objective function." )

    .def ( "minimize", &Fitter::calcBestFit,
	   "minimize () -> Boolean\n"
	   "\n"
	   "Attempts to do a minimization, returns True if successful." )

    .def ( "setFixedFlags", &Fitter::setFixedFlags,
	   "setFixedFlags ( sequence ) -> None\n"
	   "\n"
	   "Sets which parameters that should be fixed during a fit.   A\n"
	   "`1' in the sequence indicates corresponding parameters is fixed." )

    .def ( "setLimits",
	   ( void ( Fitter::* ) // function pointer cast
	     ( unsigned int, double, double ) ) // signature
	   &Fitter::setLimits )

    .def ( "setLimits",
	   ( void ( Fitter::* ) // function pointer cast
	     ( const std::string &, double, double ) ) // signature
	   &Fitter::setLimits,
	   "setLimits ( name, low, high ) -> None\n"
	   "setLimits ( index, low, high ) -> None\n"
	   "\n"
	   "Sets the limits on a model function parameter.  Only minimizers\n"
	   "based on Minuit support this feature, others throw an exception." )

    .def ( "setStepSize",
	   ( void ( Fitter::* ) // function pointer cast
	     ( unsigned int, double ) ) // signature
	   &Fitter::setStepSize )

    .def ( "setStepSize",
	   ( void ( Fitter::* ) // function pointer cast
	     ( const std::string &, double ) ) // signature
	   &Fitter::setStepSize,
	   "setStepSize ( name, value ) -> None\n"
	   "setStepSize ( index, value ) -> None\n"
	   "\n"
	   "Sets the step size for the model function parameter.   Only\n"
	   "minimizers based on Minuit support this function, others\n"
	   "throw an exception." )

   ;
}

} // namespace Python
} // namespace hippodraw
