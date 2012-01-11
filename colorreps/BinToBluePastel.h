/* -*- mode: c++ -*- */

/** @file

BinToBluePastel class interface

Copyright (C) 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToBluePastel.h,v 1.1 2007/01/22 23:50:45 pfkeb Exp $

*/

#ifndef _BinToBluePastel_H_
#define _BinToBluePastel_H_

#include "BinToColorMap.h"

namespace hippodraw {

/** This class implements a value to Prism color transform.

    @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/

  class MDL_HIPPOPLOT_API BinToBluePastel : public BinToColorMap
{
 private:

  static int reds[256];
  static int greens [256];
  static int blues [256];

 public:

  /** The constructor. */
  BinToBluePastel ( const char * );

  /** The copy constructor. */
  BinToBluePastel ( const BinToBluePastel & );

  BinToColor * clone () const;

};

} // namespace hippodraw

#endif // _BinToBluePastel_H_
