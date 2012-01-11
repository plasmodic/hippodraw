/** @file

St2DHistogram class implementation

Copyright (C) 2003-2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: St2DHistogram.cxx,v 1.77 2005/10/27 23:40:58 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "St2DHistogram.h"

#include "projectors/StHist2DProjector.h"
#include "reps/ColorBoxPointRep.h"

#include <cassert>

using namespace hippodraw;

St2DHistogram::St2DHistogram ( )
{
  m_name = "Static 2D Histogram";

  m_projector = new StHist2DProjector();
  m_rep = new ColorBoxPointRep ();
}

DataRep * St2DHistogram::clone ()
{
  return new St2DHistogram ( *this );
}

bool St2DHistogram::acceptFunction ( int num )
{
  return num == 2;
}

bool
St2DHistogram::
hasNTupleBindings ( ) const
{
  return false;
}

using namespace hippodraw;

bool
St2DHistogram::
hasAxis ( hippodraw::Axes::Type axis ) const
{
  return axis == Axes::X || axis == Axes::Y || axis ==Axes::Z;
}


void
St2DHistogram::
setBinContents ( const DataSource * source )
{
  BinningProjector * projector 
    = dynamic_cast < BinningProjector * > ( m_projector );
  assert ( projector != 0 );

  projector -> setBinContents ( source );
}
