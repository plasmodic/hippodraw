/** @file

hippodraw::Map1Projector class implementation

Copyright (C) 2001-2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: Map1Projector.cxx,v 1.92 2007/04/27 18:14:52 pfkeb Exp $

*/

#include <limits.h>

#ifdef _MSC_VER
// Include max() and min() missing from MicroSoft Visual C++.
#include "msdevstudio/MSconfig.h"
#endif

#include "Map1Projector.h"

#include "axes/AxisModelBase.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/NTuple.h"

#include <algorithm>
#include <numeric>

#include <cfloat>

#include <cassert>

using namespace hippodraw;

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::accumulate;
using std::find;
using std::max;
using std::min;
using std::string;
using std::vector;
#endif

Map1Projector::Map1Projector ( )
  : NTupleProjector ( 2 ),
    m_x_label ( "Item" ),
    m_y_option ( "Y error (optional)" )
{
  m_binding_options.push_back ( "Y" );
  m_min_bindings = 1;
  addPointReps();
}

/** @note Must initialize with the copy constructor of ProjectorBase
    here, otherise the default constructor will be used.  This is
    because it is a virtual base class of the inherited classes.
 */
Map1Projector::
Map1Projector ( const Map1Projector & projector )
  : ProjectorBase ( projector ),
    NTupleProjector( projector ),
    m_x_label ( projector.m_x_label )
{  
  addPointReps();
}

ProjectorBase * Map1Projector::clone()
{
  return new Map1Projector( *this );
}

bool
Map1Projector::
inRange ( int row ) const
{
  bool yes = true;
  double x = row;
  const Range & x_range = m_x_axis->getRange ( false );
  if ( x_range.excludes ( x ) ) {
    yes = false;
  }
  else {
    const Range & y_range = m_y_axis->getRange ( false );
    if ( y_range.excludes ( m_ntuple -> 
			    valueAt ( row, m_columns[0] ) ) ) yes = false;
  }

  return yes;
}


void Map1Projector::setYErrorOption ( bool enable )
{
  const string name ( "Y error" );
  vector< string >:: iterator first 
    = find ( m_binding_options.begin (),
	     m_binding_options.end (),
	     name );
  if ( first != m_binding_options.end () && !enable ) {
    m_binding_options.erase ( first );
    m_columns[1] = UINT_MAX;
  }
  else if ( enable ) {
    m_binding_options.push_back ( name );
  }
}

void Map1Projector::changedNTuple()
{
  unsigned int cols = m_ntuple->columns () - 1;
  if ( m_columns[0] > cols ) m_columns[0] = cols;
  if ( m_columns[1] > cols ) m_columns[1] = UINT_MAX;
}

Range Map1Projector::valueRange () const
{
  return dataRangeOn ( Axes::Y );
}

Range
Map1Projector::
dataRangeOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y );

  if ( axis == Axes::X ) {
    double pos = getPosOn ( Axes::X );
    return Range ( 0.0, m_ntuple->rows (), pos );
  }
  // It has to be Y.
  if ( m_columns[1] == UINT_MAX ) {
    return dataRange ( m_columns[0] );
  }
  //It has to be Y with an error.
  return dataRangeWithError ( m_columns[0], m_columns[1] );
}

double
Map1Projector::
getPosOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y );

  if ( axis == Axes::X ) {
    return m_ntuple -> empty () ? DBL_MAX : 1.0;
  }
  // It has to be Y.
  if ( m_columns[1] == UINT_MAX ) {
    return getPos ( m_columns[0] );
  }
  //It has to be Y with an error.
  return getPosWithError ( m_columns[0], m_columns[1] );
}

const string & Map1Projector::getXLabel() const
{
  return m_x_label;
}

const string & Map1Projector::getYLabel ( bool ) const
{
  return m_ntuple->getLabelAt( m_columns[0] );
}

namespace dp = hippodraw::DataPoint2DTuple;


double
Map1Projector::
getAverage ( hippodraw::Axes::Type axis ) const
{
  Map1Projector * p = const_cast < Map1Projector * > ( this );
  p -> prepareValues ();

  unsigned int col = 2; // bad value
  switch ( axis ) {

  case Axes::X:
    col = dp::X;
    break;
    
  case Axes::Y:
    col = dp::Y;
      break;
    
  default:
    break;
  }
  assert ( col < 2 );

  const DataSource * ntuple = getProjectedValues ();
  const vector < double > & data = ntuple -> getColumn ( col );

  unsigned int size = ntuple -> rows ();
  double sum = 0.0;
  sum = accumulate ( data.begin(), data.end(), sum );

  return sum / size;
}

void Map1Projector::addPointReps()
{
  m_pointreps.push_back ( "Symbol" );
}

DataSource *
Map1Projector::
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
Map1Projector::
fillProjectedValues ( DataSource * ntuple, bool ) const
{
  unsigned int y_col = m_columns[0];
  unsigned int y_err = m_columns[1];
  unsigned int size = m_ntuple -> rows ();

  ntuple -> clear();
  ntuple -> reserve ( size );

  vector < double > row ( dp::SIZE );
  for ( unsigned int i = 0; i < size; i++ ) {
    if ( acceptRow ( i, m_cut_list ) == false ||
	 inRange ( i ) == false ) continue;
    row[dp::X] = i;
    row[dp::Y] = m_ntuple -> valueAt ( i, y_col );

    double ye 
      = y_err < UINT_MAX ? (m_ntuple -> valueAt ( i, y_err ) ) : 0.0;
    row[dp::XERR] = 0.0;
    row[dp::YERR] = ye;
    ntuple -> addRow ( row );
  }
}

void
Map1Projector::
prepareValues ()
{
  if ( m_proj_values == 0 ) {
    m_proj_values = createNTuple ();
  } else {
    fillProjectedValues ( m_proj_values );
  }

  setDirty ( false );
}
