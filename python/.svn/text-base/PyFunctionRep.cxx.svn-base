/** @file

hippodraw::PyFunctionRep class implementation
 
Copyright (C) 2002-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: PyFunctionRep.cxx,v 1.55 2007/08/10 18:42:30 pfkeb Exp $

*/

// for dll interface warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

// include first to avoid _POSIX_C_SOURCE warning.
#include <boost/python.hpp>

#include "PyFunctionRep.h"

#include "PyApp.h"
#include "PyDataRep.h"
#include "QtDisplay.h"

#include "controllers/FunctionController.h"
#include "datareps/FunctionRep.h"
#include "functions/FunctionBase.h"
#include "pattern/FactoryException.h"
#include "plotters/PlotterBase.h"

using std::vector;
using namespace boost::python;

namespace hippodraw {
namespace Python {

void
export_Function()
{
  class_ < PyFunctionRep >
    ( "Function",
      "This class wraps a FunctionBase object with a DataRep. This allows\n"
      "it to be drawn in a Display.   It also provides interface to member\n"
      "functions of FunctionBase, although the user could obtain a\n"
      "reference to the FunctionBase itself to do so.",
      init < const std::string &, PyDataRep * >
      ( "Function ( string,  DataRep ) -> Function\n"
	"Function ( FunctionBase, DataRep ) -> Function\n"
	"\n"
	"The first form creates a function using the string to find the\n"
	"FunctionBase in the FunctionFactory.   The second form creates a\n"
	"using an existing FunctionBase object.   Both forms use the DataRep\n"
	"as target for fitting and drawing\n" ) )

     .def ( init < FunctionBase *, PyDataRep * >
            ( ) )

    .def ( init < FunctionBase * >
            ( ) )

    .def ( "addTo", &PyFunctionRep::addTo,
	   "addTo ( Display ) -> None\n"
	   "\n"
	   "Adds the Function to a Display. taking the Display's selected\n"
	   "DataRep as its target." )

    .def ( "parmNames", &PyFunctionRep::parmNames,
	   return_value_policy < copy_const_reference > (),
	   "parmNames ( None ) -> list\n"
	   "\n"
	   "Returns a list of parameter names." )
   
    .def ( "parameters", &PyFunctionRep::parameters,
	   return_value_policy < copy_const_reference > (),
	   "parameters ( None ) -> list\n"
	   "\n"
	   "Returns a list of the function's parameter values." )

    .def ( "principleErrors", &PyFunctionRep::principleErrors,
	   return_value_policy < copy_const_reference > (),
	   "principleErrors ( None ) -> list\n"
	   "\n"
	   "Returns the errors on the parameters." )

    .def ( "errors", &PyFunctionRep::principleErrors,
	   return_value_policy < copy_const_reference > (),
	   "errors ( None ) - > list\n"
	   "\n"
	   "Returns the errors on the parameters.  The errors are calculated\n"
	   "by a fitter, thus the values returned are only valid after\n"
	   "having done a fit." )

    .def ( "fit", &PyFunctionRep::fitFunction,
	   "fit ( None ) -> boolean\n"
	   "\n"
	   "Attempts to fit the the function to the target DataRep.\n"
	   "Uses the currently selected fitter, unless one was explicitly\n"
	   "set.  Note the fit is always done to linear sum if more than one\n"
	   "function is on the data." )

    .def ( "setParameters", &PyFunctionRep::setParameters,
	   "setParameters ( list ) -> None\n"
	   "\n"
	   "Sets the function's parameter values." )

    .def ( "valueAt", &PyFunctionRep::operator(),
	   "valueAt ( x ) -> value\n"
	   "\n"
	   "Returns the function's value at given coordinate." )

    .def ( "chiSquared", &PyFunctionRep::objectiveValue,
	   "chiSquare ( None ) -> value\n"
	   "\n"
	   "Returns the Chi-Squared." )

    .def ( "objectiveValue", &PyFunctionRep::objectiveValue,
	   "objectiveValue ( None ) -> value\n"
	   "\n"
	   "Returns the objective Value that the fitter minimizes.\n"
	   "Typically it is the Chi-Squared." )

    .def ( "degreesOfFreedom", &PyFunctionRep::degreesOfFreedom,
	   "degressOfFreedom ( None ) -> value\n"
	   "\n"
	   "Returns the number of degrees of freedom a fitter would have." )

//     .def ( "covarianceMatrix", 
// 	   &PyFunctionRep::covarianceMatrix,
// 	   return_value_policy < copy_const_reference > (),
// 	   "Returns the covariance matrix" )

    .def ( "setFixedFlags", &PyFunctionRep::setFixedFlags,
	   "setFixedFlags ( list ) -> None\n"
	   "\n"
	   "Set which parameters should be held fixed during fitting." )

    .def ( "setFitter", &PyFunctionRep::setFitter,
	   "setFitter ( string ) -> None\n"
	   "\n"
	    "Sets the fitter by name from fitter factory." )

    .def ( "getFitterName", &PyFunctionRep::getFitterName,
	   return_value_policy < copy_const_reference > (),
	   "getFitterName ( None ) -> string\n"
	   "\n"
	   "Returns the current fitter name." )

    .def ( "createResidualsDisplay",
	   &PyFunctionRep::createResidualsDisplay,
	   return_value_policy < manage_new_object > (),
	   "createResidualsDisplay ( None ) -> Display\n"
	   "\n"
	   "Returns residuals Display object. The residuals display is an\n"
	   "XY plot showing the difference between the function values and\n"
	   "the target DataRep values." )

    .def ( "setFitRange",
	   &PyFunctionRep::setFitRange,
	   "setFitRange ( low, high ) -> None\n"
	   "\n"
	   "Sets the range of the coordinate axis that is used for fitting." )

    .def ( "setFitRangeEnabled",
	   &PyFunctionRep::setFitRangeEnabled,
	   "setFitRange ( boolean ) -> None\n"
	   "\n"
	   "Enabled use of the fit range" )
	   
    ;
}

} // namespace Python
} // namespace hippodraw

