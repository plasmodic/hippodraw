/** @file

Profile2DProjector class implementation

Copyright (C) 2000-2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: Profile2DProjector.cxx,v 1.122 2006/03/17 00:02:54 pfkeb Exp $

*/

#ifdef _MSC_VER
// include max() and min() missing from Microsoft Visual C++
//#include "msdevstudio/MSconfig.h"
#endif //_MSC_VER

#include "Profile2DProjector.h"

#include "axes/AxisModelBase.h"

#include "binners/BinsBase.h"
#include "binners/BinsFactory.h"
#include "binners/BinnerAxis.h"
#include "binners/BinnerAxisFactory.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/NTuple.h"

#include <cassert>

using namespace hippodraw;

using std::list;
using std::max;
using std::string;
using std::vector;

Profile2DProjector::Profile2DProjector( )
  : BinningProjector ( 2 ),
    NTupleProjector ( 4 )
{
  m_binding_options.push_back ( "X" );
  m_binding_options.push_back ( "Y" );
  m_binding_options.push_back ( "Z" );
  m_min_bindings = 3;

  BinnerAxisFactory * binner_factory = BinnerAxisFactory::instance();
  BinnerAxis * x = binner_factory -> create ( "BinnerLinear" );
  BinnerAxis * y = binner_factory -> create ( "BinnerLinear" );

  BinsFactory * factory = BinsFactory::instance();
  m_binner = factory->create ( "Bins2DProfile" );

  m_binner->setBinnerOn ( x, Axes::X );
  m_binner->setBinnerOn ( y, Axes::Y );
  addPointReps();
}

/** @note Must initialize with the copy constructor of ProjectorBase
    here, otherise the default constructor will be used.  This is
    because it is a virtual base class of the inherited classes.
 */
Profile2DProjector::
Profile2DProjector ( const Profile2DProjector & projector )
  : ProjectorBase ( projector ),
    BinningProjector ( projector ),
    NTupleProjector ( projector ),
    m_value_range( projector.m_value_range )
{
  addPointReps();
}

ProjectorBase * Profile2DProjector::clone()
{
  return new Profile2DProjector( *this );
}

void Profile2DProjector::changedNTuple()
{
  unsigned int cols = m_ntuple->columns () - 1;
  if ( m_columns[0] > cols ) m_columns[0] = cols;
  if ( m_columns[1] > cols ) m_columns[1] = cols;
  if ( m_columns[2] > cols ) m_columns[2] = cols;

  m_binner->setDirty();
}

/** @request Support averaging with a weight. */
void Profile2DProjector::execute()
{
  if ( m_ntuple->isNull () ) return;
  unsigned int ix = m_columns[0];
  unsigned int iy = m_columns[1];
  unsigned int iz = m_columns[2];
  unsigned int iw = m_columns[3];
  bool have_weight = false;
//    bool iw = m_weight >= 0;
//    if ( have_weight ) {
//      weight = &(m_ntuple->operator [] ( m_weight ) );
//    }

  // Use integer indexing to ensure that it will take everything from the
  // same row, including the cut values.

  m_binner->reset();
  unsigned int size = m_ntuple -> rows ();
  for ( unsigned int i = 0; i < size; i++ ) 
    {
      if ( acceptRow ( i, m_cut_list ) == false ) continue;

      double x = m_ntuple -> valueAt ( i, ix );
      double y = m_ntuple -> valueAt ( i, iy );
      double z = m_ntuple -> valueAt ( i, iz );

      double w = 1.0;
      if ( have_weight ) {
	w = m_ntuple -> valueAt ( i, iw );
      }
      m_binner->accumulate( x, y, z, w );
    }
}

/* virtual */
bool Profile2DProjector::isAxisBinned ( const std::string & axis ) const
{
  if ( axis == m_binding_options[0]
       || axis == m_binding_options[1] ) return true;
  return false;
}

Range Profile2DProjector::valueRange() const
{
  if ( isDirty () ) {
    // Work around const.
    Profile2DProjector * p = const_cast <Profile2DProjector *> ( this );
    p->prepareValues ();
  }
  
  return dataRangeOn ( Axes::Z );
}

namespace dp = hippodraw::DataPoint3DTuple;

