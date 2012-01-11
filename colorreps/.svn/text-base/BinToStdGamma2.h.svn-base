/* -*- mode: c++ -*- */

/** @file

BinToStdGamma2 class interface

Copyright (C) 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToStdGamma2.h,v 1.1 2007/01/22 18:59:01 pfkeb Exp $

*/

#ifndef _BinToStdGamma2_H_
#define _BinToStdGamma2_H_

#include "BinToColorMap.h"

namespace hippodraw {

/** This class implements a value to Prism color transform.

    @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/

  class MDL_HIPPOPLOT_API BinToStdGamma2 : public BinToColorMap
{
 private:

  static int reds[256];
  static int greens [256];
  static int blues [256];

 public:

  /** The constructor. */
  BinToStdGamma2 ( const char * );

  /** The copy constructor. */
  BinToStdGamma2 ( const BinToStdGamma2 & );

  BinToColor * clone () const;

};

} // namespace hippodraw

#endif // _BinToStdGamma2_H_
