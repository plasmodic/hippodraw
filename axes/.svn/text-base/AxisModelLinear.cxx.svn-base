/** @file

AxisModelLinear class implementation

Originally from C code written at SLAC by Jonas Karlsson in August
1990. Modified substantially since August 1990 by Gravina, Kunz,
Pavel, Rensing, and Oded Wurman.

Copyright (C) 2000-2004   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: AxisModelLinear.cxx,v 1.110 2005/12/05 18:47:47 pfkeb Exp $

*/

#ifdef _MSC_VER
// Include max() and min() missing from MicroSoft Visual C++.
#include "msdevstudio/MSconfig.h"
#endif //_MSC_VER

#include "AxisModelLinear.h"

#include "AxisTick.h"

#include <algorithm>

#include <cmath>
#include <cstdio>

using std::abs;
using std::ceil;
using std::floor;
using std::log10;
using std::max;
using std::min;
using std::pow;
using std::string;
using std::vector;

namespace hippodraw {

AxisModelLinear::AxisModelLinear ( AxisLoc label_loc, 
				   AxisLoc scale_loc )
  : AxisModelBase ( label_loc, scale_loc )
{    
}

AxisModelLinear::AxisModelLinear( const AxisModelBase & axis_model )
  : AxisModelBase( axis_model )
{
}

AxisModelLinear::~AxisModelLinear()
{
}

/* virtual */
AxisModelBase * AxisModelLinear::clone() const
{
  return new AxisModelLinear( *this );
}

// bool FLT_EQUAL( double x, double y )
// {
//   return ( abs( x - y ) <= 100.0 *  DBL_EPSILON  );
// }

bool AxisModelLinear::isLog() const 
{ 
  return false; 
}

bool AxisModelLinear::needPMag () const
{
  return m_use_pmag;
}

const Range &
AxisModelLinear::adjustValues ( const Range & limit )
{
  //Because the low value, the high value, and the length value of the
  //range were so frequently used, I added those three fields. There 
  //should be an improvement in performance.
  double mylow, myhigh;
  
  //The use of a step field and of a mag field will be explained when
  //they are first initialized.
  double step, magnitude;
  
  adjustLogValues();
  const int N_NICE = 6;
#ifndef __STDC__
  static
#endif
    float nice[N_NICE] = { 1.0, 2.0, 2.5,
			   4.0, 5.0, 7.5 };

  const Range & init_range = getRange ( false );
  double low = init_range.low ();
  double high = init_range.high ();
  if ( low == high ) {  // all values in same bin
    if ( low > 0.0 ) low *= 0.95;
    else  low *= 1.05;

    if ( high > 0.0 ) high *= 1.05;
    else high *= 0.95;

    setRange ( low, high, low );
  }  
  //setTickStep();
  double range_length;
  
  int i;
  
  //double tick_step = getTickStep();
  //tick_step /= m_scale_factor;
  //myhigh = mylow = floor( low / tick_step ) * tick_step;

  // This increases myhigh so that "myrange" covers the whole range
  // and then some.
  //while( myhigh <= high ) myhigh += tick_step;
  mylow  = low  - 0.05*(high-low);
  myhigh = high + 0.05*(high-low);

  range_length = myhigh - mylow;

  // We have now decided on a range.  This tries to move low/high a
  // little to end up on a nice number.

  // First checks if either end is near 0.0
  if( low >= 0.0 && range_length > ( 1.05 * high ) ) {
    Range range ( 0.0, range_length );
    setIntersectRange ( range, limit );
    return m_range;
  }
  if( high <= 0.0 && -range_length < ( 1.05 * low ) ) {
    Range range ( -range_length, 0.0 );
    setIntersectRange ( range, limit );
    return m_range;
  }

  // magnitude is used to hold the magnitude of the high or low values.

  i = N_NICE - 1;
  if( myhigh != 0.0 )
    magnitude = ceil( log10( abs( myhigh ) ) );
  else
    magnitude = ceil( log10( abs( mylow ) ) );
  
  // What this part does is go through the low, giving it round
  // numbers first, but more coarse over time.

  do {
    step = nice[i] * pow( 10.0, magnitude );
    mylow = floor( low / step ) * step;
    myhigh = mylow + 1.05 * range_length;
    i--;
    if( i < 0 ) {
      i = N_NICE - 1;
      magnitude--;
    }
  } while( myhigh < high );

  Range range ( mylow, myhigh, init_range.pos() );  

  setIntersectRange ( range, limit );

  return m_range;
}

const Range &
AxisModelLinear::adjustLogValues()
{
  // This doesn't do jack.
  return getRange(false);
}

/** @todo We will have to put a check here to keep it from hitting a
    length of 0. */
Range AxisModelLinear::calcLow ( int parm, bool dragging )
{
  startDragging ( dragging );

  double length = m_start_range.length ();
  double low = m_start_range.low ();
  double high = m_start_range.high ();

  double multiplier = ( parm - 50 ) / 50.0;
  double new_low = min ( low + length * multiplier, high );

  if( new_low == m_range.high() ) return m_range;

  return Range ( new_low, high, m_range.pos() );
}

/** @todo We will have to put a check here to keep it from hitting a
    length of 0. */
Range AxisModelLinear::calcHigh ( int parm, bool dragging )
{
  startDragging ( dragging );

  double length = m_start_range.length ();
  double low = m_start_range.low ();
  double high = m_start_range.high ();

  double multiplier = ( parm - 50 ) / 50.0;
  double new_high = max ( high + length * multiplier, low );

  if( new_high == m_range.low() ) return m_range;

  return Range ( low, new_high, m_range.pos() );
}

} // namespace hippodraw