using namespace hippodraw;

PyFunctionRep::PyFunctionRep ( const std::string & name, PyDataRep * rep )
{
  PyApp::lock ();
  try {
    FunctionController * controller = FunctionController::instance ();
    DataRep * datarep = rep -> getDataRep ();
    m_rep = controller -> createFunctionRep ( name, datarep );
    m_target = 0;
  }
  catch ( const FactoryException & e ) {
    PyApp::unlock ();
    throw e;
  }
  PyApp::unlock ();
}

PyFunctionRep::PyFunctionRep ( FunctionBase * function, PyDataRep * rep )
{
  PyApp::lock ();
  try {
    FunctionController * controller = FunctionController::instance ();
    DataRep * datarep = rep -> getDataRep ();
    m_rep = controller -> createFunctionRep ( function, datarep );
    m_target = 0;
  }
  catch ( const FactoryException & e ) {
    PyApp::unlock ();
    throw e;
  }
  PyApp::unlock ();
}

PyFunctionRep::
PyFunctionRep ( FunctionBase * function )
{
  PyApp::lock ();
  try {
    FunctionController * controller = FunctionController::instance ();
    m_rep = controller -> createFunctionRep ( function, 0 );
    m_target = 0;
  }
  catch ( const FactoryException & e ) {
    PyApp::unlock ();
    throw e;
  }
  PyApp::unlock ();
}

DataRep * PyFunctionRep::getRep () const
{
  return m_rep;
}

void PyFunctionRep::addTo ( QtDisplay * display )
{
  PyApp::lock();
  FunctionController * controller = FunctionController::instance ();
  m_target = display->display ();

  try { 
    controller->addFunction ( m_target, m_rep );
    m_target -> setActivePlot ( -1, true );
  }
  catch ( const std::runtime_error & e ) {
    PyApp::unlock ();
    throw e;
  }
  PyApp::unlock ();
}

