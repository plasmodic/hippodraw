/* -*- mode: c++ -*- */

/** @file

BinToEosA class interface

Copyright (C) 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToOcean.h,v 1.1 2007/01/28 19:04:26 pfkeb Exp $

*/

#ifndef _BinToOcean_H_
#define _BinToOcean_H_

#include "BinToColorMap.h"

namespace hippodraw {

/** This class implements a value to nature colors

        @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/

  class MDL_HIPPOPLOT_API BinToOcean : public BinToColorMap
{
 private:

  static int reds[256];
  static int greens [256];
  static int blues [256];

 public:

  /** The constructor. */
  BinToOcean ( const char * );

  /** The copy constructor. */
  BinToOcean ( const BinToOcean & );

  BinToColor * clone () const;

};

} // namespace hippodraw

#endif // _BinToOcean_H_
