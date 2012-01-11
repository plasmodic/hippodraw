/** @file

hippodraw::FunctionController class implementation

Copyright (C) 2001-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FunctionController.cxx,v 1.274 2007/09/14 22:42:36 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "FunctionController.h"

#include "Gammaq.h"
#include "DisplayController.h"

#include "datareps/CompositeFunctionRep.h"
#include "datareps/FunctionParameter.h"
#include "datareps/FunctionRep1.h"
#include "datareps/FunctionRep2.h"

#include "datasrcs/DataSourceController.h"
#include "datasrcs/NTuple.h"
#include "datasrcs/TupleCut.h"

#include "functions/FunctionBase.h"
#include "functions/FunctionFactory.h"

#include "graphics/ViewBase.h"

#include "minimizers/Fitter.h"
#include "minimizers/FitterFactory.h"
#include "minimizers/NumLinAlg.h"

#include "pattern/string_convert.h"
#include "plotters/PlotterBase.h"
#include "plotters/CompositePlotter.h"
#include "projectors/BinningProjector.h"
#include "projectors/NTupleProjector.h"

#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>

#include <cmath>
#include <cassert>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::distance;
using std::find;
using std::find_if;
using std::list;
using std::mem_fun;
using std::string;
using std::vector;
using std::sqrt;
using std::cos;
using std::sin;
using std::atan;
using std::min;
using std::max;
using std::abs;
#endif

using namespace hippodraw;

using namespace Numeric;

FunctionController * FunctionController::s_instance = 0;

FunctionController::FunctionController ( )
  : m_plotter ( 0 ), 
    m_x ( 0 ), 
    m_y ( 0 ),
    m_confid_count ( 0 )
{
  m_deltaXSq.resize( 6 );

  m_deltaXSq[ 0 ] = 15.1;
  m_deltaXSq[ 1 ] = 18.4;
  m_deltaXSq[ 2 ] = 21.1;
  m_deltaXSq[ 3 ] = 23.5;
  m_deltaXSq[ 4 ] = 25.7;
  m_deltaXSq[ 5 ] = 27.8;
}

FunctionController::~FunctionController ( )
{
}

FunctionController * FunctionController::instance ( )
{
  if ( s_instance == 0 ) {
    s_instance = new FunctionController ( );
  }
  return s_instance;
}

const vector < string > &
FunctionController::
getFitterNames () const
{
  FitterFactory * factory = FitterFactory::instance ();

  return factory -> names ();
}

const std::string &
FunctionController::
getDefaultFitter () const
{
  FitterFactory * factory = FitterFactory::instance ();

  return factory -> getDefault ();
}

int FunctionController::
getUniqueNonFunctionIndex ( const PlotterBase * plotter ) const
{
  int index = -1;
  int count = 0;

  int number = plotter->getNumDataReps ();
  for ( int i = 0; i < number; i++ ) {
    DataRep * r = plotter->getDataRep ( i );
    FunctionRep * fr = dynamic_cast < FunctionRep * > ( r );
    if ( fr != 0 ) continue;
    index = i;
    count++;
  }
  if ( count != 1 ) index = -1;

  return index;
}

void
FunctionController::
findFunctions ( const PlotterBase * plotter ) const
{
  m_func_reps.clear ();
  int number = plotter->getNumDataReps ();

  for ( int i = 0; i < number; i++ ) {
    DataRep * rep = plotter->getDataRep ( i );
    FunctionRep * frep = dynamic_cast < FunctionRep * > ( rep );
    if ( frep == 0 ) continue;
    m_func_reps.push_back ( frep );
  }
}

FunctionRep * 
FunctionController::
createFunctionRep ( const std::string & name,
		    DataRep * rep )
{
  FunctionFactory * factory = FunctionFactory::instance ();
  FunctionBase * function = factory -> create ( name );
  if ( function == 0 ) {
    string what ( "FunctionController: Unable to create function of type `" );
    what += name;
    what += "'";
    throw std::runtime_error ( what );
  }

  return createFunctionRep ( function, rep );
}

FunctionRep * 
FunctionController::
createFunctionRep ( FunctionBase * function, 
		    DataRep * rep )
{
  FunctionRep * frep = 0;

  if ( function -> isComposite () ) {
    frep = new CompositeFunctionRep ( function, rep );
  }
  else {
    unsigned int dims = function -> dimensions ();
    if ( dims == 2 ) {
      frep = new FunctionRep2 ( function, rep );
    }
    else {
      frep = new FunctionRep1 ( function, rep );
    }
  }

  if ( rep != 0 ) {// only makes sense to add a fitter if function has a target
    FitterFactory * factory = FitterFactory::instance ();
    const string & name = factory -> getDefault ();
    bool ok = setFitter ( frep, name );
    if ( ok == false ) {
      delete frep;
      frep = 0;
    }
  }

  return frep;
}

/** @deprecated This function is here to maintain compatibility with
    Python programs using the SIP interface.
 */
