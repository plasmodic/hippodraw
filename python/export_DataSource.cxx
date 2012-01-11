/** @file 

Part of Python extension module for hippodraw::DataSource class.

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_DataSource.cxx,v 1.9 2006/10/01 23:36:20 pfkeb Exp $

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

#include "datasrcs/DataSource.h"

using std::vector;
using namespace boost::python;

namespace hippodraw {
namespace Python {

void
export_DataSource()
{
  class_ < DataSource, bases<>,
    DataSource, boost::noncopyable >
    ( "DataSource",
      "This class is an abstract base class defining the data table,\n"
      "called an ntuple.  That is a table with some number of columns and a\n"
      "perhaps large number of rows. It implements some of the\n"
      "but the actual data storage is done in a derived class.",
      no_init )

    .add_property ( "columns", 
		    &DataSource::columns )

    .add_property ( "rows",
		    &DataSource::rows )

    .def ( "getTitle", &DataSource::title,
	   return_value_policy < copy_const_reference > (),
	   "getTitle () -> string\n"
	   "\n"
	   "Returns the title of the ntuple." )

    .def ( "setName", &DataSource::setName,
	   "setName ( string ) -> None\n"
	   "\n"
	   "Sets the name of the ntuple.   The name should be a unique\n"
	   "string withing a running application.   It may appear in the\n"
	   "Inspector." )

    .def ( "setTitle", &DataSource::setTitle,
	   "setTitle ( string ) -> None\n"
	   "\n"
	   "Sets the title of the ntuple.  The title by default appears at\n"
	   "the top of a Display." )
	   
    .def ( "getLabels", &DataSource::getLabels,
	   return_value_policy < copy_const_reference > (),
	   "getLabels () -> list\n"
	   "\n"
	   "Returns list of column labels." )

    .def ( "getColumn", 
	   ( const std::vector < double > & (DataSource::* ) // fptr
	     ( unsigned int ) const) // function signature
	   &DataSource::getColumn,
	   return_value_policy < copy_const_reference> (),
	   "getColumn ( label ) -> list\n"
	   "getColumn ( index ) -> list\n"
	   "\n"
	   "Get a column by its label or index.   Returns copy of the\n"
	   "contents." )

    .def ( "getColumn", 
	   ( const std::vector < double > & (DataSource::* ) // fptr
	     ( const std::string & ) const) // function signature
	   &DataSource::getColumn,
	   return_value_policy < copy_const_reference> () )

    .def ( "replaceColumn",
	   ( void ( DataSource::* ) // function pointer
	     ( const std::string &,
	       const std::vector < double > & ) ) // signature
	   &DataSource::replaceColumn,
	   "replaceColumn ( label, list ) -> None\n"
	   "\n"
	   "Replaces the column of data by abel." )

    .def ( "clear",
	   &DataSource::clear,
	   "clear () -> None\n"
	   "\n"
	   "Clears the data elements of the DataSource.   That is, remove\n"
	   "all the rows while keeping the column labels." )

   ;
}

} // namespace Python
} // namespace hippodraw
