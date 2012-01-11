/** @file

hippodraw::Quadratic2 class implementation

Copyright (C) 2000, 2003, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Quadratic2.cxx,v 1.4 2007/04/23 21:57:20 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "Quadratic2.h"

#include "FunctionHelper.h"

#include <cassert>
#include <cmath>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::vector;
#endif

namespace hippodraw {

Quadratic2::Quadratic2 ( )
{
  initialize ();
}

Quadratic2::Quadratic2 ( double yscale, double y0, double x0 )
{
  initialize ();

  m_parms[0] = yscale;
  m_parms[1] = y0;
  m_parms[2] = x0;
}

void Quadratic2::initialize ()
{
  m_name = "yscale*(x - x0)**2 + y0";

  m_parm_names.push_back ( "yscale" );
  m_parm_names.push_back ( "y0" );
  m_parm_names.push_back ( "x0" );

  resize ();
}

FunctionBase * Quadratic2::clone () const
{
  return new Quadratic2 ( *this );
}

double Quadratic2::operator () ( double x ) const
{
  return m_parms[0] * ( x - m_parms[2] ) * ( x - m_parms[2] ) + m_parms[1];
}

/* virtual */
void 
Quadratic2::
initialParameters ( const FunctionHelper * helper )
{
  double min_x = helper->minCoord ();
  double max_x = helper->maxCoord ();

  double min_y = helper->minValue ();
//   double max_y = helper->maxValue ();

  m_parms[0] = 1.;
  m_parms[1] = min_y;
  m_parms[2] = std::sqrt(std::fabs(min_x*max_x));
}

double Quadratic2::derivByParm ( int i, double x ) const
{
  switch ( i ) {
  case 0 :
    return (x - m_parms[2]) * (x - m_parms[2]);
    break;

  case 1 :
    return 1.;
    break;

  case 2 :
    return -2.*m_parms[0]*(x - m_parms[2]);
    break;

  default :
    assert (false );
    break;
  }
  return 0.0;
}

} // namespace hippodraw

