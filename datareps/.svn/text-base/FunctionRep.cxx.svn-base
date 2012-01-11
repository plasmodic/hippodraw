/** @file

hippodraw::FunctionRep class implementation

Copyright (C) 2002-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FunctionRep.cxx,v 1.115 2007/09/14 22:42:36 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "FunctionRep.h"

#include "FunctionParameter.h"

#include "datasrcs/DataSource.h"
#include "datasrcs/TupleCut.h"

#include "functions/FunctionBase.h"
#include "minimizers/Fitter.h"
#include "projectors/FunctionProjector.h"
#include "reps/CutRangeRep.h"

#include <cassert>

using std::string;
using std::vector;

using namespace hippodraw;

FunctionRep::
FunctionRep ( FunctionBase * function,DataRep * rep )
  : m_in_composite ( false )
{
  m_name = "Function";
  m_target = rep;

  ProjectorBase * target = 0;

  if ( m_target != 0 ) {
    m_target -> addObserver ( this );

    const DataSource * ntuple = rep -> getProjectedValues ();
    DataSource * nt = const_cast < DataSource * > ( ntuple );
    nt -> addObserver ( this );
    target = rep -> getProjector ();
  }

  m_projector = new FunctionProjector ( function, target );

  // The following is needed to ensure FunctionProjector::principleErrors()
  // knows the correct number of parameters for this function.
  dynamic_cast<FunctionProjector *>(m_projector)->saveParameters();

}

FunctionRep::
FunctionRep ( const FunctionRep & rep )
  : DataRep ( rep )
{
  m_cut_rep = rep.m_cut_rep -> clone ();
}

FunctionRep::
~FunctionRep ()
{
  if ( m_target != 0 ) {
    m_target -> removeObserver ( this );

    const DataSource * target = m_target -> getProjectedValues ();
    DataSource * nt = const_cast < DataSource * > ( target );
    nt -> removeObserver ( this );
  }
}

DataRep * FunctionRep::clone ()
{
  return new FunctionRep ( *this );
}

bool FunctionRep::hasNTupleBindings () const
{
  return false;
}

void FunctionRep::initializeWith ( DataRep * rep )
{
  m_target = rep;
  ProjectorBase * projector = rep -> getProjector ();

  FunctionProjector * fp = dynamic_cast < FunctionProjector * > ( m_projector);

  fp->initializeFunction ( projector );
}

FunctionBase * FunctionRep::getFunction () const
{
  FunctionProjector * fp = dynamic_cast < FunctionProjector * > ( m_projector);

  return fp->function ();
}

DataRep * FunctionRep::getTarget () const
{
  return m_target;
}

void FunctionRep::setTarget ( DataRep * target )
{
  m_target = target;
}

bool FunctionRep::isComposite () const
{
  FunctionBase * function = getFunction();
  return function->isComposite ();
}

bool
FunctionRep::
isInComposite () const
{
  return m_in_composite;
}

void
FunctionRep::
setInComposite ( bool yes )
{
  m_in_composite = yes;
}

bool
FunctionRep::
setFitter ( Fitter * fitter )
{
  if ( fitter -> needsIntegrated () == true ) {
    bool yes = m_target -> isAxisBinned ( Axes::X );
    if ( ! yes ) return false;
  }

  FunctionBase * function = getFunction ();
  fitter -> setFunction ( function );

  const DataSource * target = m_target -> getProjectedValues ();
  fitter -> setDataSource ( target );

  FunctionProjector * fp = dynamic_cast < FunctionProjector * > ( m_projector);
  assert ( fp );

  fp -> setFitter ( fitter );

  return true;
}

Fitter *
FunctionRep::
getFitter ( ) const
{
  FunctionProjector * fp = dynamic_cast < FunctionProjector * > ( m_projector);
  assert ( fp );

  return fp -> getFitter ();
}

const string &
FunctionRep::
getFitterName () const
{
  Fitter * fitter = getFitter ();

  return fitter -> name ();
}

bool FunctionRep::fitFunction ( )
{

  FunctionProjector * fp = dynamic_cast < FunctionProjector * > ( m_projector);
  assert ( fp );

  bool ok = fp->fitFunction ( );
  
  fp -> calcPrincipleErrors ();
  notifyObservers ();

  return ok;
}

double
FunctionRep::
objectiveValue () const
{
  FunctionProjector * fp = dynamic_cast < FunctionProjector * > ( m_projector);

  return fp -> objectiveValue ();
}

const vector < vector < double > > &
FunctionRep::
covarianceMatrix () const
{
  FunctionProjector * projector 
    = dynamic_cast < FunctionProjector * > ( m_projector );

  return projector -> covariance ();
}

int
FunctionRep::
degreesOfFreedom () const
{
  FunctionProjector * projector 
    = dynamic_cast < FunctionProjector * > ( m_projector );

  return projector -> degreesOfFreedom ();
}

const vector< double > &
FunctionRep::
principleErrors() const
{
  FunctionProjector * fp = dynamic_cast < FunctionProjector * > ( m_projector);
  assert ( fp );
  
  return fp->principleErrors();
}

void FunctionRep::saveParameters ( )
{
  FunctionProjector * fp = dynamic_cast < FunctionProjector * > ( m_projector);
  fp->saveParameters ();
}

void
FunctionRep::
setPrincipleErrors ( std::vector < double >::const_iterator begin, 
		     std::vector < double >::const_iterator end )
{
  FunctionProjector * fp = dynamic_cast < FunctionProjector * > ( m_projector);

  fp -> setPrincipleErrors ( begin, end );
}

void FunctionRep::restoreParameters ()
{
  FunctionProjector * fp = dynamic_cast < FunctionProjector * > ( m_projector);
  fp->restoreParameters ();
  notifyObservers ();
}

void FunctionRep::setParameters ( const std::vector<double> & params )
{
  FunctionProjector * fp = dynamic_cast < FunctionProjector * > ( m_projector);
  fp->setParameters (params);
  notifyObservers ();
}

void
FunctionRep::
setFixedFlags ( const std::vector < int > & flags )
{
  Fitter * fitter = getFitter ();
  fitter -> setFixedFlags ( flags );
}

const vector < string > & FunctionRep::parmNames () const
{
  FunctionBase * function = getFunction ();

  return function->parmNames ();
}

const vector < double > & FunctionRep::parameters () const
{
  FunctionBase * function = getFunction ();

  return function->getParameters ();
}

const vector < int > &
FunctionRep::
getFixedFlags ( ) const
{
  const Fitter * fitter = getFitter ();

  return fitter -> getFixedFlags ();
}

void
FunctionRep::
setIgnoreError(bool ignore)
{ 

  Fitter * fitter = getFitter ();
  if ( fitter != 0 ) {
    fitter -> setUseErrors ( ! ignore );
  }
}

void
FunctionRep::
willDelete ( const Observable * observed )
{
  const DataRep * rep = dynamic_cast < const DataRep * > ( observed );
  if ( rep != 0 ) {
    m_target = 0;
    const DataSource * projvalues = rep -> getProjectedValues ();
    DataSource * source = const_cast < DataSource * > ( projvalues );
    source -> removeObserver ( this );
  }
}

bool
FunctionRep::
getIgnoreErrors() const
{
  bool yes = false;
  Fitter * fitter = getFitter ();
  if (fitter !=0) {
    yes = ! fitter -> getUseErrors ();
  }

  return yes;
}

bool
FunctionRep::
isTargetable () const
{
  return false;
}

void
FunctionRep::
setCutRange ( const Range & range )
{
  if ( m_cut_rep == 0 ) {
    m_cut_rep = new CutRangeRep ();
    TupleCut cut;
    m_cut_rep -> addCut ( cut );
  }

  const vector < TupleCut > & cut_list = m_cut_rep -> getCuts ();
  const TupleCut & c = cut_list.front(); // to be index later
  TupleCut & cut = const_cast < TupleCut &> ( c );

  const ProjectorBase * target = m_target -> getProjector ();
  const string & label = target -> getXLabel ();
  cut.setLabel ( label );
  cut.setRange ( range );

  FunctionProjector * projector 
    = dynamic_cast < FunctionProjector * > ( m_projector );
  assert ( projector != 0 );

  projector -> setFitCut ( & cut );
  projector -> setFitRange ( true );
}

/** @bug @@@@@@ This needs fixing for two dimension functions.
 */
