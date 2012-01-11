/* -*- mode: c++ -*- */

/** @file

BinToBlueWhite class interface

Copyright (C) 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToGRBW.h,v 1.1 2007/01/21 23:36:57 pfkeb Exp $

*/

#ifndef _BinToGRBW_H_
#define _BinToGRBW_H_

#include "BinToColorMap.h"

namespace hippodraw {

/** This class implements a value to Prism color transform.

    @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/

  class MDL_HIPPOPLOT_API BinToGRBW : public BinToColorMap
{
 private:

  static int reds[256];
  static int greens [256];
  static int blues [256];

 public:

  /** The constructor. */
  BinToGRBW ( const char * );

  /** The copy constructor. */
  BinToGRBW ( const BinToGRBW & );

  BinToColor * clone () const;

};

} // namespace hippodraw

#endif // _BinToGRBW_H_
