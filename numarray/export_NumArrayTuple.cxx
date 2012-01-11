/** @file 

Part of Python extension module for hippodraw::NumArrayTuple class.

Copyright (C) 2004, 2006, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_NumArrayTuple.cxx,v 1.25 2007/07/02 18:17:15 pfkeb Exp $

*/

// for have numarray etc
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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

#define PY_ARRAY_UNIQUE_SYMBOL HippoPyArrayHandle

//#include "num_util.h"

#include "NumArrayTuple.h"

using namespace boost::python;

namespace hippodraw {
namespace Python {

void
export_NumArrayTuple()
{
  import_array();

#ifdef HAVE_NUMERIC
  numeric::array::set_module_and_type 
    ( "Numeric",
      "ArrayType" );
#else
#ifdef HAVE_NUMPY
  numeric::array::set_module_and_type 
    ( "numpy",
      "ndarray" );
#else
  numeric::array::set_module_and_type (); // default numarray & NDarray
#endif
#endif

  class_ < NumArrayTuple, bases < DataSource > > ( "NumArrayTuple",
		"Interface to numarray Python extension module.",
		 init< > ( "Creates an empty NumArrayTuple" ) )

//     .add_property ( "rows", &NumArrayTuple::rows )

    .def ( "setLabels", &NumArrayTuple::setLabels,
	   args ( "[label, ...]" ), 
	   "Sets the labels of the columns." )

    .def ( "getLabel", &NumArrayTuple::getLabelAt,
	   return_value_policy < copy_const_reference > () )

//     .def ( "getLabels", &NumArrayTuple::getLabels,
// 	   return_value_policy < copy_const_reference > () )

    .def ( "addColumn", 
	   &NumArrayTuple::addColumn,
	   "Adds the array as a new column with label" )

    .def ( "replaceColumn",
	   ( void ( NumArrayTuple:: * ) //function pointer
	     ( unsigned int, numeric::array ) ) // signature
	   &NumArrayTuple::replaceColumn,
	   "Replaces existing column with new array\n"
	   "Args:\n"
	   "\t index\n"
	   "\t numarray" )

    .def ( "replaceColumn",
	   ( void ( NumArrayTuple:: * ) //function pointer
	     ( const std::string &, numeric::array ) ) // signature
	   &NumArrayTuple::replaceColumn,
	   "Replaces existing column with new array\n"
	   "Args:\n"
	   "\t label\n"
	   "\t numarray" )

    .def ( "notifyObservers",
	   &NumArrayTuple::notifyObservers,
	   "Sends update message to all observers." )

    ;
}

} // namespace Python
} // namespace hippodraw
