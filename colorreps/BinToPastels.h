/* -*- mode: c++ -*- */

/** @file

BinToPastels class interface

Copyright (C) 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToPastels.h,v 1.1 2007/01/23 19:24:49 pfkeb Exp $

*/

#ifndef _BinToPastels_H_
#define _BinToPastels_H_

#include "BinToColorMap.h"

namespace hippodraw {

/** This class implements a value to pastel colors

    @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/

  class MDL_HIPPOPLOT_API BinToPastels : public BinToColorMap
{
 private:

  static int reds[256];
  static int greens [256];
  static int blues [256];

 public:

  /** The constructor. */
  BinToPastels ( const char * );

  /** The copy constructor. */
  BinToPastels ( const BinToPastels & );

  BinToColor * clone () const;

};

} // namespace hippodraw

#endif // _BinToPastels_H_
