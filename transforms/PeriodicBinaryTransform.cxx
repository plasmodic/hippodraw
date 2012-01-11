/** @file

hippodraw::PeriodicBinaryTransform class implementation

Copyright (C) 2003-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.


*/

// for wcslib
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "transforms/PeriodicBinaryTransform.h"

#include "axes/AxisModelBase.h"
#include "graphics/Rectangle.h"
#include "UnaryTransform.h"

#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <sstream>
#include <cassert>

using namespace hippodraw;

using std::max;
using std::abs;
using std::vector;

/** Should never be called.. Sets the function name to "nil".
    Should be overriden by derived class.  If function name
    is "nil", then that indicated error condition that default
    constructor was called directly. 
*/
PeriodicBinaryTransform::PeriodicBinaryTransform() :
  BinaryTransform(),
  m_x_limits ( -180.0, +180.0 ),
  m_y_limits ( - 90.0, + 90.0 ),
  m_x_offset ( 0.0 ),
  m_y_offset ( 0.0 ),
  m_x_units  ( UNITS_DEGREES ),
  m_y_units  ( UNITS_DEGREES )
{
}

PeriodicBinaryTransform::PeriodicBinaryTransform ( UnaryTransform * z,
						   bool is_periodic,
						   bool needs_grid,
						   bool needs_x_ticks,
						   bool needs_y_ticks,
						   double xlo, double xhi, 
						   double xunits,
						   double ylo, double yhi,
						   double yunits) :
  BinaryTransform ( z, is_periodic, needs_grid, needs_x_ticks, needs_y_ticks ),
  m_x_limits ( xlo, xhi ),
  m_y_limits ( ylo, yhi ),
  m_x_offset ( 0.0 ),
  m_y_offset ( 0.0 ),
  m_x_units  ( xunits ),
  m_y_units  ( yunits )
{
}

PeriodicBinaryTransform::
PeriodicBinaryTransform ( const PeriodicBinaryTransform & t ) :
  BinaryTransform ( t ),
  m_x_limits( t.limitX() ),
  m_y_limits( t.limitY() ),
  m_x_offset( t.rotation(Axes::X) ),
  m_y_offset( t.rotation(Axes::Y) ),
  m_x_units ( t.units(Axes::X) ),
  m_y_units ( t.units(Axes::Y) )
{
}

PeriodicBinaryTransform::~PeriodicBinaryTransform ()
{
  // Does nothing 
}

void PeriodicBinaryTransform::setLimitX (const Range &limits)
{
  m_x_limits = limits;
}

const Range & PeriodicBinaryTransform::limitX () const
{
  return m_x_limits;
}


void PeriodicBinaryTransform::setLimitY (const Range &limits)
{
  m_y_limits = limits;
}

const Range & PeriodicBinaryTransform::limitY () const
{
  return m_y_limits;
}


double PeriodicBinaryTransform::rotation(hippodraw::Axes::Type axis) const
{
  return (axis == Axes::X) ? m_x_offset : m_y_offset;
}

void PeriodicBinaryTransform::setRotation(double offset, 
    hippodraw::Axes::Type axis)
{
  if (axis == Axes::X)
	  m_x_offset = offset;
  else if (axis == Axes::Y) 
	  m_y_offset = offset;
}

double PeriodicBinaryTransform::units(hippodraw::Axes::Type axis) const
{
  return (axis == Axes::X) ? m_x_units : m_y_units;
}

void PeriodicBinaryTransform::setUnits(double units, hippodraw::Axes::Type axis)
{
  if (axis == Axes::X)
	  m_x_units = units;
  else if (axis == Axes::Y)
	  m_y_units = units;
}

double
PeriodicBinaryTransform::
moduloAdd( double a1, double a2, hippodraw::Axes::Type axis ) const
{
  if ( axis == Axes::X ) {
    return moduloAddX ( a1,  a2);
  }
  else if ( axis == Axes::Y ) {
    return moduloAddY ( a1,  a2);
  }
  else  return a1 + a2;
}

double
PeriodicBinaryTransform::
moduloSub( double s1, double s2, hippodraw::Axes::Type axis ) const
{
  if ( axis == Axes::X ) {
    return moduloSubX ( s1,  s2);
  }
  else if ( axis == Axes::Y ) {
    return moduloSubY ( s1,  s2);
  }
  else return s1 - s2;
}

double PeriodicBinaryTransform::moduloAddX( double x1, double x2 ) const
{
  while (x2 > m_x_limits.length())
	x2 -= m_x_limits.length();

  if ( x2 < -DBL_EPSILON )
    return moduloSubX ( x1,  -x2 );
  
  double overshoot = x1 + x2 - m_x_limits.high();
  
  if ( overshoot > DBL_EPSILON ) {
    return  m_x_limits.low() + overshoot;
  }
  else {
    return x1 + x2;  
  }
}

