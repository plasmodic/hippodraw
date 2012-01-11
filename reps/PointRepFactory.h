/* -*- mode: c++ -*-  */

/** @file

PointRepFactory class interface

Copyright (C) 2002-2004  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: PointRepFactory.h,v 1.6 2005/10/30 00:10:18 pfkeb Exp $

*/

#ifndef _PointRepFactory_H_
#define _PointRepFactory_H_

#include "pattern/Factory.h"

namespace hippodraw {

class RepBase;

/** A factory for creation of objects whose class is derived from PointRepBase.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API PointRepFactory : public Factory < RepBase >
{

 private:

  /** A pointer to the singleton object. */
  static PointRepFactory * s_instance;

  /** A private copy constructor to avoid copying. */
  PointRepFactory ( const PointRepFactory & );

  /** A default constructor for prevent creation except by itself. */
  PointRepFactory();

  /** Initializes the factory with a set of default PointReps. */
  void initialize ();


public:

  /** Returns a pointer to the singleton instance. */
  static PointRepFactory * instance ();

};

} // namespace hippodraw

#endif // _PointRepFactory_H_
