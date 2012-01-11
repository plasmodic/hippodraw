
/** @file

hippodraw::EqualEntriesHist1DProjector class implementation

Copyright (C) 2000-2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: EqualEntriesHist1DProjector.cxx,v 1.4 2007/04/23 21:57:21 pfkeb Exp $

*/

#ifdef _MSC_VER
// Include max() and min() missing from MicroSoft Visual C++.
#include "msdevstudio/MSconfig.h"
#endif

#include "EqualEntriesHist1DProjector.h"

#include "axes/AxisModelBase.h"
#include "datasrcs/DataPointTuple.h"
#include "datasrcs/NTuple.h"
#include "datasrcs/NTupleSorter.h"

#include <algorithm>
#include <numeric>

#include <cfloat>

#include <cassert>
#include <limits.h>

using namespace hippodraw;

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::accumulate;
using std::find;
using std::max;
using std::min;
using std::list;
using std::string;
using std::vector;
#endif

EqualEntriesHist1DProjector::EqualEntriesHist1DProjector()
  : NTupleProjector ( 2 ),
    m_y_label ( "Density" ),
    m_bin_num (100),
    m_start_num (100)
{
  m_binding_options.push_back ( "X" );
  m_binding_options.push_back ( "Weight (optional)" );
  m_min_bindings = 1;
  addPointReps();
}

/** @note Must initialize with the copy constructor of ProjectorBase
    here, otherise the default constructor will be used.  This is
    because it is a virtual base class of the inherited classes.
 */
EqualEntriesHist1DProjector::
EqualEntriesHist1DProjector ( const EqualEntriesHist1DProjector & projector )
  : NTupleProjector ( projector ),
    m_y_label ( projector.m_y_label ),
    m_bin_num ( projector.m_bin_num ),
    m_start_num ( projector.m_start_num )
{
  addPointReps();
}

ProjectorBase * EqualEntriesHist1DProjector::clone()
{
  return new EqualEntriesHist1DProjector( *this );
}

void EqualEntriesHist1DProjector::changedNTuple()
{
  unsigned int cols = m_ntuple->columns () - 1;
  if ( m_columns[0] > cols ) {
    m_columns[0] = cols;
  }
  if ( m_columns[1] > cols ) m_columns[1] = UINT_MAX;
}


double
EqualEntriesHist1DProjector::
getPosOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y );

  if ( axis == Axes::X ) {
    return getPos ( m_columns[0] );
  }
  // Y

  return 0;
  //return getPosOnValue ();
}


Range
EqualEntriesHist1DProjector::
dataRangeOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y );

  if ( axis == Axes::X ) {
    return dataRange ( m_columns[0] );
  }
  // Y
  return dataRangeOnValue ();
}


const string & EqualEntriesHist1DProjector::getYLabel ( bool density ) const
{
  return m_y_label;
}


namespace dp = hippodraw::DataPoint2DTuple;


// ToDo:: need implementation to get all information.
double
EqualEntriesHist1DProjector::
getAverage ( hippodraw::Axes::Type axis ) const
{
  return 0;
}


int
EqualEntriesHist1DProjector::
getUnderflow () const
{
  return 0;
}

int
EqualEntriesHist1DProjector::
getOverflow () const
{
  return 0;
}


DataSource *
EqualEntriesHist1DProjector::
createNTuple () const
{
  unsigned int columns = 4;
  NTuple * ntuple = new NTuple ( columns );

  vector < string > labels;
  labels.push_back ( "X" );
  labels.push_back ( "Value" );
  labels.push_back ( dp::WIDTH );
  labels.push_back ( dp::ERROR );

  ntuple->setLabels ( labels );

  fillProjectedValues ( ntuple );

  return ntuple;
}