double PeriodicBinaryTransform::moduloSubX( double x1, double x2 ) const
{
  while (x2 > m_x_limits.length())
	x2 -= m_x_limits.length();

  if ( x2 < -DBL_EPSILON ) return moduloAddY ( x1, -x2 );
  
  double undershoot = m_y_limits.low() - ( x1 - x2 );
    
  if ( undershoot > DBL_EPSILON )  return  m_y_limits.high() - undershoot;
  else return x1 - x2;
}

double PeriodicBinaryTransform::moduloAddY( double y1, double y2 ) const
{
  while (y2 > m_y_limits.length())
	y2 -= m_y_limits.length();

  if ( y2 < -DBL_EPSILON ) return moduloSubY ( y1,  -y2 );
  
  double overshoot = y1 + y2 - m_y_limits.high();
    
  if ( overshoot > DBL_EPSILON ) {
    return  m_y_limits.low() + overshoot;
  }
  else {
    return y1 + y2;
  }
  
}

double PeriodicBinaryTransform::moduloSubY( double y1, double y2 ) const
{
  while (y2 > m_y_limits.length())
	y2 -= m_y_limits.length();

  if ( y2 < -DBL_EPSILON ) return moduloAddY ( y1, -y2 );
  
  double undershoot = m_y_limits.low() - ( y1 - y2 );
    
  if ( undershoot > DBL_EPSILON )  return  m_y_limits.high() - undershoot;
  else return y1 - y2;
}


Rect PeriodicBinaryTransform::calcRectangle ( const Range & lat,
					   const Range & lon )
{
  double x_lo = lat.low ();
  double x_hi = lat.high ();
  
  double y_lo = lon.low ();
  double y_hi = lon.high ();

  double x, y;
  
  double x_min =  1000;
  double x_max = -1000;
  double y_min =  1000;
  double y_max = -1000;

  int n = 50;
  double dx = ( x_hi - x_lo ) / n;
  double dy = ( y_hi - y_lo ) / n;

  
  // Finding out xmin, xmax, ymin, ymax along line y =  y_lo
  for ( int i = 0; i <= n; i++)
    {
      x = x_lo + i * dx; 
      y = y_lo;
      transform ( x, y );
      x_min = ( x_min < x ) ? x_min : x;
      x_max = ( x_max > x ) ? x_max : x;
      y_min = ( y_min < y ) ? y_min : y;
      y_max = ( y_max > y ) ? y_max : y;
    }

  // Finding out xmin, xmax, ymin, ymax along line y =  y_hi
  for ( int i = 0; i <= n; i++)
    {
      x = x_lo + i * dx; 
      y = y_hi;
      transform ( x, y );
      x_min = ( x_min < x ) ? x_min : x;
      x_max = ( x_max > x ) ? x_max : x;
      y_min = ( y_min < y ) ? y_min : y;
      y_max = ( y_max > y ) ? y_max : y;
    }

  // Finding out xmin, xmax, ymin, ymax along line x =  x_lo
  for ( int i = 0; i <= n; i++)
    {
      x = x_lo; 
      y = y_lo + i * dy;
      transform ( x, y );
      x_min = ( x_min < x ) ? x_min : x;
      x_max = ( x_max > x ) ? x_max : x;
      y_min = ( y_min < y ) ? y_min : y;
      y_max = ( y_max > y ) ? y_max : y;
    }

  // Finding out xmin, xmax, ymin, ymax along line x =  x_hi
  for ( int i = 0; i <= n; i++)
    {
      x = x_hi; 
      y = y_lo + i * dy;
      transform ( x, y );
      x_min = ( x_min < x ) ? x_min : x;
      x_max = ( x_max > x ) ? x_max : x;
      y_min = ( y_min < y ) ? y_min : y;
      y_max = ( y_max > y ) ? y_max : y;
    }
    
  return Rect ( x_min, y_min, x_max - x_min, y_max - y_min );
}


void PeriodicBinaryTransform::validate ( Range & lat, Range & lon ) const
{
  if ( lat.low () < m_x_limits.low() ) lat.setLow ( m_x_limits.low() );
  if ( lat.high () > m_x_limits.high() ) lat.setHigh ( m_x_limits.high() );

  if ( lon.low () < m_y_limits.low() ) lon.setLow ( m_y_limits.low() );
  if ( lon.high () > m_y_limits.high() ) lon.setHigh ( m_y_limits.high() );
}

/** @todo Are the implementations of the functions called, duplicates
    of what is in the derived classes of LinearTransform?
 */
const vector < AxisTick > &
PeriodicBinaryTransform::
setTicks ( AxisModelBase & model, hippodraw::Axes::Type axis )
{
  if ( axis == Axes::Z ) {
    return m_z -> setTicks ( model );
  }
  //else
  setTickStep ( model );
  setFirstTick ( model );
  return genTicks ( model, axis );
}

/** Sets the range of given axis to be a new "nice" within the limits given */
void
PeriodicBinaryTransform::
adjustValues ( AxisModelBase &,
	       hippodraw::Axes::Type,
	       const Range & )
{
  // Does not do anything as of now 
  return;
}

inline double FLT_EQUAL( double x, double y )
{
  return ( (double)abs( x - y ) <= 2.0 * ( y * FLT_EPSILON + FLT_MIN ) );
}