FunctionBase *
FunctionController::
addFunction ( PlotterBase * plotter, const std::string & name )
{
  DataRep * rep = plotter -> getDataRep ( 0 );
  FunctionRep * frep = getFunctionRep ( plotter );// might be null
  addFunction ( plotter, name, frep, rep );
  
  frep = getFunctionRep ( plotter ); // now should be LinearSum

  return frep -> getFunction ();
}

/** @request Check that previously set fixed flags and/or limits get
    propagated to new linear sum function.
 */
FunctionRep *
FunctionController::
addFunction ( PlotterBase * plotter,
	      const std::string & name,
	      FunctionRep * frep,
	      DataRep * rep )
{
  FunctionRep * func_rep = createFunctionRep ( name, rep );
  func_rep -> initializeWith ( rep );

  return addFunctionRep ( plotter, rep, frep, func_rep );
}

void
FunctionController::
addFunction ( PlotterBase * plotter, 
	      FunctionRep * func_rep )
{
  int index = plotter->activePlotIndex ();

  if ( index < 0 ) {
    index = getUniqueNonFunctionIndex ( plotter );
  }
  if ( index >= 0 ) {
    plotter->setActivePlot ( index, false );
    DataRep * drep = plotter->getDataRep ( index );
    func_rep -> initializeWith ( drep );
    fillFunctionReps ( m_func_reps, plotter, drep );
    FunctionRep * frep = 0;
    if ( m_func_reps.empty () == false ) {
      frep = m_func_reps.front ();
    }
    addFunctionRep ( plotter, drep, frep, func_rep );
  }
}


void
FunctionController::
addDataRep ( PlotterBase * plotter, DataRep * rep )
{
  FunctionRep * frep = dynamic_cast < FunctionRep * > ( rep );
  if ( frep == 0 ) { // not a function rep
    DisplayController * controller = DisplayController::instance ();
    controller -> addDataRep ( plotter, rep );
  }
  else { // a function rep
    if ( frep -> isComposite () == false ) {
      DataRep * target = frep -> getTarget ();
      addFunctionRep ( plotter, target, 0, frep );
    }
  }
}

void
FunctionController::
setTupleCut ( FunctionRep * rep )
{
  rep -> setTupleCut ();
  rep -> setCutRange ( true );
}

void
FunctionController::
setTupleCut ( const PlotterBase * plotter, DataRep * data_rep )
{
  data_rep -> addCut ();
  FunctionRep * func_rep = getFunctionRep ( plotter, data_rep );
  if ( func_rep != 0 ) {
    setTupleCut ( func_rep );
  }
}

void
FunctionController::
removeTupleCut ( const PlotterBase * plotter, DataRep * data_rep )
{
  FunctionRep * func_rep = getFunctionRep ( plotter, data_rep );
  if ( func_rep != 0 ) {
    func_rep -> removeCut ();
  }
}

FunctionRep *
FunctionController::
addFunctionRep ( PlotterBase * plotter,
		 DataRep * drep,
		 FunctionRep * frep,
		 FunctionRep * func_rep )
{
  FunctionRep * return_rep = frep;

  plotter->addDataRep ( func_rep );

  fillFunctionReps ( m_func_reps, plotter, drep );

  FitterFactory * factory = FitterFactory::instance ();
  const string & name = factory -> getDefault ();
  setFitter ( func_rep, name );

  if ( frep != 0 ) {
    if ( frep -> isComposite () ) {
      frep -> addToComposite ( func_rep );
    }
    else {
      FunctionRep * composite = createFunctionRep ( "Linear Sum", drep );
      composite -> initializeWith ( drep );
      plotter -> addDataRep ( composite );
      setFitter ( composite, name );
      setTupleCut ( func_rep );
      composite -> addToComposite ( frep );
      composite -> addToComposite ( func_rep );
      return_rep = composite;
    }
  }
  func_rep->notifyObservers ();
  setTupleCut ( func_rep );
  if ( return_rep == 0 ) return_rep = func_rep;

  return return_rep;
}

