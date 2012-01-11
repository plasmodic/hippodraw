/** @file

Hist2DProjImp class implementation.

Copyright (C) 2000-2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: Hist2DProjImp.cxx,v 1.231 2006/01/31 23:42:10 xiefang Exp $

*/

#ifdef _MSC_VER
// for max() and min() missing from Microsoft Visual C++.
#include "msdevstudio/MSconfig.h"
#endif //_MSC_VER

#include "Hist2DProjImp.h"

#include "axes/AxisModelBase.h"

#include "binners/BinsBase.h"
#include "binners/BinsFactory.h"
#include "binners/BinnerAxis.h"
#include "binners/BinnerAxisFactory.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/NTuple.h"

#include <cassert>

using std::list;
using std::max;
using std::string;
using std::vector;

namespace hippodraw {

Hist2DProjImp::Hist2DProjImp( )
  : BinningProjector ( 2 )
{
  BinnerAxisFactory * binner_factory = BinnerAxisFactory::instance ();
  BinnerAxis * x = binner_factory -> create ( "BinnerLinear" );
  BinnerAxis * y = binner_factory -> create ( "BinnerLinear" );

  BinsFactory * factory = BinsFactory::instance ();
  m_binner = factory->create ( "Bins2DHist" );

  m_binner->setBinnerOn ( x, Axes::X );
  m_binner->setBinnerOn ( y, Axes::Y );

  m_z_label_entries = "Entries / bin";
  m_z_label_density = "Density";

  addPointReps();
}

/** Initializing virtual base here means it is getting done also by
    the derived classes of this class, but should not do any harm and
    gets rid of warning.
 */
Hist2DProjImp::
Hist2DProjImp ( const Hist2DProjImp & projector )
  : ProjectorBase ( projector ),
    BinningProjector ( projector ),
    m_z_label_entries ( projector.m_z_label_entries ),
    m_z_label_density ( projector.m_z_label_density ),
    m_value_range( projector.m_value_range )
{
  addPointReps();
}

/** @note For some reason, placing empty destructor here decrease code
    size by 7 kbytes.
 */
Hist2DProjImp::~Hist2DProjImp()
{
}

bool Hist2DProjImp::isValueBinned () const
{
  return true;
}

double 
Hist2DProjImp::
getPosOnValue () const
{
  Range range = dataRangeOnValue ();

  return range.pos ();
}


Range
Hist2DProjImp::
preferredRange ( hippodraw::Axes::Type  axis ) const
{
  Range range = dataRangeOn ( axis );
  if ( axis == Axes::Z ) {
    range.setLow ( 0.0 );
  }

  return range;
}

namespace dp = hippodraw::DataPoint3DTuple;

Range
Hist2DProjImp::
dataRangeOnValue () const
{
  Hist2DProjImp * p = const_cast < Hist2DProjImp * > ( this );
  p->prepareValues ();
  if ( m_proj_values -> empty () ) {
    return Range ( 0.0, 1.0, 0.5 );
  }

  const vector < double > & values = m_proj_values -> getColumn( dp::Z );
  return  Range ( values );
}

Range Hist2DProjImp::valueRange() const
{
  return dataRangeOn ( Axes::Z );
}

/** @todo Should not be changing the AxisModelBase Range. */
void
Hist2DProjImp::
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
      if( range.low() < 0.0 ) return;
      model->setRange ( range.low(), range.high(), getPosOn ( Axes::X ) );
      const Range & range2 = model->getRange ( false );
      setBinnerRange ( axis, range2, const_width );
    } else {
      setBinnerRange ( axis, range, const_width );
    }
  }
}

const Range &
Hist2DProjImp::
setBinWidth ( hippodraw::Axes::Type axis, double width )
{
  assert ( axis == Axes::X || axis == Axes::Y );

  const Range & range = m_binner->setBinWidth ( axis, width );
  checkScaling ();

  setDirty ( true );

  return range;
}

void
Hist2DProjImp::
setOffset ( hippodraw::Axes::Type axis, double offset )
{
  if ( axis == Axes::Z ) return;
  assert ( axis == Axes::X || axis == Axes::Y );

  m_binner->setOffset ( axis, offset );
  if( axis == Axes::X )
    m_x_axis->setRange( m_binner->getRange ( Axes::X ), true );
  else
    m_y_axis->setRange( m_binner->getRange ( Axes::Y ), true );

  setDirty ( true );
}


void Hist2DProjImp::setZLabel()
{
  m_z_label_entries = "Entries / bin";
}

const string & Hist2DProjImp::getZLabel() const
{
  bool scaling = m_z_axis->isScaling ();

  if ( scaling ) return m_z_label_entries;
  return m_z_label_density;
}

bool Hist2DProjImp::hasZAxis() const
{
  return true;
}

void Hist2DProjImp::checkScaling ()
{
  if ( m_z_axis == 0 ) return;

  bool yes = m_binner->hasEqualWidths ();

  if ( yes ) {
    double width = m_binner->scaleFactor ();
    m_z_axis->setScaleFactor ( width );
  }
  else {
    m_z_axis->setScaling ( false );
  }

}

void Hist2DProjImp::addPointReps()
{
  m_pointreps.push_back ( "ColorBox" );
  m_pointreps.push_back ( "Contour" );
  //m_pointreps.push_back ( "ColorSymbol" );
}

bool
Hist2DProjImp::
wantsScaleFactor ( const std::string & axis ) const
{
  return axis == "Z" || axis == "z";
}

} // namespace hippodraw

