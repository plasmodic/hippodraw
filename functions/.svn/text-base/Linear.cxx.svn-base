/**@file

hippodraw::Linear class implementation

Copyright (C) 2000, 2003, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Linear.cxx,v 1.40 2007/04/23 21:57:20 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "Linear.h"

#include "FunctionHelper.h"

#include <cassert>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::vector;
#endif

namespace hippodraw {

Linear::Linear ( )
{
  initialize ();
}

Linear::Linear ( double intercept, double slope )
{
  initialize ();

  m_parms[0] = intercept;
  m_parms[1] = slope;
}

void Linear::initialize ()
{
  m_name = "Linear";
  m_parm_names.push_back ( "Intercept" );
  m_parm_names.push_back ( "Slope" );

  resize ();
}

FunctionBase * Linear::clone () const
{
  return new Linear ( *this );
}

double Linear::operator () ( double x ) const
{
  return x * m_parms[1] + m_parms[0];
}

/* virtual */
void 
Linear::
initialParameters ( const FunctionHelper * helper )
{
  double min_x = helper->minCoord ();
  double max_x = helper->maxCoord ();

  double min_y = helper->minValue ();
  double max_y = helper->maxValue ();

  m_parms[1] = ( max_y - min_y ) / ( max_x - min_x );
  m_parms[0] = max_y - m_parms[1] * max_x;
}

double Linear::derivByParm ( int i, double x ) const
{
  switch ( i ) {
  case 0 :
    return 1.0;
    break;

  case 1 :
    return x;
    break;

  default:
    assert ( false );
    break;
  }
  return 0.0;
}

} // namespace hippodraw

