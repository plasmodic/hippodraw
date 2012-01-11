/* -*- mode: c++ -*- */

/** @file

BinToBlueRed class interface

Copyright (C) 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToBlueRed.h,v 1.1 2007/01/22 22:35:05 pfkeb Exp $

*/

#ifndef _BinToBlueRed_H_
#define _BinToBlueRed_H_

#include "BinToColorMap.h"

namespace hippodraw {

/** This class implements a value to Prism color transform.

    @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/

  class MDL_HIPPOPLOT_API BinToBlueRed : public BinToColorMap
{
 private:

  static int reds[256];
  static int greens [256];
  static int blues [256];

 public:

  /** The constructor. */
  BinToBlueRed ( const char * );

  /** The copy constructor. */
  BinToBlueRed ( const BinToBlueRed & );

  BinToColor * clone () const;

};

} // namespace hippodraw

#endif // _BinToBlueRed_H_
