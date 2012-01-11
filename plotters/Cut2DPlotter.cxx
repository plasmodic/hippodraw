/** @file

hippodraw::Cut2DPlotter class implementation

Copyright (C) 2000-2005, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

 $Id: Cut2DPlotter.cxx,v 1.33 2007/04/23 21:57:21 pfkeb Exp $

*/

// for truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "Cut2DPlotter.h"

#include "datareps/DataRep.h"
#include "datasrcs/TupleCut.h"
#include "reps/CutRangeRep.h"

#include <cassert>

using std::vector;

namespace hippodraw {

Cut2DPlotter::
Cut2DPlotter ( )
  : CutPlotter ( "Cut2DPlotter" )
{
  setEnableZ ( true );
}

Cut2DPlotter::Cut2DPlotter ( const Cut2DPlotter & plotter )
  : CutPlotter ( plotter )
{
  setEnableZ ( true );
}

Cut2DPlotter::~Cut2DPlotter ()
{
}

PlotterBase * Cut2DPlotter::clone()
{
  return new Cut2DPlotter ( *this );
}

void
Cut2DPlotter::
addTupleCut ( DataRep * rep )
{
  m_datarep = rep;
  m_datarep -> addCut ( TupleCut () );
  m_datarep -> addCut ( TupleCut () );
}


void
Cut2DPlotter::
addDataRep ( DataRep * rep )
{
  XyPlotter::addDataRep ( rep );
}

void
Cut2DPlotter::
setAxisBinding ( const std::vector < std::string > & bindings )
{
  assert ( bindings.size () == 2 );

  CutPlotter::setAxisBinding ( bindings );
}

void Cut2DPlotter::setCutRangeFull ( )
{
  const Range & rangeX = getRange ( Axes::X, false );
  m_datarep -> setCutRangeAt (rangeX, 0 );

  const Range & rangeY = getRange ( Axes::Y, false );
  m_datarep -> setCutRangeAt ( rangeY, 1 );
  
  updateTargets ();
}

void Cut2DPlotter::toggleInverted ( )
{
  m_datarep -> toggleInverted ( 0 );
  m_datarep -> toggleInverted ( 1 );

  updateTargets ();
}

Range Cut2DPlotter::getCutRangeY ( )
{
  TupleCut * cut = getCutAt ( 1 );
  Range range = cut -> getRange ();

  return range;
}

} // namespace hippodraw
