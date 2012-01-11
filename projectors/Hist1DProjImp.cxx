/** @file

hippodraw::Hist1DProjImp class implementation

Copyright (C) 2003-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: Hist1DProjImp.cxx,v 1.26 2007/04/23 21:57:21 pfkeb Exp $

*/

#ifdef _MSC_VER
// Include max() and min() missing from Microsoft Visual C++.
#include "msdevstudio/MSconfig.h"
#endif

#include "Hist1DProjImp.h"

#include "axes/AxisModelBase.h"

#include "binners/BinsBase.h"
#include "binners/BinsFactory.h"
#include "binners/BinnerAxis.h"
#include "binners/BinnerAxisFactory.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/NTuple.h"

#include <cassert>

using namespace hippodraw;

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::list;
using std::max;
using std::string;
using std::vector;
#endif

Hist1DProjImp::Hist1DProjImp( )
  : BinningProjector ( 1 )
{
  BinnerAxisFactory * binner_factory = BinnerAxisFactory::instance ();
  BinnerAxis * binner = binner_factory -> create ( "BinnerLinear" );

  BinsFactory * factory = BinsFactory::instance ();
  m_binner = factory->create ( "Bins1DHist" );

  m_binner->setBinnerOn ( binner, Axes::X );

  m_y_label_entries = "Entries / bin";
  m_y_label_density = "Density";

  addPointReps();
}

/** Initializing virtual base here means it is getting done also by
    the derived classes of this class, but should not do any harm and
    gets rid of warning.
 */
Hist1DProjImp::
Hist1DProjImp ( const Hist1DProjImp & projector )
  : BinningProjector ( projector ),
    m_y_label_entries ( projector.m_y_label_entries ),
    m_y_label_density ( projector.m_y_label_density )
{
  addPointReps();
}


Hist1DProjImp::
~Hist1DProjImp()
{
}

void
Hist1DProjImp::
addPointReps()
{
  m_pointreps.push_back ( "Column" );
  m_pointreps.push_back ( "FilledColumn" );
  m_pointreps.push_back ( "Symbol" );
  m_pointreps.push_back ( "Line" );
}

void
Hist1DProjImp::
checkScaling ()
{
  if ( m_y_axis == 0 ) return;

  bool yes = m_binner->hasEqualWidths ();
  if ( yes ) {
    double width = m_binner->scaleFactor ();
    m_y_axis->setScaleFactor ( width );
  }
  else {
    m_y_axis->setScaling ( false );
  }
}

bool
Hist1DProjImp::
isValueBinned () const
{
  return true;
}

double 
Hist1DProjImp::
getPosOnValue () const
{
  Range range = dataRangeOnValue ();

  return range.pos ();
}

Range
Hist1DProjImp::
preferredRange ( hippodraw::Axes::Type  axis ) const
{
  Range range = dataRangeOn ( axis );
  if ( axis == Axes::Y ) {
    range.setLow ( 0.0 );
  }

  return range;
}


namespace dp = hippodraw::DataPoint2DTuple;

Range
Hist1DProjImp::
dataRangeOnValue ( ) const
{
  Hist1DProjImp * p = const_cast < Hist1DProjImp * > ( this );
  p->prepareValues ();
  if ( m_proj_values -> empty () ) {
    return Range ( 0.0, 1.0, 0.5 );
  }

  const vector < double > & values = m_proj_values -> getColumn( dp::Y );
  return  Range ( values );
}

/** @todo Should not be changing the AxisModelBase Range. */
void 
Hist1DProjImp::
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
Hist1DProjImp::
reset( )
{
}

Range
Hist1DProjImp::
valueRange () const
{
   return dataRangeOn ( Axes::Y );
}

bool
Hist1DProjImp::
wantsScaleFactor ( const std::string & axis ) const
{
  return axis == "Y" || axis == "y";
}
