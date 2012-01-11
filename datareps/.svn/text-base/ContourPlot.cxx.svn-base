/** @file

hippodraw::ContourPlot class implementation

Copyright (C) 2003-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: ContourPlot.cxx,v 1.34 2006/02/12 20:51:34 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "ContourPlot.h"

#include "projectors/DyHist2DProjector.h"
#include "reps/ContourPointRep.h"

using namespace hippodraw;

ContourPlot::ContourPlot ( )
  : DataRep ()
{
  m_name = "Contour Plot";
  m_projector = new DyHist2DProjector();
  m_rep = new ContourPointRep();
}

DataRep * ContourPlot::clone ()
{
  return new ContourPlot( *this );
}

bool
ContourPlot::
hasAxis ( hippodraw::Axes::Type axis ) const
{
  return axis == Axes::X || axis == Axes::Y || axis ==Axes::Z;
}
