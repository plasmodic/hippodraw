/** @file

hippodraw::AxisModelLog class implementation

Originally from C code written at SLAC by Jonas Karlsson in
August 1990. Modified substantially since August 1990 by Gravina, 
Kunz, Pavel, Rensing, and Oded Wurman.

Copyright (C) 2000-2005  The Board of Trustees of The Leland * Stanford
Junior University.  All Rights Reserved.

$Id: AxisModelLog.cxx,v 1.106 2005/12/05 18:47:47 pfkeb Exp $

*/

#ifdef _MSC_VER
// Includes max() and min() missing from MicroSoft Visual C++.
#include "msdevstudio/MSconfig.h"
#endif //_MSC_VER

#include "AxisModelLog.h"

#include "AxisTick.h"

#include <algorithm>

#include <cassert>
#include <cmath>
#include <cstdio>

using std::abs;
using std::max;
using std::min;
using std::string;
using std::vector;

using namespace hippodraw;

AxisModelLog::AxisModelLog ( AxisLoc label_loc, 
			     AxisLoc scale_loc )
  : AxisModelBase ( label_loc, scale_loc )
{    
}

AxisModelLog::AxisModelLog ( const AxisModelBase & axis_model )
  : AxisModelBase ( axis_model )
{
  // We just changed to log.  Therefore assure positive minimum.
  adjustLogValues();
}

AxisModelLog::~AxisModelLog()
{
}

/* virtual */
AxisModelBase * AxisModelLog::clone() const
{
  return new AxisModelLog( *this );
}
bool AxisModelLog::isLog() const 
{ 
  return true; 
}

void AxisModelLog::setTickStep()
{
  const Range & range = getRange(true);
  double low = range.low();
  double high = range.high();
  double rangeMag = high / low;
  
  // The following algorithm determines the magnitude of the range.
  m_rmag = floor( log10( rangeMag ) );

  // This is used to determine the first tick.
  m_pmag = ceil( log10( low ) );

  // Now we find the magnitude between ticks, getting the minimum
  // number of ticks without going below 4.
  double m_tmag = floor( m_rmag / 3.0 );

  m_tick_step = pow( 10.0, m_tmag );
}

const Range & 
AxisModelLog::adjustValues ( const Range & limit )
{

  //Because the low value, the high value, and the length value of the
  //range were so frequently used, I added those three fields. There 
  //should be an improvement in performance.
  
  double mylow, myhigh;

  // We want to make sure that this is autoscaled. Therefore, to 
  // be on the safe side we set the minimum range to 0, so that 
  // the minimum positive value is used.
  //   Range log( 0.0, getRange().high(), getRange().pos() );
  //   setRange( log );
  
  adjustLogValues();
  setTickStep(); // Needed for nextStep() and prevStep().

  const Range & init_range = getRange(false);
  double low = init_range.low();
  double high = init_range.high();

  myhigh = mylow = pow( 10.0, m_pmag );

  // This decreases mylow so that "myrange" covers the whole range
  // and then some.
  while( mylow >= low * m_scale_factor ) {
    mylow = prevStep( mylow );
  }
  
  // This increases myhigh so that "myrange" covers the whole range
  // and then some.
  while( myhigh <= high * m_scale_factor ) {
    myhigh = nextStep( myhigh );
  }
  
  // If the range has a magnitude < 10.0, reduce the minimum of the
  // range by one tick mark.
  if( myhigh / mylow < 10.0 ) {
    mylow = prevStep( mylow );
  }
  
  // If the range still has a magnitude < 10.0, increase the maximum
  // of the range by one tick mark until the magnitude is 10.0.
  while( myhigh / mylow < 10.0 ) {
    myhigh = nextStep( myhigh );
  }

  myhigh /= m_scale_factor;
  mylow  /= m_scale_factor;

  Range new_range ( mylow, myhigh, init_range.pos() );
  
  // Compare the newrange with init_range. If new range is too wide
  // compared to init_range, then do not set newrange.

  double new_width = new_range.length();
  double init_width = init_range.length();
  
  if ( new_width > init_width * 10 ){  // This 10 is a hack. Could be any
                                       // decent number.
    if ( low < 0 ) {
      low *= 1.05;    // This 5% is also a hack.
    }
    else{
      low *= 0.95;
    }
    
    if ( high < 0 ){
      high *= 0.95;
    }
    else{
      high *= 1.05;
    }
    
    Range newRange ( low, high, init_range.pos() );
    setIntersectRange ( newRange, limit );
    return m_range;
  
  }

  setIntersectRange ( new_range, limit );
  return m_range;
  
  //  // The following sets the range too wide.  Oded, what did you have
  //  // in mind with this?
  //  mylow = getRange(false).low() * sqrt( getRange(false).low() / 
  //					  getRange(false).high()     );
  //  myhigh = getRange(false).high() * sqrt( getRange(false).high() / 
  //					    getRange(false).low()    );
  //  Range range(mylow, myhigh, getRange(false).pos());
  //  setIntersectRange ( range, limit );
  //  return m_range;

}

