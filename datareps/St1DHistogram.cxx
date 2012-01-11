/** @file

St1DHistogram class implementation.

Copyright (C) 2000-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: St1DHistogram.cxx,v 1.80 2005/10/27 23:40:58 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "St1DHistogram.h"

#include "projectors/StHist1DProjector.h"
#include "reps/ColumnPointRep.h"

#include <cassert>

using namespace hippodraw;

St1DHistogram::St1DHistogram ( )
{
  m_name = "Static Histogram";

  m_projector = new StHist1DProjector();
  m_rep = new ColumnPointRep();
}

DataRep * St1DHistogram::clone ()
{
  return new St1DHistogram ( *this );
}

bool St1DHistogram::acceptFunction ( int num )
{
  return num == 1;
}

bool
St1DHistogram::
hasErrorDisplay () const
{
  return true;
}

bool
St1DHistogram::
hasNTupleBindings ( ) const
{
  return false;
}

void
St1DHistogram::
setBinContents ( const DataSource * source )
{
  BinningProjector * projector 
    = dynamic_cast < BinningProjector * > ( m_projector );
  assert ( projector != 0 );

  projector -> setBinContents ( source );
}

using namespace hippodraw;

bool
St1DHistogram::
hasAxis ( hippodraw::Axes::Type axis ) const
{
  return axis == Axes::X || axis == Axes::Y;
}

