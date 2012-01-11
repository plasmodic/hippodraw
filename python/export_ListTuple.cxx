/** @file 

Exports hippodraw::ListTuple class to Python

Copyright (C) 2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_ListTuple.cxx,v 1.11 2006/06/21 20:36:32 pfkeb Exp $

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

#include "ListTuple.h"

using namespace boost::python;

namespace hippodraw {
namespace Python {

void
export_ListTuple()
{
  class_ < ListTuple, bases < DataSource > >
    ( "ListTuple",
      "A derived class from DataSource containing columns as references to\n"
      "Python list objects.  This allows the data to be used without making\n"
      "a copy of it.   However, access to the data is slower than for most\n"
      "of the other classes derived from DataSource",
      init< >
      ( "ListTuple ( None ) -> ListTuple\n"
	"\n"
	"Creates an empty ListTuple" ) )

//     .add_property ( "rows", &ListTuple::rows )

    .def ( "setLabels", &ListTuple::setLabels,
	   "setLabels ( list or tuple ) -> None\n"
	   "\n"
	   "Sets the labels of the columns from list of string objects.\n"
	   "For an empty ListTuple object, implicitly sets the number of\n"
	   "columns." )

    .def ( "getLabel", &ListTuple::getLabelAt,
	   return_value_policy < copy_const_reference > (),
	   "getLabel ( index ) -> string\n"
	   "\n"
	   "Returns label of column." )

//     .def ( "getLabels", &ListTuple::getLabels,
// 	   return_value_policy < copy_const_reference > () )

    .def ( "addColumn", 
	   &ListTuple::addColumn,
	   with_custodian_and_ward < 1, 2 > (),
	   "addColumn ( string, list ) -> value\n"
	   "\n"
	   "Adds the array as a new column with label from the string.\n"
	   "Returns the index of the new column."  )

    .def ( "replaceColumn",
	   ( void ( ListTuple:: * ) //function pointer
	     ( unsigned int, boost::python::list ) ) // signature
	   &ListTuple::replaceColumn,
	   with_custodian_and_ward < 1, 2 > (),
	   "replaceColumn ( index, list ) -> None\n"
	   "\n"
	   "Replaces existing column with list\n" )

    .def ( "replaceColumn",
	   ( void ( ListTuple:: * ) //function pointer
	     ( const std::string &, boost::python::list ) ) // signature
	   &ListTuple::replaceColumn,
	   with_custodian_and_ward < 1, 2 > (),
	   "replaceColumn ( string, list ) -> None\n"
	   "\n"
	   "Replaces existing column with new array\n" )

    .def ( "notifyObservers",
	   &ListTuple::notifyObservers,
	   "notifyObservers ( None ) -> None\n"
	   "\n"
	   "Sends update message to all observers.  Use this member if the\n"
	   "contents of the reference Python list has changed and Display\n"
	   "objects bound to it require updating." )

    ;
}

} // namespace Python
} // namespace hippodraw
