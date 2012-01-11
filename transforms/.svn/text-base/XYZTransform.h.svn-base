/* -*- mode: c++ -*-
 *
 * HippoPlot XYZTransform  class interface
 *
 * Copyright (C) 2001-2003   The Board of Trustees of The Leland
 * Stanford Junior University.  All Rights Reserved.
 *
 * $Id: XYZTransform.h,v 1.13 2006/05/27 00:00:13 xiefang Exp $
 */

#ifndef _XYZTransform_H_
#define _XYZTransform_H_

#include "XYZTransformBase.h"

#include <vector>

namespace hippodraw {

class UnaryTransform;

/** A transform that transforms the X, Y and Z coordinates independently.
 
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/
class MDL_HIPPOPLOT_API XYZTransform : public XYZTransformBase
{

  /** The transform on the X axis. */
  UnaryTransform * m_x;

  /** The transform on the Y axis. */
  UnaryTransform * m_y;

  /** The transform on the Z axis. */
  UnaryTransform * m_z;

public:

  /** The following constructor takes three base constructors as
      arguments. */
  XYZTransform ( UnaryTransform * x, UnaryTransform * y, UnaryTransform * z );

  /** The copy constructor. */
  XYZTransform ( const XYZTransform & );

  /** The virtual destructor. */
  virtual ~XYZTransform();

  /** Creates a new Transform object by copying an existing one. */
#ifdef CLONE_DEFECT
  virtual TransformBase * clone () const;
#else
  virtual XYZTransform  * clone () const;
#endif

  /** Returns the transform function on the X axis. */
  TransformBase * xTransform () const;

  /** Returns the transform function on the Y axis. */
  TransformBase * yTransform () const;

  /** Returns the transform function on the Z axis. */
  TransformBase * zTransform () const;

  virtual void transform ( double & x, double & y, double & z ) const;

  virtual void transform ( std::vector< double > & x, 
			   std::vector< double > & y,
			   std::vector< double > & z ) const;

  virtual void validate ( Range & x, Range & y, Range & z ) const;

  /** Returns a Range whose values are the limits on the X coordinate. */
  virtual const Range & limitX () const;

  /** Returns a Range whose values are the limits on the Y coordinate. */
  virtual const Range & limitY () const;

  /** Returns a Range whose values are the limits on the Z coordinate. */
  virtual const Range & limitZ () const;

  /** Returns true if Z axis is linear. */
  virtual bool isLinearInZ () const;
};

} // namespace hippodraw

#endif // _XYZTransform_H_
