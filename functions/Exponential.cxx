/**@file

hippodraw::Exponential class implementation

Copyright (C) 2000, 2003, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Exponential.cxx,v 1.4 2007/04/23 21:57:20 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "Exponential.h"

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

Exponential::Exponential ( )
{
  initialize ();
}

Exponential::Exponential ( double prefactor, double scale )
{
  initialize ();

  m_parms[0] = prefactor;
  m_parms[1] = scale;
}

void Exponential::initialize ()
{
  m_name = "Exponential";
  m_parm_names.push_back ( "Prefactor" );
  m_parm_names.push_back ( "Scale" );

  resize ();
}

FunctionBase * Exponential::clone () const
{
  return new Exponential ( *this );
}

double Exponential::operator () ( double x ) const
{
   return m_parms[0]*exp( -x/m_parms[1] );
}

/* virtual */
void 
Exponential::
initialParameters ( const FunctionHelper * helper )
{
   double min_x = helper->minCoord ();
   double max_x = helper->maxCoord ();
   max_x = (min_x + max_x)/2.;

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

double Exponential::derivByParm ( int i, double x ) const
{
  switch ( i ) {
  case 0 :
    return exp( -x/m_parms[1] );
    break;

  case 1 :
     return operator()(x)*(x/m_parms[1]/m_parms[1]);
    break;

  default:
    assert ( false );
    break;
  }
  return 0.0;
}

} // namespace hippodraw

