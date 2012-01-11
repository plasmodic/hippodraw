/* -*- mode: c++ -*- */

/** @file

St2DHistogram class interface.

Copyright (C) 2003-2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

 $Id: St2DHistogram.h,v 1.53 2005/10/30 00:10:10 pfkeb Exp $

*/

#ifndef _St2DHistogram_H_
#define _St2DHistogram_H_

#include "datareps/DataRep.h"

namespace hippodraw {

/** A derived class of DataRep that creates a two dimensional static
    histogram display.  That is, one that has a addValues function and the
    binning parameters are fixed.  No binning can be redone.
    
    @bug This class is not save and restored to document file.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

 */
class MDL_HIPPOPLOT_API St2DHistogram
  : public hippodraw::DataRep
{
 public:

  /** The default constructor.  Creates a display with histogram
      components.*/
  St2DHistogram ( );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual DataRep  * clone ();

  virtual bool acceptFunction ( int num );

  /** Returns false. 
   */
  virtual bool hasNTupleBindings () const;

  virtual bool hasAxis ( hippodraw::Axes::Type ) const;


  /** Sets the contents of the binner from the data source.  Sets the
      contents of the binner from the data source.  The @a source
      object should have the form described by
      hippodraw::DataPoint3DTuple.
   */
  void setBinContents ( const DataSource * source );

};

} // namespace hippodraw

#endif // _St2DHistogram
