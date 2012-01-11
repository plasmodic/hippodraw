/** @file

hippodraw::Erfc class implementation

Copyright (C) 2000, 2001, 2003, 2005, 2007  The Board of Trustees of The
Leland Stanford Junior University.  All Rights Reserved.


$Id: Erfc.cxx,v 1.9 2007/04/23 21:57:20 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "Erfc.h"
#include "FunctionHelper.h"

#include <cmath>

#include <cassert>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::exp;
using std::vector;
#endif

namespace hippodraw {

Erfc::Erfc ( )
{
  initialize ();
}

Erfc::Erfc(double m, double s, double n)
{
  initialize();
  
  m_parms[MEAN]  = m;
  m_parms[SIGMA] = s;
  m_parms[NORM]  = n;  
}

void Erfc::initialize ()
{
  m_name = "Erfc";

  m_parm_names.push_back ( "Mean" );
  m_parm_names.push_back ( "Sigma" );
  m_parm_names.push_back ( "Normalization" );

  resize ();
}

FunctionBase * Erfc::clone () const
{
  return new Erfc ( *this );
}

/// returns Erfc computed at the reduced variable @f$x =
/// \frac{value-mean}{sigma}@f$
double Erfc::operator () (double value) const
{
  double result = 1; // The return value

  double x = calcRed(value); //reduced variable

  result = calcErfc(x);  //this method is a copy of ROOT method in TMath.cxx
  
  result *= m_parms[NORM];

  return result;
}


/* virtual */
void  Erfc::initialParameters ( const FunctionHelper * helper )
{
   m_parms[MEAN] = helper->meanCoord();
   m_parms[SIGMA] = 1;
   m_parms[NORM] = helper->maxValue () / 2;
}


double Erfc::derivByParm ( int i, double value ) const
{
   double result = 0;

  switch ( i ) {
  case MEAN :
    result = -1.0/m_parms[SIGMA] * m_parms[NORM] * derivByRed ( calcRed(value) );
    return result; 
    break;

  case SIGMA :
    result = - calcRed(value)/m_parms[SIGMA] * m_parms[NORM] * derivByRed ( calcRed(value) );
    return result; 
    break;

  case NORM :
   result = operator () ( value ) / m_parms[NORM];
   return result; 
   break;

  default :
    assert ( false );
    break;
  }

  return 0.0; 
}


//Derivative wrt reduced is gaussian!
double Erfc::derivByRed( const double x ) const
{ 
  //I don't know if this is platform independant: std::M_2_SQRTPI from math.h
  static const double m_2_sqrtpi = 1.12837916709551257390; // 2/sqrt(pi)

  double result = - m_2_sqrtpi; // minus sign comes from the fact that x is lower
                                // bound of Erfc definition
  result *= exp(-x*x);
  return result;
}


//this is the copy from ROOT TMath.cxx
double Erfc::calcErfc(double x) const
{
  // Compute the complementary error function erfc(x).
  // Erfc(x) = (2/sqrt(pi)) Integral(exp(-t^2))dt between x and infinity
  //
  //--- Nve 14-nov-1998 UU-SAP Utrecht
  
  // The parameters of the Chebyshev fit
  const double 
    a1 = -1.26551223,   a2 = 1.00002368,
    a3 =  0.37409196,   a4 = 0.09678418,
    a5 = -0.18628806,   a6 = 0.27886807,
    a7 = -1.13520398,   a8 = 1.48851587,
    a9 = -0.82215223,  a10 = 0.17087277;

   double v = 1; // The return value

   double z = x>=0 ? x : -x; //absolute value

   if (z <= 0) return v; // erfc(0)=1

   double t = 1/(1+0.5*z);

   v = t*exp((-z*z) +a1+t*(a2+t*(a3+t*(a4+t*(a5+t*(a6+t*(a7+t*(a8+t*(a9+t*a10)))))))));

   if (x < 0) v = 2-v; // erfc(-x)=2-erfc(x)

   return v;
}

} // namespace hippodraw
