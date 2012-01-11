/* -*- mode: c++ -*- */

/** @file

BinToHueSatValue1 class interface

Copyright (C) 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToHueSatValue1.h,v 1.1 2007/01/23 21:51:50 pfkeb Exp $

*/

#ifndef _BinToHueSatValue1_H_
#define _BinToHueSatValue1_H_

#include "BinToColorMap.h"

namespace hippodraw {

/** This class implements a value to pastel colors

    @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/

  class MDL_HIPPOPLOT_API BinToHueSatValue1 : public BinToColorMap
{
 private:

  static int reds[256];
  static int greens [256];
  static int blues [256];

 public:

  /** The constructor. */
  BinToHueSatValue1 ( const char * );

  /** The copy constructor. */
  BinToHueSatValue1 ( const BinToHueSatValue1 & );

  BinToColor * clone () const;

};

} // namespace hippodraw

#endif // _BinToHueSatValue1_H_
