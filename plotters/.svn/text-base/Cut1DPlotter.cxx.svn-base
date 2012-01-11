/** @file

hippodraw::Cut1DPlotter class implementation

Copyright (C) 2000-2005 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

 $Id: Cut1DPlotter.cxx,v 1.33 2007/04/23 21:57:21 pfkeb Exp $

*/

// for truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "Cut1DPlotter.h"

#include "datareps/DataRep.h"
#include "datasrcs/TupleCut.h"

#include "reps/CutRangeRep.h"
#include <cassert>

using std::vector;

using namespace hippodraw;

Cut1DPlotter::Cut1DPlotter ( )
  : CutPlotter ( "Cut1DPlotter" )
{
}

Cut1DPlotter::Cut1DPlotter ( const Cut1DPlotter & plotter )
  : CutPlotter ( plotter )
{
}

Cut1DPlotter::~Cut1DPlotter ()
{
}

PlotterBase * Cut1DPlotter::clone()
{
  return new Cut1DPlotter ( *this );
}

void
Cut1DPlotter::
addTupleCut ( DataRep * rep )
{
  m_datarep=rep;
  m_datarep -> addCut ( TupleCut () );
}
  

void
Cut1DPlotter::
addDataRep ( DataRep * rep )
{
  XyPlotter::addDataRep ( rep );
}

void
Cut1DPlotter::
setAxisBinding ( const std::vector < std::string >  & bindings )
{
  assert ( bindings.size () == 1 );

  CutPlotter::setAxisBinding ( bindings );
}

void Cut1DPlotter::setCutRangeFull ( )
{
  const Range & range = getRange ( Axes::X, false );
  m_datarep -> setCutRangeAt ( range, 0 );
  
  updateTargets ();
}

void
Cut1DPlotter::
toggleInverted ()
{
  m_datarep -> toggleInverted ( 0 );

  updateTargets ();
}

const std::string &
Cut1DPlotter::
getCutLabel () const
{
  const vector < TupleCut > & cuts = m_datarep -> getCuts ();
  const TupleCut & cut = cuts[0]; // so Doxygen sees it.

  return cut.getLabel ();
}
