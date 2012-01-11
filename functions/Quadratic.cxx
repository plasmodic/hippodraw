/** @file

hippodraw::Quadratic class implementation

Copyright (C) 2000, 2003, 2007  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Quadratic.cxx,v 1.39 2007/04/23 21:57:20 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "Quadratic.h"

#include "FunctionHelper.h"

#include <cassert>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::vector;
#endif

namespace hippodraw {

Quadratic::Quadratic ( )
{
  initialize ();
}

Quadratic::Quadratic ( double intercept, double linear, double quad )
{
  initialize ();

  m_parms[0] = intercept;
  m_parms[1] = linear;
  m_parms[2] = quad;
}

void Quadratic::initialize ()
{
  m_name = "Quadratic";

  m_parm_names.push_back ( "Intercept" );
  m_parm_names.push_back ( "Linear" );
  m_parm_names.push_back ( "Quad" );

  resize ();
}

FunctionBase * Quadratic::clone () const
{
  return new Quadratic ( *this );
}

double Quadratic::operator () ( double x ) const
{
  return m_parms[0] + x * ( m_parms[1] + x * m_parms[2] );
}

/* virtual */
void 
Quadratic::
initialParameters ( const FunctionHelper * helper )
{
  double min_x = helper->minCoord ();
  double max_x = helper->maxCoord ();

  double min_y = helper->minValue ();
  double max_y = helper->maxValue ();

  m_parms[1] = ( max_y - min_y ) / ( max_x - min_x );
  m_parms[0] = max_y - m_parms[1] * max_x;
  m_parms[2] = m_parms[1] / ( 5.0 * ( max_x - min_x ) );
}

double Quadratic::derivByParm ( int i, double x ) const
{
  switch ( i ) {
  case 0 :
    return 1.0;
    break;

  case 1 :
    return x; 
    break;

  case 2 :
    return x * x;
    break;

  default :
    assert (false );
    break;
  }
  return 0.0;
}

} // namespace hippodraw