Range
Profile2DProjector::
dataRangeOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );
  if ( axis == Axes::X ) {
    return dataRange ( m_columns[0] );
  }
  if ( axis == Axes::Y ) {
    return dataRange ( m_columns[1] );
  }
  // else  axis == Z )
  if ( isDirty () ) {
    Profile2DProjector * p = const_cast < Profile2DProjector * > ( this );
    p ->prepareValues ();
    p -> setDirty ( false );
  }
  const vector < double > & values = m_proj_values -> getColumn ( dp::Z );

  return Range ( values );
}

/** @todo Seems to be duplicated code here. 
 */
double
Profile2DProjector::
getPosOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );
  if ( axis == Axes::X ) {
    return getPos ( m_columns[0] );
  }
  if ( axis == Axes::Y ) {
    return getPos ( m_columns[1] );
  }
  if ( axis == Axes::Z ) {
    return getPos ( m_columns[2] );
  }
  return DBL_MAX;
}

/** @todo Should not be changing the AxisModelBase Range. */
void
Profile2DProjector::
setRange ( hippodraw::Axes::Type axis, bool const_width )
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  AxisModelBase * model = 0;
  if ( axis == Axes::X ) {
    model = m_x_axis;
  } else if ( axis == Axes::Y ) {
    model = m_y_axis;
  }

  if ( axis != Axes::Z ) {
    const Range & range = model->getRange (false);
    if( model->isLog() ) {
      if ( range.low() < 0.0 ) return;
      model->setRange ( range.low(), range.high(), getPosOn ( Axes::X ) );
      const Range & range2 = model->getRange ( false );
      setBinnerRange ( axis, range2, const_width );
    } else {
      setBinnerRange ( axis, range, const_width );
    }
  }

  setDirty ( true );
}

/** @todo Avoid duplicated code in Hist2DProjector. */
const Range &
Profile2DProjector::
setBinWidth ( hippodraw::Axes::Type axis, double width )
{
  assert ( axis == Axes::X || axis == Axes::Y );

  const Range & range = m_binner->setBinWidth ( axis, width );

  setDirty ( true );

  return range;
}

/** @todo Change design: this method is identical to one in
    Hist2DProjector. 
*/
// void
const Range &
Profile2DProjector::
setBinWidth ( Axes::Type axis, 
	      int parm, 
	      bool dragging )
{
  double new_width = m_binner->calcBinWidth ( axis, parm, dragging );

  return setBinWidth ( axis, new_width );
}

/** @todo Change design: this method is identical to one in
    Hist2DProjector. 
    @todo use Axes::convert.
*/
void Profile2DProjector::setOffset ( const std::string & axis, 
				       int parm, 
				       bool dragging )
{
  if ( ( axis == "X" || axis == "Y" )  == false ) return;
  
  double new_offset = m_binner->calcOffset ( axis, parm, dragging );

  Axes::Type at;
  if ( axis == "X" ) {
    at = Axes::X;
  }
  else { // "Y"
    at = Axes::Y;
  }

  if( new_offset == 0.0 ) return; // no one cares
  if( !dragging ) // reset
    setOffset( at, 0.0 );
  else
    setOffset( at, new_offset );

  setDirty ( true );
}

/* virtual */
void
Profile2DProjector::
setOffset ( hippodraw::Axes::Type axis, double offset )
{
  if( axis == Axes::Z ) return;
  assert ( axis == Axes::X || axis == Axes::Y );

  m_binner->setOffset ( axis, offset );
  if( axis == Axes::X )
    m_x_axis->setRange( m_binner->getRange ( Axes::X ), true );
  else
    m_y_axis->setRange( m_binner->getRange ( Axes::Y ), true );

  setDirty ( true );
}

const string & Profile2DProjector::getZLabel() const
{
  return m_ntuple->getLabelAt( m_columns[2] );
}

void Profile2DProjector::addPointReps()
{
  m_pointreps.push_back ( "ColorBox" );
  m_pointreps.push_back ( "Contour" );
}

void
Profile2DProjector::
setBinnerRange ( hippodraw::Axes::Type axis,
		 const Range & range, bool const_width )
{
  m_binner -> setRange ( axis, range, const_width );
  checkScaling ();

  setDirty ( true );
}

void
Profile2DProjector::
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
Profile2DProjector::
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
