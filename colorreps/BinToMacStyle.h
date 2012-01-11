/* -*- mode: c++ -*- */

/** @file

BinToMacStyle class interface

Copyright (C) 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToMacStyle.h,v 1.1 2007/01/28 17:31:04 pfkeb Exp $

*/

#ifndef _BinToMacStyle_H_
#define _BinToMacStyle_H_

#include "BinToColorMap.h"

namespace hippodraw {

/** This class implements a value to pastel colors

    @bug @@@@  This color map doesn't look the same as in IDL.
    @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/

  class MDL_HIPPOPLOT_API BinToMacStyle : public BinToColorMap
{
 private:

  static int reds[256];
  static int greens [256];
  static int blues [256];

 public:

  /** The constructor. */
  BinToMacStyle ( const char * );

  /** The copy constructor. */
  BinToMacStyle ( const BinToMacStyle & );

  BinToColor * clone () const;

};

} // namespace hippodraw

#endif // _BinToMacStyle_H_
