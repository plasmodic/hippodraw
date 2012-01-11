/* -*- mode: c++ -*- */

/** @file

BinToBWLinear class interface

Copyright (C) 2000, 2003, 2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToBWLinear.h,v 1.1 2007/01/21 21:10:54 pfkeb Exp $

*/

#ifndef _BinToBWLinear_H_
#define _BinToBWLinear_H_

#include "BinToColorMap.h"

namespace hippodraw {

/** The class derived from the BinToColor class. The BinToColor class
    implements the transformation of a value to a color which belongs
    to a grey scale (in contrast to the BinToColorScale class,
    which has a color scale).  

    @author Stephane Bonneaud <gandalf@slac.stanford.edu> 
	@author Tao Xu <taox@stanford.edu>
*/

class MDL_HIPPOPLOT_API BinToBWLinear : public BinToColorMap
{
 private:

 public:

  /** The constructor. */
  BinToBWLinear ( const char * );

  /** The copy constructor. */
  BinToBWLinear ( const BinToBWLinear & );

  BinToColor * clone () const;

};

} // namespace hippodraw

#endif // _BinToBWLinear_H_
