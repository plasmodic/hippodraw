/* -*- mode: c++ -*- */

/** @file

BinToMovingBrkPt class interface

Copyright (C) 2004  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToMovingBrkPt.h,v 1.8 2005/10/30 00:10:17 pfkeb Exp $
 
*/

#ifndef _BinToMovingBrkPt_H_
#define _BinToMovingBrkPt_H_

#include "BinToColor.h"

namespace hippodraw {

/** The class derived from the BinToColor class. The BinToMovingBrkPt class
    implements the transformation of a value to a color which belongs
    to a color scale (in contrast to the BinToGreyScale class,
    which has a gray scale). In contrast to the the BinTo3Color model this 
    class has a variable break point the user varies to improve the color 
    plot 

    @author Joy Rajiv <jrajiv@stanford.edu>
   
    Found on this web page
    http://www.ks.uiuc.edu/Research/vmd/vmd-1.7.1/ug/node76.html

 */
class MDL_HIPPOPLOT_API BinToMovingBrkPt : public BinToColor
{
 public:

  /** The constructor.
   */
  BinToMovingBrkPt ( const char * name );

  /** The copy constructor. */
  BinToMovingBrkPt ( const BinToMovingBrkPt & );

  virtual BinToColor * clone () const;

  virtual void doubleToColor ( double value, Color & color ) const;

  /** Returns @c true.  Returns @c true as this class has control
      points with which the value to color transform can be varied.
  */
  virtual bool hasControlPoints () const;

};

} // namespace hippodraw

#endif // _BinToMovingBrkPt_H_
