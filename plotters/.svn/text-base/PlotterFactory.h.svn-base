/* -*- mode: c++ -*-
 *
 * HippoPlot PlotterFactory class interface
 *
 * Copyright (C)  2002, 2003  The Board of Trustees of The Leland Stanford
 * Junior University.  All Rights Reserved.
 *
 * $Id: PlotterFactory.h,v 1.4 2005/10/30 00:10:13 pfkeb Exp $
 *
 */

#ifndef _PlotterFactory_H_
#define _PlotterFactory_H_

#include "pattern/Factory.h"

namespace hippodraw {

class PlotterBase;

/** This Singleton class maintains a list of plotters.  They can be
    looked up by their advertised name.  New plotters are created by
    cloning existing ones. */
class MDL_HIPPOPLOT_API PlotterFactory : public Factory < PlotterBase >
{

 private:

  /** The pointer to the singleton object. */
  static PlotterFactory * s_instance;

  /** A private copy constructor to avoid copying. */
  PlotterFactory ( const PlotterFactory & );

  /** The default constructor for prevent creation except by
      itself. */
  PlotterFactory ( );

  /** Initializes the factory with a set of default Plotters. */
  void initialize ();

public:

  /** Returns the pointer to the singleton instance. */
  static PlotterFactory * instance ();

};

} // namespace hippodraw

#endif // _PlotterFactory_H_
