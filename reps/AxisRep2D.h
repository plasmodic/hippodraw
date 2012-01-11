/* -*- mode: c++ -*- */

/** @file

hippodraw::AxisRep2D class interface

Copyright (C) 2000-2004, 2006  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: AxisRep2D.h,v 1.63 2006/02/12 20:51:35 pfkeb Exp $ 

*/

#ifndef _AxisRep2D_H_
#define _AxisRep2D_H_

#include "AxisRepBase.h"

namespace hippodraw {

class AxisModelBase;
class AxisTick;

/** The class for the 2D axes drawing.  This class uses the
    information kept by the AxisModelBase hierarchy objects. It
    communicates with the ViewBase hierarchy objects in order to get
    the information of the actual display. It is, therefore, carrying
    out adequate calculations of fonts and positions for drawing in
    good proportions to the axes, their labels, and the main title.

    @author Stephane Bonneaud <gandalf@slac.stanford.edu> 
    @author Kaustuv <kaustuv@stanford.edu>
    @author Sanket B. Malde <sanket@stanford.edu>

    @todo Implement minor ticks.

*/

class MDL_HIPPOPLOT_API AxisRep2D : public AxisRepBase
{

 public:
  /** The default constructor. */
  AxisRep2D();

  /** The copy constructor. */
  AxisRep2D( const AxisRep2D & axis_rep );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  AxisRepBase * clone();
  
  /** Draws the labels of the z axis. */
  virtual void drawZLabels ( const AxisModelBase & axis_model,
			     ViewBase & view, 
			     const std::string & Z_label );

  /** Takes care of the z ticks drawing (i.e., the ticks on the top of
   *  the plot). It draws the labels of the ticks which are on the
   *  top of the plot. */
  virtual void drawAllZTicks( const AxisModelBase & axis_model,
			      const TransformBase & transform,
			      ViewBase & view );

};

} // namespace hippodraw

#endif // _AxisRep2D_H_
