/* -*- mode: c++ -*- */

/** @file

XYZPlot class interface

Copyright (C) 2000, 2001, 2003-2005   The Board of Trustees of The
Leland Stanford Junior University.  All Rights Reserved.

$Id: XYZPlot.h,v 1.34 2005/10/30 00:10:11 pfkeb Exp $

*/

#ifndef _XYZPlot_H_
#define _XYZPlot_H_

#include "datareps/DataRep.h"

namespace hippodraw {

/** A DataRep that displays a triplet of data from three columns, two
    on the X and Y axis and the third as Color.   An example is shown below

    @image html xyzplot.png "XYZPlot data representation"
    @image latex xyzplot.eps "XYZPlot data representation"

    @author Paul_Kunz@slac.stanford.edu 
    @author Oded Wurman <owurman@stanford.edu>
*/

class MDL_HIPPOPLOT_API XYZPlot
  : public hippodraw::DataRep
{
 public:

  /** The default constructor. */
  XYZPlot ( );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual DataRep * clone();

  virtual bool hasAxis ( hippodraw::Axes::Type ) const;

   /** Only one pointrep is allowed, ColorSymbolPointRep, so this method will
       throw an exception if called with any other type of pointrep.
    */
   virtual void setRepresentation( RepBase * pointrep );

};

} // namespace hippodraw

#endif // _XYZPlot
