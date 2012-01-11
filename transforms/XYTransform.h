/* -*- mode: c++ -*-*/

/** @file

hippodraw::XYTransform  class interface

Copyright (C) 2001-2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

 $Id: XYTransform.h,v 1.32 2006/02/02 22:53:15 xiefang Exp $

*/

#ifndef _XYTransform_H_
#define _XYTransform_H_

#include "BinaryTransform.h"

#include <vector>

namespace hippodraw {

class UnaryTransform;
class Range;

/** A transform that transforms the X and Y coordinates independently.

@author Paul_Kunz@slac.stanford.edu 
*/
class MDL_HIPPOPLOT_API XYTransform : public BinaryTransform
{

private:

  /** The transform on the X axis. */
  UnaryTransform * m_x;

  /** The transform on the Y axis. */
  UnaryTransform * m_y;

public:

  /** The following constructor takes three base constructors as
      arguments. */
  XYTransform ( UnaryTransform * x, 
		UnaryTransform * y,
		UnaryTransform * z );

  /** The copy constructor. */
  XYTransform ( const XYTransform & );

  /** The virtual destructor. */
  virtual ~XYTransform();

  /** Creates a new Transform object by copying an existing one. */
#ifdef CLONE_DEFECT
  virtual TransformBase * clone () const;
#else
  virtual XYTransform   * clone () const;
#endif

  /** Returns the transform function on the X axis. */
  TransformBase * xTransform () const;

  /** Returns the transform function on the Y axis. */
  TransformBase * yTransform () const;

  virtual bool isLinearInXY () const;

  virtual void transform ( double & x, double & y ) const;

  virtual bool inverseTransform ( double & x, double & y ) const;

  virtual void transform ( std::vector< double > & x, 
			   std::vector< double > & y ) const;

  virtual double aspectRatio () const;

  virtual Rect calcRectangle ( const Range & x, 
			       const Range & y );

  virtual void validate ( Range & x, Range & y ) const;

  /** Returns a Range whose values are the limits on the X coordinate. */
  virtual const Range & limitX () const;

  /** Returns a Range whose values are the limits on the Y coordinate. */
  virtual const Range & limitY () const;

  virtual const std::vector < AxisTick > &
  setTicks ( AxisModelBase & axis_model, hippodraw::Axes::Type axis );

  /** Sets the range of given axis to be a new "nice" within the
      limits given 
  */
  virtual void adjustValues ( AxisModelBase & model,
			      hippodraw::Axes::Type  axes,
			      const Range & limit );
};

} // namespace hippodraw

#endif // _XYTransform_H_
