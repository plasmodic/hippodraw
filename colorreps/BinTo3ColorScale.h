/* -*- mode: c++ -*- */

/** @file

BinTo3ColorScale class interface

Copyright (C) 2000, 2003, 2004  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinTo3ColorScale.h,v 1.3 2005/10/30 00:10:17 pfkeb Exp $
 
*/

#ifndef _BinTo3ColorScale_H_
#define _BinTo3ColorScale_H_

#include "BinToColor.h"

namespace hippodraw {

/** The class derived from the BinToColor class. The BinTo3ColorScale class
    implements the transformation of a value to a color which belongs
    to a color scale (in contrast to the BinToGreyScale class,
    which has a gray scale).  

    @author Joy Rajiv <jrajiv@stanford.edu>
   
    Found on this web page
    http://www.ks.uiuc.edu/Research/vmd/vmd-1.7.1/ug/node76.html

 */
class MDL_HIPPOPLOT_API BinTo3ColorScale : public BinToColor
{
 public:

  /** The constructor.
   */
  BinTo3ColorScale ( const char * name );

  /** The copy constructor. */
  BinTo3ColorScale ( const BinTo3ColorScale & );

  virtual BinToColor * clone () const;

  virtual void doubleToColor ( double value, Color & color ) const;

};

} // namespace hippodraw

#endif // _BinTo3ColorScale_H_
