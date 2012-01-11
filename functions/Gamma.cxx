/**@file

hippodraw::Gamma class implementation

Copyright (C) 2000, 2003, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Gamma.cxx,v 1.1 2007/11/29 22:58:57 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "Gamma.h"

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

Gamma::Gamma ( )
{
  initialize ();
}

Gamma::Gamma ( double prefactor, double shape, double rate)
{
  initialize ();

  m_parms[0] = prefactor;
  m_parms[1] = shape;
  m_parms[2] = rate;
}

void Gamma::initialize ()
{
  m_name = "Gamma";
  m_parm_names.push_back ( "Prefactor" );
  m_parm_names.push_back ( "Shape" );
  m_parm_names.push_back ( "Rate" );

  resize ();
}

FunctionBase * Gamma::clone () const
{
  return new Gamma ( *this );
}

double Gamma::operator () ( double x ) const
{
   double logval = log(m_parms[0]) + (m_parms[1] - 1.0)*log(x) - x/m_parms[2];

   logval -= lgamma(m_parms[1]) + log(m_parms[2])*m_parms[1];

   return exp(logval);
}

/* virtual */
void 
Gamma::
initialParameters ( const FunctionHelper * helper )
{
   /* Mean is shape / scale, variance is shape / scale^2 */
   m_parms[0] = helper->getTotal()*(helper->maxCoord ()-helper->minCoord ())/helper->size();
   m_parms[2] = pow(helper->stdCoord(),2)/helper->meanCoord();
   m_parms[1] = helper->meanCoord()/m_parms[2];
}

double Gamma::derivByParm ( int i, double x ) const
{
  switch ( i ) {
  case 0 :
    return operator()(x)/m_parms[0];
    break;

  default:
    assert ( false );
    break;
  }
  return 0.0;
}

bool Gamma::hasDerivatives() const
{
  return false; /* No computation of digammas for me */
}

} // namespace hippodraw

