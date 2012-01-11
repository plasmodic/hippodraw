/** @file

hippdraw::LogParabola class implementation

Copyright (C) 2004, 2005, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: LogParabola.cxx,v 1.3 2007/04/23 21:57:20 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "LogParabola.h"

#include "FunctionHelper.h"

#include <cmath>
#include <cassert>
#include <iostream>

using std::distance;

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::log;
using std::exp;
using std::pow;
using std::vector;
#endif

namespace hippodraw {

LogParabola::LogParabola ( )
{
  initialize ();
}

LogParabola::LogParabola ( double n, double a, double b)
{
  initialize ();
  
  m_parms[NORM] = n;
  m_parms[ALPHA] = a;
  m_parms[BETA] = b;
}

void LogParabola::initialize ()
{
  m_name = "LogParabola";

  m_parm_names.push_back ( "norm" );
  m_parm_names.push_back ( "alpha" );
  m_parm_names.push_back ( "beta" );

  resize ();
}

FunctionBase * LogParabola::clone () const
{
  return new LogParabola ( *this );
}

double LogParabola::operator () ( double x ) const
{
  double logx = log(x);
  return m_parms[0]*pow(x,(-(m_parms[1] + m_parms[2]*logx)));
}

void 
LogParabola::initialParameters ( const FunctionHelper * helper )
{
  double min_x = helper->minCoord ();
  double max_x = helper->maxCoord ();
  int size = helper->size();
  double total = helper->getTotal ();

  m_parms[NORM] = total * ( max_x - min_x ) / size;
  m_parms[ALPHA] = 0.1;
  m_parms[BETA] = 0.1;
}

double LogParabola::derivByParm ( int ipar, double x) const
{
  double  logx = log(x);
  double dfdnorm = pow(x,( -(m_parms[1] + m_parms[2]*logx)));

  double deriv =0.;

  switch (ipar){
  case NORM:
    deriv = dfdnorm;
    break;
  case ALPHA:
    deriv = -m_parms[0]*logx*dfdnorm;
    break;
  case BETA:
    deriv = -m_parms[0]*logx*logx*dfdnorm;
    break;
  default:
    std::cout<<"parameter index not found : "<<ipar<<std::endl;
    std::cout<<"I know about "<<NORM<<" "<<ALPHA<<" "<<BETA<<" "<<std::endl;
    assert ( false ); 
    break;
  }
  return deriv;
}

bool
LogParabola::hasDerivatives () const
{
  return true;
}

} // namespace hippodraw

