/** @file

export HippoDraw classes to Python to build extension module.

Copyright (C) 2003-2006  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: exportToPython.cxx,v 1.57 2007/07/02 18:17:16 pfkeb Exp $

*/

#ifdef HAVE_CONFIG_H
//for have qt app
#include "config.h"
#endif

#ifdef _MSC_VER
# pragma warning(disable:4231)  // nonstandard extension used 'extern'
				// before...
# pragma warning(disable:4251)  // needs to have dll-interface used by client
# pragma warning(disable:4275)  // non dll-interface struct
# pragma warning(disable:4800)  // 'int' : forcing value to bool
				// 'true' or 'false' (performance
				// warning)
#endif

#include "exportToPython.h"

// include first to avoid _POSIX_C_SOURCE redefined warnings
#include "pyconversions.h"

#include "QtCut.h"
#include "PyDataRep.h"
#include "PyDataSource.h"


using std::vector;
using namespace hippodraw::Python;
using namespace boost::python;

namespace hippodraw {
  namespace Python {

#if __GNUC__ < 3

    /** Translate a C++ exception to a Python one.  The exception @a e
	must a derived class of std::exception.  

	@attention This function seems to be required by GCC less than
	verion 3.0.
    */
    void 
    translate ( const std::exception & e ) 
    {
      PyErr_SetString ( PyExc_RuntimeError, e.what() );
    }
#endif

void
exportToPython ()
{
#if __GNUC__ < 3
  register_exception_translator < std::exception > ( & translate );
#endif

  /* The following is neede for PyGILState_Ensure / _Release to
     work. */
  PyEval_InitThreads();

  // The order is important.   Base class before derived classes

#ifdef HAVE_QT_APP
	export_CanvasWindow ();
#endif
	export_DataSource ();
	export_DataArray ();
	export_ListTuple (); // derived from DataSource
#ifdef HAVE_NUMARRAY
	export_NumArrayTuple (); // derived from DataSource
#endif
#ifdef HAVE_ROOT
	export_RootNTuple (); // derived from DataSource
	export_RootController (); 
#endif
	export_CutController ();
#ifdef HAVE_CFITSIO
	export_FitsController ();
	export_FitsNTuple (); // derived from DataSource
#endif
	export_NTuple ();    // derived from DataSource
	export_PyNTuple ();  // derived from NTuple
	export_CircularBuffer (); // derived from NTuple
	export_NTupleController ();
	export_Observer ();
	export_QtDisplay ();
	export_QtCut (); // derived from QtDisplay
	export_Function ();
#ifdef HAVE_QT_APP
	export_Canvas ();
#endif
	export_HDApp ();
	export_DataRep ();
	export_RepBase ();
	export_Factory_PointRep ();
	export_PointRepFactory ();
	export_Fitter ();
	export_FitterFactory ();
	// order is important, base class first
	export_FCNBase ();
	export_StatedFCN ();
	export_NTupleFCN ();
#ifndef BOOST_DEFECT
	export_FunctionBase ();
#endif
	export_FunctionFactory ();
	export_SymbolType ();
	export_LineStyle ();
	export_Color ();

    std_vector_to_tuple < double > ();
    std_vector_to_tuple < int > ();
    std_vector_to_tuple < std::string > ();
    std_vector_to_tuple < QtDisplay * > ();
    std_vector_to_tuple < PyDataRep * > ();

    from_python_sequence < std::vector < std::string >,
                           variable_capacity_policy > ();

    from_python_sequence < std::vector < double >,
                           variable_capacity_policy > ();

    from_python_sequence < std::vector < int >,
                           variable_capacity_policy > ();

    from_python_sequence < std::vector < QtDisplay * >,
                           variable_capacity_policy > ();

    from_python_sequence < std::vector < QtCut * >,
                           variable_capacity_policy > ();

    from_python_sequence < std::vector < std::vector < double > >,
                           variable_capacity_policy > ();

}

} // end namespace Python
} // end namespace hippodraw
