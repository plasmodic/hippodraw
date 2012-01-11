/* -*- mode: c++ -*- */

/** @file

hippodraw::DyHistogramEqualEntries class interface.

Copyright (C) 2000, 2002-2006  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DyHistogramEqualEntries.h,v 1.2 2007/07/20 19:29:56 pfkeb Exp $

*/

#ifndef _DyHistogramEqualEntries_H_
#define _DyHistogramEqualEntries_H_

#include "DataRep.h"

namespace hippodraw {

/** A derived class of DataRep that creates a one dimensional
    dynamic histogram display.  

    An example of DyHistogramEqualEntries is shown below.
    @image html dyhistogram.png 
    @image latex dyhistogram.eps

    This data representation takes a sequence of numbers and
    histograms it.  The current implementation uses one column of a
    NTuple.  For some @ref overview_pointrep objects, such as
    ColumnPointRep it depends on its @ref overview_projector to return
    the @ref overview_projectedvalue objects in increasing order of X
    value.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket B. Malde <sanket@stanford.edu>
 */
class MDL_HIPPOPLOT_API DyHistogramEqualEntries
  : public hippodraw::DataRep
{
private:

  /** The target DataRep to which this DataRep will normalize
      itself.
  */
  const DataRep * m_target;

public:

  /** The default constructor.  Creates a display with histogram
      components.*/
  DyHistogramEqualEntries ( );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual DataRep  * clone ();

  virtual bool acceptFunction ( int num );

  /** Returns @c true.  Returns @c true because both PointRepBase
      classes used by this class are capable of displaying error on the
      data points. 
  */
  virtual bool hasErrorDisplay () const;

  /** Sets the representation of the data points.  If changing to
      SymbolPointRep, then also set error display on.  

      @sa DataRep::setPointRep.
  */
  virtual void setPointRep ( RepBase * rep );

  virtual bool hasAxis ( hippodraw::Axes::Type ) const;

};

} // namespace hippodraw

#endif // _DyHistogramEqualEntries
