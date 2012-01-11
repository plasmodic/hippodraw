/** @file

hippodraw::Map2Projector class implementation

Copyright (C) 2000-2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: Map2Projector.cxx,v 1.174 2007/04/23 21:57:21 pfkeb Exp $

*/

#include <limits.h>

#ifdef _MSC_VER
// Include max() and min() missing from Microsoft Visual C++.
#include "msdevstudio/MSconfig.h"
#endif

#include "Map2Projector.h"

#include "axes/Range.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/NTuple.h"

#include <algorithm>

#include <cfloat>

#include <cassert>

using namespace hippodraw;

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::find;
using std::max;
using std::min;
using std::string;
using std::vector;
#endif

Map2Projector::Map2Projector ( )
  : NTupleProjector ( 4 ),
    m_x_option ( "X error (optional)" ),
    m_y_option ( "Y error (optional)" )
{
  m_binding_options.push_back ( "X" );
  m_binding_options.push_back ( "Y" );
  m_min_bindings = 2;
  addPointReps();
}

/** @note Must initialize with the copy constructor of ProjectorBase
    here, otherise the default constructor will be used.  This is
    because it is a virtual base class of the inherited classes.
 */
Map2Projector::
Map2Projector ( const Map2Projector & projector )
  : ProjectorBase ( projector ),
    NTupleProjector ( projector )
{
  addPointReps();
}

// For some reason, implementing empty destructor decrease code size
// by 5 kbytes.
Map2Projector::~Map2Projector()
{
}

ProjectorBase * Map2Projector::clone()
{
  return new Map2Projector( *this );
}

void Map2Projector::setXErrorOption ( bool enable )
{
  const string name ( m_x_option );
  vector< string >:: iterator first 
    = find ( m_binding_options.begin (),
	     m_binding_options.end (),
	     name );

  if ( first != m_binding_options.end () && !enable ) {
    m_binding_options.erase ( first );
    m_columns[2] = UINT_MAX;
  }
  else if ( enable ) {
    m_binding_options.push_back ( name );
  }
}

/** @todo Remove duplication of code in this method with
    setXErrorOption. */
void Map2Projector::setYErrorOption ( bool enable )
{
  const string name ( m_y_option );
  vector< string >:: iterator first 
    = find ( m_binding_options.begin (),
	     m_binding_options.end (),
	     name );
  if ( first != m_binding_options.end () && !enable ) {
    m_binding_options.erase ( first );
    m_columns[3] = UINT_MAX;
  }
  else if ( enable ) {
    m_binding_options.push_back ( name );
  }
}

void Map2Projector::changedNTuple()
{
  unsigned int cols = m_ntuple->columns () - 1;
  if ( m_columns[0] > cols ) m_columns[0] = cols;
  if ( m_columns[1] > cols ) m_columns[1] = cols;
  if ( m_columns[2] > cols ) m_columns[2] = UINT_MAX;
  if ( m_columns[3] > cols ) m_columns[3] = UINT_MAX;
}

Range Map2Projector::valueRange () const
{
  return dataRangeOn ( Axes::Y );
}

Range
Map2Projector::
dataRangeOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y );

  if ( axis == Axes::X ) {
    if ( m_columns[2] == UINT_MAX ) {
      return dataRange ( m_columns[0] );
    } else {
      return dataRangeWithError ( m_columns[0], m_columns[2] );
    }
  }
  // It has to be Y.
  if ( m_columns[3] == UINT_MAX ) {
    return dataRange ( m_columns[1] );
  }
  // It has to be Y with an error.
  return dataRangeWithError ( m_columns[1], m_columns[3] );
}

double
Map2Projector::
getPosOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y );

  if ( axis == Axes::X ) {
    if ( m_columns[2] == UINT_MAX ) {
      return getPos ( m_columns[0] );
    } else {
      return getPosWithError ( m_columns[0], m_columns[2] );
    }
  }
  // It has to be Y.
  if ( m_columns[3] == UINT_MAX ) {
    return getPos ( m_columns[1] );
  }
  // It has to be Y with an error.
  return getPosWithError ( m_columns[1], m_columns[3] );
}

void Map2Projector::addPointReps()
{
  m_pointreps.push_back ( "Symbol" );
  m_pointreps.push_back ( "Line" );
  m_pointreps.push_back ( "Column" );
}

namespace dp = hippodraw::DataPoint2DTuple;

DataSource *
Map2Projector::
createNTuple () const
{

  unsigned int x_col = m_columns[0];
  unsigned int y_col = m_columns[1];

  unsigned int x_err = m_columns[2];
  unsigned int y_err = m_columns[3];

  unsigned int columns = 4;
  NTuple * ntuple = new NTuple ( columns );

  vector < string > labels;
  labels.push_back ( m_ntuple -> getLabelAt ( x_col ) );
  labels.push_back ( m_ntuple -> getLabelAt ( y_col ) );

  if ( x_err < UINT_MAX ) {
    labels.push_back ( m_ntuple -> getLabelAt ( x_err ) );
  } else {
    labels.push_back ( dp::WIDTH );
  }

  if ( y_err < UINT_MAX ) {
    labels.push_back ( m_ntuple -> getLabelAt ( y_err ) );
  } else {
    labels.push_back ( dp::ERROR );
  }
  ntuple->setLabels ( labels );

  unsigned int size = m_ntuple -> rows ();
  ntuple -> reserve ( size );

  fillProjectedValues ( ntuple );

  return ntuple;
}

/** Fills the NTuple with the projected values.  Fills the %NTuple
    with the projected values from the data in the bound columns.  If
    either the x-error or y-error columns are not bound, and the
    %NTuple has columns that match one created by projector with width
    and error columns, then take that data points as the projected
    values.   @sa DataPoint2DTuple.
 */
void
Map2Projector::
fillProjectedValues ( DataSource * ntuple, bool in_range ) const
{
  ntuple -> clear ();

  unsigned int x_col = m_columns[0];
  unsigned int y_col = m_columns[1];

  unsigned int x_err = m_columns[2];
  unsigned int y_err = m_columns[3];

  const vector < string > & labels = m_ntuple -> getLabels ();
  unsigned int size = labels.size();
  if ( size > 2 ) {
    if ( x_err == UINT_MAX &&
	 labels [ dp::XERR ] == dp::WIDTH ) x_err = dp::XERR;
    if ( size > 3 ) {
      if ( y_err == UINT_MAX &&
	   labels [ dp::YERR ] == dp::ERROR ) y_err = dp::YERR;
    }
  }
  size = m_ntuple -> rows ();
  vector < double > row ( dp::SIZE );
  for ( unsigned int i = 0; i < size; i++ ) {
    if ( acceptRow ( i, m_cut_list ) == false ||
	 ( in_range == true && inRange ( i ) == false ) ) continue;

    row[dp::X] = m_ntuple -> valueAt ( i, x_col );
    row[dp::Y] = m_ntuple -> valueAt ( i, y_col );


    double xe 
      = x_err < UINT_MAX ? m_ntuple -> valueAt ( i, x_err ) : 0.0;
    double ye 
      = y_err < UINT_MAX ? m_ntuple -> valueAt( i, y_err ) : 0.0;

    row[dp::XERR] = xe;
    row[dp::YERR] = ye;

    ntuple -> addRow ( row );
  }
}

void
Map2Projector::
prepareValues ()
{
  if ( m_proj_values == 0 ) {
    m_proj_values = createNTuple ();
  }
  else if ( isDirty () ) {
    fillProjectedValues ( m_proj_values, false );
  }

  setDirty ( false );
}
