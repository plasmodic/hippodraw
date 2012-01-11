/* -*- mode: c++ -*- */

/** @file

BinToEosA class interface

Copyright (C) 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToRainbowIDL.h,v 1.1 2007/02/16 18:46:52 pfkeb Exp $

*/

#ifndef _BinToRainbowIDL_H_
#define _BinToRainbowIDL_H_

#include "BinToColorMap.h"

namespace hippodraw {

/** This class implements a value to nature colors

        @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/

  class MDL_HIPPOPLOT_API BinToRainbowIDL : public BinToColorMap
{
 private:

  static int reds[256];
  static int greens [256];
  static int blues [256];

 public:

  /** The constructor. */
  BinToRainbowIDL ( const char * );

  /** The copy constructor. */
  BinToRainbowIDL ( const BinToRainbowIDL & );

  BinToColor * clone () const;

};

} // namespace hippodraw

#endif // _BinToRainbowIDL_H_
