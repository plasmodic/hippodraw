/** @file

hippodraw::Bins2DBase class implementation

Copyright (C) 2000-2004, 2006, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Bins2DBase.cxx,v 1.96 2007/04/23 21:57:20 pfkeb Exp $

*/


#ifdef _MSC_VER
// Include max() and min() missing from MicroSoft Visual C++.
#include "msdevstudio/MSconfig.h"
#endif

#include "Bins2DBase.h"

#include "BinnerAxis.h"

#include "datasrcs/NTuple.h"

#include <numeric>

#include <cassert>

using std::string;
using std::vector;


using namespace hippodraw;

Bins2DBase::Bins2DBase ( const char * name )
  : BinsBase ( name ),
    binner_axisX ( 0 ),
    binner_axisY ( 0 )
{
}

Bins2DBase::Bins2DBase ( const Bins2DBase & binner )
  : BinsBase( binner ),
    binner_axisX ( 0 ),
    binner_axisY ( 0 ),
    m_data ( binner.m_data )
{
  if ( binner.binner_axisX != 0 ) {
    binner_axisX = binner.binner_axisX->clone ();
  }

  if ( binner.binner_axisY != 0 ) {
    binner_axisY = binner.binner_axisY->clone ();
  }

  m_values_dirty = true;
}

Bins2DBase::~Bins2DBase ()
{
  if ( binner_axisX ) delete binner_axisX;
  if ( binner_axisY ) delete binner_axisY;
}

int 
Bins2DBase::
getNumberOfAxes () const
{
  return 2;
}

void Bins2DBase::resize ()
{
  int nbx = numberOfBins ( Axes::X );
  int nby = numberOfBins ( Axes::Y );

  if ( nbx == 0 || 
       nby == 0 ) return;

  resize ( nbx, nby );
}

const BinnerAxis * 
Bins2DBase::
getBinnerOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X ||
	   axis == Axes::Y    );

  if ( axis == Axes::X ) return binner_axisX;
  // else Y
  return binner_axisY;
}


void
Bins2DBase::
setBinnerOn ( BinnerAxis * binner, hippodraw::Axes::Type axis )
{
  assert ( axis == Axes::X || axis == Axes::Y );

  if ( axis == Axes::X ) {
    if ( binner_axisX ) delete binner_axisX;
    binner_axisX = binner;
  }
  else { // else Y
    if ( binner_axisY ) delete binner_axisY;
    binner_axisY = binner;
  }
  resize ();

  m_values_dirty = true;
}

double
Bins2DBase::
getLow ( hippodraw::Axes::Type axis ) const
{
  if ( axis == Axes::X ) return binner_axisX->axisGetLow();
  if ( axis == Axes::Y ) return binner_axisY->axisGetLow();

  assert ( false );
  return 0.0;
}

void
Bins2DBase::
setNumberOfBins ( hippodraw::Axes::Type axis, int nb ) 
{
  assert ( axis == Axes::X || axis == Axes::Y );

  if ( axis == Axes::X ) {
    binner_axisX->axisSetNumberOfBins( nb );
  
    if ( numberOfBins ( Axes::Y ) > 0 ) {
      setNumberOfBins ( Axes::Y, numberOfBins ( Axes::Y ) );
    }
  }
  else { // Y
    binner_axisY->axisSetNumberOfBins ( nb );
  }

  m_num_bins = numberOfBins ( Axes::X ) * numberOfBins ( Axes::Y );
}

int Bins2DBase::numberOfBins() const
{
  return m_num_bins;
}

int
Bins2DBase::
numberOfBins ( hippodraw::Axes::Type axis) const
{
  assert ( axis == Axes::X || axis == Axes::Y );

  if ( axis == Axes::X && 
       binner_axisX != 0 ) return binner_axisX->axisNumberOfBins();
  // else Y
  if ( binner_axisY != 0 ) return binner_axisY->axisNumberOfBins();

  return 0;
}

int Bins2DBase::binNumberX( double x ) const
{
  return binner_axisX->axisBinNumber( x );
}

int Bins2DBase::binNumberY( double y ) const
{
  return binner_axisY->axisBinNumber( y );
}

double
Bins2DBase::
binWidth ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y );

  if ( axis == Axes::X )  return binner_axisX->getConstWid();
  // else Y
  return binner_axisY->getConstWid ();
}

double Bins2DBase::binWidthX ( int i ) const
{
  return binner_axisX->axisBinWidth( i );
}

double Bins2DBase::binWidthY ( int i ) const
{
  return binner_axisY->axisBinWidth( i );
}

