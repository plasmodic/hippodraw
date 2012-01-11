/* -*- mode: c++ -*- */

/** @file

YPlot class interface.

Copyright (C)  2000-2002, 2004, 2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: YPlot.h,v 1.23 2005/10/30 00:10:11 pfkeb Exp $

*/

#ifndef _YPlot_H_
#define _YPlot_H_

#include "datareps/DataRep.h"

namespace hippodraw {

/** Displays on the Y axis the values of a single column using the row
    number as the coordinate of the X axis.   An example is shown below

    @image html yplot.png "YPlot data representation"
    @image latex yplot.eps "YPlot data representation"

    @todo Fix a bug in reading a document as the point size does not 
          get restored.

    @todo Allow one to use another column as the error on a value.
    Don't forget to implement hasErrorDisplay() after doing so.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket B. Malde <sanket@stanford.edu>
*/

class MDL_HIPPOPLOT_API YPlot
  : public hippodraw::DataRep
{
 public:

  /** The default constructor. */
  YPlot ( );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual DataRep * clone();

  bool acceptFunction ( int num );

  virtual bool hasAxis ( hippodraw::Axes::Type ) const;

};

} // namespace hippodraw

#endif // _YPlot
