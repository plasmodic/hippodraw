/** @file

hippodraw::ColorPlot class implementation

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: ColorPlot.cxx,v 1.85 2006/08/23 18:14:11 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "ColorPlot.h"

#include "projectors/DyHist2DProjector.h"
#include "reps/ColorBoxPointRep.h"


using namespace hippodraw;

ColorPlot::ColorPlot ( )
  : DataRep ()
{
  m_name = "Color Plot";
  m_projector = new DyHist2DProjector ();
  m_rep = new ColorBoxPointRep ();
}

DataRep * ColorPlot::clone ()
{
  return new ColorPlot( *this );
}

bool
ColorPlot::
hasAxis ( Axes::Type axis ) const
{
  return axis == Axes::X || axis == Axes::Y || axis ==Axes::Z;
}
