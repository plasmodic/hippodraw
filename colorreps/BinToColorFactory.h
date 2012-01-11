/* -*- mode: c++ -*-  */

/** @file

BinToColorFactory class interface

Copyright (C) 2002-2005  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToColorFactory.h,v 1.4 2005/10/30 00:10:17 pfkeb Exp $

*/

#ifndef _BinToColorFactory_H_
#define _BinToColorFactory_H_

#include "pattern/Factory.h"

namespace hippodraw {

class BinToColor;

/** A factory for creation of objects whose class is derived from BinToColor.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API BinToColorFactory : public Factory < BinToColor >
{

 private:

  /** A pointer to the singleton object. */
  static BinToColorFactory * s_instance;

  /** A private copy constructor to avoid copying. */
  BinToColorFactory ( const BinToColorFactory & );

  /** A default constructor for prevent creation except by itself. */
  BinToColorFactory();

  /** Initializes the factory with a set of default BinToColors. */
  void initialize ();
 
public:

  /** Returns a pointer to the singleton instance. */
  static BinToColorFactory * instance ();

};

} // namespace hippodraw

#endif // _BinToColorFactory_H_
