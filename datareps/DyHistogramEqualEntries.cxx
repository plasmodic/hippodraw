/** @file

DyHistogramEqualEntries class implementation.

Copyright (C) 2000-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DyHistogramEqualEntries.cxx,v 1.2 2007/07/20 19:29:56 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "DyHistogramEqualEntries.h"

#include "projectors/EqualEntriesHist1DProjector.h"
#include "reps/ColumnPointRep.h"

// using namespace hippodraw;
namespace hippodraw {

DyHistogramEqualEntries::DyHistogramEqualEntries ( )
  : DataRep ()
{
  m_name = "Histogram (EqualEntries)";

  m_projector = new EqualEntriesHist1DProjector( );
  m_rep = new ColumnPointRep();
}

DataRep * DyHistogramEqualEntries::clone ()
{
  return new DyHistogramEqualEntries ( *this );
}

bool DyHistogramEqualEntries::acceptFunction ( int num )
{
  return num == 1;
}

bool
DyHistogramEqualEntries::
hasErrorDisplay () const
{
  return true;
}

void
DyHistogramEqualEntries::
setPointRep ( RepBase * rep )
{
  DataRep::setPointRep ( rep );
  if ( rep -> name () == "Symbol" ) {
    rep -> setSize ( 4. );
    setErrorDisplay ( Axes::Y, true );
    notifyObservers ();
  }
}

bool
DyHistogramEqualEntries::
hasAxis ( hippodraw::Axes::Type axis ) const
{
  return axis == Axes::X || axis == Axes::Y;
}

} // namespace hippodraw
