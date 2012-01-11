/** @file

hippodraw::StatedFCN class implemenation.

Copyright (C) 2003-2006  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: StatedFCN.cxx,v 1.24 2006/09/12 17:35:06 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "StatedFCN.h"

#include "functions/FunctionBase.h"
 
#include <stdexcept>

using std::string;
using std::vector;

using namespace hippodraw;

StatedFCN::
StatedFCN ( )
  : m_function ( 0 ),
    m_needs_derivs ( false )
{
}

StatedFCN::
StatedFCN ( const StatedFCN & fcn )
  : FCNBase (),
    m_fixed_flags ( fcn.m_fixed_flags ),
    m_function ( fcn.m_function ),
    m_needs_derivs ( fcn.m_needs_derivs )
{
}

void
StatedFCN::
copyFrom ( const StatedFCN * other )
{
  m_fixed_flags = other -> m_fixed_flags;
  m_function = other -> m_function;

}

bool
StatedFCN::
hasFunction () const
{
  return m_function != 0;
}

/** @bug @@@@ Should be able to display a function without fitting.
    Use examples/fuction_rep.py to see the problem.
 */
void
StatedFCN::
setFunction ( FunctionBase * function )
{
  bool yes = isCompatible ( function );
  if ( yes == false ) {
    string what ( "StatedFCN: The function `" );
    what += function -> name ();
    what += "' can not provide partial\n";
    what += "derivatives needed by this fitter.";
    throw std::runtime_error ( what );
  }

  m_function = function;
  unsigned int size = m_function -> size();
  m_fixed_flags.resize ( size );

  for ( unsigned int i = 0; i < size; i++ ) {
    m_fixed_flags [i] = 0;
  }
}

const vector < string > &
StatedFCN::
getParmNames () const
{
  return m_function -> parmNames ();
}

const vector < double > &
StatedFCN::
getParameters ( ) const
{
  return m_function -> getParameters ( );
}

void
StatedFCN::
setParameters ( const std::vector < double > & parms )
{
  m_function -> setParameters ( parms );
}

void
StatedFCN::
fillFreeParameters ( std::vector < double > & free_parms ) const
{
  free_parms.clear();
  const vector < double > & parms = m_function -> getParameters ();
  unsigned int size = parms.size ();
  for ( unsigned int i = 0; i < size; i++ ) {
    if ( m_fixed_flags[ i ] == 0 ) {
      free_parms.push_back ( parms[ i ] );
    }
  }
}

unsigned int
StatedFCN::
getNumberFreeParms () const
{
  unsigned int number = 0;
  unsigned int size = m_fixed_flags.size ();
  for ( unsigned int i = 0; i < size; i++ ) {
    if ( m_fixed_flags[i] == 0 ) number++;
  }

  return number;
}

const vector < int > &
StatedFCN::
getFixedFlags () const
{
  return m_fixed_flags;
}

void
StatedFCN::
setFixedFlags ( const std::vector < int > & flags )
{
  m_fixed_flags = flags;
}

void
StatedFCN::
setFreeParameters ( const std::vector < double > & free_parms )
{
  vector < double > parms = m_function -> getParameters ();
  unsigned int size = parms.size ();
  unsigned int j = 0;

  for ( unsigned int i = 0; i < size; i++ ) {
    if ( m_fixed_flags [i] == 0 ) {
      parms[i] = free_parms[j];
      j++;
    }
  }

  m_function -> setParameters ( parms );
}

void
StatedFCN::
fillFreeDerivatives ( std::vector < double > & derives, double x )
{
  derives.clear();
  const vector < double > & parms = m_function -> getParameters ();
  unsigned int size = parms.size();

  for ( unsigned int i = 0; i < size; i++ ) {
    if ( m_fixed_flags [i] == 0 ) {
      double value = m_function -> derivByParm ( i, x );
      derives.push_back ( value );
    }
  }
}

double
StatedFCN::
operator () ( const std::vector < double > & parms ) const
{
  m_function -> setParameters ( parms );

  return objectiveValue ();
}

void
StatedFCN::
setNeedsDerivatives ( bool yes )
{
  m_needs_derivs = yes;
}

bool
StatedFCN::
isCompatible ( const FunctionBase * function ) const
{
  bool yes = true;
  if ( m_needs_derivs &&
       ( function -> hasDerivatives () == false ) ) {
    yes = false;
  }

  return yes;
}