void
FunctionController::
removeFunction ( PlotterBase * plotter,
		 FunctionRep * frep )
{
  if ( frep -> isComposite () ) {
    CompositeFunctionRep * composite
      = dynamic_cast < CompositeFunctionRep * > ( frep );
    const vector < FunctionRep * > & freps = composite -> getFunctionReps ();
    unsigned int size = freps.size();

// remove from end so we don't destroy validty of reference vector
    for ( int i = size -1; i >= 0; i-- ) {
      FunctionRep * rep = freps[i];
      plotter -> removeDataRep ( rep );
      composite -> removeFromComposite ( rep );
      delete rep;
    }
    plotter -> removeDataRep ( frep );
    delete frep;
  }
  else {
    fillFunctionReps ( m_func_reps, plotter, 0 ); // get all
    vector < FunctionRep * > :: iterator it 
      = find ( m_func_reps.begin(), m_func_reps.end (),
	       frep );
    assert ( it != m_func_reps.end () );
    m_func_reps.erase ( it );
 
    plotter -> removeDataRep ( frep );
    for ( unsigned int i = 0; i < m_func_reps.size (); i++ ) {
      FunctionRep * rep = m_func_reps[i];
      if ( rep -> isComposite () ) {
	rep -> removeFromComposite ( frep );
      }
    }
    // Remove all composites who now have one or none functions within them.
  
    vector < FunctionRep * >::iterator iter = m_func_reps.begin ();
  
    // This while loop is tricky because modifying the vector that we
    // are iterating over.   
    while ( iter != m_func_reps.end() ){
      FunctionRep * rep = *iter;
      if ( rep -> isComposite () ) {
	CompositeFunctionRep * composite
	  = dynamic_cast < CompositeFunctionRep * > ( rep );
	if ( composite -> count() < 2 ) {
	  plotter->removeDataRep ( composite );
	  const CompositeFunctionRep::FunctionRepList_t & reps 
	    = composite -> getFunctionReps ();
	  FunctionRep * crep = reps.front ();
	  crep -> setInComposite ( false );
	  delete composite;
	  iter = m_func_reps.erase ( iter ); // updates iter 
	}
	else {
	  iter ++;
	}
      }
      else {
	iter ++;
      }
    }
    delete frep; // the functionrep
  }
}

bool
FunctionController::
hasFunction ( const PlotterBase * plotter, const DataRep * rep )
{
  bool yes = false;
  assert ( plotter != 0 );

  fillFunctionReps ( m_func_reps, plotter, rep );
  yes = m_func_reps.empty () == false;

  return yes;
}

const vector< string > & 
FunctionController::
functionNames ( PlotterBase * plotter, DataRep * rep )
{
  fillFunctionReps ( m_func_reps, plotter, rep );

  m_f_names.clear ();
  vector< FunctionRep * >:: const_iterator it = m_func_reps.begin ();

  for ( ; it != m_func_reps.end (); ++it ) {
    FunctionRep * fp = *it;
    if ( rep != 0 && fp->getTarget() != rep ) continue;
    FunctionBase * function = fp->getFunction ();
    const string & name = function->name ();
    m_f_names.push_back ( name );
  }

  return m_f_names;
}

void
FunctionController::
fillFunctionReps ( std::vector < FunctionRep * > & freps,
		   const PlotterBase * plotter,
		   const DataRep * drep ) const
{
  freps.clear ();
  int number = plotter -> getNumDataReps ();
  for ( int i = 0; i < number; i++ ) {
    DataRep * rep = plotter -> getDataRep ( i );
    FunctionRep * frep = dynamic_cast < FunctionRep * > ( rep );
    if ( frep != 0 ) {
      if ( drep != 0 ) {
	if ( frep -> getTarget () == drep ) {
	  freps.push_back ( frep );
	}
      }
      else { // any target
	freps.push_back ( frep );
      }
    }
  }
}

void
FunctionController::
fillTopLevelFunctionReps ( std::vector < FunctionRep * > & freps,
			   const PlotterBase * plotter,
			   const DataRep * drep ) const
{
  freps.clear ();
  fillFunctionReps ( m_func_reps, plotter, drep );
  unsigned int size = m_func_reps.size ();
  for ( unsigned int i = 0; i < size; i++ ) {
    FunctionRep * rep = m_func_reps[i];
    if ( rep -> isInComposite () == false ) {
      freps.push_back ( rep );
    }
  }
}