void PeriodicBinaryTransform::setTickStep( AxisModelBase & axis )
{
  const Range & range = axis.getRange(false);
  double rangeLength = range.length();

  double scale_factor = axis.getScaleFactor();
  rangeLength *= scale_factor;
  
  // The following algorithm determines the magnitude of the range...
  double rmag = floor( log10( rangeLength ) );
  axis.setRMag( rmag );
  
  double scalelow = range.low() * scale_factor;
  double scalehigh = range.high() * scale_factor;

  // We will also need the largest magnitude for labels.
  double pmag = max( floor( log10( abs ( scalehigh ) ) ), 
		     floor( log10( abs ( scalelow ) ) ) );
  axis.setPMag( pmag );
  
  axis.setTickStep( rangeLength / 4.0 );
}

void PeriodicBinaryTransform::setFirstTick( AxisModelBase & axis )
{
  const Range & range = axis.getRange(false);
  
  axis.setFirstTick ( range.low() );
}


/** @todo Handle too many ticks better, rather than doing nothing.
    See more detailed comment in code.
	@todo Do not use sprintf, it cause warnings from VS 8.
	*/
const vector < AxisTick > &
PeriodicBinaryTransform::
genTicks( AxisModelBase & axis, hippodraw::Axes::Type axistype )
{ 
  double y = 0.0, ylabel;
  
  int num_ticks = 0;
  m_ticks.clear();
  double pmag = axis.getPMag();
  double rmag = axis.getRMag();
  double first_tick = axis.getFirstTick();
  double tick_step  = axis.getTickStep();
  double scale_factor = axis.getScaleFactor();
  
  // pmag will get set to 0 if it is less than or equal to 3.  This
  // is used later to determine scientific notation.  However, m_rmag
  // is still needed as the original magnitude for calculations such
  // as decimal place notation, and rounding to nice numbers.
  
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
  if (decimals < 0) 
    decimals = 0;
   
  sprintf( pstr, "%%1.%df", decimals );

  y = first_tick;
  const Range & range = axis.getRange(false);
  double range_high = range.high();
  range_high *= scale_factor;
  
  while( y <= range_high || FLT_EQUAL( range_high, y ) )
    {
      double value = 0.0;
      
      // Use the rotation about the x axis to change the y-ticks, but in
      // the y units 
      value = moduloAddY( y, rotation(Axes::X)/UNITS_RADIANS * units(Axes::Y) );
      
      // Now we either keep the original magnitude
      // or reduce it in order to express it in scientific notation.
      
      if ( use_pmag ) ylabel = value / pow( 10.0, pmag );
      else ylabel = value;
      
      value /= scale_factor;
      sprintf( labl, pstr, ylabel );
      m_ticks.push_back( AxisTick ( y, labl ) );
      
      num_ticks++;
      if ( tick_step == 0.0 ) break;
      y += tick_step;
      
    }
  
  return m_ticks;
}


bool
PeriodicBinaryTransform::
isLinearInXY () const
{
  return false;
}

void
PeriodicBinaryTransform::
rotateByOffsets (double &lat, double &lon) const
{
  double beta=0;
  double alpha=0;
  double gamma=0;
  double xunits=1, yunits=1;

  double latshift, lonshift;

  alpha = rotation (Axes::X);
  gamma = rotation (Axes::Y);

  if (alpha == 0 && gamma == 0)
	return;

  lonshift = (limitX().high() + limitX().low())/2.0;
  latshift = (limitY().high() + limitY().low())/2.0;

  xunits = units(Axes::X);
  yunits = units(Axes::Y);

  // shift to zero-centered coordinates
  lon -= lonshift;
  lat -= latshift;

  // convert to radians 
  lon = lon * UNITS_RADIANS / xunits;
  lat = lat * UNITS_RADIANS / yunits;

  // Convert the latitude and longitudes into the cartesian coordinates
  // Assume a unit sphere ( Radii does not matter in rotation )
  double x = sin( lon ) * cos( lat );
  double y = cos( lon ) * cos( lat );
  double z = sin( lat );

  // First give a rotation about the x axis ( conventionally denoted by alpha )
  double rx =   x;
  double ry =   y * cos( alpha ) + z * sin( alpha );
  double rz = - y * sin( alpha ) + z * cos( alpha );

  x = rx; y = ry; z = rz;

  // Now give a rotation about the y axis ( conventionally denoted by beta )
  rx =   x * cos( beta ) - z * sin( beta );
  ry =   y;
  rz =   x * sin( beta ) + z * cos( beta );

  x = rx; y = ry; z = rz;

  // Now give a rotation about the z axis ( conventionally denoted by gamma )
  rx =   x * cos( gamma ) + y * sin( gamma );
  ry = - x * sin( gamma ) + y * cos( gamma );
  rz =   z;

  x = rx; y = ry; z = rz;

  // Convert back to latitude and longitudes 
  lat = asin( z ) / UNITS_RADIANS * yunits;
  lon = atan2( x, y ) / UNITS_RADIANS * xunits;

  // Shift back into coordinate window.
  lat += latshift;
  lon += lonshift;
}

