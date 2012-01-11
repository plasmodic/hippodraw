/** @file

DataRepFactory class implementation

Copyright (C) 2001-2003, 2005   The Board of Trustees of The Leland
stanford Junior University.  All Rights Reserved.

$Id: DataRepFactory.cxx,v 1.46 2006/09/14 20:45:56 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "DataRepFactory.h"

// A list of default DataReps.
#include "ColorPlot.h"
#include "ContourPlot.h"
#include "Profile2D.h"
#include "ProfileContour.h"
#include "DyHistogram.h"
#include "DyHistogramEqualEntries.h"
#include "Image.h"
#include "LineDataRep.h"
#include "ProfileHist.h"
#include "ScatterPlot.h"
#include "St1DHistogram.h"
#include "St2DHistogram.h"
#include "StripChart.h"
#include "VariableMesh.h"
#include "XYPlot.h"
#include "XYZPlot.h"
#include "YPlot.h"

using namespace hippodraw;

DataRepFactory * DataRepFactory::s_instance = 0;

DataRepFactory::DataRepFactory ( )
{
}

DataRepFactory::
~DataRepFactory ()
{
}

DataRepFactory * DataRepFactory::instance ()
{
  if ( s_instance == 0 ) {
    s_instance = new DataRepFactory ( );
    s_instance->initialize ();
  }
  return s_instance;
}


void DataRepFactory::initialize ()
{
  add ( new ColorPlot () );
  add ( new ContourPlot () );
  add ( new Profile2D () );
  add ( new ProfileContour () );
  add ( new DyHistogram () );
  add ( new DyHistogramEqualEntries () );
  add ( new Image () );
  add ( new LineDataRep () );
  add ( new ProfileHist () );
  add ( new ScatterPlot () );
  add ( new St1DHistogram () );
  add ( new St2DHistogram () );
  add ( new StripChart () );
  add ( new VariableMesh () );
  add ( new XYPlot () );
  add ( new XYZPlot () );
  add ( new YPlot () );
}
