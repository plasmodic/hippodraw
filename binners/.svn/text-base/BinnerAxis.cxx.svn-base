/** @file

hippodraw::BinnerAxis class implementation

Copyright (C) 2000-2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinnerAxis.cxx,v 1.41 2007/04/02 22:42:11 pfkeb Exp $

*/

//inconsistent dll
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "BinnerAxis.h"

using std::vector;
using std::string;

using namespace hippodraw;

int BinnerAxis::s_num_bins = 50;
double BinnerAxis::s_bin_factor = 0.75;

BinnerAxis::BinnerAxis ( const char * name )
  : m_name ( name ), 
    m_dragging ( false ),
    m_num_bins ( s_num_bins ), 
    m_range( 0.0, 50.0 ),
    m_offset( 0.0 ),
    m_width ( -1.0 )
{
}

BinnerAxis::BinnerAxis ( const BinnerAxis & binner )
  : m_name ( binner.m_name ),
    m_dragging ( false ),
    m_num_bins( binner.m_num_bins ),
    m_range ( binner.m_range ),
    m_offset ( binner.m_offset ),
    m_width ( binner.m_width )
{
}

BinnerAxis::~BinnerAxis ()
{
}

const string &
BinnerAxis::
name () const
{
  return m_name;
}

bool
BinnerAxis::hasEqualWidths () const
{
  return false;
}

double
BinnerAxis::axisGetLow() const
{
  return m_range.low();
}

double
BinnerAxis::axisGetHigh() const
{
  return m_range.high();
}

const Range &
BinnerAxis::
getRange() const
{
  return m_range;
}

int
BinnerAxis::axisNumberOfBins () const
{
  return m_num_bins;
}

void
BinnerAxis::setStartRange ( bool dragging ) const
{
  if ( m_dragging == false ) {
    m_range_start = m_range;
  }

  m_dragging = dragging;
}

void
BinnerAxis::setStartWidth ( bool dragging ) const
{
  if ( m_dragging == false ) {
    m_width_start = m_width;
  }

  m_dragging = dragging;
}


const vector< double > & BinnerAxis::binEdges ()
{
  
  if( m_bin_edges.size() == 0 )
    {
      m_bin_edges.resize( m_num_bins + 1);
      
      m_bin_edges[0] = m_range.low();

      for( int i = 0; i < m_num_bins; i ++ )
	m_bin_edges[i] = m_bin_edges[i-1] + axisBinWidth( i );

      m_bin_edges[ m_num_bins + 1 ] = m_range.high();
    }
  
  return m_bin_edges;
}

double
BinnerAxis::
calcBinWidth ( int parm, bool dragging ) const
{
  setStartWidth ( dragging );

  double multiplier = ( 50 - parm ) / 50.0;
  int num_start = getNob ( m_width_start );
  if ( num_start == 1 ) {
    multiplier *= 4.0;
  }
  double num_new = num_start + num_start * multiplier;

  num_new = std::max ( 1.0, num_new );
  m_num_bins = static_cast < int > ( num_new );
  double new_width = calcWidthParm ( m_num_bins );

  return new_width;
}

