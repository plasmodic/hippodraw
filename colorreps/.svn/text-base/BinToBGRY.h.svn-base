/* -*- mode: c++ -*- */

/** @file

BinToBGRY class interface

Copyright (C) 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToBGRY.h,v 1.1 2007/01/22 18:59:01 pfkeb Exp $

*/

#ifndef _BinToBGRY_H_
#define _BinToBGRY_H_

#include "BinToColorMap.h"

namespace hippodraw {

/** This class implements a value to Prism color transform.

    @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/

  class MDL_HIPPOPLOT_API BinToBGRY : public BinToColorMap
{
 private:

  static int reds[256];
  static int greens [256];
  static int blues [256];

 public:

  /** The constructor. */
  BinToBGRY ( const char * );

  /** The copy constructor. */
  BinToBGRY ( const BinToBGRY & );

  BinToColor * clone () const;

};

} // namespace hippodraw

#endif // _BinToBGRY_H_
