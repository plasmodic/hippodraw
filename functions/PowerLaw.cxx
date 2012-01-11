/** @file

hippodraw::PowerLaw class implementation

Copyright (C) 2000, 2003, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: PowerLaw.cxx,v 1.5 2007/04/23 21:57:20 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "PowerLaw.h"

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

PowerLaw::PowerLaw ( )
{
  initialize ();
}

PowerLaw::PowerLaw ( double prefactor, double index )
{
  initialize ();

  m_parms[0] = prefactor;
  m_parms[1] = index;
}

void PowerLaw::initialize ()
{
  m_name = "PowerLaw";
  m_parm_names.push_back ( "Prefactor" );
  m_parm_names.push_back ( "Index" );

  resize ();
}

FunctionBase * PowerLaw::clone () const
{
  return new PowerLaw ( *this );
}

double PowerLaw::operator () ( double x ) const
{
   return m_parms[0]*pow(x, m_parms[1]);
}

/* virtual */
void 
PowerLaw::
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
        m_parms[0] = max_y/pow(max_x, m_parms[1]);
        return;
     }
  } catch (...) {
// do nothing
  }

// default behavior
  min_y = max(helper->minValue(), 1.);
  max_y = helper->maxValue();
  m_parms[1] = log( max_y/min_y ) / log( max_x/min_x );
  m_parms[0] = max_y/pow(max_x, m_parms[1]);
}

double PowerLaw::derivByParm ( int i, double x ) const
{
  switch ( i ) {
  case 0 :
    return pow(x, m_parms[1]);
    break;

  case 1 :
     return m_parms[0]*pow(x, m_parms[1])*log(x);
    break;

  default:
    assert ( false );
    break;
  }
  return 0.0;
}

} // namespace hippodraw

