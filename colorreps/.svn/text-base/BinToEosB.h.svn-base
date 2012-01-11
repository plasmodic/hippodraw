/* -*- mode: c++ -*- */

/** @file

BinToEosA class interface

Copyright (C) 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToEosB.h,v 1.1 2007/01/28 19:04:26 pfkeb Exp $

*/

#ifndef _BinToEosB_H_
#define _BinToEosB_H_

#include "BinToColorMap.h"

namespace hippodraw {

/** This class implements a value to pastel colors

        @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/

  class MDL_HIPPOPLOT_API BinToEosB : public BinToColorMap
{
 private:

  static int reds[256];
  static int greens [256];
  static int blues [256];

 public:

  /** The constructor. */
  BinToEosB ( const char * );

  /** The copy constructor. */
  BinToEosB ( const BinToEosB & );

  BinToColor * clone () const;

};

} // namespace hippodraw

#endif // _BinToEosB_H_
