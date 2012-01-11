/* -*- mode: c++ -*- */

/** @file

ScatterPlot class interface

Copyright (C)  2000-2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

 $Id: ScatterPlot.h,v 1.46 2005/10/30 00:10:10 pfkeb Exp $
*/

#ifndef _ScatterPlot_H_
#define _ScatterPlot_H_

#include "datareps/DataRep.h"

#include "reps/SymbolPointRep.h"

namespace hippodraw {

/** A DataRep taking two columns and plotting their values as a small
    dot.  An example is shown below.

    @image html scatterplot.png "ScatterPlot data representation"
    @image latex scatterplot.eps "ScatterPlot data representation"

    @author Stephane Bonneaud <gandalf@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API ScatterPlot
  : public hippodraw::DataRep
{

public:

  /** The default constructor. */
  ScatterPlot ( );

  /** The clone function returns an object of its own kind which is a
      copy of this object. */
  DataRep * clone ();

  /** Returns true if @a number is equal to @c 1. */
  virtual bool acceptFunction ( int number );

  virtual bool hasAxis ( hippodraw::Axes::Type ) const;
  
  virtual bool hasZoomY ( ) const;

};

} // namespace hippodraw

#endif // _ScatterPlot_H_
