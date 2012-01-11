/** @file

hippodraw::Chi2Dist class implementation

Copyright (C) 2000, 2003, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Chi2Dist.cxx,v 1.6 2007/04/23 21:57:20 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "Chi2Dist.h"

#include "FunctionHelper.h"

#include <cmath>
#include <cassert>
#include <iostream>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::vector;
#endif

namespace hippodraw {

Chi2Dist::Chi2Dist ( )
{
  initialize ();
}

Chi2Dist::Chi2Dist ( double norm, double nu, double scale )
{
  initialize ();

  m_parms[0] = norm;
  m_parms[1] = nu;
  m_parms[2] = scale;
}

void Chi2Dist::initialize ()
{
   m_name = "Chi-square dist.";
   m_parm_names.push_back ( "Norm" );
   m_parm_names.push_back ( "#dof" );
   m_parm_names.push_back ( "x scale" );

   resize ();
}

FunctionBase * Chi2Dist::clone () const
{
  return new Chi2Dist ( *this );
}

double Chi2Dist::operator () ( double x ) const
{
   return m_parms[0]*exp( -x/2./m_parms[2])
      *pow(x/2./m_parms[2], m_parms[1]/2. - 1. );
}

/* virtual */
void 
Chi2Dist::
initialParameters ( const FunctionHelper * helper )
{
   m_parms[2] = 1.;
   m_parms[1] = helper->meanCoord();
   m_parms[0] = 1.;
   m_parms[0] = helper->maxValue()/operator()(m_parms[1]);
}

double Chi2Dist::derivByParm ( int i, double x ) const
{
  switch ( i ) {
  case 0 :
     return operator()(x)/m_parms[0];
    break;

  case 1 :
     return operator()(x)/2.*log(x/2./m_parms[2]);
    break;

  case 2 :
     return operator()(x)*(x/2./m_parms[2] - (m_parms[1]/2.-1.))/m_parms[2];
     break;

  default:
    assert ( false );
    break;
  }
  return 0.0;
}

} // namespace hippodraw