void
FunctionController::
setErrorsFromComposite ( const PlotterBase * plotter,
			 const FunctionRep * composite )
{
  const vector < double > & errors = composite -> principleErrors();
  vector < double >::const_iterator begin = errors.begin();

  DataRep * target = composite -> getTarget ();
  fillFunctionReps ( m_func_reps, plotter, target );

  vector < FunctionRep * >::iterator first = m_func_reps.begin();

  while ( first != m_func_reps.end() ) {
    FunctionRep * rep = *first++;
    if ( rep -> isComposite() ) continue;
    const vector < double > & t = rep -> principleErrors ();
    unsigned int number = t.size();
    rep->setPrincipleErrors ( begin, 
			      begin + number );
    begin += number;
  }

}

bool
FunctionController::
fitFunction ( PlotterBase * plotter, unsigned int )
{
  FunctionRep * rep = getFunctionRep ( plotter );

  return fitFunction ( plotter, rep );
}

FunctionRep *
FunctionController::
getComposite ( const PlotterBase * plotter, FunctionRep * rep )
{
  const DataRep * target = rep -> getTarget ();
  fillFunctionReps ( m_func_reps, plotter, target );
  for ( unsigned int i = 0; i < m_func_reps.size (); i++ ) {
    FunctionRep * fr = m_func_reps[i];
    if ( fr -> isComposite () ) {
      CompositeFunctionRep * composite 
	= dynamic_cast < CompositeFunctionRep * > ( fr );
      bool yes = composite -> isMember ( rep );
      if ( yes ) {
	rep = composite;
	break;
      }
    }
  }

  return rep;
}

bool
FunctionController::
fitFunction ( PlotterBase * plotter, FunctionRep * rep )
{
  rep = getComposite ( plotter, rep );
  bool ok = rep -> fitFunction ();
  if ( rep -> isComposite () ) {
    setErrorsFromComposite ( plotter, rep );
  }

  return ok;
}

bool
FunctionController::
tryFitFunction ( PlotterBase * plotter, FunctionRep * func_rep )
{
  saveParameters ( plotter );
  double chi_sq = func_rep -> objectiveValue ();
  bool ok = fitFunction ( plotter, func_rep );
  if ( ok ) {
    double new_chi_sq = func_rep -> objectiveValue ();
    if ( new_chi_sq > chi_sq ) {
      restoreParameters ( plotter );
    }
  } else {
    restoreParameters ( plotter );
  }

  return ok;
}

void
FunctionController::
setFitRange ( PlotterBase * plotter, const Range & range )
{
  if ( hasFunction ( plotter, 0 ) ) {
    FunctionRep * frep = getFunctionRep ( plotter );
    frep -> setCutRange ( range );
    plotter -> update ();
  }
}

void
FunctionController::
setFitRange ( PlotterBase * plotter, double low, double high )
{
  const Range range ( low, high );
  setFitRange ( plotter, range );
}

void FunctionController::saveParameters ( PlotterBase * plotter )
{
  findFunctions ( plotter );
  vector< FunctionRep * >::iterator it = m_func_reps.begin ();

  for ( ; it != m_func_reps.end (); ++it ) {
    FunctionRep * frep = *it;
    FunctionBase * function = frep->getFunction ();
    if ( function->isComposite () ) continue;
    frep->saveParameters ();
  }
}

void FunctionController::restoreParameters ( PlotterBase * plotter )
{
  findFunctions ( plotter );
  vector< FunctionRep * >::iterator it = m_func_reps.begin ();

  for ( ; it != m_func_reps.end (); ++it ) {
    FunctionRep * frep = *it;
    FunctionBase * function = frep->getFunction ();
    if ( function->isComposite () ) continue;
    frep->restoreParameters ();
  }

}

FunctionRep *
FunctionController::
getFunctionRep ( const PlotterBase * plotter ) const
{
  FunctionRep * frep = 0;
  DataRep * rep = 0;
  int index = plotter->activePlotIndex ();

  if ( index >= 0 ) {
    rep = plotter -> getDataRep ( index );
  }
  else {
    rep = plotter -> getTarget ();
  }

  FunctionRep * test = dynamic_cast < FunctionRep * > ( rep );
  if ( test != 0 ) { // if ctrl-clicked, could be the function
    frep = test;
  }
  else {
    frep = getFunctionRep ( plotter, rep );
  }

  return frep;
}

