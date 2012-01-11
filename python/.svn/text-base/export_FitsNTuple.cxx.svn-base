/** @file 

Part of Python extension module for hippodraw::FitsNTuple class.

Copyright (C) 2005, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_FitsNTuple.cxx,v 1.6 2006/10/01 23:36:20 pfkeb Exp $

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

#include "fits/FitsNTuple.h"

using std::vector;
using namespace boost::python;

namespace hippodraw {
namespace Python {

void
export_FitsNTuple()
{
  class_ < FitsNTuple, bases < DataSource > > 
    ( "FitsNTuple",
      "a derived class of DataSource that references a table or image in a\n"
      "FITS file.  A FitNTuple object must be created by the FitsController."
      "Changes made to this object will not be reflected in the FITS file.",
      no_init )

    .def ( "addColumn",
	   ( int ( FitsNTuple::* ) // function pointer
	     ( const std::string &,
	       const std::vector < double > & ) )  // signature
	   &FitsNTuple::addColumn,
	   "addColumn ( string, list or tuple ) -> value\n"
	   "\n"
	   "Adds a column to the data source. The string argument is used\n"
	   "for the label of the new column.   The length of the new column\n"
	   "must the same as existing columns.   The value returns is the\n"
	   "index to the column." )

    .def ( "replaceColumn",
	   ( void ( DataSource::* ) // function pointer
	     ( const std::string &,
	       const std::vector < double > & ) ) // signature
	   &DataSource::replaceColumn,
	   "replaceColumn ( index, list ) -> None\n"
	   "replaceColumn ( label, list ) -> None\n"
	   "\n"
	   "Replaces the column of data by index or label.   Does not\n"
	   "modify the FITS file." )

    .def ( "replaceColumn",
	   ( void ( FitsNTuple::* ) // function pointer
	     ( unsigned int,
	       const std::vector < double > & ) ) // signature
	   &FitsNTuple::replaceColumn )
    ;
}

} // namespace Python
} // namespace hippodraw
