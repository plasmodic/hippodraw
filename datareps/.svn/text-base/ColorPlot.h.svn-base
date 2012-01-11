/* -*- mode: c++ -*- */

/** @file

ColorPlot class interface

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: ColorPlot.h,v 1.56 2006/04/13 23:39:36 pfkeb Exp $

*/

#ifndef _ColorPlot_H_
#define _ColorPlot_H_

#include "DataRep.h"

namespace hippodraw {
/** A DataRep for a two dimensional histogram by showing the density
    of any bin with colored squares.  

    The ColorPlot uses a projector to manage a two dimensional
    histogram.  It represents the values of bins in the histogram as a
    colored box. It is similar to the ContourPlot and uses the same
    projector class.  It differs in the point representation class
    that it uses.  An example is shown below.
    
    @image html colorplot.png "ColorPlot data representation"
    @image latex colorplot.eps "ColorPlot data representation"

    @author Stephane Bonneaud <gandalf@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/
class MDL_HIPPOPLOT_API ColorPlot : public DataRep
{

public:

  /** The default constructor. */
  ColorPlot ( );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  DataRep * clone();

  virtual bool hasAxis ( hippodraw::Axes::Type ) const;

};

} // namespace hippodraw
#endif // _ColorPlot
