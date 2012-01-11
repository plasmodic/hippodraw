/** @file

hippodraw::PointRepFactory class implementation

Copyright (C) 2002-2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: PointRepFactory.cxx,v 1.13 2006/02/04 17:27:26 pfkeb Exp $

*/

#ifdef _MSC_VER
// A long identifier warning.
#include "msdevstudio/MSconfig.h"
#endif

#include "reps/PointRepFactory.h"
#include "datasrcs/TupleCut.h"

#include "ColorBoxPointRep.h"
#include "FilledColumnPointRep.h"
#include "ContourPointRep.h"
#include "ColorSymbolPointRep.h"
#include "CutRangeRep.h"
#include "LineFunctionRep.h"

namespace hippodraw {

PointRepFactory * PointRepFactory::s_instance = 0;

PointRepFactory::PointRepFactory ()
{
}

PointRepFactory * PointRepFactory::instance ()
{
  if ( s_instance == 0 ) {
    s_instance = new PointRepFactory ();
    s_instance->initialize ();
  }
  return s_instance;
}

void PointRepFactory::initialize ()
{
  add ( new ColorBoxPointRep () );
  add ( new ContourPointRep () );
  add ( new ColumnPointRep () );
  add ( new FilledColumnPointRep () );
  add ( new LinePointRep () );
  add ( new SymbolPointRep () );
  add ( new ColorSymbolPointRep () );
  add ( new CutRangeRep () );
  add ( new LineFunctionRep () );
}

} // namespace hippodraw
