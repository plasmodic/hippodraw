/* -*- mode: c++ -*- */

/** @file

BinToStern class interface

Copyright (C) 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToStern.h,v 1.1 2007/01/22 22:35:05 pfkeb Exp $

*/

#ifndef _BinToStern_H_
#define _BinToStern_H_

#include "BinToColorMap.h"

namespace hippodraw {

/** This class implements a value to Prism color transform.

    @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/

  class MDL_HIPPOPLOT_API BinToStern : public BinToColorMap
{
 private:

  static int reds[256];
  static int greens [256];
  static int blues [256];

 public:

  /** The constructor. */
  BinToStern ( const char * );

  /** The copy constructor. */
  BinToStern ( const BinToStern & );

  BinToColor * clone () const;

};

} // namespace hippodraw

#endif // _BinToStern_H_
