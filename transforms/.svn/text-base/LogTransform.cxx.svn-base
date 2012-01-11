/** @file

hippodraw::LogTransform class implementation

Copyright (C) 2001-2004, 2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: LogTransform.cxx,v 1.48 2006/09/26 20:47:51 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "transforms/LogTransform.h"

#include "axes/AxisModelBase.h"
#include "axes/AxisTick.h"

#include <algorithm>
#include <vector>

#include <cmath>
#include <cstdio>

#include <cassert>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::abs;
using std::log10;
using std::pow;
using std::transform;
using std::vector;
#endif

using namespace hippodraw;

LogTransform::LogTransform ()
  : UnaryTransform ( 10 * DBL_MIN, DBL_MAX )
{
  m_name = "Log";
}

LogTransform::~LogTransform ()
{
}

LogTransform::LogTransform ( const LogTransform & lt )
  : UnaryTransform ( lt )
{
}

#ifdef CLONE_DEFECT
TransformBase * LogTransform::clone () const
#else
LogTransform  * LogTransform::clone () const
#endif
{
  return new LogTransform ( *this );
}

bool
LogTransform::
isLinear () const
{
  return false;
}

void LogTransform::transform ( double & x ) const
{
  x = log10 ( x );
}

void LogTransform::inverseTransform ( double & x ) const
{
  x = pow ( 10.0, x );
}

void
LogTransform::
transform ( std::vector < double > & x ) const
{
//  #ifdef TRANSFORM_DEFECT
  vector< double >::iterator first = x.begin();
  for ( ; first != x.end(); ++first ) {
    *first = log10 ( *first );
  }
//  #else

  // The following doesn't work for MS BC++ nor gcc 3.0.  It does work
  // with older versions of gcc and Sun CC, however.  So, who is right?

//    std::transform ( x.begin (), x.end (), x.begin (), log10 );
//  #endif
}

/* virtual */
void LogTransform::validate ( Range & range ) const
{
  double lo = range.low ();
  double hi = range.high ();

  if ( hi <= 0.0 ) hi = 1.0;
//   if ( lo <= 0.0 ) lo = hi - 0.99 * hi;
  if ( lo <= 0.0 ) lo = range.pos();

  range.setLow ( lo );
  range.setHigh ( hi );
}

/* virtual */
const vector < AxisTick > &
LogTransform::
setTicks ( AxisModelBase & axis )
{
  setTickStep( axis );
  setFirstTick( axis );

  return genTicks( axis );
}

void LogTransform::setTickStep( AxisModelBase & axis )
{
  const Range & range = axis.getRange(true);
  double low = range.low();
  double high = range.high();
  double rangeMag = high / low;
  
  // The following algorithm determines the magnitude of the range.
  double rmag = floor( log10( rangeMag ) );

  // This is used to determine the first tick.
  double pmag = ceil( log10( low ) );

  // Now we find the magnitude between ticks, getting the minimum
  // number of ticks without going below 4.
  double tmag = floor( rmag / 3.0 );

  double tick_step = pow( 10.0, tmag );

  axis.setRMag( rmag );
  axis.setPMag( pmag );
  axis.setTickStep( tick_step );
  
}

void LogTransform::setFirstTick( AxisModelBase & axis )
{
  const Range & range = axis.getRange(true);
  double low = range.low();
  
  //double high = range.high();
  
  // This sets the first tick as the low value rounded up to the
  // nearest power of 10.
  double pmag = axis.getPMag();
  double first_tick = pow( 10.0, pmag );
  double tmp = 0.0;
  while( ( tmp = prevStep( first_tick, axis )) >= low ) {
    first_tick = tmp;
  }

  axis.setFirstTick( first_tick );
}

/** @todo Should have this function in one place instead of
    duplicating it where needed.  Not sure where it should go,
    however. 
*/
inline double FLT_EQUAL( double x, double y )
{
  return ( (double)abs( x - y ) <= 2.0 * ( y * FLT_EPSILON + FLT_MIN ) );
}

/** @todo This is an ugly way to display scientific notation.  Can we
    improve it?  
    @todo Use C++ stringstream insted of sprintf(). */
