/* -*- mode: c++ -*- */

/** @file

ContourPlot class interface

Copyright (C) 2000-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: ContourPlot.h,v 1.39 2005/10/27 23:40:57 pfkeb Exp $

*/

#ifndef _ContourPlot_H_
#define _ContourPlot_H_

#include "DataRep.h"

namespace hippodraw {

/** A DataRep for a two dimensional histogram by showing the density
    of any bin with colored contour lines.  

    The ContourPlot uses a projector to manage a two dimension
    histogram.  It represents the values of the histogram bins as
    colored contour lines. It is similar to the ColorPlot and uses the
    same projector class.  It differs in the point representation
    class it uses.  An example is shown below.

    @image html contourplot.png "ContourPlot data representation"
    @image latex contourplot.eps "ContourPlot data representation"

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket B. Malde <sanket@stanford.edu>
*/

class MDL_HIPPOPLOT_API ContourPlot : public DataRep
{

public:

  /** The default constructor. */
  ContourPlot ( );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  DataRep * clone();

  virtual bool hasAxis ( hippodraw::Axes::Type ) const;
  
};

} // namespace hippodraw
#endif // _ContourPlot
