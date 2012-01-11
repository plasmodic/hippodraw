/* -*- mode: c++ -*- */

/** @file

hippodraw::BinToColorScaleMovingBrkPt class interface

Copyright (C) 2004, 2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinToColorScaleMovingBrkPt.h,v 1.14 2006/08/20 16:12:27 pfkeb Exp $
 
*/

#ifndef _BinToColorScaleMovingBrkPt_H_
#define _BinToColorScaleMovingBrkPt_H_

#include "BinToColor.h"

using std::vector;

namespace hippodraw {

/** The class derived from the BinToColor class. This class implements
    the transformation of a value to a color like the classic rainbow,
    but  has  two  control  parameters  to vary  the  transform.   One
    parameter is the  distance between the break points  and the other
    is the position of the center between the break points.

    @author Joy Rajiv <jrajiv@stanford.edu>

    The member function doubleToColor() was taken from C code written
    by @author Paul Bourke <pdb@swin.edu.au>

    Found on this web page
    http://astronomy.swin.edu.au/~pbourke/colour/colourramp/

 */
class MDL_HIPPOPLOT_API BinToColorScaleMovingBrkPt : public BinToColor
{

public:

  /** The constructor.
   */
  BinToColorScaleMovingBrkPt ( const char * name );

  
  /** Another constructor with control points specified
   */
  BinToColorScaleMovingBrkPt (const std::vector < double > & sv, 
			      const char * model_name);

  /** The copy constructor. */
  BinToColorScaleMovingBrkPt ( const BinToColorScaleMovingBrkPt & );

  virtual BinToColor * clone () const;

  virtual void doubleToColor ( double value, Color & color ) const;

  /** Returns @c true.  Returns @c true as this class has control
      points with which the value to color transform can be varied.
  */
  virtual bool hasControlPoints () const;

};

} // namespace hippodraw

#endif // _BinToColorScaleMovingBrkPt_H_
