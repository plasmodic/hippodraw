/* -*- mode: c++ -*- */

/** @file

BinToRedTemp class interface

Copyright (C) 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToRedTemp.h,v 1.1 2007/01/21 23:52:44 pfkeb Exp $

*/

#ifndef _BinToRedTemp_H_
#define _BinToRedTemp_H_

#include "BinToColorMap.h"

namespace hippodraw {

/** This class implements a value to Prism color transform.

    @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/

  class MDL_HIPPOPLOT_API BinToRedTemp : public BinToColorMap
{
 private:

  static int reds[256];
  static int greens [256];
  static int blues [256];

 public:

  /** The constructor. */
  BinToRedTemp ( const char * );

  /** The copy constructor. */
  BinToRedTemp ( const BinToRedTemp & );

  BinToColor * clone () const;

};

} // namespace hippodraw

#endif // _BinToRedTemp_H_
