/* -*- mode: c++ -*- */

/** @file

hippodraw::AxisRepColor class interface

Copyright (C) 2000, 2001, 2003-2006   The Board of Trustees of The
Leland Stanford Junior University.  All Rights Reserved.

$Id: AxisRepColor.h,v 1.49 2006/09/25 16:19:25 pfkeb Exp $ 
 
*/

#ifndef _AxisRepColor_H_
#define _AxisRepColor_H_

#include "AxisRepBase.h"

namespace hippodraw {

class AxisModelBase;
class AxisTick;

/** The class for the 2D axes drawing with color.  This class uses the
    information kept by the AxisModelBase hierarchy objects. It
    communicates with the ViewBase hierarchy objects in order to get
    the information of the actual display. It is, therefore, carrying
    out adequate calculations of fonts and positions for drawing in
    good proportions to the axes, their labels, and the main title.

    @author Kaustuv <kaustuv@stanford.edu>
    @author Sanket B. Malde <sanket@stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
	@author Tao Xu <taox@stanford.edu>

    @todo Implement minor ticks.
 */
class MDL_HIPPOPLOT_API AxisRepColor : public AxisRepBase
{

 protected:

  /** The z coordinate of the origin of the coordinate system. */
  float m_axis_z_origin;

  /** The depth of the rectangle in which the drawing is made. */
  float m_axis_depth;

 public:
  /** The default constructor. */
  AxisRepColor();

  /** The copy constructor. */
  AxisRepColor( const AxisRepColor & axis_rep );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  AxisRepBase * clone();
  
  /** Draws the labels of the z axis. */
  void drawZLabels ( const AxisModelBase & axis_model,
		     ViewBase & view, 
		     const std::string & z_label );

  
  /** Takes care of the z ticks drawing (that is, the ticks on the top
     of the plot). It also draws the labels of the ticks which are on
     the top of the plot.
  */
  void drawAllZTicks( const AxisModelBase & axis_model,
		      const TransformBase & transform,
		      ViewBase & view );
private:

    /** Draws the Z tick lines. */
  void drawZTickLines ( const AxisModelBase & axisModel,
			const TransformBase & transform,
			ViewBase & view );
  
  /** Draws the labels for the Z ticks. */
  void drawZTickLabels ( const AxisModelBase & axisModel,
			 const TransformBase & transform,
			 ViewBase & view );

  /** Draws the color scale bar.
   */
  virtual void drawColorScale ( const BinToColor &, ViewBase & );

};

} // namespace hippodraw

#endif // _AxisRepColor_H_
