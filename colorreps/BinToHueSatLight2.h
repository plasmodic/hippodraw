/* -*- mode: c++ -*- */

/** @file

BinToHueSatLight2 class interface

Copyright (C) 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToHueSatLight2.h,v 1.1 2007/01/23 19:24:49 pfkeb Exp $

*/

#ifndef _BinToHueSatLight2_H_
#define _BinToHueSatLight2_H_

#include "BinToColorMap.h"

namespace hippodraw {

/** This class implements a value to pastel colors

    @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/

  class MDL_HIPPOPLOT_API BinToHueSatLight2 : public BinToColorMap
{
 private:

  static int reds[256];
  static int greens [256];
  static int blues [256];

 public:

  /** The constructor. */
  BinToHueSatLight2 ( const char * );

  /** The copy constructor. */
  BinToHueSatLight2 ( const BinToHueSatLight2 & );

  BinToColor * clone () const;

};

} // namespace hippodraw

#endif // _BinToHueSatLight2_H_
