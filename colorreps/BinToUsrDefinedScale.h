/* -*- mode: c++ -*- */

/** @file

BinToUsrDefinedScale class interface

Copyright (C) 2004 The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: BinToUsrDefinedScale.h,v 1.3 2005/10/30 00:10:17 pfkeb Exp $
 
*/

#ifndef _BinToUsrDefinedScale_H_
#define _BinToUsrDefinedScale_H_

#include "BinToColor.h"

namespace hippodraw {

/** The class derived from the BinToColor class. This class implements
    the transformation of a value to a color like the classic rainbow,
    but  has  two  control  parameters(user specified)  to vary  the  
    transform. Unlike the other models with control parameters this
    model does not have sliders to change the parameters. They can be
    explicitly changed my the user by clicking on the Edit model button in 
    the Inspector. One parameter is the  distance between the break points  
    and the other is the position of the center between the break points.

    @author Joy Rajiv <jrajiv@stanford.edu>

    The member function doubleToColor() was taken from C code written by
    @author Paul Bourke <pdb@swin.edu.au>

    Found on this web page
    http://astronomy.swin.edu.au/~pbourke/colour/colourramp/

 */
class MDL_HIPPOPLOT_API BinToUsrDefinedScale : public BinToColor
{

public:

  /** The constructor.
   */
  BinToUsrDefinedScale ( const char * name );

  
  /** Another constructor with control points specified
   */
  BinToUsrDefinedScale ( const std::string & model_name );

  /** The copy constructor. */
  BinToUsrDefinedScale ( const BinToUsrDefinedScale & );

  virtual BinToColor * clone () const;

  virtual void doubleToColor ( double value, Color & color ) const;

  /** Returns @c true.  Returns @c true as this class has control
      points with which the value to color transform can be varied.
  */
  virtual bool hasControlPoints () const;

  /** Returns @c true.
   */
   virtual bool isUserDefined () const;

};

} // namespace hippodraw

#endif // _BinToUsrDefinedScale_H_
