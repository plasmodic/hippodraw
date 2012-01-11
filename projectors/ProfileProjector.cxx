/** @file

hippodraw::ProfileProjector class implementation

Copyright (C) 2000-2004 2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: ProfileProjector.cxx,v 1.209 2007/04/23 21:57:21 pfkeb Exp $

*/

#include <limits.h>

#ifdef _MSC_VER
// Include max() and min() missing from Microsoft Visual C++.
#include "msdevstudio/MSconfig.h"
#endif

#include "ProfileProjector.h"

#include "axes/AxisModelBase.h"

#include "binners/BinsBase.h"
#include "binners/BinsFactory.h"
#include "binners/BinnerAxis.h"
#include "binners/BinnerAxisFactory.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/NTuple.h"

#include <algorithm>

#include <cassert>

using namespace hippodraw;

#include <stdio.h>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::list;
using std::max;
using std::string;
using std::vector;
#endif

ProfileProjector::ProfileProjector( )
  : BinningProjector ( 1 ),
    NTupleProjector ( 3 )
{
  m_binding_options.push_back ( "X" );
  m_binding_options.push_back ( "Y" );
  m_binding_options.push_back ( "Weight (optional)" );
  m_min_bindings = 2;

  BinnerAxisFactory * binner_factory = BinnerAxisFactory::instance ();
  BinnerAxis * binner = binner_factory -> create ( "BinnerLinear" );

  BinsFactory * factory = BinsFactory::instance();
  m_binner = factory -> create ( "Bins1DProfile" );

  m_binner->setBinnerOn ( binner, Axes::X );
  addPointReps();
}

/** @note Must initialize with the copy constructor of ProjectorBase
    here, otherise the default constructor will be used.  This is
    because it is a virtual base class of the inherited classes.
 */
ProfileProjector::
ProfileProjector ( const ProfileProjector & projector )
  : ProjectorBase ( projector ),
    BinningProjector ( projector ),
    NTupleProjector ( projector )
{
  addPointReps();
}

ProjectorBase * ProfileProjector::clone()
{
  return new ProfileProjector( *this );
}

/** @todo This implementation is the same as the Hist1DProjector. */
void ProfileProjector::changedNTuple()
{
  unsigned int cols = m_ntuple->columns () - 1;
  if ( m_columns[0] > cols ) m_columns[0] = cols;
  if ( m_columns[1] > cols ) m_columns[1] = cols;
  if ( m_columns[2] > cols ) m_columns[2] = cols;

  m_binner->setDirty ( );
}

void ProfileProjector::execute ()
{
  if ( m_ntuple->isNull () ) return;

  // Get the data and the optional weight column. 
  unsigned int & x_col = m_columns[0];
  unsigned int & y_col = m_columns[1];
  unsigned int & w_col = m_columns[2];
  unsigned int size = m_ntuple -> rows ();

  bool have_weight = w_col < UINT_MAX;

  // Use integer indexing to ensure that it will take everything from the
  // same row, including the cut values.

  m_binner->reset ();

  for ( unsigned int i = 0; i < size; i++ )
    {
      if ( acceptRow ( i, m_cut_list ) == false ) continue;

      double x = m_ntuple -> valueAt ( i, x_col );
      double y = m_ntuple -> valueAt ( i, y_col );
      double w = 1.0;
      if ( have_weight) {
    w = m_ntuple -> valueAt ( i, w_col );
      }
      m_binner->accumulate( x, y, w );
    }
}

Range ProfileProjector::valueRange () const
{
  return dataRangeOn ( Axes::Y );
}

namespace dp = hippodraw::DataPoint2DTuple;

Range
ProfileProjector::
dataRangeOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y );

  if ( axis == Axes::X ) {
    return dataRange ( m_columns[0] );
  }

  ProfileProjector * p = const_cast<ProfileProjector *> ( this );
  p->prepareValues ();

  // If no points are left due to cuts, then returns zero range.
  if ( m_proj_values -> empty () ) {
    return Range ( 0.0, 0.0 );
  }
  double max = DBL_MIN;
  double min = DBL_MAX;

  const vector < double > & values = m_proj_values -> getColumn ( dp::Y );
  const vector < double > & errors = m_proj_values -> getColumn ( dp::YERR );
  for ( unsigned int i = 0; i < values.size(); i++ ) {
    double hi = values[i] + errors[i];
    double lo = values[i] - errors[i];
    max = std::max ( max, hi );
    min = std::min ( min, lo );
  }

  return Range ( min, max );
}

double
ProfileProjector::
getPosOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y );

  if ( axis == Axes::X ) {
    return getPos ( m_columns[0] );
  }

  // If no points are left due to cuts, then returns DBL_MAX.
  if ( m_proj_values -> empty() ) {
    return DBL_MAX;
  }

  double pos = DBL_MAX;

  const vector < double > & values = m_proj_values -> getColumn ( dp::Y );
  const vector < double > & errors = m_proj_values -> getColumn ( dp::YERR );
  for ( unsigned int i = 0; i < values.size (); i++ ) {
    double lo = values[i] - errors[i];
    if ( lo > 0.0 &&
	 lo < pos ) pos = lo;
  }

  return pos;
}

/* virtual */
bool ProfileProjector::isAxisBinned ( const std::string & axis ) const
{
  if ( axis == m_binding_options[0] ) {
    return true;
  }
  return false;
}

void ProfileProjector::addPointReps()
{
  m_pointreps.push_back ( "Symbol" );
}

void
ProfileProjector::
setRange ( hippodraw::Axes::Type axis, bool const_width )
{
  assert ( m_binner );
  assert ( axis == Axes::X || axis == Axes::Y );

  if ( axis == Axes::X ) {
    const Range & range = m_x_axis->getRange( false );
    if( m_x_axis->isLog() ) {
      if( range.low() < 0.0 ) return;
      m_x_axis->setRange ( range.low(), range.high(), getPosOn ( Axes::X ) );
      const Range & range2 = m_x_axis->getRange( false );
      setBinnerRange ( axis, range2, const_width );
    } 
    else {
      setBinnerRange ( axis, range, const_width );

    }
  }
}

void
ProfileProjector::
setBinnerRange ( hippodraw::Axes::Type axis,
		 const Range & range,
		 bool const_width )
{
  m_binner -> setRange ( axis, range, const_width );
  checkScaling ();

  setDirty ( true );
}

void
ProfileProjector::
update ( const Observable * object )
{
  const DataSource * datasource 
    = dynamic_cast < const DataSource * > ( object );

  if ( datasource != 0 ) {
    NTupleProjector::update ( object );
  }
  else {
    BinningProjector::update ( object );
  }
}

void
ProfileProjector::
willDelete ( const Observable * object )
{
  const DataSource * datasource 
    = dynamic_cast < const DataSource * > ( object );

  if ( datasource != 0 ) {
    NTupleProjector::willDelete ( object );
  }
  else {
    BinningProjector::willDelete ( object );
  }
}