void
FunctionRep::
setCutRange ( bool yes )
{
  FunctionProjector * projector 
    = dynamic_cast < FunctionProjector * > ( m_projector );
  assert ( projector != 0 );

  projector -> setFitRange ( yes );
}

void
FunctionRep::
setTupleCut ()
{
  bool yes = m_target -> hasCut ();
  if ( yes ) {
    const vector < TupleCut > & cut_list = m_target -> getCuts ();

    const TupleCut & temp = cut_list.front ();
    TupleCut & cut = const_cast < TupleCut & > ( temp );

    FunctionProjector * projector 
      = dynamic_cast < FunctionProjector * > ( m_projector );
    assert ( projector != 0 );

    projector -> setFitCut ( & cut );
    projector -> setFitRange ( true );
  }
}

void
FunctionRep::
removeCut ()
{
  FunctionProjector * projector 
    = dynamic_cast < FunctionProjector * > ( m_projector );
  projector -> setFitCut ( 0 );
  projector -> setFitRange ( false );

  DataRep::removeCut (); // If this object owns it
  m_target -> removeCut (); // if target owns it.
  m_target -> setDirty ( true );
}

bool
FunctionRep::
hasZoomY () const
{
  return true;
}

void
FunctionRep::
fillFunctionParameters( std::vector<FunctionParameter> & function_parms) const
{
  function_parms.clear ();
  const vector < string > & parm_names = parmNames ();
  const vector < double > & values = parameters ();
  const vector < double > & errors = principleErrors ();
  const vector < int > & flags = getFixedFlags ();

  unsigned int count = parm_names.size ();
  for ( unsigned int i = 0; i < count; i++ ) {
    FunctionParameter fp ( parm_names[i],
			   values[i],
			   errors[i],
			   flags[i] != 0,
			   0., // lower limit
			   0., // upper limit
			   false, // has lower
			   false ); // has upper
    function_parms.push_back ( fp );
  }
}

void
FunctionRep::
addToComposite ( FunctionRep * )
{
  // does nothing
}

void
FunctionRep::
removeFromComposite ( FunctionRep * )
{
  // does nothing
}

const string &
FunctionRep::
functionName () const
{
  FunctionBase * function = getFunction ();

  return function -> name ();
}