const vector < AxisTick > &
LogTransform::
genTicks ( AxisModelBase & axis )
{
  double ylabel;

  int num_ticks = 0;

  m_ticks.clear ();
  
  // mag is used for scientific notation.
  double mag;

  char pstr[10];
  char labl[10];
  
  double first_tick = axis.getFirstTick();
  double tick_step  = axis.getTickStep();
  double scale_factor = axis.getScaleFactor();
  double max_ticks    = axis.getMaxTicks();
  const Range & range = axis.getRange(true);

  double range_low = range.low();
  double range_high = range.high();

  double last_tick = first_tick;//pow( 10.0, ceil( log10( range_high ) ) );
  double tmp = 0.0;
  while ( ( tmp = nextStep( last_tick, axis ) ) <= range_high ) {
    last_tick = tmp;
  }

  // The flag for scientific notation activation.
  bool sci_not = 
    ( floor( log10( last_tick ) ) > 3.0 ) ||
    ( floor ( log10 ( first_tick ) ) ) < -3.0;
  if( sci_not ) {
    sprintf( pstr, "%%1.0fe%%d" );
  }

  double value = first_tick / tick_step;
  bool fresh = true;
  while( value <= range_high || FLT_EQUAL( range_high, value ) ) {
    if( num_ticks >= max_ticks ) {
      // So far, this has only occurred for empty histograms. The
      //easy fix was to do nothing, but there ought to be a better
      // way to handle this.
      return m_ticks;
    }

    if( !fresh ) {
      value = nextStep( value, axis );
    } else {
      value *= tick_step;
      fresh = false;
    }

    if( value > range_high ) break;
    if( sci_not ) {
      mag = floor( log10( value ) );
      ylabel = value / pow( 10.0, mag );
      sprintf( pstr, "%%1.0fe%%d" );
      sprintf( labl, pstr, ylabel, static_cast<int>( mag ) );
    } else {
      ylabel = value;
      double tmp = floor( log10( value ) );
      if( tmp > 0.0 ) tmp = 0.0;
      tmp = fabs( tmp );
      sprintf( pstr, "%%1.%df", static_cast<int>( tmp ) );
      sprintf( labl, pstr, ylabel );
    } 
    
    double y = value / scale_factor;
    m_ticks.push_back( AxisTick ( y, labl ) );
    
    num_ticks++;
  }

  if ( num_ticks < 3 )
    {

      m_ticks.clear();
      
      double xx = (log10(range_high) - log10(range_low)) / 4;
      double yy = log10(range_low);
    
      for(int i=1; i<4; i++)
	{
	
	  value = pow (10.0, xx * i + yy);
          
	  if( value > range_high ) continue;
	  
	  double tmp = floor( log10( value ) );
	  if( tmp > 0.0 ) tmp = 0.0;
	  tmp = fabs( tmp );
	  if (tmp == 0.0)
	    {
	      value = floor(value);
	    }
      
	  sprintf( pstr, "%%1.%df", static_cast<int>( tmp ) );
	  sprintf( labl, pstr, value); 
      
	  double y = value / scale_factor;
	  
	  m_ticks.push_back( AxisTick ( y, labl ) );
	}
    }

  return m_ticks;
}

double LogTransform::nextStep ( double current, AxisModelBase & axis )
{
  double tick_step = axis.getTickStep(); // Must already be called
  if( tick_step == 1.0 ) {
    int bottom = static_cast<int>( current /
				 pow( 10.0, floor( log10( current ) ) ) );
    // Look! I used a switch statement in C++!!!!!  What this does
    // is go through and add the intermediate 2 and 5 ticks if the
    // powers of 10 alone would not have given the minimum number of
    // ticks.  m_tick_step is completely ignored if the flag is
    // true, since it is assumed to be 0.
    
    /** @todo: Change this to an array of good ticks rather than
	hardcoded in here. */
    switch( bottom ) {
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
      current *= 2.0;
//       assert ( false );
    }
  } else {
    current *= tick_step;
  }
  return current;
}

/** @todo Change this to an array of good ticks rather than hardcoded
    in here.  */
double LogTransform::prevStep ( double current, AxisModelBase & axis )
{
  double tick_step = axis.getTickStep(); // It must already be called.
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

const Range &
LogTransform::adjustValues ( AxisModelBase & axis, const Range & limit )
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
  
  adjustLogValues( axis );
  setTickStep( axis ); // For tick steps are needed by nextStep() & prevStep().

  const Range & init_range = axis.getRange(false);
  double low = init_range.low();
  double high = init_range.high();

  myhigh = mylow = pow( 10.0, axis.getPMag() );

  // This decreases mylow so that "myrange" covers the whole range
  // and then some.
  double scale_factor = axis.getScaleFactor();
  while( mylow >= low * scale_factor ) {
    mylow = prevStep( mylow, axis );
  }
  
  // This increases myhigh so that "myrange" covers the whole range
  // and then some.
  while( myhigh <= high * scale_factor ) {
    myhigh = nextStep( myhigh, axis );
  }
  
  // If the range has a magnitude < 10.0, reduce the minimum of the
  // range by one tick mark.
  if( myhigh / mylow < 10.0 ) {
    mylow = prevStep( mylow, axis );
  }
  
  // If the range still has a magnitude < 10.0, increase the maximum
  // of the range by one tick mark until the magnitude is 10.0.
  while( myhigh / mylow < 10.0 ) {
    myhigh = nextStep( myhigh, axis );
  }

  myhigh /= scale_factor;
  mylow  /= scale_factor;

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
    axis.setIntersectRange ( newRange, limit );
    return axis.getRange( false );
  
  }

  axis.setIntersectRange ( new_range, limit );

  return axis.getRange( false );
}


const Range &  LogTransform::adjustLogValues ( AxisModelBase & axis )
{
  const Range & r = axis.getRange( false );
  double low = r.low();
  double high = r.high();
  double pos = r.pos();
  
  if( low > 0.0 ) return r;

  if( pos == high ) { // Will give no range
    double l = pos / 10.0;
    double h = pos * 10.0;
    axis.setRange ( l, h, pos );

    return axis.getRange( false );
  }
  if( pos == DBL_MAX || pos <= 0.0 ) { // No positive values!!!
    axis.setRange ( 0.01, 100.0, 1.0 );
    return axis.getRange( false );
  }
  if ( low <= 0.0 ) axis.setRange ( 0.5 * pos, high, pos );
  else  axis.setRange ( pos, high, pos );

  return axis.getRange( false );
}
