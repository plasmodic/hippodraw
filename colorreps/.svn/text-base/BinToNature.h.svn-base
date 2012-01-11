/* -*- mode: c++ -*- */

/** @file

BinToEosA class interface

Copyright (C) 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToNature.h,v 1.1 2007/01/28 19:04:26 pfkeb Exp $

*/

#ifndef _BinToNature_H_
#define _BinToNature_H_

#include "BinToColorMap.h"

namespace hippodraw {

/** This class implements a value to nature colors

        @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/

  class MDL_HIPPOPLOT_API BinToNature : public BinToColorMap
{
 private:

  static int reds[256];
  static int greens [256];
  static int blues [256];

 public:

  /** The constructor. */
  BinToNature ( const char * );

  /** The copy constructor. */
  BinToNature ( const BinToNature & );

  BinToColor * clone () const;

};

} // namespace hippodraw

#endif // _BinToNature_H_