const Range & 
AxisModelLog::adjustLogValues ()
{
  const Range & r = getRange(false);
  double low = r.low();
  double high = r.high();
  double pos = r.pos();
  
  if( low > 0.0 ) return r;

  if( pos == high ) { // Will give no range
//     setRange ( pos / 10.0, pos * 10.0, pos );
    double l = pos / 10.0;
    double h = pos * 10.0;
    setRange ( l, h, pos );

    return m_range;
  }
  if( pos == DBL_MAX || pos <= 0.0 ) { // No positive values!!!
    setRange ( 0.01, 100.0, 1.0 );
    return m_range;
  }
  setRange ( pos, high, pos );

  return m_range;
}

double AxisModelLog::nextStep ( double current )
{
  double tick_step = getTickStep(); // Must already be called
  if( tick_step == 1.0 ) {
    int base = static_cast<int>( current /
				 pow( 10.0, floor( log10( current ) ) ) );
    // Look! I used a switch statement in C++!!!!!  What this does
    // is go through and add the intermediate 2 and 5 ticks if the
    // powers of 10 alone would not have given the minimum number of
    // ticks.  m_tick_step is completely ignored if the flag is
    // true, since it is assumed to be 0.
    
    /** @todo: Change this to an array of good ticks rather than
	hardcoded in here. */
    switch( base ) {
    case 1:
      current *= 2.0;
      break;
    case 2:
      current /= 2.0;
      current *= 5.0;
      break;
    case 3:
      current /= 4.0;
      current *= 5.0;
      break;
    case 4: // a 5 becomes a 4 sometimes because of round of error
    case 5:
      current *= 2.0;
      break;
    default:
      assert ( false );
    }
  } else {
    current *= tick_step;
  }
  return current;
}

/** @todo Change this to an array of good ticks rather than hardcoded
    in here.  */
double AxisModelLog::prevStep ( double current )
{
  double tick_step = getTickStep(); // It must already be called.
  if( tick_step == 1.0 ) {
    int base = static_cast<int>( current /
				 pow( 10.0, floor( log10( current ) ) ) );
    // Look! I used a switch statement in C++!!!!!  What this does
    // is go through and add the intermediate 2 and 5 ticks if the
    // powers of 10 alone would not have given the minimum number of
    // ticks.  m_tick_step is completely ignored if the flag is
    // true, since it is assumed to be 0.
    
    switch( base ) {
    case 1:
      current /= 2.0;
      break;
    case 2:
      current /= 2.0;
      break;
    case 4:
      current /= 5.0;
      current *= 2.0;
      break;
    case 5:
      current /= 5.0;
      current *= 2.0;
      break;
    default:
      assert ( false );
    }
  } else {
    current /= tick_step;
  }
  return current;
}

/** @todo Check is in place to keep it from length of 0,
	but it's fairly hacked (length > 0.0001).  This whole
	topic needs discussion. */
Range AxisModelLog::calcLow ( int parm, bool dragging )
{
  startDragging ( dragging );

  double low = m_start_range.low ();
  double high = m_start_range.high ();
  double k = log10 ( high / low );

  double x = ( parm - 50 ) / 50.0;

  double new_low = low * pow ( 10.0, k * x );

  new_low = max ( new_low, 10.0 * DBL_EPSILON );
  new_low = min ( new_low,  high - 100.0 * DBL_EPSILON );

  if( abs( new_low - m_range.high() ) < 0.0001 ) return m_range;

  return Range ( new_low, high, m_range.pos() );
}

/** @todo Check is in place to keep it from length of 0,
	but it's fairly hacked (length > 0.0001).  This whole
	topic needs discussion. */
Range AxisModelLog::calcHigh ( int parm, bool dragging )
{
  startDragging ( dragging );

  double low = m_start_range.low ();
  double high = m_start_range.high ();
  double k = log10 ( high / low );

  double multiplier = ( parm - 50 ) / 50.0;

  double new_high = high * pow ( 10.0, k * multiplier );

  if( abs( new_high - m_range.low() ) < 0.0001 ) return m_range;

  return Range ( low, new_high, m_range.pos() );
}
