/** @file

LinearTransform class implementation

Copyright (C)  2001-2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: LinearTransform.cxx,v 1.35 2007/08/27 18:24:30 pfkeb Exp $

*/

#include "LinearTransform.h"

#include "axes/AxisModelBase.h"
#include "axes/AxisTick.h"

#include <cmath>
#include <cstdio>

using std::abs;
using std::max;
using std::vector;

namespace hippodraw {

/** Should never be called directly. Sets the function name to "nil".
    Should be overriden by derived class.  If function name
    is "nil", then that indicated error condition that default
    constructor was called directly. */
LinearTransform::LinearTransform ()
  : UnaryTransform ( -DBL_MAX, DBL_MAX )
{
  m_name = "Linear";
}

LinearTransform::~LinearTransform ()
{
}

LinearTransform::LinearTransform ( const LinearTransform & lt )
  : UnaryTransform ( lt )
{
}

#ifdef CLONE_DEFECT
TransformBase   * LinearTransform::clone () const
#else
LinearTransform * LinearTransform::clone () const
#endif 
{
  return new LinearTransform ( *this );
}

bool
LinearTransform::
isLinear () const
{
  return true;
}

void
LinearTransform::
transform ( double & ) const
{
}

void
LinearTransform::
inverseTransform ( double & ) const
{
}

void
LinearTransform::
transform ( std::vector < double > & ) const
{
}

/* virtual */
void
LinearTransform::
validate ( Range & ) const
{
  // Nothing to be done.
}

const vector < AxisTick > &
LinearTransform::
setTicks ( AxisModelBase & axis )
{
  setTickStep( axis );
  setFirstTick( axis );

  return genTicks( axis );
}

inline double FLT_EQUAL( double x, double y )
{
  return ( (double)abs( x - y ) <= 2.0 * ( y * FLT_EPSILON + FLT_MIN ) );
}

void LinearTransform::setTickStep( AxisModelBase & axis )
{
  static float goodTicks[] = { 5.0, 4.0, 2.0, 1.0 };
  int tickIndex;
  
  const Range & range = axis.getRange(false);
  double rangeLength = range.length();

  double scale_factor = axis.getScaleFactor();
  rangeLength *= scale_factor;
  const int MIN_TICKS = 3;
  
  // The following algorithm determines the magnitude of the range...
  double rmag = floor( log10( rangeLength ) );
  
  // ...and then decreases by one magnitude if it would allow less than
  // 3 ticks (e.g., the range is 25000 and the magnitude is 10000.  This
  // would allow for 2.5 ticks while we in fact would rather have a
  // magnitude of 1000, multiplied by a constant, as below).

  if( rangeLength / pow( 10.0, rmag ) < MIN_TICKS ) {
    rmag--;
  }

  axis.setRMag( rmag );
  
  double scalelow = range.low() * scale_factor;
  double scalehigh = range.high() * scale_factor;

  // We will also need the largest magnitude for labels.
  double pmag = max( floor( log10( abs ( scalehigh ) ) ), 
		     floor( log10( abs ( scalelow ) ) ) );

  // This if statement changes the magnitude so that if the high or
  // low is exactly a power of 10, we will give labels from
  // [1,10] * 10^mag and not [0,1] * 10^mag.
  if( pow( 10.0, pmag ) == scalehigh ||
      pow( 10.0, pmag ) == scalelow ) pmag--;

  axis.setPMag( pmag );
  
  // Now we determine the above stated constant.  The magnitude is already
  // known, so we see what's the closest we can get to exactly 3 ticks
  // under this magnitude.  In the above example, a range of 25000 was
  // given with a magnitude of 1000.  This algorithm will recognize that
  // 5.0 * 1000 will give 5 ticks, which is a good number to have.  If
  // the range was 12000 and magnitude 1000 then 5.0 * 1000 would give
  // only 2 ticks, not enough.  The loop would then proceed to 4.0 * 1000
  // which will give exactly 3 ticks.
  double tick_step = 0;
  for( tickIndex = 0;
       rangeLength /
	 ( tick_step = goodTicks[tickIndex] * pow( 10.0, rmag ) )
	 < MIN_TICKS;
       tickIndex++ ){};

  axis.setTickStep( tick_step );
}


void LinearTransform::setFirstTick( AxisModelBase & axis )
{
  const Range & range = axis.getRange(true);
  double low = range.low();
  double tick_step = axis.getTickStep();

  // This sets the first tick as the low value rounded up to the
  // nearest tick step.  If the low value fell on a tick, then that is
  // the first tick.  Otherwise, it is the next tick inside the range
  // of the data.
  axis.setFirstTick( ceil( low / tick_step ) * tick_step );
}


/** @todo Handle too many ticks better, rather than doing nothing.
    See more detailed comment in code. */
const vector < AxisTick > &
LinearTransform::
genTicks( AxisModelBase & axis )
{ 
  double y = 0.0, ylabel;
  
  int num_ticks = 0;
  m_ticks.clear();
  double pmag = axis.getPMag();
  double rmag = axis.getRMag();
  double first_tick = axis.getFirstTick();
  double tick_step  = axis.getTickStep();
  double scale_factor = axis.getScaleFactor();
  double max_ticks    = axis.getMaxTicks();
    
  // pmag will get set to 0 if it is less than or equal to 3.  This
  // is used later to determine scientific notation.  However, m_rmag
  // is still needed as the original magnitude for calculations such
  // as decimal place notation, and rounding to nice numbers.
  
  //   if( fabs( m_pmag ) <= 3.0 ) m_pmag = 0.0;
  bool use_pmag = abs ( pmag ) > 3.0;

  axis.setUsePMag ( use_pmag );
  
  char pstr[10];
  char labl[10];

  int decimals = 0;

  // The following if-else block decides the pstr string, which holds
  // the number of decimal places in the label.

  //   if( fabs( m_pmag ) > 3.0 ) {
  if ( use_pmag ) {  
    // If we are greater than mag 3, we are showing scientific
    // notation.  How many decimals we show is determined by the
    // difference between the range magnitude and the power magnitude.
  
    decimals = static_cast<int>( pmag - rmag );
    // minumum 1 decimal in scientific notation
    
    if( !decimals ) decimals++;
  
  } else {
    
    if( rmag > 0.0 ){
    
      // If we are less than mag 3 and positive, then no decimal
      // accuracy is needed.
      
      decimals = 0;
      
    } else {
    
      // If we are less than mag 3 and negative, then we are suddenly
      // looking at decimal numbers not in scientific notation.
      // Therefore we hold as many decimal places as the magnitude.
      
      decimals = static_cast<int>( abs( rmag ) );
      
    }
  
  }
  // @todo decimals should never be negative here, but it does end up
  //    negative in some cases. See the "dirty fix" in Range.cxx, that
  //    dirty-fixed this problem too. But a better fix is needed. 
  if (decimals < 0) {
    decimals = 0;
  }
  
  sprintf( pstr, "%%1.%df", decimals );

  y = first_tick;
  const Range & range = axis.getRange(false);
  double range_high = range.high();
  range_high *= scale_factor;
  range_high += 100. * DBL_EPSILON;

 // while( y <= range_high || FLT_EQUAL( range_high, y ) ) {
  while( y <= range_high ) {
  
    if( num_ticks >= max_ticks ) {
    
      // HERE So far, this has only occurred for empty histograms. The
      //easy fix was to do nothing, but there ought to be a better
      // way to handle this.
      
      return m_ticks;
    
    }

    // The following expression is used to round to the nearest nice
    // number, and then return to the original magnitude.
    
    double value = floor( y / pow( 10.0, rmag ) + 0.5 ) *
      pow( 10.0, rmag );

    // Now that the number is nice, we either keep the original magnitude
    // or reduce it in order to express it in scientific notation.
    
    if ( use_pmag )  ylabel = value / pow( 10.0, pmag );
    else ylabel = value;

    value /= scale_factor;
    sprintf( labl, pstr, ylabel );
    m_ticks.push_back( AxisTick ( value, labl ) );
    
    num_ticks++;
    y += tick_step;
  
  }

  return m_ticks;
}

const Range & LinearTransform::adjustValues ( AxisModelBase & axis,
					      const Range & limit )
{
  //Because the low value, the high value, and the length value of the
  //range were so frequently used, I added those three fields. There 
  //should be an improvement in performance.
  double mylow, myhigh;
  
  //The use of a step field and of a mag field will be explained when
  //they are first initialized.
  double step, magnitude;
  
  const int N_NICE = 6;
#ifndef __STDC__
  static
#endif
    float nice[N_NICE] = { 1.0, 1.5, 2.0,
			   3.0, 5.0, 7.5 };

  const Range & init_range = axis.getRange ( false );
  double low = init_range.low ();
  double high = init_range.high ();

  if ( ( high - low ) < 10.* DBL_MIN  ) {  // all values in same bin
    if ( low > 0.0 ) low *= 0.95;
    else  low *= 1.05;

    if ( low == 0. ) { // special case
      high = low + 1000. * FLT_EPSILON; // large enough so tick algo works
    }
    else {
      if ( high > 0.) high *= 1.05;
      else high *= 0.95;
    }

    axis.setRange ( low, high, low );
  }  
  double range_length;
  
  int i;
  
  // This increases myhigh so that "myrange" covers the whole range
  // and then some.

  mylow  = low  - 0.05*(high-low);
  myhigh = high + 0.05*(high-low);

  range_length = myhigh - mylow;

  // We have now decided on a range.  This tries to move low/high a
  // little to end up on a nice number.

  // First checks if either end is near 0.0
  //checking high against 22*low is equivalent to the
  //original condition
  if( low >= 0.0 && high > 22 * low ) {
    Range range ( 0.0, range_length );
    axis.setIntersectRange ( range, limit );
    return axis.getRange( false );
  }
  if( high <= 0.0 && low < 22 * high ) {
    Range range ( -range_length, 0.0 );
    axis.setIntersectRange ( range, limit );
    return axis.getRange( false );
  }

  // magnitude is used to hold the magnitude of the high or low values.
  magnitude = floor(log10(abs(range_length)));
  float norm = range_length / pow(10., magnitude);
  float r, x;

  float r_previous = 10;
  for (i = 0; i < N_NICE; i++) {
    r = abs(norm / nice[i] - 1);
    if (r < r_previous) {
      r_previous = r;
      x = nice[i];
    } else
      break;
  }
  //takes as for the step one fifth of nice[i]
  step = 0.2 * x * pow(10, magnitude - 1);
  mylow = floor(mylow / step) * step;
  myhigh = ceil(myhigh / step) * step;

  Range range ( mylow, myhigh, init_range.pos() );  

  axis.setIntersectRange ( range, limit );

  return axis.getRange( false );
}

} // namespace hippodraw

