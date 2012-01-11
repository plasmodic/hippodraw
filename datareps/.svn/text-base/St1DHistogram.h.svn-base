/* -*- mode: c++ -*- */

/** @file

St1DHistogram class interface.

Copyright (C) 2001-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: St1DHistogram.h,v 1.56 2005/10/30 00:10:10 pfkeb Exp $

*/

#ifndef _St1DHistogram_H_
#define _St1DHistogram_H_

#include "datareps/DataRep.h"

namespace hippodraw {

/** A derived class of DataRep that creates a one dimensional static
    histogram display.  That is, one that has a fill function and the
    binning parameters are fixed.  No binning can be redone.
    
    @todo Decide how this class helps or is the implementation of AIDA
    Histogram1D.
 */
class MDL_HIPPOPLOT_API St1DHistogram
  : public hippodraw::DataRep
{
 public:

  /** The default constructor.  Creates a display with histogram
      components.*/
  St1DHistogram ( );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual DataRep  * clone ();

  virtual bool acceptFunction ( int num );

  /** Returns @c true.  Returns @c true because both PointRepBase
      classes used by this class are capable of displaying error on the
      data points. 
  */
  virtual bool hasErrorDisplay () const;

  /** Returns false. 
   */
  virtual bool hasNTupleBindings () const;

  /** Sets the contents of the binner from the data source.  Sets the
      contents of the binner from the data source.  The @a source
      object should have the form described by
      hippodraw::DataPoint2DTuple.
   */
  void setBinContents ( const DataSource * source );

  virtual bool hasAxis ( hippodraw::Axes::Type ) const;

};

} // namespace hippodraw

#endif // _St1DHistogram
