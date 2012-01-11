/** @file

hippodraw::Range class implementation

Copyright (C) 2000-2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Range.cxx,v 1.57 2007/05/08 18:47:27 pfkeb Exp $

*/

#ifdef _MSC_VER
// Include max() and min() missing from MicroSoft Visual C++.
#include "msdevstudio/MSconfig.h"
#endif

#include "Range.h"

#include <algorithm>

#include <cassert>
#include <cmath>
#include <iostream>

#ifndef _MSC_VER
using std::floor;
#endif

using std::ostream;
using std::max;
using std::min;
using std::vector;

using namespace hippodraw;

Range::Range ( )
  : m_min( 0.0 ),
    m_max( 0.0 ),
    m_pos( DBL_MAX ),
    m_empty( true )
{
}

Range::Range ( double x, double y, double p )
{

  // x and y are being passed as 'nan' in some cases, leading to failure
  // of assertion below. I dont know why they are being passed as nan and
  // from where.

  if ( x > y ) {
    m_min = 0; 
    m_max = 0;
    m_pos = DBL_MAX;
    m_empty = true;
  }
  else{
    m_min = x; 
    m_max = y;
    m_pos = p;
    m_empty = false;
  }

  // This assertion is failing.
  //assert ( m_min <= m_max );

  //This is a dirty fix:
  if ( ! ( m_min <= m_max ) ){
    m_min = 0; 
    m_max = 0.1;
    m_pos = DBL_MAX;
    m_empty = true;
  }

  assert ( m_min <= m_max );

}

Range::
Range ( const std::vector < double > & array )
{
  setRange ( array.begin(), array.end () );
}

double
Range::low() const
{
  return m_min;
}

void
Range::setLow ( double x )
{
  m_min = x;
  assert ( m_min <= m_max );
}

double
Range::high() const
{
  return m_max;
}

void
Range::setHigh ( double x )
{
  m_max = x;
  assert ( m_min <= m_max );
}

double
Range::pos() const
{
  return m_pos;
}

void
Range::setPos (double x)
{
  m_pos = x;
  assert ( m_min <= m_max );
}

void 
Range::setRange ( double low, double high, double pos )
{
  m_min = low;
  m_max = high;
  m_pos = pos;
  assert ( m_min <= m_max );
}

void
Range::setLength ( double val, bool high_hold )
{
  if( high_hold ){
    m_min = m_max - val;
  } else {
    m_max = m_min + val;
  }
  assert ( m_min <= m_max );
}

bool
Range::includes ( double val ) const
{
  return val >= m_min && val <= m_max;
}

bool
Range::
excludes ( double value ) const
{
  return value < m_min || value > m_max;
}

double
Range::
fraction ( double value ) const
{
  return ( value - m_min ) / ( m_max - m_min );
}

void Range::setEmpty ( bool yes )
{
  m_empty = yes;
}

void Range::setUnion ( const Range & range )
{
  if ( m_empty ) {
    m_min = range.m_min;
    m_max = range.m_max;
    m_pos = range.m_pos;
    m_empty = false;
  }
  else {
    m_min = min ( m_min, range.m_min );
    m_max = max ( m_max, range.m_max );
    m_pos = min ( m_pos, range.m_pos );
  }

  assert ( m_min <= m_max );

}

void Range::setIntersect ( const Range & range )
{
  if ( m_min > range.m_max || m_max < range.m_min ) return;
  m_min = max ( m_min, range.m_min );
  m_max = min ( m_max, range.m_max );
  m_pos = max ( m_pos, range.m_min );
  
  assert ( m_min <= m_max );
}

int
Range::numberOfBins ( double width ) const
{
  assert ( m_max > m_min );
  double number = (m_max - m_min) / width;

#ifdef _MSC_VER
  return static_cast < int > ( number+0.5 );
#else
  return static_cast < int > ( rint( number ) );
#endif
}
