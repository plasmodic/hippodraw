/** @file

Export hippodraw::QtRootNTuple to Python.

Copyright (C) 2005, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_RootNTuple.cxx,v 1.15 2007/07/02 18:17:16 pfkeb Exp $

*/

#ifdef HAVE_CONFIG_H
// for have numarray
#include "config.h"
#endif

// For truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

// include first to avoid _POSIX_C_SOURCE warning.
#include <boost/python.hpp>

#include "root/QtRootNTuple.h"

#ifdef HAVE_NUMARRAY
#include "numarray/num_util.h"
#endif

#include <utility>

using std::string;
using std::vector;

using namespace boost::python;

namespace hippodraw {
namespace Python {

void
export_RootNTuple()
{
  class_ < QtRootNTuple, bases<DataSource>,
    QtRootNTuple, boost::noncopyable>
    ( "RootNTuple",
      "The RootNTuple class provides access to data in a ROOT file.\n"
      "A ROOT TTree is taken as an ntuple and a ROOT TBranch is taken as a\n"
      "column, the TBranch name as the column label.\n\n"
      "If the hippo module was configured with numeric array support,\n"
      "consider using DataArray to wrap an object of this class.",
      no_init )

    .add_property ( "rows", &QtRootNTuple::rows )

    .add_property ( "columns", &QtRootNTuple::columns )

    .def ( "addColumn",
	   &QtRootNTuple::addColumn,
	   "addColumn ( label, list ) -> value\n"
	   "\n"
	   "Adds a column to the ntuple.  Adds a column to the ntuple by \n"
	   "copying the contents of the list.   The new column will take its\n"
	   "label from the first argument.   Returns the index of the newly \n"
	   "created column.\n\n"
	   "Note: The column is not added to the ROOT file, just logically\n"
	   "to it for the current session.\n")

    .def ( "replaceColumn",
	   ( void ( RootNTuple::* ) // function pointer
	     ( const std::string &,
	       const std::vector < double > & ) ) // signature
	   &RootNTuple::replaceColumn,
	   "replaceColumn ( index, list ) -> None\n"
	   "replaceColumn ( label, list ) -> None\n"
	   "\n"
	   "Replaces the column of data by index or label.   Does not\n"
	   "modify the ROOT file." )

    .def ( "replaceColumn",
	   ( void ( RootNTuple::* ) // function pointer
	     ( unsigned int,
	       const std::vector < double > & ) ) // signature
	   &RootNTuple::replaceColumn )

    .def ( "getColumn", 
	   ( const std::vector < double > & (QtRootNTuple::* ) // fptr
	     ( const std::string & ) const) // function signature
	   &QtRootNTuple::getColumn,
	   return_value_policy < copy_const_reference> (),
	   "getColumn ( index ) -> list\n"
	   "getColumn ( label ) -> list\n"
	   "getColumn ( label, indices ) -> list\n"
	   "\n"
	   "Returns a copy of the column as a list.  In the first form the\n"
	   "the column is specified by its index.   In the second form it\n"
	   "is specified by its label which could be a simple variable or\n"
	   "an array variable.  If an array variable, the indices should be\n"
	   "part of the label.   The third form, the column is specified by\n"
	   "an array variables name and list of indices.\n" )

    .def ( "getColumn", 
	   ( const std::vector < double > & (QtRootNTuple::* ) // fptr
	     ( const std::string &,
	       const std::vector < int > & ) const) // function signature
	   &QtRootNTuple::getColumn,
	   return_value_policy < copy_const_reference> () )

    .def ( "getColumn", 
	   ( const std::vector < double > & (QtRootNTuple::* ) // fptr
	     ( unsigned int ) const) // function signature
	   &QtRootNTuple::getColumn,
	   return_value_policy < copy_const_reference> () )

    .def ( "getRow",
	   &QtRootNTuple::getRow,
	   return_value_policy < copy_const_reference> (),
	   "getRow ( index ) -> tuple\n"
	   "\n"
	   "Returns a row from the ROOT file." )

    .def ( "isMultiDimensional",
	   &QtRootNTuple::isMultiDimensional,
	   "isMultiDimensional ( label ) -> Boolean\n"
	   "\n"
	   "Returns True if the column specified by label is an array." )

     .def ( "rowDataDimSize",
            &QtRootNTuple::getColumnShape,
            return_value_policy < return_by_value > (),
	    "rowDataDimSize ( label )\n"
	    "\n"
            "Returns a tuple of the dimensions (shape) of the array elements\n"
	    "for the column specified by label." )

    .def ( "getLabels",
	   &QtRootNTuple::getLabels,
	   return_value_policy < copy_const_reference > (),
	   "getLabels () -> list\n"
	   "\n"
	   "Returns the column labels.  Note: if a column is an array the\n"
	   "shape does not appear as part of the label.\n" )

#ifdef HAVE_NUMARRAY
    .def ( "valueAt",
	   &QtRootNTuple::valueAt,
	   return_value_policy < return_by_value > (),
	   "valueAt ( index, label ) -> array\n"
	   "\n"
	   "Returns the numerical array for the array variable in column\n"
	   "specified by label, for the row index.   Note: this method is\n"
	   "only available if the hippo module was configured for numerical\n"
	   "array support." )
#endif
    ;
}

} // namespace Python
} // namespace hippodraw
