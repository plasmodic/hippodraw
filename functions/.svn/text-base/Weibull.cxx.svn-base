/**@file

hippodraw::Weibull class implementation

Copyright (C) 2000, 2003, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Weibull.cxx,v 1.1 2007/11/29 22:58:57 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "Weibull.h"

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

Weibull::Weibull ( )
{
  initialize ();
}

Weibull::Weibull ( double prefactor, double scale, double shape )
{
  initialize ();

  m_parms[0] = prefactor;
  m_parms[1] = scale;
  m_parms[2] = shape;
}

void Weibull::initialize ()
{
  m_name = "Weibull";
  m_parm_names.push_back ( "Prefactor" );
  m_parm_names.push_back ( "Scale" );
  m_parm_names.push_back ( "Shape" );

  resize ();
}

FunctionBase * Weibull::clone () const
{
  return new Weibull ( *this );
}

double Weibull::operator () ( double x ) const
{
   return m_parms[0]*exp( -pow(x/m_parms[1],m_parms[2]) )*pow(x,m_parms[2] - 1.0);
}

/* virtual */
void 
Weibull::
initialParameters ( const FunctionHelper * helper )
{
   double min_x = helper->minCoord ();
   double max_x = helper->maxCoord ();
   max_x = (min_x + max_x)/2.;

   m_parms[2] = 1.0;

   try {
      double min_y = helper->valueAt (min_x);
      double max_y = helper->valueAt (max_x);
      if (min_y != 0 && max_y != 0) {  // success!
         m_parms[1] = ( min_x - max_x ) / log( max_y/min_y );
         m_parms[0] = max_y / exp( -max_x/m_parms[1] );
         return;
      }
   } catch (std::string &message) {
      std::cerr << message << std::endl;
   }

// All cleverness fails, so use default values....   
   m_parms[0] = 1.;
   m_parms[1] = 1.;
}

double Weibull::derivByParm ( int i, double x ) const
{
  switch ( i ) {
  case 0 :
    return operator()(x)/m_parms[0];
    break;

  case 1 :
    return operator()(x)*pow(x/m_parms[1],m_parms[2]) * m_parms[2]/m_parms[1];
    break;

  case 2 :
    return operator()(x)*(log(x) - pow(x/m_parms[1],m_parms[2]) * log(x/m_parms[1]));
    break;

  default:
    assert ( false );
    break;
  }
  return 0.0;
}

} // namespace hippodraw

