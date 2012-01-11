/* -*- mode: c++ -*- */

/** @file

BinToColorScale class interface

Copyright (C) 2000, 2003, 2004  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToFullColorScale.h,v 1.2 2005/10/30 00:10:17 pfkeb Exp $
 
*/

#ifndef _BinToFullColorScale_H_
#define _BinToFullColorScale_H_

#include "BinToColor.h"

namespace hippodraw {

/** The class derived from the BinToColor class. The BinToFullColorScale class
    implements the transformation of a value to a color which belongs
    to a color scale (in contrast to the BinToGreyScale class,
    which has a gray scale). The mapping is such that the entire rainbow
    color range can be obtained unlike the BinToColorScale class where a slice
    of the color scale is missing.

    @author Joy Rajiv <jrajiv@stanford.edu>
   

    The member function doubleToColor() was taken from C code writen by
    @author Paul Bourke <pdb@swin.edu.au>

    Found on this web page
    http://astronomy.swin.edu.au/~pbourke/colour/colourramp/

 */
class MDL_HIPPOPLOT_API BinToFullColorScale : public BinToColor
{


 public:

  /** The constructor.
   */
  BinToFullColorScale ( const char * name );

  /** The copy constructor. */
  BinToFullColorScale ( const BinToFullColorScale & );

  virtual BinToColor * clone () const;

  virtual void doubleToColor ( double value, Color & color ) const;

};

} // namespace hippodraw

#endif // _BinToFullColorScale_H_
