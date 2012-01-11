/** @file

Export hippodraw::QtCut to Python

Copyright (C) 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_QtCut.cxx,v 1.6 2007/07/02 18:17:16 pfkeb Exp $

*/

// For truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// include first to avoid _POSIX_C_SOURCE warning.
#include <boost/python.hpp>

#include "QtCut.h"

#include "PyDataSource.h"

#include "datasrcs/NTuple.h"

using std::vector;
using namespace boost::python;

namespace hippodraw {
namespace Python {

void
export_QtCut()
{
  class_ <QtCut, bases<QtDisplay> >
    ( "Cut",
      "An object of this class applies a cut or filter on the data that is\n"
      "used as input to its target displays.",
      init < const DataSource&, 
             const std::vector < std::string > & >
      ( "Cut ( DataSource, sequence ) -> Cut\n"
	"Cut ( DataArray, sequence ) -> Cut\n"
	"Cut ( DataSource, sequence, Display, value, value ) -> Cut\n"
	"Cut ( DataArray, sequence, Display, value, value ) -> Cut\n"
	"\n"
	"Creates a Cut bound to DataSource.  For each form, the first\n"
	"argument is the DataSource and the second argument is sequence of\n"
	"strings specifying the binding.  The latter two forms apply the\n"
	"Cut to a Display with low and high values for the Cut range." ) )

    .def ( init < const DataSource &,
	   const std::vector < std::string > &,
	   QtDisplay *, double, double > () )

    .def (  init < const PyDataSource&, 
	    const std::vector < std::string > & > () )

    .def ( init < const PyDataSource &,
	   const std::vector < std::string > &,
	   QtDisplay *, double, double > () )

#ifdef HAVE_NUMARRAY
    .def ( init < PyDataSource &,
	   std::string ,
	   boost::python::numeric::array,
	   QtDisplay * > () )
#endif // HAVE_NUMARRAY

    .def ( "addTarget", &QtCut::addTarget,
	   "addTarget ( Display ) -> None\n"
	   "\n"
	   "Adds a Display to the list of targets." )

    .def ( "addTargets", &QtCut::addTargets,
	   "addTargets ( sequence ) -> None\n"
	   "\n"
	   "Adds Display objects in the sequence to the list targets" )

    .def ( "setCutRange", &QtCut::setCutRange,
	   "setCutRange ( value, value, string ) -> None\n"
	   "\n"
	   "Sets the range of the Cut.  Arguments are low and high values,\n"
	   "and the axis." )

    .def ( "cutRange", &QtCut::cutRange,
	   "cutRange () -> tuple\n"
	   "\n"
	   "Returns a pair of values of the range of each Cut." )

    .def ( "setEnabled", &QtCut::setEnabled,
	   "setEnabled ( Boolean ) -> None\n"
	   "\n"
	   "Sets the cut to be enabled or not" )

    .def ( "toggleInverted", &QtCut::toggleInverted,
	   "toggleInverted () -> None\n"
	   "\n"
	   "Toggles the inversion status" )

    .def ( "createNTuple", &QtCut::createNTuple,
	   return_value_policy < manage_new_object > (),
	   "createNTuple ( sequence, sequence, DataSource ) -> NTuple\n"
	   "\n"
	   "Create a new NTuple using cut list and column list." )

    .staticmethod("createNTuple")

    .def ( "createTnt", &QtCut::createTnt,
	   "createTnt ( sequence, sequence, DataSource, string, string )"
	   "-> None\n"
	   "\n"
	   "Create a new TNT file using cut list and column list." )

    .staticmethod("createTnt")

    .def ( "createFits", &QtCut::createFits,
	   "createFits ( sequence, sequence, DataSource, string, string )"
	   "->None\n"
	   "\n"
	   "Create a new FITS file using cut list and column list.\n"
	   "This method is deprecated, use the FitsController instead.")

    .staticmethod("createFits")
	  
    ;

}

} // namespace Python
} // namespace hippodraw
