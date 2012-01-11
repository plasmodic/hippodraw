/* -*- mode: c++ -*-
 *
 * HippoPlot BinsFactory class interface
 *
 * Copyright (C) 2003   The Board of Trustees of The Leland Stanford
 * Junior University.  All Rights Reserved.
 *
 * $Id: BinsFactory.h,v 1.2 2005/10/30 00:10:10 pfkeb Exp $
 *
 */

#ifndef _BinsFactory_H_
#define _BinsFactory_H_

#include "pattern/Factory.h"

namespace hippodraw {

class BinsBase;

/** A Factory singleton class for creating objects whose class derives
    from BinsBase.  These classes are used by BinningProjector objects
    to accumulate data into bins.  They differ in how they create @ref
    overview_projectedvalue objects.  Some may take the accumulated
    total as the value while other may take the mean of the
    accumulation.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
 */

class MDL_HIPPOPLOT_API BinsFactory : public Factory <BinsBase>
{

 private:

  /** A pointer to the singleton object. */
  static BinsFactory * s_instance;

  /** A private copy constructor to prevent copying. */
  BinsFactory ( const BinsFactory & );

  /** Initializes the factory with a set of default functions. */
  void initialize ();

protected:

  /** A default constructor for prevent creation except by the
      itself or the derived classes. */
  BinsFactory();

 public:

  /** Returns a pointer to the singleton instance. */
  static BinsFactory * instance ();

};

} // namespace hippodraw

#endif // _BinsFactory_H_
