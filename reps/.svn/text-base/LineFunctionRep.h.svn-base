/* -*- mode: c++ -*- */

/** @file

LineFunctionRep class interface

Copyright (C) 2005 The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: LineFunctionRep.h,v 1.50 2005/10/30 00:10:17 pfkeb Exp $

*/

#ifndef _LineFunctionRep_H_
#define _LineFunctionRep_H_

#include "LinePointRep.h"

namespace hippodraw {

class FunctionBase;

/** A LinePointRep class that draws a function by trying to make a
    smooth line after the transform to the view coordinate system.
    variety of styles available are enumerated in the @c enum @c
    hippodraw::Line::Style.

    @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API LineFunctionRep : public LinePointRep
{

private:

  /** Smooths the curve.
   */
  void smoothCurve ( const Range & range,
		     TransformBase * transform,
		     const FunctionBase * function );

public:

  /** The default constructor. */
  LineFunctionRep();

  /** Constructor that takes size as argument. */
  LineFunctionRep ( float size );

  /** The copy constructor. */
  LineFunctionRep( const LineFunctionRep & point_rep );

  /** The destructor. */
  virtual ~LineFunctionRep();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual RepBase * clone();

  /** Draw the projected values.  Draws the function over the @a range
       Smooths the curve after transforming with @a transform with @a
       function.  Draw to @a view.
  */
  void drawProjectedValues ( const Range & range,
			     const FunctionBase * function,
			     TransformBase * transform,
			     ViewBase * view );

};

} // namespace hippodraw

#endif // _LineFunctionRep_H_
