/** @file

hippodraw::LineProjector class implementation

Copyright (C) 2004-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: LineProjector.cxx,v 1.12 2006/08/05 22:06:57 pfkeb Exp $

*/

#include "LineProjector.h"

#include "axes/AxisModelBase.h"
#include "datasrcs/NTuple.h"

using namespace hippodraw;

using std::string;
using std::vector;

LineProjector::
LineProjector ( hippodraw::Axes::Type axis, double value )
  : m_value ( value ),
    m_axis ( axis )
{
}

LineProjector::
LineProjector ( const LineProjector & projector )
  : ProjectorBase ( projector ),
    m_value ( projector.m_value ),
    m_axis ( projector.m_axis )
{
}

LineProjector::~LineProjector ()
{
  // nothing to be done
}

ProjectorBase * LineProjector::clone ()
{
  return new LineProjector ( *this );
}

void
LineProjector::
addPointReps()
{
}
bool
LineProjector::
isEmpty () const
{
  return false;
}

Range
LineProjector::
dataRangeOn ( hippodraw::Axes::Type axis ) const
{
  Range range;
  if ( m_axis == Axes::X ) {
    if ( axis == Axes::X ) {
      range.setRange ( m_value, m_value, m_value );
    }
    else if ( axis == Axes::Y ) {
      range = m_y_axis -> getRange ( false );
    }
  }
  else if ( m_axis == Axes::Y ) {
    if ( axis == Axes::X ) {
      range = m_x_axis -> getRange ( false );
    }
    else if ( axis == Axes::Y ) {
      range.setRange ( m_value, m_value, m_value );
    }
  }

  return range;
}

const std::string &
LineProjector::
getTitle() const
{
  return m_title;
}

Range
LineProjector::
valueRange () const
{
  Range range;
  if ( m_axis == Axes::X ) {
    range = m_y_axis -> getRange ( false );
  }
  else {
    range = m_x_axis -> getRange ( false );
  }

  return range;
}

double
LineProjector::
getPosOn ( hippodraw::Axes::Type axis ) const
{
  double pos = DBL_MIN;
  if ( m_axis == Axes::X ) {
    if ( axis == Axes::X ) {
      pos = std::max ( pos, m_value );
    }
    else if ( axis == Axes::Y ) {
      const Range & range = m_y_axis -> getRange ( false );
      pos = range.pos();
    }
  }
  else if ( m_axis == Axes::Y ) {
    if ( axis == Axes::X ) {
      const Range & range = m_x_axis -> getRange ( false );
      pos = range.pos();
    }
    else if ( axis == Axes::Y ) {
      pos = std::max ( pos, m_value );
    }
  }

  return pos;
}

const std::string & 
LineProjector::
getXLabel() const
{
  return m_title;
}

const std::string & 
LineProjector::
getYLabel( bool ) const
{
  return m_title;
}

int
LineProjector::
getNumberOfEntries () const
{
  return 1;
}

int
LineProjector::
getUnderflow () const
{
  return -1;
}

int 
LineProjector::
getOverflow () const
{
  return -1;
}


void LineProjector::prepareValues ()
{
  m_proj_values = createNTuple ();
  fillProjectedValues ( m_proj_values );

  setDirty ( false );
}

namespace dp = hippodraw::DataPoint2DTuple;

DataSource *
LineProjector::
createNTuple () const
{
  unsigned int columns = dp::SIZE;
  NTuple * ntuple = new NTuple ( columns );

  const char * values[] = { "X", "Y", "nil", "nil" };
  vector < string > labels ( values, values + 4 );
  ntuple -> setLabels ( labels );

  fillProjectedValues ( ntuple );

  return ntuple;
}

void
LineProjector::
fillProjectedValues ( DataSource * ntuple, bool ) const
{
  ntuple -> clear ();

  vector < double > row ( dp::SIZE );
  row[dp::XERR] = 0.;
  row[dp::YERR] = 0.;

  if ( m_axis == Axes::X ) { // vertical line
    const Range & range = m_y_axis -> getRange ( false );
    row[dp::X] = m_value;
    row[dp::Y] = range.low ();
    ntuple -> addRow ( row );
    row[dp::Y] = range.high ();
    ntuple -> addRow ( row );
  }
  else { // horizontal line
    const Range & range = m_x_axis -> getRange ( false );
    row[dp::X] = range.low ();
    row[dp::Y] = m_value;
    ntuple -> addRow ( row );
    row[dp::X] = range.high ();
    ntuple -> addRow ( row );
  }
}

void
LineProjector::
setValue ( double value )
{
  m_value = value;
}
