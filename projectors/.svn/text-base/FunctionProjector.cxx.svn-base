/** @file

hippodraw::FunctionProjector class implementation

Copyright (C) 2000-2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: FunctionProjector.cxx,v 1.214 2007/04/23 21:57:21 pfkeb Exp $

*/

// For truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "FunctionProjector.h"

#include "ProjectorHelper.h"

#include "axes/AxisModelBase.h"
#include "datasrcs/NTuple.h"
#include "functions/FunctionBase.h"
#include "minimizers/Fitter.h"

#include <cmath>

#include <cassert>

using std::copy;
using std::list;
using std::string;
using std::vector;

using namespace hippodraw;

string FunctionProjector::s_x_label ("X");

FunctionProjector::
FunctionProjector ( FunctionBase * function,
		    ProjectorBase * targetProjector )
   :  ProjectorBase (),
      m_target ( targetProjector ),
      m_fitter ( 0 )
{
  m_function = function;
  assert ( m_function != 0 );
  m_z_label = function -> name ();
}

/** @todo To be implemented. */
FunctionProjector::
FunctionProjector ( const FunctionProjector & projector )
  : ProjectorBase ( projector )
{
}

FunctionProjector::~FunctionProjector ()
{
  delete m_function;
  delete m_fitter;
}

/** @todo To be implemented. */
ProjectorBase * FunctionProjector::clone ()
{
  return 0;
}

FunctionBase * FunctionProjector::function () const
{
  return m_function;
}

bool FunctionProjector:: hasSavedParameters () const
{
  return m_save_parms.empty() == false;
}

void FunctionProjector::saveParameters ()
{
  assert ( m_function != 0 );

  m_save_parms = m_function->getParameters ();
  m_principleErrors.resize ( m_save_parms.size(), 0. );
}

void FunctionProjector::restoreParameters ()
{
  assert ( m_function != 0 );
  assert ( ! m_save_parms.empty () );

  m_function->setParameters ( m_save_parms );

  setDirty ( true );
}

void
FunctionProjector::
setFitter ( Fitter * fitter )
{
  m_fitter = fitter;
}

Fitter *
FunctionProjector::
getFitter ( ) const
{
  return m_fitter;
}

void
FunctionProjector::
initializeFunction ( ProjectorBase * projector )
{
  assert ( m_function != 0 );

  m_target = projector;
  if ( m_target != 0 ) {
    projector->prepareValues();
    ProjectorHelper helper ( projector -> getProjectedValues () );
    m_function->initialParameters ( & helper );
  }

  saveParameters ();
}

bool
FunctionProjector::
fitFunction ( )
{
  assert ( m_function != 0 && m_target != 0 );
  assert ( m_fitter != 0 );
  m_target -> prepareValues ();

  if ( hasSavedParameters() == false ) {
    saveParameters ();
  }
  // Calculate the best fit
  bool ok = m_fitter->calcBestFit ();
  
  setDirty ( true );
  
  return ok;
}

const string & FunctionProjector::getTitle() const
{
  return m_function->name ();
} 

void
FunctionProjector::
setRange ( hippodraw::Axes::Type, bool )
{
  setDirty ( true );
}

const std::string & FunctionProjector::getXLabel () const
{
  return s_x_label;
}

const string & FunctionProjector::getYLabel ( bool ) const
{
  return m_function->name ();
}

/** @todo Return value range of function. */
Range FunctionProjector::valueRange() const
{
  return Range ( 0, 0 );
}

int
FunctionProjector::
getNumberOfEntries () const
{
  // A call to this function for a FunctionProjector is meaningless.
  // Thus it returns meaningless negative number
  return -1;
}

int
FunctionProjector::
getUnderflow () const
{
  return -1;
}

int 
FunctionProjector::
getOverflow () const
{
  return -1;
}

Range
FunctionProjector::
dataRangeOn ( hippodraw::Axes::Type axis ) const
{
  AxisModelBase * model = 0;
  switch ( axis ) {
  case Axes::X :
    model = m_x_axis;
    break;
  case Axes::Y :
    model = m_y_axis;
    break;
  case Axes::Z :
    model = m_z_axis;
    break;
  default:
    assert ( false );
  }

  return model -> getRange ( false );
}

double FunctionProjector::objectiveValue () const
{
  return m_fitter->objectiveValue ();
}

void
FunctionProjector::
calcPrincipleErrors() const
{
  int num_parms = m_save_parms.size ();
  m_principleErrors.clear();
  m_principleErrors.resize( num_parms, 0.0 );
  if ( m_fitter != 0 ) {
    m_fitter -> calcCovariance ( m_covariance );
    int n = m_covariance.size();
    if ( n != 0 ) {
      const vector < int > & fixed = m_fitter -> getFixedFlags ();
      int ic = 0;
      for( int i = 0; i < num_parms; i++ ) {
	if ( fixed [ i ] == 0 ) {
	  m_principleErrors[i] = sqrt( m_covariance[ic][ic] );
	  ic++;
	}
	else {
	  m_principleErrors[i] = 0.0;
	}
      }
    }
  }
}

const vector < double > & 
FunctionProjector::
principleErrors() const
{
  return m_principleErrors;
}

void
FunctionProjector::
setPrincipleErrors ( std::vector < double > :: const_iterator begin,
		     std::vector < double > :: const_iterator end )
{
  unsigned int size = std::distance ( begin, end );
  assert ( size == m_principleErrors.size () );

  copy ( begin, end, m_principleErrors.begin() );
}

int FunctionProjector::degreesOfFreedom () const
{
  return m_fitter->calcDegreesOfFreedom ();
}

double
FunctionProjector::
getPosOn ( hippodraw::Axes::Type ) const
{
  return 0.0;
}

const string & FunctionProjector::getZLabel() const
{
  return m_z_label;
}

void FunctionProjector::addPointReps()
{
}

bool
FunctionProjector::
isEmpty () const
{
  return m_function == 0;
}

void FunctionProjector::setParameters ( const std::vector<double> & params )
{
  assert ( m_function != 0 );

  std::vector<double> myParams = m_function->getParameters();

  if ( myParams.size() == params.size() ) {
     m_function->setParameters ( params );
     setDirty ( true );
  }
}

const vector < vector < double > > &
FunctionProjector::
covariance ( ) const
{
  if ( m_covariance.empty () ) {
    m_fitter -> calcCovariance ( m_covariance );
  }

  return m_covariance;
}

DataSource *
FunctionProjector::
createNTuple () const
{
  // does nothing
  return 0;
}

void
FunctionProjector::
fillProjectedValues ( DataSource *, bool ) const
{
  // does nothing.
}

void
FunctionProjector::
setFitCut ( TupleCut * cut )
{
  m_fitter -> setFitCut ( cut );
}

void
FunctionProjector::
setFitRange ( bool yes )
{
  m_fitter -> setFitRange ( yes );
}
