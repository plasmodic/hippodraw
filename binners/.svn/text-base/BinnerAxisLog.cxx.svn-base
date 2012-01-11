/** @file

hippodraw::BinnerAxisLog  class implementation

Copyright (C) 2000-2004, 2006, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinnerAxisLog.cxx,v 1.78 2007/04/23 21:57:20 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "binners/BinnerAxisLog.h"

#include <algorithm>
#include <stdexcept>

#include <cmath>
#include <cassert>


#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::max;
using std::pow;
using std::upper_bound;
using std::vector;
#endif

using namespace hippodraw;

BinnerAxisLog::
BinnerAxisLog ()
  : BinnerAxis ( "BinnerLog" )
{
}

BinnerAxisLog::BinnerAxisLog ( const BinnerAxisLog & binner )
  : BinnerAxis ( binner ),
    bins ( binner.bins )
{
}

BinnerAxisLog::~BinnerAxisLog ()
{
}

BinnerAxis *
BinnerAxisLog::clone ()
{
  return new BinnerAxisLog ( *this );
}

void
BinnerAxisLog::axisSetNumberOfBins( int nb )
{
  m_num_bins = nb;
}

int
BinnerAxisLog::axisBinNumber ( double x ) const
{
  vector<double>::const_iterator it
    = upper_bound ( bins.begin(), bins.end(), x );
  int i = it - bins.begin();
  if( i < 1 ) i = 0;
  if ( i > m_num_bins ) i = m_num_bins + 1;

  return i;
}

double
BinnerAxisLog::
getCoordinate ( int i ) const
{
  assert ( i < m_num_bins && !( i < 0 ) );

  double low = bins[i];
  double high = bins[i+1];

  return sqrt(low*high);
}

double
BinnerAxisLog::axisBinWidth ( int bins_number) const
{
  bins_number = std::min ( bins_number, m_num_bins -1 );
  bins_number = std::max ( bins_number, 0 );

  double width = bins[bins_number+1] - bins[bins_number];
  assert ( width >= 0.0 );

  return width;
}

/* virtual */
double
BinnerAxisLog::getConstWid ( ) const
{
  return m_width;
}  

double
BinnerAxisLog::
getBinWidth ( ) const
{
  return m_width;
}  

void
BinnerAxisLog::setConstWid ( )
{
  m_width = calcWidthParm ( m_num_bins );
}

double
BinnerAxisLog::calcWidthParm ( int num_bins ) const
{
  double width_parm = 0;
  double low = m_range.low();
  double high = m_range.high();
  width_parm = log10 ( high / low ) / num_bins;

  return width_parm;
}

void
BinnerAxisLog::setBins ()
{
  double low = m_range.low ();
  assert ( low > 0.0 );

  vector<double>::iterator it = bins.begin();
  *it++ = low;

  for ( int i = 1; it != bins.end(); ++it, i++ ) {
    *it = low * pow ( 10.0, i * m_width );
  }
}

const Range &
BinnerAxisLog::
setRange ( const Range & range, bool hold_width )
{
  m_range = range;

  if ( hold_width ) {
    m_num_bins = getNob ( range );
  }
  else {
    m_width = calcWidthParm ( m_num_bins );
  }

  double low = m_range.low ();
  double  high = 1.0;
  if (low <= 0) {
     low = m_range.high()/pow(10., m_num_bins*m_width);
     m_range.setLow(low);
  } else {
     high = low * pow ( 10.0, m_num_bins * m_width );
     m_range.setHigh(high);
  }

  if (high <= 0) { //sloppy, using high as a flag
     throw std::runtime_error("BinnerAxisLog::Range: attempt to set range "
                              "to negative values.");
  }

  bins.resize( m_num_bins + 1, 0.0 );
  setBins();

  return m_range;
}

const Range &
BinnerAxisLog::setBinWidth ( double wid )
{
  assert ( wid > 0.0 );

  m_width = wid;
  m_num_bins = getNob ( m_width );

  double low = m_range.low ();
  double high = low * pow ( 10.0, m_num_bins * wid );
  m_range.setHigh ( high );
  bins.resize( m_num_bins + 1, 0.0 );
  setBins();

  return m_range;
}

double
BinnerAxisLog::calcOffset ( int parm, bool dragging ) const
{
  setStartRange ( dragging );

  return ( parm - 50 ) / 50.0;
}

/** @todo Maybe be able to remove this function and other getOffset
    functions. */
double
BinnerAxisLog::getOffset () const
{
  return m_offset;
}

const void
BinnerAxisLog::setOffset ( double offset )
{
  double oldoff = m_offset;
  m_offset = offset;
  double change = m_offset - oldoff;

  if( offset == 0.0 ) return; // resetting...
  double low = m_range_start.low ();
  double width = low * pow ( 10.0,  m_width ); // first bin width
  double woffset = width * change;
  double new_low = low * pow ( 10.0, woffset * m_width );
  double new_high = new_low * pow ( 10.0, ( bins.size() -1 ) * m_width );

  Range r( new_low, new_high );
  setRange( r );
}

double
BinnerAxisLog::scaleFactorWid ( )
{
  return 1.0;
}

int BinnerAxisLog::getNob ( const Range & range ) const
{
  double low = range.low ();
  double high = range.high ();
  int tmp = static_cast< int > ( ceil ( log10 ( high/low ) /  
					m_width  ) );

  return tmp ? tmp : 1;
}

int BinnerAxisLog::getNob ( double wid ) const
{
  double low = m_range.low ();
  double high = m_range.high ();
  int tmp = static_cast < int > ( ceil ( log10 ( high/low ) / 
					  wid  ) );
  return tmp ? tmp : 1;
}