/** @bug @@@@@@ When multiple independent functions are allowed, this
    method will fail.
 */
FunctionRep *
FunctionController::
getFunctionRep ( const PlotterBase * plotter, const DataRep * datarep ) const
{
  FunctionRep * frep = 0;

  fillFunctionReps ( m_func_reps, plotter, datarep );

  for ( unsigned int i = 0; i < m_func_reps.size(); i++ ) {
    FunctionRep * rep = m_func_reps[i];
    const DataRep * drep = rep -> getTarget ();
    if ( drep != datarep ) continue;
    frep = rep;
    if ( frep -> isComposite () ) break; // use composite if found
  }

  return frep;
}

ViewBase * FunctionController::
createFuncView ( const ViewFactory * factory,
		 PlotterBase * plotter,
		 const std::string & name )
{
  FunctionRep * frep = getFunctionRep ( plotter );
  assert ( frep != 0 );

  DisplayController * controller = DisplayController::instance ();
  //string nullstring ("");
  string nullstring ("");

  ViewBase * view = controller->createTextView ( factory, frep, name, 
      nullstring );
  assert ( view != 0 );

  // Set the parent plotter of the text plotter.
  view->getPlotter()->setParentPlotter(plotter);

  return view;
}

bool
FunctionController::
setFitter ( const PlotterBase * plotter, const std::string & name )
{
  FunctionRep * frep = getFunctionRep ( plotter );
  bool ok = false;
  if ( frep != 0 ) {
    ok = setFitter ( frep, name );
  }

  return ok;
}

const string &
FunctionController::
getFitterName ( const PlotterBase * plotter )
{
  FunctionRep * frep = getFunctionRep ( plotter );
  Fitter * fitter = frep -> getFitter ();

  return fitter -> name ();
}

Fitter *
FunctionController::
getFitter ( const PlotterBase * plotter )
{
  FunctionRep * frep = getFunctionRep ( plotter );
  Fitter * fitter = frep -> getFitter ();

  return fitter;
}


bool
FunctionController::
setFitter ( FunctionRep * frep, const std::string & name )
{
  FitterFactory * factory = FitterFactory::instance ();
  Fitter * fitter = factory -> create ( name );

  return frep -> setFitter ( fitter );
}

void
FunctionController::
setDefaultFitter ( const std::string & name )
{
  FitterFactory * factory = FitterFactory::instance ();
  factory -> setDefault ( name );
}

bool
FunctionController::
changeFitter ( const PlotterBase * plotter,
	       const DataRep * drep,
	       const std::string & name )
{
  FitterFactory * factory = FitterFactory::instance ();
  Fitter * proto = factory -> prototype ( name );

  FunctionRep * frep = getFunctionRep ( plotter, drep );
  const FunctionBase * func = frep -> getFunction ();
  bool yes = proto -> isCompatible ( func );
  if ( yes ) {
    const Fitter * old_fitter = frep -> getFitter ();
    Fitter * new_fitter = factory -> create ( name );
    new_fitter -> copyFrom ( old_fitter );
    frep -> setFitter ( new_fitter );
  }
  return yes;
}

double
FunctionController::
getObjectiveValue ( const PlotterBase * plotter, const DataRep * datarep )
{
  FunctionRep * rep = getFunctionRep ( plotter, datarep );

  return rep -> objectiveValue ();
}

const vector < vector < double > > &
FunctionController::
getCovarianceMatrix ( const PlotterBase * plotter )
{
  FunctionRep * rep = getFunctionRep ( plotter );

  return rep -> covarianceMatrix ();
}

/** @deprecated This function will only work correctly if there is only one
    DataRep contained by the plotter.  It remain here for backward
    compatibility with the SIP interface.
*/
double
FunctionController::
getChiSquared ( const PlotterBase * plotter )
{
  const DataRep * rep = plotter -> getTarget ();

  return getObjectiveValue ( plotter, rep );
}

int
FunctionController::
getDegreesOfFreedom ( const PlotterBase * plotter )
{
  FunctionRep * rep = getFunctionRep ( plotter );

  return rep -> degreesOfFreedom ();
}

