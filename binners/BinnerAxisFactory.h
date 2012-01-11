/* -*- mode: c++ -*-
 *
 * HippoPlot BinnerAxisFactory class interface
 *
 * Copyright (C) 2003   The Board of Trustees of The Leland Stanford
 * Junior University.  All Rights Reserved.
 *
 * $Id: BinnerAxisFactory.h,v 1.5 2005/10/30 00:10:09 pfkeb Exp $
 *
 */

#ifndef _BinnerAxisFactory_H_
#define _BinnerAxisFactory_H_

#include "pattern/Factory.h"

namespace hippodraw {

class BinnerAxis;

/** A Factory singleton class for creating objects whose class derives
    from BinnerAxis.  These classes are used by BinsBase objects
    to calculate a bin index.  They differ in how the bin edges are
    determined.  An example would be equal width bins which would be
    used by a simple histogram.  However, the bin sizes do not have to
    be equal width.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
 */

class MDL_HIPPOPLOT_API BinnerAxisFactory : public Factory < BinnerAxis >
{

 private:

  /** A pointer to the singleton object. */
  static BinnerAxisFactory * s_instance;

  /** A private copy constructor to prevent copying. */
  BinnerAxisFactory ( const BinnerAxisFactory & );

  /** Initializes the factory with a set of default functions. */
  void initialize ();

protected:

  /** A default constructor for prevent creation except by the
      itself or the derived classes. */
  BinnerAxisFactory();

 public:

  /** Returns a pointer to the singleton instance. */
  static BinnerAxisFactory * instance ();

};

} // namespace hippodraw

#endif // _BinnerAxisFactory_H_
