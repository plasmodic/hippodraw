/* -*- mode: c++ -*- */

/** @file

BinToEosA class interface

Copyright (C) 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToEosA.h,v 1.1 2007/01/28 17:31:04 pfkeb Exp $

*/

#ifndef _BinToEosA_H_
#define _BinToEosA_H_

#include "BinToColorMap.h"

namespace hippodraw {

/** This class implements a value to pastel colors

        @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/

  class MDL_HIPPOPLOT_API BinToEosA : public BinToColorMap
{
 private:

  static int reds[256];
  static int greens [256];
  static int blues [256];

 public:

  /** The constructor. */
  BinToEosA ( const char * );

  /** The copy constructor. */
  BinToEosA ( const BinToEosA & );

  BinToColor * clone () const;

};

} // namespace hippodraw

#endif // _BinToEosA_H_
