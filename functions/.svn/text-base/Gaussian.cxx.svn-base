/** @file

hippodraw::Gaussian class implementation

Copyright (C) 2000-2004, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Gaussian.cxx,v 1.58 2007/08/24 22:19:16 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "Gaussian.h"

#include "FunctionHelper.h"

#include <cmath>
#include <cassert>

using std::distance;

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::exp;
using std::vector;
#endif

using namespace hippodraw;

Gaussian::Gaussian ( )
{
  initialize ();
}

Gaussian::Gaussian ( double n, double m, double s )
{
  initialize ();
  
  m_parms[norm] = n;
  m_parms[mean] = m;
  m_parms[sigma] = s;
}

void Gaussian::initialize ()
{
  m_name = "Gaussian";

  m_parm_names.push_back ( "Norm" );
  m_parm_names.push_back ( "Mean" );
  m_parm_names.push_back ( "Sigma" );

  resize ();
}

FunctionBase * Gaussian::clone () const
{
  return new Gaussian ( *this );
}

double Gaussian::operator () ( double x ) const
{
  double value = 0.0;
  if ( m_parms[sigma] != 0.0 ) {
    double t = ( x - m_parms[mean] ) / m_parms[sigma];
    t = 0.5 * t*t;
    if ( fabs ( t ) < 50.0 ) {
      value = exp ( -t ) / ( 2.50662828 * m_parms[sigma] );
    }
  } // sigma == 0.
  else {
    if ( x == m_parms[mean] ) value = 1.0;
  }
  return value * m_parms[norm];
}

/* virtual */
void 
Gaussian::
initialParameters ( const FunctionHelper * helper )
{
  double min_x = helper->minCoord ();
  double max_x = helper->maxCoord ();
  int size = helper->size();
  double total = helper->getTotal ();

  m_parms[norm] = total * ( max_x - min_x ) / size;
  m_parms[mean] = helper->meanCoord ();
  m_parms[sigma] = helper->stdCoord ();
}

double Gaussian::derivByParm ( int i, double x ) const
{
  switch ( i ) {
  case norm :
    return derivByNorm ( x );
    break;

  case mean :
    return derivByMean ( x );
    break;

  case sigma :
    return derivBySigma ( x );
    break;

  default :
    assert ( false );
    break;
  }
  return 0.0;
}

double Gaussian::derivByNorm ( double x ) const
{
  if ( m_parms[sigma] != 0.0 ) {
    double t = ( x - m_parms[mean] ) / m_parms[sigma];
    t = 0.5 * t*t;
    if ( fabs ( t ) > 50.0 ) {
      return 0.0;
    }
    else {
      return exp ( -t ) / ( 2.50662828 * m_parms[sigma] );
    }
  } // sigma == 0.0
  else {
    if ( x != m_parms[mean] ) {
      return 0.0;
    } else {
      return 1.0;
    }
  }
}

double Gaussian::derivByMean ( double x ) const
{
  double dx = x - m_parms[mean];
  if ( m_parms[sigma] != 0.0 ) {
    return m_parms[norm] * dx 
      * exp ( -dx*dx / ( 2.0 * m_parms[sigma] * m_parms[sigma] ) ) 
      / ( 2.50662828 * m_parms[sigma] * m_parms[sigma] * m_parms[sigma] );
  }
  else {
    if ( x != m_parms[mean] ) return 0.0;
    else return 1.0;
  }
}

double Gaussian::derivBySigma ( double x ) const
{
  if ( m_parms[sigma] == 0.0 ) return 0.0;
  double dx = x - m_parms[mean];
  double p2 = m_parms[sigma] * m_parms[sigma];
  double ex = exp ( -dx*dx / ( 2.0 * p2 ) );
  return m_parms[norm] * ( dx*dx * ex / ( p2*p2) - ex / p2 ) / 2.50662828; 
}