NTuple *
FunctionController::
createNTuple ( const PlotterBase * plotter, const FunctionRep * rep )
{
  NTuple * ntuple = 0;
  if ( rep == 0 ) {
    int old_index = plotter -> activePlotIndex ();
    int index = getUniqueNonFunctionIndex ( plotter );
    if ( index < 0 ) return 0;

    int saved_index = plotter -> activePlotIndex ();
    PlotterBase * pb = const_cast < PlotterBase * > ( plotter );
    pb -> setActivePlot ( index, false ); // no redraw
    ntuple = plotter -> createNTuple ();
    pb -> setActivePlot ( saved_index, false );
    rep = getFunctionRep ( plotter );
    pb -> setActivePlot ( old_index, true );
  }
  else {
    const DataRep * target = rep -> getTarget ();
    ntuple = target -> createNTuple ();
  }
  if ( rep != 0 ) {
    const FunctionBase * function = rep -> getFunction ();
    vector < double > & x = ntuple -> getColumn ( 0 ); // X coordinate
    vector < double > & y = ntuple -> getColumn ( 1 ); // Y coordinate
    unsigned int size = ntuple -> rows ();
    vector < double > values ( size );
    vector < double > residuals ( size );
  
    for ( unsigned int i = 0; i < x.size(); i++ ) {
      values [i] = function -> operator () ( x[i] );
      residuals [i] =  y[i] - values [i];
    }

    ntuple -> addColumn ( function->name (), values );
    ntuple -> addColumn ( "Residuals", residuals );
  }

  return ntuple;
}

PlotterBase *
FunctionController::
createResidualsDisplay ( PlotterBase * plotter, const FunctionRep * frep )
{
  NTuple * ntuple = createNTuple ( plotter, frep );
  ntuple -> setTitle ( plotter -> getTitle () );
  const string old_label ( "Residuals" );
  DataSourceController::instance () -> registerNTuple ( ntuple );
  vector < string > bindings ( 4 );
  bindings[0] = ntuple -> getLabelAt ( 0 );
  bindings[1] = old_label;
  bindings[2] = "nil";
  bindings[3] = ntuple -> getLabelAt ( 3 );
  bool axis_scaled = plotter -> isAxisScaled ( Axes::Y );
  if ( axis_scaled ) {
    int i = ntuple -> indexOf ( old_label );
    const string new_label ("Residuals (scaled)"); 
    ntuple -> setLabelAt ( new_label , i );
    bindings[1] = new_label;
  }
  DisplayController * controller = DisplayController::instance ();

// Make scaling of x-axis match that of the original plot.
  PlotterBase * new_plotter = controller -> createDisplay ( "XY Plot",
                                                            *ntuple,
                                                            bindings );
  if ( axis_scaled ) {
    double factor = plotter -> getScaleFactor ( Axes::Y );
    new_plotter -> setScaleFactor ( Axes::Y, factor );
  }
  controller->setLog( new_plotter, "x", 
                      controller -> getLog( plotter, "x" ) );
  
  return new_plotter;
}

PlotterBase *
FunctionController::
createNewEllipsoidDisplay ( PlotterBase * masterPlot, FunctionRep * rep )
{
  int n = 100;
  double xmin, xmax, ymin, ymax;
  
  // Set up the NTuple
  NTuple* ntuple = new( NTuple );
  ellipsoidNTuple ( masterPlot, rep, ntuple, n, xmin, xmax, ymin, ymax );
  ntuple -> setTitle ( masterPlot -> getTitle () );
  
  vector < string > bindings ( 1 );
  bindings[0] = ntuple ->  getLabelAt ( 0 );
  
  DisplayController * dcontroller = DisplayController::instance ();

  // Z plot. X and Y coordinates are merely indices
  PlotterBase * ellipsePlot = dcontroller ->
    createDisplay ( "Image", *ntuple, bindings );
  
  // Rescale and translate the Z plot so that  the plot
  // becomes contained in the rectangle bound by xmin, xmax
  // ymin and ymax.
  ellipsePlot -> setBinWidth( "x", ( xmax - xmin ) / ( n - 1.0 ) );
  ellipsePlot -> setBinWidth( "y", ( ymax - ymin ) / ( n - 1.0 ) );
  
  ellipsePlot -> setOffsets ( xmin, ymin );
  
  ellipsePlot -> setRange ( string( "X" ), xmin, xmax );
  ellipsePlot -> setRange ( string( "Y" ), ymin, ymax );
  
  // Establish the relation between source masterPlot and
  // this new plot. This relationship shall be exploited
  // when we refresh the error plots.
  int index = dcontroller -> activeDataRepIndex( masterPlot );
  ellipsePlot -> setParentDataRepIndex( index );
  ellipsePlot -> setParentPlotter( masterPlot );
  
  return ellipsePlot;
}

