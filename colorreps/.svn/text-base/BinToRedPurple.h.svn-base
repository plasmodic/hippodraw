/* -*- mode: c++ -*- */

/** @file

BinToRedPurple class interface

Copyright (C) 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToRedPurple.h,v 1.1 2007/01/22 19:53:56 pfkeb Exp $

*/

#ifndef _BinToRedPurple_H_
#define _BinToRedPurple_H_

#include "BinToColorMap.h"

namespace hippodraw {

/** This class implements a value to Prism color transform.

    @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/

  class MDL_HIPPOPLOT_API BinToRedPurple : public BinToColorMap
{
 private:

  static int reds[256];
  static int greens [256];
  static int blues [256];

 public:

  /** The constructor. */
  BinToRedPurple ( const char * );

  /** The copy constructor. */
  BinToRedPurple ( const BinToRedPurple & );

  BinToColor * clone () const;

};

} // namespace hippodraw

#endif // _BinToRedPurple_H_
