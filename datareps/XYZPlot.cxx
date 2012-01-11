/** @file

XYZPlot class implementation

Copyright (C) 2000-2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: XYZPlot.cxx,v 1.26 2005/05/16 18:39:19 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "XYZPlot.h"

#include "projectors/Map3Projector.h"
#include "reps/ColorSymbolPointRep.h"

#include <stdexcept>

using namespace hippodraw;

XYZPlot::XYZPlot ()
  : DataRep ()
{
  m_name = "XYZ Plot";

  m_projector = new Map3Projector();
  m_rep = new ColorSymbolPointRep ( Symbol::SOLIDSQUARE, 4.0 );
}

DataRep * XYZPlot::clone ()
{
  return new XYZPlot ( *this );
}

bool
XYZPlot::
hasAxis ( hippodraw::Axes::Type axis ) const
{
  return axis == Axes::X || axis == Axes::Y || axis ==Axes::Z;
}


void XYZPlot::setRepresentation( RepBase * pointrep ) {
   ColorSymbolPointRep * rep = dynamic_cast<ColorSymbolPointRep *>(pointrep);
   if (rep == 0) {
      std::string what("Only ColorSymbol pointreps are allowed ");
      what += "for this datarep.";
      throw std::runtime_error(what);
   }
}