void
FunctionController::
ellipsoidNTuple ( PlotterBase* masterPlot, FunctionRep * frep,
		  NTuple* nt, int n,
		  double & xmin, double & xmax,
		  double & ymin, double & ymax )
{
  string text ( "Confidence ellipse " );
  text += String::convert ( ++m_confid_count );
  nt -> setName ( text );

  DataSourceController * controller = DataSourceController::instance ();
  controller -> registerNTuple ( nt );

  assert( frep );
  
  // Get projected covariance i.e. take the sub-matrix
  // out of the covariance matrix which corresponds
  // to the 2 parameters of interest whose correlation
  // we would like to see.
  vector< vector< double > > Sigma( 2 );
  Sigma[0].resize( 2, 0.0 );
  Sigma[1].resize( 2, 0.0 );

  const vector < vector < double > > & covariance 
    = frep -> covarianceMatrix ();  

  Sigma[0][0] = covariance [m_x][m_x];
  Sigma[0][1] = covariance [m_x][m_y];
  Sigma[1][0] = covariance [m_y][m_x];
  Sigma[1][1] = covariance [m_y][m_y];

  // Invert the projected covariance 
  vector< vector< double > > SigmaInv;
  invertMatrix( Sigma, SigmaInv );
  
  // Decide the center of the ellipse to be parameter
  // value of the 2 parameters of interest whose correlation
  // we would like to see.
  vector< double > xbar( 2 );
  
  const Fitter * fitter = getFitter ( masterPlot );
  vector < double > free_parms;
  fitter -> fillFreeParameters ( free_parms );
  xbar[ 0 ] = free_parms [ m_x ];
  xbar[ 1 ] = free_parms [ m_y ];
  
  // Get the bounding ellipse and its bounds. Bounding ellipse is the
  // 99.99% confidence ellipsoid. For mu = 2 the delta chi-square turns from 
  // Numerical Recipes in C as 18.4. etc etc.
  unsigned int mu = free_parms.size();
  NTuple * boundingTuple = ellipse( xbar, SigmaInv, m_deltaXSq[ mu - 1 ] ); 
  
  xmin = boundingTuple -> minElement ( 0 );
  xmax = boundingTuple -> maxElement ( 0 );
  ymin = boundingTuple -> minElement ( 1 );
  ymax = boundingTuple -> maxElement ( 1 );
  
  delete boundingTuple; // Served its purpose
  
  // Create the NTuple for the contour plot
  // This NTuple is to be build column-wise
  // keeping in view the way Z-plot works.
  vector< double > p( n * n ), a( 2 );
  
  double dx    = ( xmax - xmin ) / ( n - 1.0 );
  double dy    = ( ymax - ymin ) / ( n - 1.0 );
  double delta = 0.0;
  
  for( int i = 0; i < n; i++ )
    for( int j = 0; j < n; j++ )
      {
	a[ 0 ] = xmin + i * dx - xbar[ 0 ] ;
	a[ 1 ] = ymin + j * dy - xbar[ 1 ];
	delta  = quadraticProduct( SigmaInv, a );
	p[ n * i + j ] = 1 - gammq( mu / 2.0, delta / 2.0 ); 
      }

  nt -> clear();
  nt -> addColumn( "Percent", 100 * p );
  
  return;
}

PlotterBase *
FunctionController::
refreshEllipsoidDisplay ( PlotterBase * plot, FunctionRep * frep )
{
  int n = 100;
  double xmin, xmax, ymin, ymax;
  
  PlotterBase* masterPlot = plot->getParentPlotter();

  // Set up the NTuple
  NTuple* ntuple = new( NTuple );
  ellipsoidNTuple ( masterPlot, frep, ntuple, n, xmin, xmax, ymin, ymax );
  ntuple -> setTitle ( masterPlot -> getTitle () );
  
  // First get the selected DataRep from the plotter. The DataRep 
  // will have a projector that is a NTupleProjector. It doesn't
  // know, but we do, so we downcast and set the NTuple.
  DataRep * drep = plot -> selectedDataRep();
  NTupleProjector * ntProjector =
    dynamic_cast < NTupleProjector * > ( drep -> getProjector() );
  ntProjector -> setNTuple ( ntuple );
  
  NTuple * nt = const_cast < NTuple * > ( ntuple );
  nt  -> addObserver ( ntProjector );
    
  // Rescale and translate the Z plot so that  the plot
  // becomes contained in the rectangle bound by xmin, xmax
  // ymin and ymax.
  plot -> setBinWidth( "x", ( xmax - xmin ) / ( n - 1.0 ) );
  plot -> setBinWidth( "y", ( ymax - ymin ) / ( n - 1.0 ) );
  
  plot -> setOffsets ( xmin, ymin );
  
  plot -> setRange ( string( "X" ), xmin, xmax );
  plot -> setRange ( string( "Y" ), ymin, ymax );
    
  return plot;
}

