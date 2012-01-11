/** @file 

Part of Python extension module for hippodraw::StatedFCN class.

Copyright (C) 2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_StatedFCN.cxx,v 1.12 2007/01/31 23:05:10 pfkeb Exp $

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

#include "minimizers/StatedFCN.h"

#include "datasrcs/DataSource.h"
#include "functions/FunctionBase.h"

using std::vector;

using namespace boost::python;

namespace hippodraw {
namespace Python {

  /** @bug @@@@@@ Can export objective value but not degrees of
      freedom.
   */
void
export_StatedFCN()
{
#ifdef HAVE_MINUIT2
  class_ < StatedFCN, bases < ROOT::Minuit2::FCNBase >, 
#else
  class_ < StatedFCN, bases < FCNBase >, 
#endif
    StatedFCN, boost::noncopyable >
    ( "StatedFCN",
      "This a base class for objective function used in fitting that\n"
      "maintains the state of the function's parameters.",
      no_init )

    .def ( "objectiveValue", &StatedFCN::objectiveValue,
	   "objectiveValue () -> value\n"
	   "\n"
	   "Returns the objective value.   For example, the objective\n"
	   "might be the Chi-Squared when derived class returns it as\n"
	   "its objective value." )

    .def ( "degreesOfFreedom", &StatedFCN::degreesOfFreedom,
	   "degreesOfFreedom () -> value\n"
	   "\n"
	   "Returns the degrees of freedom as integer." )

    .def ( "setFunction", &StatedFCN::setFunction,
	   "setFunction ( FunctionBase ) -> None\n"
	   "\n"
	   "Sets the model function." )

    .def ( "setDataSource", 
	   ( void ( StatedFCN::* ) // function pointer
	     (const DataSource * ) ) // function signature
	   &StatedFCN::setDataSource )

    .def ( "setDataSource",
	   ( void ( StatedFCN::* ) // function pointer
	     ( const DataSource *,
	       int dimension,
	       const std::vector < int > &  ) ) // function signature
	   &StatedFCN::setDataSource,
	   "setDataSource ( DataSource ) -> None\n"
	   "setDataSource ( DataSource, value, sequence ) -> None\n"
	   "\n"
	   "The first form sets the data source with default indexes.  The\n"
	   "second form takes in addition the dimensionality of the\n"
	   "coordinate and a column indexes from the sequence." )

    .def ( "setUseErrors", &StatedFCN::setUseErrors,
	   "setUseErrors ( Boolean ) -> None\n"
	   "\n"
	   "Sets the FCN to use errors on the data source if they exist." )

    ;
}

} // namespace Python
} // namespace hippodraw
