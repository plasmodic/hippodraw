/** @file

hippodraw::Fitter class implemenation.

Copyright (C) 2003, 2005, 2006  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Fitter.cxx,v 1.36 2006/01/28 22:54:45 pfkeb Exp $

*/

#include "Fitter.h"

#include "StatedFCN.h"

#include <stdexcept>

using std::string;
using std::vector;

using namespace hippodraw;

Fitter::
Fitter ( const char * name )
  : m_name ( name ),
    m_fcn ( 0 ),
    m_max_iterations ( 100 )
{
}

Fitter::
Fitter ( const Fitter & fitter )
  : m_name ( fitter.m_name ),
    m_max_iterations ( fitter.m_max_iterations )
{
  if ( fitter.m_fcn != 0 ) m_fcn = fitter.m_fcn -> clone ();
}

Fitter::
~Fitter ()
{
  if ( m_fcn != 0 ) delete m_fcn;
}

void
Fitter::
copyFrom ( const Fitter * fitter )
{
  m_fcn -> copyFrom ( fitter -> m_fcn );
}

const std::string &
Fitter::
name () const
{
  return m_name;
}

void
Fitter::
setFCN ( StatedFCN * fcn )
{
  if ( m_fcn != 0 ) delete m_fcn;

  m_fcn = fcn;
}

StatedFCN *
Fitter::
getFCN ()
{
  return m_fcn;
}

bool
Fitter::
isCompatible ( const FunctionBase * function ) const
{
  bool yes = false;
  if ( m_fcn != 0 ) {
    yes = m_fcn -> isCompatible ( function );
  }
  return yes;
}

void
Fitter::
setFunction ( FunctionBase * function )
{
  if ( m_fcn != 0 ) {
    m_fcn -> setFunction ( function );
  }
}

void
Fitter::
setDataSource ( const DataSource * source )
{
  if ( m_fcn != 0 ) {
    m_fcn -> setDataSource ( source );
    m_fcn -> setUseErrors ();
  }
}

void
Fitter::
setUseErrors ( bool yes )
{
  if ( m_fcn != 0 ) {
    m_fcn -> setUseErrors ( yes );
  }
}

bool
Fitter::
getUseErrors () const
{
  bool yes = false;
  if ( m_fcn != 0 ) {
    yes = m_fcn -> getUseErrors ();
  }

  return yes;
}

bool
Fitter::
needsIntegrated () const
{
  bool yes = true;
  if ( m_fcn != 0 ) {
    yes = m_fcn -> needsIntegrated ();
  }
  return yes;
}

void
Fitter::
fillFreeParameters ( std::vector < double > & free_parms) const
{
  return m_fcn -> fillFreeParameters ( free_parms );
}

void
Fitter::
setFixedFlags ( const std::vector < int > & flags )
{
  m_fcn -> setFixedFlags ( flags );
}

const vector < int > &
Fitter::
getFixedFlags ( ) const
{
  return m_fcn -> getFixedFlags ();
}

void
Fitter::
setLimits ( unsigned int, double, double )
{
  string what ( "The " );
  what += m_name;
  what += " minimizer does not support limits on parameters";
  throw std::runtime_error ( what );
}

unsigned int
Fitter::
getParameterIndex ( const std::string & name )
{
  unsigned int index = UINT_MAX;
  const vector < string > & names = m_fcn -> getParmNames ();
  unsigned int size = names.size();
  for ( unsigned int i = 0; i < size; i++ ) {
    const string & pname = names [i];
    if ( pname == name ) {
      index = i;
      break;
    }
  }
  if ( index == UINT_MAX ) {
    string what ( "No parameter named `" );
    what += name;
    what += "' for this function.";
    throw std::runtime_error ( what );
  }

  return index;
}

void
Fitter::
setLimits ( const std::string & name, double lower, double upper )
{
  unsigned int index = getParameterIndex ( name );
  setLimits ( index, lower, upper );
}

void
Fitter::
setStepSize ( unsigned int, double )
{
  string what ( "This " );
  what += m_name;
  what += " minimizer does not support setting step size.";
  throw std::runtime_error ( what );
}

void
Fitter::
setStepSize ( const std::string & name, double size )
{
  unsigned int index = getParameterIndex ( name );
  setStepSize ( index, size );
}

double
Fitter::objectiveValue () const
{
  return m_fcn -> objectiveValue ();
}

int
Fitter::
calcDegreesOfFreedom () const
{
  return m_fcn -> degreesOfFreedom();
}

int
Fitter::
calcCovariance ( std::vector < std::vector < double > > & )
{
  return EXIT_FAILURE;
}

void
Fitter::
setFitCut ( TupleCut * cut )
{
  m_fcn -> setFitCut ( cut );
}

void
Fitter::
setFitRange ( bool yes )
{
  m_fcn -> setFitRange ( yes );
}
