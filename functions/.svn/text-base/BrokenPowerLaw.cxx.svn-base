/**@file

hippodraw::BrokenPowerLaw class implementation

Copyright (C) 2000, 2003, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BrokenPowerLaw.cxx,v 1.4 2007/04/23 21:57:20 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "BrokenPowerLaw.h"

#include "FunctionHelper.h"

#include <cmath>
#include <cassert>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::max;
using std::vector;
#endif

namespace hippodraw {

BrokenPowerLaw::BrokenPowerLaw ( )
{
  initialize ();
}

BrokenPowerLaw::BrokenPowerLaw ( double prefactor, double index1,
                                 double index2, double x_break)
{
  initialize ();

  m_parms[0] = prefactor;
  m_parms[1] = index1;
  m_parms[2] = index2;
  m_parms[3] = x_break;
}

void BrokenPowerLaw::initialize ()
{
  m_name = "BrokenPowerLaw";
  m_parm_names.push_back ( "Prefactor" );
  m_parm_names.push_back ( "Index1" );
  m_parm_names.push_back ( "Index2" );
  m_parm_names.push_back ( "Break" );

  resize ();
}

FunctionBase * BrokenPowerLaw::clone () const
{
  return new BrokenPowerLaw ( *this );
}

double BrokenPowerLaw::operator () ( double x ) const
{
   if (x < m_parms[3]) {
      return m_parms[0]*pow(x/m_parms[3], m_parms[1]);
   } else {
      return m_parms[0]*pow(x/m_parms[3], m_parms[2]);
   }
}

/* virtual */
void 
BrokenPowerLaw::
initialParameters ( const FunctionHelper * helper )
{
  double min_x = helper->minCoord ();
  double max_x = helper->maxCoord ();
  max_x = (min_x + max_x)/2.;

  double min_y, max_y;
  try {
     min_y = helper->valueAt(min_x);
     max_y = helper->valueAt(max_x);
     if (min_y != 0 && max_y != 0) {
        m_parms[1] = log( max_y/min_y ) / log( max_x/min_x );
        m_parms[2] = m_parms[1];
        m_parms[3] = helper->meanCoord();
        m_parms[0] = max_y/pow(max_x/m_parms[3], m_parms[1]);
        return;
     }
  } catch (...) {
// do nothing
  }

// default behavior
  min_y = max(helper->minValue(), 1.);
  max_y = helper->maxValue();
  m_parms[1] = log( max_y/min_y ) / log( max_x/min_x );
  m_parms[2] = m_parms[1];
  m_parms[3] = helper->meanCoord();
  m_parms[0] = max_y/pow(max_x/m_parms[3], m_parms[1]);
}

double BrokenPowerLaw::derivByParm ( int i, double x ) const
{
  switch ( i ) {
  case 0 :
     return operator()(x)/m_parms[0];
     break;

  case 1 :
     if (x < m_parms[3]) {
        return operator()(x)*log(x/m_parms[3]);
     } else {
        return 0;
     }
     break;

  case 2 :
     if (x < m_parms[3]) {
        return 0;
     } else {
        return operator()(x)*log(x/m_parms[3]);
     }
     break;

  case 3 :
     if (x < m_parms[3]) {
        return -m_parms[1]*operator()(x)/m_parms[3];
     } else {
        return -m_parms[2]*operator()(x)/m_parms[3];
     }
     break;

  default:
    assert ( false );
    break;
  }
  return 0.0;
}

} // namespace hippodraw
