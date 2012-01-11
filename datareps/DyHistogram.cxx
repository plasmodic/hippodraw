/** @file

DyHistogram class implementation.

Copyright (C) 2000-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DyHistogram.cxx,v 1.89 2007/07/20 19:29:56 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "DyHistogram.h"

#include "projectors/DyHist1DProjector.h"
#include "reps/ColumnPointRep.h"

// using namespace hippodraw;
namespace hippodraw {

DyHistogram::DyHistogram ( )
  : DataRep ()
{
  m_name = "Histogram";

  m_projector = new DyHist1DProjector();
  m_rep = new ColumnPointRep();
}

DataRep * DyHistogram::clone ()
{
  return new DyHistogram ( *this );
}

bool DyHistogram::acceptFunction ( int num )
{
  return num == 1;
}

bool
DyHistogram::
hasErrorDisplay () const
{
  return true;
}

void
DyHistogram::
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
DyHistogram::
hasAxis ( hippodraw::Axes::Type axis ) const
{
  return axis == Axes::X || axis == Axes::Y;
}

} // namespace hippodraw
