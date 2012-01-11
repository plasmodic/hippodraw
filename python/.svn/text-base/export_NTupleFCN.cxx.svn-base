/** @file 

Part of Python extension module for hippodraw::NTupleFCN class.

Copyright (C) 2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_NTupleFCN.cxx,v 1.6 2006/03/31 22:33:20 pfkeb Exp $

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

#include "minimizers/NTupleFCN.h"

#include "datasrcs/DataSource.h"
#include "functions/FunctionBase.h"

using std::vector;
using namespace boost::python;

namespace hippodraw {
namespace Python {

void
export_NTupleFCN()
{
  class_ < NTupleFCN, bases < StatedFCN >, 
    NTupleFCN, boost::noncopyable >
    ( "NTupleFCN",
      "A objective function class used in fitting that uses a DataSource to\n"
      "access the data\n",
      no_init )

    .def ( "setDataSource", 
	   ( void ( NTupleFCN::* ) // function pointer
	     (const DataSource * ) ) // function signature
	   &NTupleFCN::setDataSource )

    .def ( "setDataSource",
	   ( void ( NTupleFCN::* ) // function pointer
	     ( const DataSource *,
	       int dimension,
	       const std::vector < int > &  ) ) // function signature
	   &NTupleFCN::setDataSource,
	   "setDataSource ( DataSource ) -> None\n"
	   "setDataSource ( DataSource, value, sequence ) -> None\n"
	   "\n"
	   "The first form sets the data source with default indexes.  The\n"
	   "second form takes in addition the dimensionality of the\n"
	   "coordinate and a column indexes from the sequence." )

    ;
}

} // namespace Python
} // namespace hippodraw
