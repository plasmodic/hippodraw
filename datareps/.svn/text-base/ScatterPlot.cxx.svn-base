/** @file

ScatterPlot class implementation

Copyright (C) 2000-2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: ScatterPlot.cxx,v 1.68 2005/10/30 00:10:10 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "ScatterPlot.h"

#include "projectors/Map2Projector.h"

namespace hippodraw {

ScatterPlot::ScatterPlot ( )
{
  m_name = "Scatter Plot";

  m_projector = new Map2Projector();
  m_rep = new SymbolPointRep ( Symbol::SOLIDSQUARE, 2.0 );
}

DataRep * ScatterPlot::clone ()
{
  return new ScatterPlot( *this );
}

bool ScatterPlot::acceptFunction ( int num )
{
  return num == 1;
}

bool
ScatterPlot::
hasAxis ( hippodraw::Axes::Type axis ) const
{
  return axis == Axes::X || axis == Axes::Y;
}

bool
ScatterPlot::
hasZoomY ( ) const
{
  return true;
}

} // namespace hippodraw

