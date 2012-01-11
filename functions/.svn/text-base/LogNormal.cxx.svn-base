/** @file

hippodraw::LogNormal class implementation

Copyright (C) 2004, 2005, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: LogNormal.cxx,v 1.11 2007/08/24 22:19:16 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "LogNormal.h"

#include "FunctionHelper.h"

#include <cmath>
#include <cassert>

using std::distance;

using std::exp;
using std::vector;

using namespace hippodraw;

LogNormal::LogNormal ( )
{
  initialize ();
}

LogNormal::LogNormal ( double n, double m, double s )
{
  initialize ();
  
  m_parms[norm] = n;
  m_parms[mu] = m;
  m_parms[sigma] = s;
}

void LogNormal::initialize ()
{
  m_name = "LogNormal";

  m_parm_names.push_back ( "Norm" );
  m_parm_names.push_back ( "Mu" );
  m_parm_names.push_back ( "Sigma" );

  resize ();
}

FunctionBase * LogNormal::clone () const
{
  return new LogNormal ( *this );
}

double LogNormal::operator () ( double x ) const
{
  double t = ( log ( x ) - m_parms[mu] ) / m_parms[sigma];
  double result = m_parms[norm] * exp ( -0.5 * t*t );

  return result;
}

/** @request Need better estimate of initial parameters.
 */
void 
LogNormal::
initialParameters ( const FunctionHelper * helper )
{
  m_parms[norm] = 1.0;
  m_parms[mu] =  log ( helper->meanCoord () );
  m_parms[sigma] = 0.5;
}

double
LogNormal::
derivByParm ( int , double ) const
{
  assert ( false );
  return 0.;
}

bool
LogNormal::
hasDerivatives () const
{
  return false;
}