NTuple * 
FunctionController::
ellipse ( const std::vector< double > & xbar,
	  std::vector< std::vector < double > > & SigmaInv,
	  double Csquare )
{
  // First argument should be a 2 D vector, the center of the ellipse //
  assert( xbar.size() == 2 );

  // Second argument should be a 2 x 2 SPD matrix i.e. two rows two cols //
  assert( SigmaInv.size() == 2 );
  assert( SigmaInv[0].size() == 2 || SigmaInv[1].size() == 2 );
  
  // Second argument should be a 2 x 2 -- Symmetric --  PD matrix
  // Under numerical round-offs it might not be true so we take mean of the
  // entries.
  double temp = ( SigmaInv[0][1] + SigmaInv[1][0] ) / 2;
  SigmaInv[0][1] = temp;
  SigmaInv[1][0] = temp;
  
  // Third argument should be a positive scalar "c^2", Square of "Radius" // 
  assert( Csquare > DBL_EPSILON );
  
  // The eigenvalues of the SigmaInv matrix
  double b = -( SigmaInv[0][0] + SigmaInv[1][1] );
  double c = SigmaInv[0][0] * SigmaInv[1][1] - SigmaInv[0][1] * SigmaInv[1][0];

  double lambda1 = ( -b + sqrt( b * b - 4 * c ) ) / 2;
  double lambda2 = ( -b - sqrt( b * b - 4 * c ) ) / 2;
    
  // Determining the angles the ellipse axis make with X-axis
  double alpha1 =  atan( (SigmaInv[0][0] - lambda1) / SigmaInv[0][1] );
  double a      =  sqrt( Csquare / lambda1 );
  b             =  sqrt( Csquare / lambda2 );

  // Creating a rotation matrix
  double Rot00 =  cos( alpha1 );
  double Rot11 =  cos( alpha1 );
  double Rot01 = -sin( alpha1 );
  double Rot10 =  sin( alpha1 );

  // Generating N-points on the ellipse
  int N = 100;
  vector< double > x, y;
  
  x.resize( N, 0.0 );
  y.resize( N, 0.0 );
  
  for( int i = 0; i < N; i++)
    {
      // Unrotated untranslated point
      double theta = ( 2 * M_PI * i ) / ( (double) (N-1) );
      double x0 = a * cos( theta );
      double x1 = b * sin( theta );
      
      //x = Rot * x; i.e. force in a rotation
      double xrot0 = Rot00 * x0 + Rot01 * x1;
      double xrot1 = Rot10 * x0 + Rot11 * x1;
      
      //x = x + xbar; i.e. force in a translation
      x[i] = xrot0 + xbar[0];
      y[i] = xrot1 + xbar[1];
    }
  
  // Create an NTuple out of the above vector
  NTuple* ntuple = new( NTuple );
  ntuple -> addColumn( "X", x );
  ntuple -> addColumn( "Y", y );
  
  return ntuple;
}

int FunctionController::setEllpsoidParamIndex( hippodraw::Axes::Type axes,
					       int index )
{
  assert( axes == Axes::X || axes == Axes::Y );

  if( axes == Axes::X )
    m_x = index;
  
  if( axes == Axes::Y )
    m_y = index;
      
  return EXIT_SUCCESS;
}

bool
FunctionController::
functionExists ( const std::string & name )
{
  FunctionFactory * factory = FunctionFactory::instance ();

  return factory -> exists ( name );
}

bool
FunctionController::
isCompatible ( const std::string & function,
	       const std::string & fitter )
{
  bool yes = true;

  FunctionFactory * fun_fac = FunctionFactory::instance ();
  FunctionBase * fun_proto = fun_fac -> prototype ( function );

  FitterFactory * fit_fac = FitterFactory::instance ();
  Fitter * fit_proto = fit_fac -> prototype ( fitter );

  yes = fit_proto -> isCompatible ( fun_proto );

  return yes;
}

const vector < string > &
FunctionController::
getFunctionNames () const
{
  return FunctionFactory::instance () -> names ();
}
