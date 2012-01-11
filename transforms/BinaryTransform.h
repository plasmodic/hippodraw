/* -*- mode: c++ -*- */

/** @file

hippodraw::BinaryTransform class interface

Copyright (C) 2001-2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinaryTransform.h,v 1.38 2006/05/27 00:00:12 xiefang Exp $

*/

#ifndef _BinaryTransform_H_
#define _BinaryTransform_H_

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h" // for CLONE_DEFECT
#endif

#include "TransformBase.h"

#include "axes/AxesType.h"

#include <vector>

namespace hippodraw {

  class AxisModelBase;
  class AxisTick;
  class Rect;
  class UnaryTransform;

/** A transform that transforms coordinates from one 2D coordinate
    system to another.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API BinaryTransform : public TransformBase
{

protected:
  
  /** The transform on the Z axis. */
  UnaryTransform * m_z;
  
  /** Does this binary transform needs grid? */
  bool m_needs_grid;
  
  /** Does this binary transform x ticks? */
  bool m_needs_x_ticks;
  
  /** Does this binary transform y ticks? */
  bool m_needs_y_ticks;

  /** Is this transform periodic */
  bool m_is_periodic;

  /** The default constructor. */
  BinaryTransform ();

  /** The constructor that take the z transform as argument also it takes
      and argument which states if it is a periodic transformation
      (default false), next optional argument says if this transform needs
      a grid or not (default is false), and if it needs x and y ticks or
      not (Default is true) */
  BinaryTransform ( UnaryTransform *, bool = false, bool  = false,
		    bool  = true, bool  = true);
  
  /** The copy constructor. */
  BinaryTransform ( const BinaryTransform & );
  
public:
  
  /** The virtual destructor. */
  virtual ~BinaryTransform();
  
  /** Creates a new XYTransform object by copying an existing one. */
#ifdef CLONE_DEFECT
  virtual TransformBase   * clone () const = 0;
#else
  virtual BinaryTransform * clone () const = 0;
#endif
  
  /** Returns @c true if the transform would be one to one on both the
      X and Y axes.  Clients can use this function to avoid calling
      the transform in for loops. */
  virtual bool isLinearInXY () const = 0;

  /** Returns @c true if the transform would be one to one on Z axes.  
      Clients can use this function to avoid calling the transform 
      in for loops. */
  virtual bool isLinearInZ () const;

  /** Transform the coordinates on the X and Y axes.  

      @attention The arguments are passed by reference so the values
      may changed.
   */
  virtual void transform ( double & x, double & y ) const = 0;
  
  /** Transform the transformed coordinates on X and Y axis back to
      the original true data space. Return true if success, else
      return false.
  */
  virtual bool inverseTransform ( double & x, double & y ) const = 0;
  
  /** A transform on two axes. */
  virtual void transform ( std::vector< double > & x, 
			   std::vector< double > & y ) const = 0;

  /** Returns the aspect ratio.  Some transforms require a specific
      ration of the width to the height of the display.  Returning a
      value of 2.0, for example, means that the width should be twice
      the height.  Returning a value of 0.0 means any aspect ratio may
      be used.  */
  virtual double aspectRatio () const;

  /** Returns a rectangle enclosing the transformed data space.
   */
  virtual Rect calcRectangle ( const Range & x, 
			       const Range & y ) = 0;

  /** Validates the Ranges.  Adjusts Ranges to be restricted to that
      which is valid for the transform. */
  virtual void  validate ( Range & x, Range & y ) const = 0;

  /** Returns the Range limits of the first coordinate. */
  virtual const Range & limitX () const = 0;

  /** Returns the Range limits of the second coordinate. */
  virtual const Range & limitY () const = 0;

  /** Sets and return the axis ticks on specified @a axis.
   */
  virtual const std::vector < AxisTick > &
  setTicks ( AxisModelBase & axis_model, hippodraw::Axes::Type axis ) = 0;

  /** Sets the range of given axis to be a new "nice" within the
      limits given */
  virtual void adjustValues ( AxisModelBase & model,
			      hippodraw::Axes::Type  axes,
			      const Range & limit ) = 0;
  
  /** Returns the Range limits on the third coordinate. */
  const Range & limitZ () const;

  /** Returns the transform function on the Z axis. */
  TransformBase * zTransform () const;

  /** Sets the transform function on the Z axis.  The @a transform
      object must be derived class of UnaryTransform, otherwise the
      transform object pointer is set to null.
   */
  void setZTransform ( TransformBase * transform );

  /** Transforms the z coordinate. */
  void transformZ ( double & z ) const;

  /** Inverse transforms the z coordinate. */
  void inverseTransformZ ( double & z ) const;
  
  /** Returns whether this transform needs grid */
  bool needsGrid() const;

  /** Sets whether this transform  needs grid */
  void setNeedsGrid( bool needs_grid );

  /** Returns whether this transform needs x ticks*/
  bool needsXTicks() const;

  /** Sets whether this transform  needs x ticks */
  void setNeedsXTicks( bool needs_x_ticks );

  /** Returns whether this transform needs y ticks */
  bool needsYTicks() const;

  /** Sets whether this transform  needs y yicks */
  void setNeedsYTicks( bool needs_y_ticks );

  /** Sets whether this transform is periodic */
  bool isPeriodic() const;

  
};

} // namespace hippodraw

#endif // _BinaryTransform_H_
