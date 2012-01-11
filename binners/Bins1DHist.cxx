/** @file

hippodraw::Bins1DHist class implementation

Copyright (C) 2000-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Bins1DHist.cxx,v 1.122 2007/04/23 21:57:20 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif


#include "Bins1DHist.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/NTuple.h"

#include <algorithm>
#include <numeric>

#include <cmath>

#include <cassert>

using namespace hippodraw;

using std::fill;
using std::list;
using std::max_element;
using std::min_element;
using std::string;
using std::vector;

Bins1DHist::Bins1DHist ( )
  : Bins1DBase ( "Bins1DHist" )
{
  m_min_entries=0;
}

Bins1DHist::Bins1DHist ( const Bins1DHist & binner )
  : Bins1DBase ( binner ),
    m_data( binner.m_data ),
    m_variance( binner.m_variance ),
    m_min_entries ( binner.m_min_entries )
{
  for ( int i = 0; i < 3; i++ ) m_moments[i] = binner.m_moments[i];
}

BinsBase *
Bins1DHist::clone () const
{
  return new Bins1DHist ( *this );
}

Bins1DHist::~Bins1DHist ()
{
}

void Bins1DHist::resize ( int number )
{
  m_data.resize ( number + 2 );
  m_variance.resize ( number );
  reset();

  m_values_dirty = true;
}

double Bins1DHist::minBin()
{
  return *min_element ( m_data.begin() + 1, m_data.end ( ) - 1 );
}

double Bins1DHist::maxBin()
{
  return *max_element ( m_data.begin () + 1, m_data.end () - 1 );
}

void Bins1DHist::reset()
{
  fill( m_data.begin(), m_data.end(), 0.0 );
  fill( m_variance.begin(), m_variance.end(), 0.0 );
  fill( m_moments, m_moments + 3, 0.0 );

  m_values_dirty = true;
  m_empty = true;
}

int Bins1DHist::getNumberOfEntries () const
{
  return static_cast < int > ( std::accumulate ( m_data.begin()+1, 
						 m_data.end()-1, 0.0 ) );
}

int Bins1DHist::getNumberOfEntries ( int i ) const
{
  int retval = static_cast < int > ( m_data[i+1] );

  return retval;
}

int Bins1DHist::getUnderflow () const
{
  return static_cast < int > ( m_data[0] );
}

int Bins1DHist::getOverflow () const
{
  return static_cast < int > ( *(m_data.end()-1) );
}

void Bins1DHist::accumulate ( double x, double w, double, double )
{
  int i = binNumber ( x );
  if ( i > 0 && i <= static_cast < int > (m_variance.size() ) ) {
    m_variance[i-1] += w * w;
    m_moments[0] += w;
    m_moments[1] += w * x;
    m_moments[2] += w * x * x;
  }
  m_data[i] += w;

  m_empty = false;
}

NTuple *
Bins1DHist::
createNTuple ( ) const
{ 
  unsigned int size = m_data.size ();
  NTuple * ntuple = prepareNTuple ( size );

  fillDataSource ( ntuple );

  return ntuple;
}

namespace dp = hippodraw::DataPoint2DTuple;

void
Bins1DHist::
fillDataSource ( DataSource * ntuple ) const
{
  ntuple -> clear();
  vector < double > row ( dp::SIZE );

  double total = std::accumulate ( m_data.begin() + 1,
				   m_data.end ()  - 1, 0.0 );
  double factor = m_is_scaling ? m_scale_factor / total : 1.0;

  std::vector<double>::const_iterator dit = m_data.begin() + 1;
  std::vector<double>::const_iterator vit = m_variance.begin();
  std::vector<double>::const_iterator first_non_zero = m_data.begin() + 1;
  std::vector<double>::const_iterator last_non_zero = m_data.end() - 2;

  if ( total > 0 ) {
      // Get the first and last non_zero elements.
      while ( (*first_non_zero) == 0 ) {
          first_non_zero++;
      }
      while ( (*last_non_zero) == 0 ) {
          last_non_zero--;
      }
  }
  else {
      swap ( first_non_zero, last_non_zero );
  }

  double x = getLow ( Axes::X );

  int i = 0;

  if ( last_non_zero < first_non_zero ) { // all non zero
    for (;dit !=m_data.end()-1; dit ++ ) {
      double width = binWidth ( i++ );
      double half_width = 0.5 * width;
      double y = factor * ( *dit / width );
      double yerr = factor * ( sqrt( *vit++ ) / width );
      x += half_width;
      row[dp::X] = x;
      row[dp::Y] = y;
      row[dp::XERR] = half_width;
      row[dp::YERR] = yerr;
      
      ntuple -> addRow ( row );
    
      x += half_width;
    }
    return;
  }

  // Process the leading zeros.
  for ( ; dit != first_non_zero; dit++ ) {
    double width = binWidth ( i++ );
    double half_width = 0.5 * width;
    double y = factor * ( *dit / width );
    double yerr = factor * ( sqrt( *vit++ ) / width );
    x += half_width;
    row[dp::X] = x;
    row[dp::Y] = y;
    row[dp::XERR] = half_width;
    row[dp::YERR] = yerr;

    ntuple -> addRow ( row );
    
    x += half_width;
  }

  
  // Redo the binning.
  for( ; dit != last_non_zero+1; dit++ ) { 
    double width = binWidth( i++ );
    double entries = *dit;

    double var = sqrt(*vit);
    while ( entries < m_min_entries ) {
      if (dit == last_non_zero) break; // need to change last row
      dit++; vit++;
      width += binWidth( i++ );
      entries += *dit;
      var += sqrt(*vit);
    }

    double half_width = 0.5 * width;
    x += half_width;
    double y = factor * ( entries / width );
    double yerr = factor * ( var / width );
    vit++;

    // Update last bin if the remain bins has too few entries
    if (( entries < m_min_entries ) && ( dit==last_non_zero )) {
      unsigned int numOfRows = ntuple->rows();
      unsigned int lastIndex = numOfRows-1;
      vector <double> lastRow = ntuple -> getRow( lastIndex );

      x = lastRow[dp::X] + half_width;
      y = ( lastRow[dp::Y] * lastRow[dp::XERR] * 2 + entries ) 
	/ ( width + lastRow[dp::XERR] *2 );
      yerr = ( lastRow[dp::YERR] * lastRow[dp::XERR] * 2 + var )
	/ ( width + lastRow[dp::XERR] * 2 ) ;
      half_width += lastRow[dp::XERR];
      
      ntuple -> eraseRow ( lastIndex );
    }

    row[dp::X] = x;
    row[dp::Y] = y;
    row[dp::XERR] = half_width;
    row[dp::YERR] = yerr;

    ntuple -> addRow ( row );
    
    x += half_width;
  }

  // Processing the ending zeros.
  for ( ; dit != m_data.end()-1; dit++ ) {
    double width = binWidth ( i++ );
    double half_width = 0.5 * width;
    double y = factor * ( *dit / width );
    double yerr = factor * ( sqrt( *vit++ ) / width );
    x += half_width;
    row[dp::X] = x;
    row[dp::Y] = y;
    row[dp::XERR] = half_width;
    row[dp::YERR] = yerr;

    ntuple -> addRow ( row );
    
    x += half_width;
  }
  
}

/** @todo The underflow and overflow bins do not get archived, thus
    don't get restored. Also the moments.
*/
void
Bins1DHist::
setBinContents ( const DataSource * ntuple )
{
  unsigned int size = ntuple -> rows ();
  assert ( size == m_variance.size () );

  for ( unsigned int i = 0; i < size; i++ ) {
    const vector < double > & row = ntuple -> getRow ( i );
    m_data[i+1] = row[dp::Y];
    double yerr = row[dp::YERR];
    double width = row[dp::XERR];
    m_variance[i] = yerr * yerr * width * width;
  }
}

void
Bins1DHist::
setMinEntries ( int entries )
{
  m_min_entries = entries;
}

int
Bins1DHist::
getMinEntries ()
{
  return m_min_entries;
}
