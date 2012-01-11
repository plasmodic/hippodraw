/** @file 

Part of Python extension module for hippodraw::NTuple and
hippodrw::PyNTuple classes.

Copyright (C) 2000-2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_NTuple.cxx,v 1.48 2006/10/03 18:07:41 pfkeb Exp $

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

#include "PyNTuple.h"

using std::vector;
using namespace boost::python;

namespace hippodraw {
namespace Python {

void
export_NTuple()
{
  class_ < NTuple, bases < DataSource > >
    ( "NTupleInternal",
      "A derived class of DataSource that stores its tabular data vectors of\n"
      "double precision numbers in C++.  An NTuple object can be created in\n"
      "a number of ways including reading from a file using the\n"
      "NTupleController",
      init<>
      ( "NTuple ( None ) -> NTuple\n"
	"NTuple ( value ) -> NTuple\n"
	"NTuple ( sequence ) -> NTuple\n"
	"NTuple ( NTuple ) -> NTuple\n"
	"\n"
	"The form with no arguments creates an empty NTuple with no rows\n"
	"or columns.   The form with one value argument creates an empty\n"
	"NTuple with `value' number of columns.   The form with a sequence\n"
	"argument creates an empty NTuple with the number of columns equal\n"
	"to size of the sequence.   The sequence should contain string which\n"
	"are used as the column labels.   The last form form creates an\n"
	"NTuple\n whose contents is a copy of an existing one." ) )

    .def ( init < unsigned int > ( ) )

    .def ( init < const std::vector < std::string  > & > ( ) )

    .def ( init < const NTuple & > () )

    .def ( "setLabels", &NTuple::setLabels,
	   "setLabels ( sequence ) -> None\n"
	   "\n"
	   "Sets the labels of the columns from the list of strings.  If the\n"
	   "NTuple is empty, then also sets the number of columns to be the\n"
	   "size of the list.   If the number of columns has already been\n"
	   "set, the the size of the list should be the same, otherwise\n"
	   "a RuntimeError exception is thrown." )

    .def ( "getLabel", &NTuple::getLabelAt,
	   return_value_policy < copy_const_reference > (),
	   "getLabel ( index ) -> string\n"
	   "\n"
	   "Returns the label at column index." )

    .def ( "getRow", &NTuple::getRow,
	   return_value_policy < copy_const_reference> (),
	   "getRow ( index ) -> list\n"
	   "\n"
	   "Returns the index row as list floats." )

    .def ( "setIntervalCount", &NTuple::setIntervalCount,
	   "setIntervalCount ( count ) -> None\n"
	   "\n"
	   "Sets the interval count between updates to the observers." )

    .def ( "setIntervalEnabled", &NTuple::setIntervalEnabled,
	   "setIntervalEnable ( Boolean ) -> None\n"
	   "\n"
	   "Sets the interval counting on or off" )

    ;
}

void
export_PyNTuple()
{
  class_ < PyNTuple, bases < NTuple > >
    ( "NTuple",
      "A derived class of DataSource that stores its tabular data vectors of\n"
      "double precision numbers in C++.  An NTuple object can be created in\n"
      "a number of ways including reading from a file using the\n"
      "NTupleController",
      init<>
      ( "NTuple ( None ) -> NTuple\n"
	"NTuple ( value ) -> NTuple\n"
	"NTuple ( sequence ) -> NTuple\n"
	"NTuple ( NTuple ) -> NTuple\n"
	"\n"
	"The form with no arguments creates an empty NTuple with no rows\n"
	"or columns.   The form with one value argument creates an empty\n"
	"NTuple with `value' number of columns.   The form with a sequence\n"
	"argument creates an empty NTuple with the number of columns equal\n"
	"to size of the sequence.   The sequence should contain string which\n"
	"are used as the column labels.   The last form form creates an\n"
	"NTuple\n whose contents is a copy of an existing one." ) )

    .def ( init < unsigned int > ( ) )

    .def ( init < const std::vector < std::string  > & > ( ) )

    .def ( init < const PyNTuple & > () )

    .def ( "setTitle", &PyNTuple::setTitle,
	   "setTitle ( string ) -> None\n"
	   "\n"
	   "Sets the title of the ntuple.  The title by default appears at\n"
	   "the top of a Display." )
	   
    .def ( "addColumn", 
	   &PyNTuple::addColumn,
	   "addColumn ( label, sequence ) -> index\n"
	   "\n"
	   "Adds a new column with label." )

    .def ( "append", &PyNTuple::append,
	   "append ( DataSource ) -> None\n"
	   "\n"
	   "Appends the contents of the DataSource to the NTuple." )

    .def ( "replaceColumn",
	   ( void ( PyNTuple::* ) // function pointer cast
	     ( unsigned int,
	       const std::vector < double > & ) )
	   &PyNTuple::replaceColumn,
	   "replaceColumn ( index, sequence ) -> None\n"
	   "\n"
	   "Replaces the indexed column." )

    .def ( "replaceColumn",
	   ( void ( PyNTuple::* ) // function pointer cast
	     ( const std::string &, const std::vector < double > & ) )
	   &PyNTuple::replaceColumn,
	   "replaceColumn ( label, sequence ) -> None\n"
	   "\n"
	   "Replaces the labeled  column." )

    .def ( "addRow",
	   &PyNTuple::addRow,
	   "addRow ( sequence ) -> None\n"
	   "\n"
	   "Append a row at the end." )

    .def ( "clear",
	   &PyNTuple::clear,
	   "clear () -> None\n"
	   "\n"
	   "Clears the data elements of the DataSource.   That is, remove\n"
	   "all the rows while keeping the column labels." )

    ;
}
} // namespace Python
} // namespace hippodraw