bool
Bins2DBase::hasEqualWidths () const
{
  return binner_axisX->hasEqualWidths () && binner_axisY->hasEqualWidths ();
}

double
Bins2DBase::scaleFactor () const
{
  if ( binner_axisX->hasEqualWidths () == false ||
       binner_axisY->hasEqualWidths () == false ) {
    return 1.0;
  }

  double width_x = binner_axisX->axisBinWidth ( 1 );
  double width_y = binner_axisY->axisBinWidth ( 1 );

  return width_x * width_y;
}

const Range &
Bins2DBase::
setBinWidth ( hippodraw::Axes::Type axis, double width )
{
  assert ( axis == Axes::X || axis == Axes::Y );
  assert ( width > 0.0 );

  BinnerAxis * binner = 0;
  if ( axis == Axes::X ) binner = binner_axisX;
  else binner = binner_axisY;

  const Range & range = binner->setBinWidth ( width );

  resize ();
  m_values_dirty = true;

  return range;
}

double
Bins2DBase::
calcBinWidth ( Axes::Type axis, int parm,  bool dragging ) const
{
  double new_width = -1.0;

  if ( axis == Axes::X ) {
    new_width = binner_axisX->calcBinWidth ( parm, dragging );
  }
  else { // "Y"
    new_width = binner_axisY->calcBinWidth ( parm, dragging );
  }

  return new_width;
}

double
Bins2DBase::calcOffset ( const std::string & axis,
			 int parm,
			 bool dragging ) const
{
  if ( axis == "X" ) {
    return binner_axisX->calcOffset ( parm, dragging );
  }
  //else "Y"
  return binner_axisY->calcOffset ( parm, dragging );
}

double
Bins2DBase::
getOffset ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y );

  if( axis == Axes::X ) return binner_axisX->getOffset();
  // else Y
  return binner_axisY->getOffset();
}

void
Bins2DBase::
setOffset ( hippodraw::Axes::Type axis, double offset )
{
  assert ( axis == Axes::X || axis == Axes::Y );

  BinnerAxis * binner = 0;
  if ( axis == Axes::X ) binner = binner_axisX;
  else binner = binner_axisY;

  binner->setOffset( offset );

  m_values_dirty = true;
}

const Range &
Bins2DBase::
setRange ( hippodraw::Axes::Type axis, const Range & range, bool hold_width )
{
  assert ( axis == Axes::X || axis == Axes::Y );

  BinnerAxis * binner = 0;
  if ( axis == Axes::X ) {
    binner = binner_axisX;
  } else {
    binner = binner_axisY;
  }
  const Range & new_range =  binner->setRange ( range, hold_width );
  resize ();

  return new_range;
}

/* virtual */
const Range &
Bins2DBase::
getRange ( hippodraw::Axes::Type axis )
{
  assert ( axis == Axes::X || axis == Axes::Y );

  BinnerAxis * binner = 0;
  if ( axis == Axes::X ) {
    binner = binner_axisX;
  } else {
    binner = binner_axisY;
  }

  return binner->getRange();
}


/** @todo It is possible that this method will get called before both
    the range and the bin width are final and therefore the number of
    bins could be excessive.  Thus it would be better to postpone the
    resizing until needed. 
*/
void
Bins2DBase::resize ( int nx, int ny )
{
  // The following is necessary in order to prevent a memory leak
  Bins2DBase::setNumberOfBins ( Axes::Y, 0 );

  setNumberOfBins ( Axes::X, nx );
  setNumberOfBins ( Axes::Y, ny );

  m_values_dirty = true;
}

int Bins2DBase::getNumberOfEntries () const
{
  double sum = 0.0;
  for ( unsigned int i = 1; i < m_data.size () -1; i++ ) {
    sum += std::accumulate ( m_data[i].begin()+1, m_data[i].end()-1, 0.0 );
  }

  return static_cast < int > ( sum );
}

int Bins2DBase::getUnderflow () const
{
  return -1;
}

int Bins2DBase::getOverflow () const
{
  return -1;
}


NTuple *
Bins2DBase::
prepareNTuple ( unsigned int rows ) const
{
  unsigned int columns = 6;
  NTuple * ntuple = new NTuple ( columns );
  ntuple -> reserve ( rows );

  // note: using setLabelAt instead of setLables(...) save 10KB of code
  ntuple -> setLabelAt ( "X", 0 );
  ntuple -> setLabelAt ( "Y", 0 );
  ntuple -> setLabelAt ( "Value", 0 );
  ntuple -> setLabelAt ( "Width", 0 );
  ntuple -> setLabelAt ( "Height", 0 );
  ntuple -> setLabelAt ( "Error", 0 );

  return ntuple;
}
