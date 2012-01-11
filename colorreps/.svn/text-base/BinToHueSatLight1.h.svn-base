/* -*- mode: c++ -*- */

/** @file

BinToHueSatLight1 class interface

Copyright (C) 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToHueSatLight1.h,v 1.1 2007/01/23 19:24:49 pfkeb Exp $

*/

#ifndef _BinToHueSatLight1_H_
#define _BinToHueSatLight1_H_

#include "BinToColorMap.h"

namespace hippodraw {

/** This class implements a value to pastel colors

    @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/

  class MDL_HIPPOPLOT_API BinToHueSatLight1 : public BinToColorMap
{
 private:

  static int reds[256];
  static int greens [256];
  static int blues [256];

 public:

  /** The constructor. */
  BinToHueSatLight1 ( const char * );

  /** The copy constructor. */
  BinToHueSatLight1 ( const BinToHueSatLight1 & );

  BinToColor * clone () const;

};

} // namespace hippodraw

#endif // _BinToHueSatLight1_H_
