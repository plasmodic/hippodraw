/** @file

DyHist2DProjector class implementation.

Copyright (C) 2000-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DyHist2DProjector.cxx,v 1.237 2005/09/12 16:55:10 pfkeb Exp $

*/

#ifdef _MSC_VER
// Include max() and min() missing from Microsoft Visual C++.
#include "msdevstudio/MSconfig.h"
#endif //_MSC_VER

#include "DyHist2DProjector.h"

#include "axes/AxisModelBase.h"

#include "binners/BinsBase.h"
#include "binners/BinsFactory.h"
#include "binners/BinnerAxis.h"
#include "binners/BinnerAxisFactory.h"

#include "datasrcs/NTuple.h"

#include <cassert>
#include <limits.h>

using namespace hippodraw;

using std::list;
using std::max;
using std::string;
using std::vector;

DyHist2DProjector::DyHist2DProjector( )
  : Hist2DProjImp (),
    NTupleProjector ( 3 )
{
  m_binding_options.push_back ( "X" );
  m_binding_options.push_back ( "Y" );
  m_binding_options.push_back ( "Weight (optional)" );
  m_min_bindings = 2;
}

/** @note Must initialize with the copy constructor of ProjectorBase
    here, otherise the default constructor will be used.  This is
    because it is a virtual base class of the inherited classes.
 */
DyHist2DProjector::
DyHist2DProjector ( const DyHist2DProjector & projector )
  : ProjectorBase ( projector ),
    Hist2DProjImp ( projector ),
    NTupleProjector ( projector ),
    m_z_label_entries ( projector.m_z_label_entries ),
    m_z_label_density ( projector.m_z_label_density ),
    m_value_range( projector.m_value_range )
{
}

ProjectorBase * DyHist2DProjector::clone()
{
  return new DyHist2DProjector( *this );
}

void DyHist2DProjector::changedNTuple()
{
  unsigned int cols = m_ntuple->columns () - 1;
  if ( m_columns[0] > cols ) m_columns[0] = cols;
  if ( m_columns[1] > cols ) m_columns[1] = cols;

  m_binner->setDirty();
}

void DyHist2DProjector::execute()
{
  unsigned int x_col = m_columns[0];
  unsigned int y_col = m_columns[1];
  unsigned int w_col = m_columns[2];

  bool have_weight = w_col < UINT_MAX;

  // Use integer indexing to ensure that it will take everything from the
  // same row, including the cut values.

  m_binner->reset();
  unsigned int size = m_ntuple -> rows ();
  for ( unsigned int i = 0; i < size; i++ ) 
    {
      if ( acceptRow ( i, m_cut_list ) == false ) continue;

      double x = m_ntuple -> valueAt ( i, x_col );
      double y = m_ntuple -> valueAt ( i, y_col );
      double w = 1.0;
      if ( have_weight ) {
	w = m_ntuple -> valueAt ( i, w_col );
      }
      m_binner->accumulate( x, y, w );
    }
}

/* virtual */
bool DyHist2DProjector::isAxisBinned ( const std::string & axis ) const
{
  if ( axis == m_binding_options[0]
       || axis == m_binding_options[1] ) return true;
  return false;
}

Range
DyHist2DProjector::
dataRangeOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );
  if ( axis == Axes::X ) {
    return dataRange ( m_columns[0] );
  } else if ( axis == Axes::Y ) {
    return dataRange ( m_columns[1] );
  } else {
    return dataRangeOnValue ();
  }
}

double
DyHist2DProjector::
getPosOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );
  if ( axis == Axes::X ) {
    return getPos ( m_columns[0] );
  } else if( axis == Axes::Y ) {
    return getPos ( m_columns[1] );
  } else {
    return valueRange().pos();
  }
}

const Range &
DyHist2DProjector::
setBinWidth ( Axes::Type axis,
	      int parm, 
	      bool dragging )
{
  double new_width = m_binner->calcBinWidth ( axis, parm, dragging );

  return Hist2DProjImp::setBinWidth ( axis, new_width );
}

void DyHist2DProjector::setOffset ( const std::string & axis, 
				    int parm, 
				    bool dragging )
{
  Axes::Type at = Axes::convert ( axis );
  if ( at != Axes::X && at != Axes::Y ) return;

  double new_offset = m_binner->calcOffset ( axis, parm, dragging );

  if( new_offset == 0.0 ) return; // no one cares
  if( !dragging ) // reset
    Hist2DProjImp::setOffset( at, 0.0 );
  else
    Hist2DProjImp::setOffset( at, new_offset );

  setDirty ( true );
}

void
DyHist2DProjector::
setBinnerRange ( hippodraw::Axes::Type axis,
		 const Range & range,
		 bool const_width )
{
  m_binner -> setRange ( axis, range, const_width );
  checkScaling ();

  setDirty ( true );
}

void
DyHist2DProjector::
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
DyHist2DProjector::
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
