/** @file

YPlot class implementation

Copyright (C) 2001-2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: YPlot.cxx,v 1.27 2005/10/30 00:10:11 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "YPlot.h"

#include "projectors/Map1Projector.h"
#include "reps/SymbolPointRep.h"

namespace hippodraw {

YPlot::YPlot ( )
{
  m_name = "Y Plot";

  Map1Projector * projector = new Map1Projector();
//   projector -> setYErrorOption ( true );

  m_projector = projector;
  m_rep = new SymbolPointRep ( Symbol::SOLIDSQUARE, 4.0 );
}

DataRep * YPlot::clone ()
{
  return new YPlot ( *this );
}

bool YPlot::acceptFunction ( int num )
{
  return num == 1;
}

bool
YPlot::
hasAxis ( hippodraw::Axes::Type axis ) const
{
  return axis == Axes::X || axis == Axes::Y;
}

} // namespace hippodraw

