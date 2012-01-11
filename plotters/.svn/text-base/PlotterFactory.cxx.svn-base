/** @file

PlotterFactory class implementation

Copyright (C) 2002-2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: PlotterFactory.cxx,v 1.12 2005/10/30 00:10:13 pfkeb Exp $
*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "PlotterFactory.h"

// A list of default Plotters.
#include "Cut1DPlotter.h"
#include "Cut2DPlotter.h"
#include "TextPlotter.h"

namespace hippodraw {

PlotterFactory * PlotterFactory::s_instance = 0;

PlotterFactory::PlotterFactory()
{
}

PlotterFactory * PlotterFactory::instance ()
{
  if ( s_instance == 0 ) {
    s_instance = new PlotterFactory ( );
    s_instance->initialize ();
  }
  return s_instance;
}

void PlotterFactory::initialize ()
{
  add ( new Cut1DPlotter () );
  add ( new Cut2DPlotter () );
  add ( new XyPlotter () );
  add ( new TextPlotter () );
}

} // namespace hippodraw
