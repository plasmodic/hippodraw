/* -*- mode: c++ -*- */

/** @file

BinToGreyScale class interface

Copyright (C) 2000, 2003, 2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToGreyScale.h,v 1.21 2006/09/25 16:19:25 pfkeb Exp $

*/

#ifndef _BinToGreyScale_H_
#define _BinToGreyScale_H_

#include "BinToColor.h"

namespace hippodraw {

/** The class derived from the BinToColor class. The BinToColor class
    implements the transformation of a value to a color which belongs
    to a grey scale (in contrast to the BinToColorScale class,
    which has a color scale).  

    @author Stephane Bonneaud <gandalf@slac.stanford.edu> 
	@author Tao Xu <taox@stanford.edu>
*/

class MDL_HIPPOPLOT_API BinToGreyScale : public BinToColor
{
 public:

  /** The constructor. */
  BinToGreyScale ( const char * );

  /** The copy constructor. */
  BinToGreyScale ( const BinToGreyScale & );

  BinToColor * clone () const;

  virtual void doubleToColor( double value, Color & color ) const;

  virtual bool acceptChangeColor () const;

};

} // namespace hippodraw

#endif // _BinToGreyScale_H_
