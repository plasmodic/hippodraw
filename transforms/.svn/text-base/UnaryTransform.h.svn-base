/* -*- mode: c++ -*- */

/** @file

UnaryTransform  class interface

Copyright (C) 2002-2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: UnaryTransform.h,v 1.18 2007/08/24 22:19:16 pfkeb Exp $

*/

#ifndef _UnaryTransform_H_
#define _UnaryTransform_H_

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h" // for CLONE_DEFECT
#endif

#include "TransformBase.h"

#include "axes/Range.h"

namespace hippodraw {

class AxisModelBase;
class AxisTick;

/** A transform that transforms coordinates in one dimension from one
    coordinate system to another.
  
    @author Paul_Kunz@slac.stanford.edu 
 */
class MDL_HIPPOPLOT_API UnaryTransform : public TransformBase
{

protected:

  /** The valid Range of the transform. */
  const Range m_limits;

  /** Constructor for setting m_limits. */
  UnaryTransform ( double low, double high );

public:

  /** The virtual destructor. */
  virtual ~UnaryTransform();

  /** Function to make copy of object. */
#ifdef CLONE_DEFECT
  virtual TransformBase *   clone () const = 0;
#else
  virtual UnaryTransform * clone () const = 0;
#endif

  virtual bool isLinear () const = 0;

  /** @todo implement concrete cases of single variable
      transformation.  Transforms the coordinate @a x. */
  virtual void transform ( double & x ) const = 0;

  /** Inverse transforms the coordinate @a x. */
  virtual void inverseTransform ( double & x ) const = 0;

  /** Transforms each element of the vector @a x. */
  virtual void transform ( std::vector< double > & x ) const = 0;

  /** Validates the Range.  Adjusts the Range to be restricted to that
      which is valid for the transform. */
  virtual void validate ( Range & ) const = 0;

  /** Returns a Range whose values are the limits on the coordinate. */
  virtual const Range & limits () const;

  /** Sets the vector of ticks along the axis to which this 
      unary transform is applied */
  virtual const std::vector < AxisTick > &
  setTicks ( AxisModelBase & axis ) = 0;

  /** Sets the range of given axis to be a new "nice" within the
      limits given 
  */
  virtual const Range & adjustValues ( AxisModelBase & model,
				       const Range & limit ) = 0;

};

} // namespace hippodraw

#endif // _UnaryTransform_H_
