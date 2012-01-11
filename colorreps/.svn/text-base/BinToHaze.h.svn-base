/* -*- mode: c++ -*- */

/** @file

BinToHaze class interface

Copyright (C) 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToHaze.h,v 1.1 2007/01/22 22:35:05 pfkeb Exp $

*/

#ifndef _BinToHaze_H_
#define _BinToHaze_H_

#include "BinToColorMap.h"

namespace hippodraw {

/** This class implements a value to Prism color transform.

    @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/

  class MDL_HIPPOPLOT_API BinToHaze : public BinToColorMap
{
 private:

  static int reds[256];
  static int greens [256];
  static int blues [256];

 public:

  /** The constructor. */
  BinToHaze ( const char * );

  /** The copy constructor. */
  BinToHaze ( const BinToHaze & );

  BinToColor * clone () const;

};

} // namespace hippodraw

#endif // _BinToHaze_H_
