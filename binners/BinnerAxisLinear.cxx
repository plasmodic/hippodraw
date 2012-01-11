/** @file

hippodraw::BinnerAxisLinear class implementation
*
Copyright (C) 2000-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinnerAxisLinear.cxx,v 1.73 2007/07/02 18:17:15 pfkeb Exp $

*/
// for math defect
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "BinnerAxisLinear.h"

#include <algorithm>

#include <cmath> // for ciel()

#include <cassert>


using namespace hippodraw;

BinnerAxisLinear::BinnerAxisLinear ()
  : BinnerAxis ( "BinnerLinear" )
{
  m_width = m_range.length() / m_num_bins;
}

/** @todo Why is width recalculated instead of being copied. */
BinnerAxisLinear::BinnerAxisLinear ( const BinnerAxisLinear & binner )
  : BinnerAxis ( binner )
{
}

BinnerAxisLinear::~BinnerAxisLinear()
{
}

BinnerAxis *
BinnerAxisLinear::clone()
{
  return new BinnerAxisLinear( *this );
}

bool
BinnerAxisLinear::hasEqualWidths () const
{
  return true;
}

void
BinnerAxisLinear::axisSetNumberOfBins ( int nb )
{
  m_num_bins = nb;
  m_width = m_range.length() / m_num_bins;
}

/** @note Dividing by the bin width leads to less roundoff errors then
    using the fraction of the range and multiplying by the number of
    bins.
 */
int
BinnerAxisLinear::axisBinNumber ( double x ) const
{
  double lo = m_range.low ();
  double width = axisBinWidth ( 1 );
  double f = 1.0 + ( x - lo ) / width;
  int i = static_cast < int > ( f );
  i = std::max ( i, 0 );
  i = std::min ( i, m_num_bins + 1 );

  return i;
}

double
BinnerAxisLinear::
getCoordinate ( int i ) const
{
  return m_range.low() + i * ( m_range.length () / m_num_bins );
}

double
BinnerAxisLinear::axisBinWidth ( int ) const
{
  double width = m_range.length() / m_num_bins;

  return width;
}

const Range &
BinnerAxisLinear::
setBinWidth ( double width )
{
  assert ( width > 0.0 );
  
  m_width = width;

  m_num_bins = getNob ( width );
  m_range.setLength ( m_num_bins * width );
  return m_range;
}

double
BinnerAxisLinear::
calcWidthParm ( int number ) const
{
  double new_width = ( m_range.high() - m_range.low () ) /
    ( static_cast < double > ( number ) );

  return new_width;
}

double
BinnerAxisLinear::calcOffset ( int parm, bool dragging ) const
{
  setStartRange ( dragging );

  return ( parm - 50 ) / 50.0;
}

double
BinnerAxisLinear::getOffset () const
{
  return m_offset;
}

const void
BinnerAxisLinear::setOffset ( double offset )
{
  double oldoff = m_offset;
  m_offset = offset;
  double change = offset - oldoff;

  if( offset == 0.0 ) return; // resetting...
  Range r( m_range.low() + change * m_width,
	   m_range.high() + change * m_width );

  m_range = r;
}

/** @todo Had to put std:: in here for Sun's CC, but not in
    AxisModelxxx.  So there must be a better fix.  
    
    @bug the cpp macro MATH_DEFECT is used in this method but
    configure doesn't define it.`
 */
const Range &
BinnerAxisLinear::
setRange ( const Range & range, bool hold_width )
{
  m_range = range;

  if ( hold_width && m_width < m_range.length() ) {
#ifdef MATH_DEFECT
    double number = std::ceil ( m_range.length() / m_width );
#else
    double number = ceil ( m_range.length() / m_width );
#endif
    m_num_bins = static_cast< int > ( number  );
    m_range.setLength ( m_num_bins * m_width );
  }
  else { // calc new width.
    m_width = m_range.length() / m_num_bins;
  }
  return m_range;
}

double 
BinnerAxisLinear::getConstWid ( ) const
{
  return axisBinWidth( 1 );
  //This should really return the const width variable... but this
  //raises issues with when the variable needs to be recalculated.
  //return m_const_wid;
}

double 
BinnerAxisLinear::
getBinWidth ( ) const
{
  return getConstWid ();
}

void
BinnerAxisLinear::setConstWid ( )
{
  m_width = axisBinWidth( 1 );
}

double
BinnerAxisLinear::scaleFactorWid ( )
{
  return getConstWid();
}

int BinnerAxisLinear::getNob ( const Range & range ) const 
{
  double width = getConstWid ();
#ifdef MATH_DEFECT
  int tmp 
    = static_cast < int > ( std::floor ( range.length () / width + 0.5 ) );
#else
  int tmp 
    = static_cast < int > ( floor ( range.length() / width + 0.5 ) );
#endif

  return tmp ? tmp : 1;
}

int BinnerAxisLinear::getNob ( double wid ) const
{
  int tmp = m_range.numberOfBins( wid );

  return tmp ? tmp : 1;
}
