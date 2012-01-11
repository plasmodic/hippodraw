/* -*- mode: c++ -*- */

/** @file

hippodraw::ColorBoxPointRep class interface

Copyright (C) 2000, 2002-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.
 
$Id: ColorBoxPointRep.h,v 1.60 2007/03/14 23:28:28 xiefang Exp $

*/

#ifndef _ColorBoxPointRep_H_
#define _ColorBoxPointRep_H_

#include "PointRepBase.h"

class BinaryTransform;
class PeriodicBinaryTransform;

namespace hippodraw {

class BinToColor;

/** A PointRep class that draws a colored box to represent the value.
    The following image was made with ColorBoxPointRep.

    @image html profile2d.png "Plot with ColorBoxPointRep"
    @image latex profile2d.eps "Plot with ColorBoxPointRep"

    @author Xie Fang <xiefang@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
    @author Tao Xu <taox@stanford.edu>
*/
class MDL_HIPPOPLOT_API ColorBoxPointRep : public PointRepBase
{
private:

  /** The following function transforms the projected value to a color
      scale. */
  BinToColor * m_bin_to_color;


  /** Flag to indicated that background needs to be draw. */
  bool m_need_background;

  /** Flag to indicated that box edge needs to be shown. */
  bool m_box_edge;

public:

  /** The default constructor. */
  ColorBoxPointRep( );

  /** The copy constructor. */
  ColorBoxPointRep( const ColorBoxPointRep & point_rep );

  /** The destructor. */
  ~ColorBoxPointRep();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual RepBase * clone();

  virtual const BinToColor * getValueTransform ( ) const;
  virtual void setValueTransform ( BinToColor * );

  virtual void drawProjectedValues ( const DataSource * ntuple,
				     TransformBase * transform,
				     ViewBase * view );

  /** Returns @c false as this class represents a Z value in color.
   */
  virtual bool uses ( Color::Value ) const;

  virtual  void setBoxEdge( bool show );

};

} // namespace hippodraw

#endif // _ColorBoxPointRep_H_
