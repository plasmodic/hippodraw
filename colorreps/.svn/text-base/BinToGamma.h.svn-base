/* -*- mode: c++ -*- */

/** @file

BinToGamma class interface

Copyright (C) 2000, 2003, 2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToGamma.h,v 1.1 2006/09/25 16:19:25 pfkeb Exp $

*/

#ifndef _BinToGamma_H_
#define _BinToGamma_H_

#include "BinToColor.h"

namespace hippodraw {

/** The class derived from the BinToColor class. The BinToColor class
    implements the transformation of a value to a color which belongs
    to a grey scale with gamma correction. If gamma value is equal to 1,
	this will reduce to Grey Scale.

	@author Tao Xu <taox@stanford.edu>
*/

class MDL_HIPPOPLOT_API BinToGamma : public BinToColor
{
 public:

  /** The constructor. */
  BinToGamma ( const char * name);

  /** Another constructor with control points specified
   */
  BinToGamma ( const std::string & model_name );

  /** The copy constructor. */
  BinToGamma ( const BinToGamma & );

  BinToColor * clone () const;

  virtual void doubleToColor( double value, Color & color ) const;

  /** Returns @c true.  Returns @c true as this class has control
      points with which the gamma value can be varied.
  */
  virtual bool hasControlPoints () const;

  /** Returns @c true.
   */
   virtual bool isUserDefined () const;

   virtual bool acceptChangeColor () const;

};

} // namespace hippodraw

#endif // _BinToGamma_H_