void
EqualEntriesHist1DProjector::
fillProjectedValues ( DataSource * ntuple, bool ) const
{
  ntuple -> clear();

  vector < double > row ( dp::SIZE );

  unsigned int x_col = m_columns[0];
  unsigned int size = m_ntuple -> rows ();

  // Sort a column of data source.
  vector < double > col = m_ntuple -> getColumn ( x_col );
  std::sort(col.begin(), col.end());

  // Index of the vector
  unsigned int k=0;
  for ( unsigned int i = 0; i < m_bin_num; i++ ) {
    
    // For comm_bin_numing bins with too small width
    unsigned int j=1;
    row [dp::XERR] = col[k+size/m_bin_num]- col[k];

    // Last bin can't exceed data range
    if (i==m_bin_num-1) row[dp::XERR] = col[size-1]-col[k];

    // Bin width is 0, combine with the following ones.
    while ((row[dp::XERR]==0) && (i!=m_bin_num-1)) {
      j++;
      i++;
      row[dp::XERR]=col[k+j*size/m_bin_num]-col[k];
      if ( i==m_bin_num-1 ) row[dp::XERR] = col[size-1]-col[k];
    }
    
    // Bin width is 0 and it's last bin, ignore the last bin
    if ( row[dp::XERR]==0 ) return;
    
    // Cooridinate of the middle of the bin.
    row [dp::X] = col[k] + 0.5*row[dp::XERR];

    // Density is number of entries in a bin divided by bin width. 
    row [dp::Y] = size*j/m_bin_num/row[dp::XERR]; 

    // Use half width
    row [dp::XERR] /= 2.0;
    row [dp::YERR] = 0;

    ntuple -> addRow (row);
    
    // next bin
    k+=j*size/m_bin_num;
  }
  
}

void
EqualEntriesHist1DProjector::
prepareValues ()
{
  adjustNumberOfBins();
  m_range = dataRange ( m_columns[0] );

  if ( m_proj_values == 0 ) {
    m_proj_values = createNTuple ();
  } else {
    fillProjectedValues ( m_proj_values );
  }

  setDirty ( false );
}


void
EqualEntriesHist1DProjector::
addPointReps()
{
  m_pointreps.push_back ( "Column" );
  m_pointreps.push_back ( "FilledColumn" );
  m_pointreps.push_back ( "Symbol" );
  m_pointreps.push_back ( "Line" );
}

bool EqualEntriesHist1DProjector::
isAxisBinned ( const std::string & axis ) const
{
  if ( axis == m_binding_options[0] ) {
    return true;
  }
  return false;
}

Range
EqualEntriesHist1DProjector::
valueRange () const
{
   return dataRangeOn ( Axes::Y );
}

Range
EqualEntriesHist1DProjector::
dataRangeOnValue ( ) const
{
  EqualEntriesHist1DProjector * p = const_cast < EqualEntriesHist1DProjector * > ( this );
  p->prepareValues ();
  if ( m_proj_values -> empty () ) {
    return Range ( 0.0, 1.0, 0.5 );
  }
  
  const vector < double > & values = m_proj_values -> getColumn( dp::Y );
  return  Range ( values );
}

const Range &
EqualEntriesHist1DProjector::
setBinWidth( Axes::Type axis, int parm, bool dragging )
{
  m_bin_num=m_start_num+parm-50;
  adjustNumberOfBins();
  setDirty(true);
  
  if ( !dragging ) m_start_num = m_bin_num;
  //m_range = dataRange ( m_columns[0] );
  return m_range;
}

const Range &
EqualEntriesHist1DProjector::
setBinWidth( Axes::Type axis, double number )
{
  m_bin_num=(unsigned int) number;
  adjustNumberOfBins();
  setDirty(true);
  //m_range = dataRange ( m_columns[0] );
  return m_range;
}

double
EqualEntriesHist1DProjector::
getBinWidth ( Axes::Type axis ) const
{
  return (double)m_bin_num;
}

void
EqualEntriesHist1DProjector::
adjustNumberOfBins()
{
  unsigned int size = m_ntuple -> rows ();
  if ( size-1 < m_bin_num ) m_start_num = m_bin_num  = size-1;
  if (m_bin_num < 1) m_bin_num = 1;
}
