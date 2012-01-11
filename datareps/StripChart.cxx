/** @file

StripChart class implementation

Copyright (C) 2000-2005, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: StripChart.cxx,v 1.65 2007/07/26 21:44:34 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "StripChart.h"

#include "projectors/StripChartProjector.h"
#include "reps/LinePointRep.h"

using namespace hippodraw;

StripChart::StripChart ( )
{
  m_name = "Strip Chart";

  m_projector = new StripChartProjector();
  m_rep = new LinePointRep();
}

DataRep * StripChart::clone ()
{
  return new StripChart ( *this );
}

bool StripChart::acceptFunction ( int num )
{
  return num == 1;
}

using namespace hippodraw;

bool
StripChart::
hasAxis ( hippodraw::Axes::Type axis ) const
{
  return axis == Axes::X || axis == Axes::Y;
}

bool
StripChart::
hasZoomY () const
{
  return true;
}