const vector < std::string > &  PyFunctionRep::parmNames () const
{
  PyApp::lock();
  const vector < std::string > & vec = m_rep->parmNames();
  PyApp::unlock ();

  return vec;
}


const vector < double > &  PyFunctionRep::parameters () const
{
  PyApp::lock();
  const vector < double > & vec = m_rep->parameters ();
  PyApp::unlock ();

  return vec;
}

const vector < double > &  PyFunctionRep::principleErrors () const
{
  PyApp::lock();
  const vector < double > & vec = m_rep -> principleErrors();
  PyApp::unlock ();
  
  return vec;  
}

void PyFunctionRep::setParameters ( const std::vector<double> & params )
{
  PyApp::lock();
  m_rep->setParameters(params);
  PyApp::unlock ();
}

bool PyFunctionRep::fitFunction ()
{
  PyApp::lock();
  FunctionController * controller = FunctionController::instance ();
  bool ok = controller -> fitFunction ( m_target, m_rep );
  PyApp::unlock ();

  return ok;
}

double
PyFunctionRep::
operator () ( double x )
{
  FunctionController * controller = FunctionController::instance ();
  FunctionRep * rep = controller->getComposite ( m_target, m_rep );
  FunctionBase * function = 0;
  if ( rep != 0 ) {
    function = rep->getFunction ();
  }
  else {
    function = m_rep->getFunction();
  }

  return function ->operator() ( x );
 }

/** @bug This function only works correctly if there is only one
    DataRep contained by the plotter.  The constructor of this class
    should remember which DataRep object the function is attached
    to.
*/
double
PyFunctionRep::
objectiveValue()
{
   PyApp::lock();
   FunctionController * controller = FunctionController::instance ();
   double value;
   if (m_target) {
     const DataRep * datarep = m_target -> getDataRep ( 0 );
      value = controller->getObjectiveValue( m_target, datarep );
   } else {
      value = 0;
   }
   PyApp::unlock ();

   return value;
}
const vector < vector < double > > &
PyFunctionRep::
covarianceMatrix ( )
{
  PyApp::lock();
  FunctionController * controller = FunctionController::instance ();
  const vector < vector < double > > & covariance 
    = controller -> getCovarianceMatrix ( m_target );
  PyApp::unlock ();

  return covariance;
}

int
PyFunctionRep::
degreesOfFreedom ()
{
   PyApp::lock();
   FunctionController * controller = FunctionController::instance ();
   int value = 0;
   if (m_target) {
      value = controller->getDegreesOfFreedom(m_target);
   }
   PyApp::unlock ();

   return value;
}

void
PyFunctionRep::
setFixedFlags ( const std::vector < int > & flags )
{
   PyApp::lock();

   m_rep->setFixedFlags ( flags );

   PyApp::unlock ();
}

void
PyFunctionRep::
setFitter ( const std::string & name )
{
  FunctionController * controller = FunctionController::instance ();
  controller -> setFitter ( m_rep, name );
}

const std::string &
PyFunctionRep::
getFitterName ( ) const
{
  return m_rep -> getFitterName ();
}

QtDisplay *
PyFunctionRep::
createResidualsDisplay () const
{
  FunctionController * controller = FunctionController::instance();
  PlotterBase * plotter 
    = controller -> createResidualsDisplay ( m_target, m_rep );
  QtDisplay * display = new QtDisplay ( plotter );

  return display;
}

void
PyFunctionRep::
setFitRange ( double low, double high )
{
  const Range range ( low, high );

  m_rep -> setCutRange ( range );
}

/** @bug @@@@@@ when fit Range is disabled should not display it. */
void
PyFunctionRep::
setFitRangeEnabled ( bool yes )
{
  m_rep -> setCutRange ( yes );
}
