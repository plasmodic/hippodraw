/** @file

AxisModelBase class implementation

Copyright (C) 2000-2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: AxisModelBase.cxx,v 1.132 2007/01/05 03:03:26 xiefang Exp $ 

*/

// for dll interface warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "AxisModelBase.h"
#include "AxisTick.h"

#include <algorithm>

#include <cassert>

using std::string;
using std::vector;

namespace hippodraw {

AxisModelBase::AxisModelBase ( AxisLoc label, AxisLoc scale )
  : m_range( 0.0, 1.0 ),
    m_start_range ( -1.0, 1.0 ),
    m_is_dragging ( false ),
    m_auto_range ( true ),
    m_empty( true ),
    m_scale_factor ( 1.0 ),
    m_scaling_on ( false ),
    m_first_tick( 0.0 ),
    m_max_ticks( 20 ), 
    m_num_minor_ticks( 0 ),
    m_label_location( label ),
    m_scale_location( scale ),
    m_pmag( 0.0 ),
    m_use_pmag( false ),
    m_ticks( m_max_ticks ),
    m_ticks_in_range(m_max_ticks),
    m_auto_ticks ( true )
{
}

AxisModelBase::AxisModelBase( const AxisModelBase & axis_model )
  : m_range( axis_model.m_range ),
    m_start_range ( 0.0, 1.0 ),
    m_is_dragging ( false ),
    m_auto_range( axis_model.m_auto_range ),
    m_empty( axis_model.m_empty ),
    m_scale_factor( axis_model.m_scale_factor ),
    m_scaling_on ( axis_model.m_scaling_on ),
    m_first_tick( axis_model.m_first_tick ),
    m_tick_step( axis_model.m_tick_step ),
    m_max_ticks( 20 ),
    m_num_minor_ticks( axis_model.m_num_minor_ticks ),
    m_label_location( axis_model.m_label_location ),
    m_scale_location( axis_model.m_scale_location ),
    m_pmag( axis_model.m_pmag ), 
    m_use_pmag( axis_model.m_use_pmag ),
    m_ticks( axis_model.m_ticks ),
    m_ticks_in_range( axis_model.m_ticks_in_range),
    m_auto_ticks ( axis_model.m_auto_ticks )
{
}

AxisModelBase::~AxisModelBase()
{
}

void AxisModelBase::setTickStep( const double & t_step ) 
{
  m_tick_step = t_step;
}

double AxisModelBase::getTickStep() const
{
  return m_tick_step;
}

void AxisModelBase::setFirstTick( const double & first_tick ) 
{
  m_first_tick = first_tick;
}

double AxisModelBase::getFirstTick() const
{
  return m_first_tick; 
}

double AxisModelBase::getMaxTicks() const
{
  return m_max_ticks; 
}

void AxisModelBase::setRMag( const double & rmag ) 
{
  m_rmag = rmag;
}

double AxisModelBase::getRMag() const
{
  return m_rmag;
}

void AxisModelBase::setPMag( const double & pmag ) 
{
  m_pmag = pmag;
}

double AxisModelBase::getPMag() const
{
  return m_pmag;
}

bool AxisModelBase::needPMag() const
{
  return m_use_pmag;
}

void AxisModelBase::setUsePMag( const bool & use_p_mag )
{
  m_use_pmag = use_p_mag;
}

AxisLoc AxisModelBase::getLabelLocation() const
{
  return m_label_location;
}

AxisLoc AxisModelBase::getScaleLocation() const
{
  return m_scale_location;
}

void
AxisModelBase::
setAutoTicks ( bool yes )
{
  m_auto_ticks = yes;
  if (!m_auto_ticks) adjustTicks();
}

bool
AxisModelBase::
isAutoTicks ( ) const
{
  return m_auto_ticks;
}

void
AxisModelBase::
setTicks ( const std::vector < AxisTick > & ticks)
{
  m_ticks = ticks;
  if (!m_auto_ticks) adjustTicks();
}

const vector<AxisTick> &
AxisModelBase::getTicks() const
{
  if ( m_auto_ticks ) return m_ticks;
  else return m_ticks_in_range;
  
}

void 
AxisModelBase::
adjustTicks()
{
  /* Need pre-processing to keep ticks in range. */
  m_ticks_in_range.clear();
  
  /* Only ticks within the range are useful. */
  unsigned int size = m_ticks.size();
  double range_high = m_range.high();
  double range_low = m_range.low();
  for ( unsigned int i = 0; i<size; i++ ) {
    if (( m_ticks[i].value()< range_high ) &&
	( m_ticks[i].value()> range_low ))
      m_ticks_in_range.push_back(m_ticks[i]);
    }
}


void AxisModelBase::setRange ( double low, double high, double pos )
{
  if ( low > high ) std::swap ( low, high );
  m_range.setRange ( low, high, pos );

  if (!m_auto_ticks) adjustTicks();
}

void AxisModelBase::setRange( const Range & range, bool scaled )
{
  /*  Comment out this for now to enable very small range ( range < 1e-16 );

  if ( range.length () < DBL_EPSILON ) {
    return;
  }

  */
  Range myrange = range;
  if( scaled ){
    Range newrange( range.low() / m_scale_factor,
		   range.high() / m_scale_factor,
		   range.pos() );
    myrange = newrange;
  }
  
  m_range = myrange;

  // I could just call adjustLogValues() without a check, since linear
  // does nothing in that function.  However, this assures that the
  // graph actually has content, which may or may not be important in
  // adjustLogValues(), so I'm playing it on the safe side.
  // Note that this causes problems in binned axes, because this has
  // already bypassed the binner's dialogue. Therefore, this is only a
  // secondary backup, as adjustLogValues() would have already been
  // called for binned axes through the projector.
  if( isLog() ) adjustLogValues();

  m_empty = false;

  if (!m_auto_ticks) adjustTicks();

}

void AxisModelBase::setIntersectRange ( const Range & r1, const Range & r2 )
{
  m_range = r1;
  m_range.setIntersect ( r2 );
}

void
AxisModelBase::
setRangePos ( double pos )
{
  assert( pos > 0.0 );
  m_range.setPos( pos );
}

void AxisModelBase::setUnionRange ( const Range & range )
{
  if( m_empty ) {
    m_range = range;
    m_range.setEmpty ( false );
    m_empty = false;
  }
  else {
    m_range.setUnion( range );
  }
}

void AxisModelBase::setEmpty()
{
  m_empty = true;
}

const Range & AxisModelBase::getRange(bool scaled) const
{

  if(!scaled)
    return m_range;
  
  double low = m_range.low() * m_scale_factor;
  double high = m_range.high() * m_scale_factor;

  Range range ( low, high );
  m_scaled_range = range;

  return m_scaled_range;
}

void AxisModelBase::setAutoRanging ( bool flag )
{
  m_auto_range = flag;
}

bool AxisModelBase::isAutoRanging () const
{
  return m_auto_range;
}

void AxisModelBase::setScaleFactor ( double sf )
{
  m_scale_factor = sf;
  m_scaling_on = true;
}

double AxisModelBase::getScaleFactor () const
{
  if ( m_scaling_on == false ) return 1.0;
  // else
  return m_scale_factor;
}

bool AxisModelBase:: isScaling ( ) const
{
  return m_scaling_on;
}

void AxisModelBase:: setScaling ( bool on )
{
  m_scaling_on = on;
  if ( on == false ) m_scale_factor = 1.0;
}

void AxisModelBase::startDragging ( bool dragging )
{
  if ( m_is_dragging == false  ) {
    m_start_range = m_range;
  }
  m_is_dragging = dragging;
}

} // namespace hippodraw
