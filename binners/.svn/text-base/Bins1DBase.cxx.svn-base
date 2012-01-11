/** @file

hippodraw::Bins1DBase class implementation

Copyright (C) 2000-2004, 2006, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Bins1DBase.cxx,v 1.120 2007/04/23 21:57:20 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif


#include "Bins1DBase.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/NTuple.h"

#include <cassert>

using std::string;
using std::vector;

using namespace hippodraw;

Bins1DBase::Bins1DBase ( const char * name )
  : BinsBase ( name ),
    binner_axis ( 0 )
{
}

Bins1DBase::Bins1DBase ( const Bins1DBase & binner )
  : BinsBase ( binner ),
    binner_axis ( 0 )
{
  if ( binner.binner_axis != 0 ) {
    binner_axis = binner.binner_axis->clone();
  }

  m_values_dirty = true;
}

Bins1DBase::~Bins1DBase ()
{
  if ( binner_axis ) delete binner_axis;
}

int 
Bins1DBase::
getNumberOfAxes () const
{
  return 1;
}
const BinnerAxis * 
Bins1DBase::
getBinnerOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X );

  return binner_axis;
}

void Bins1DBase::setBinnerOn ( BinnerAxis * binner,
			       hippodraw::Axes::Type axis )
{
  assert ( axis == Axes::X );

  if ( binner_axis ) delete binner_axis;

  binner_axis = binner;
  resize ();
  m_values_dirty = true;
}

double Bins1DBase::getLow ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X );

  return binner_axis->axisGetLow();
}

double Bins1DBase::getHigh() const
{
  return binner_axis->axisGetHigh();
}

int Bins1DBase::numberOfBins ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X );

  return binner_axis->axisNumberOfBins();
}

void Bins1DBase::setNumberOfBins ( hippodraw::Axes::Type axis, int nb )
{
  assert ( axis == Axes::X && nb > 0 );

  binner_axis->axisSetNumberOfBins ( nb );

  resize ();
}

bool
Bins1DBase::hasEqualWidths () const
{
  return binner_axis->hasEqualWidths ();
}

double
Bins1DBase::binWidth ( int i ) const
{
  return binner_axis->axisBinWidth(i);
}

double
Bins1DBase::scaleFactor () const
{
  return binner_axis->scaleFactorWid ();
}

double
Bins1DBase::binWidth ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X );

  return binner_axis->getConstWid();
}

const Range &
Bins1DBase::setBinWidth ( hippodraw::Axes::Type axis, double width )
{
  assert ( axis == Axes::X && 
	   width > 0.0 );

  const Range & range =  binner_axis->setBinWidth ( width );
  resize ();

  return range;
}

double
Bins1DBase::
calcBinWidth ( Axes::Type axis, int parm, bool dragging ) const
{
  assert ( axis == Axes::X );
  double new_width = binner_axis->calcBinWidth ( parm, dragging );

  return new_width;
}

double
Bins1DBase::calcOffset ( const std::string & axis, 
			 int parm, 
			 bool dragging ) const
{
  assert ( axis == "X" );

  return binner_axis->calcOffset ( parm, dragging );
}

double
Bins1DBase::getOffset ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X );
  
  return binner_axis->getOffset();
}

void
Bins1DBase::
setOffset ( hippodraw::Axes::Type axis, double offset )
{
  assert ( axis == Axes::X );

  binner_axis->setOffset ( offset );
}

const Range &
Bins1DBase::
setRange ( hippodraw::Axes::Type axis, const Range & range, bool hold_width )
{
  assert ( axis == Axes::X );

  const Range & new_range =  binner_axis->setRange ( range, hold_width );
  resize ();

  return new_range;
}

const Range &
Bins1DBase::
getRange ( hippodraw::Axes::Type axis )
{
  assert ( axis == Axes::X );

  return binner_axis->getRange();
}

void Bins1DBase::resize ()
{
  int number = numberOfBins ( Axes::X );
  resize ( number );
}

namespace dp = DataPoint2DTuple;

NTuple *
Bins1DBase::
prepareNTuple ( unsigned int rows ) const
{
  unsigned int columns = 4;
  NTuple * ntuple = new NTuple ( columns );
  ntuple -> reserve ( rows );

  vector < string > labels;
  labels.push_back ( "X" );
  labels.push_back ( "Value" );
  labels.push_back ( dp::WIDTH );
  labels.push_back ( dp::ERROR );

  ntuple->setLabels ( labels );

  return ntuple;
}
