/* -*- mode: c++ -*- */

/** @file

hippodraw::ColumnPointRep class interface

Copyright (C) 2000, 2001, 2003, 2005, 2006  The Board of Trustees of
The Leland Stanford Junior University.  All Rights Reserved.

$Id: ColumnPointRep.h,v 1.57 2006/09/25 22:45:25 pfkeb Exp $

*/

#ifndef _ColumnPointRep_H_
#define _ColumnPointRep_H_

#include "PointRepBase.h"

namespace hippodraw {

/** A PointRep class that draws a column to represent the value.  The
    following plot was made using ColumnPointRep.

    @request @@@@ Need version which draws histogram on its side.

    @image html dyhistogram.png "Plot with ColumnPointRep"
    @image latex dyhistogram.eps "Plot with ColumnPointRep"
    
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/
class MDL_HIPPOPLOT_API ColumnPointRep : public PointRepBase
{
protected:

  /** A cache of the points to be drawn along the X axis.  For
      efficiency, all the points are given to the view at once.  
  */
  std::vector< double > m_x;

  /** A cache of the points to be drawn along the Y axis.  For
      efficiency, all the points are given to the view at once. 
  */
  std::vector< double > m_y;

  /** Does the drawing based on the accumulated polyline.
   */
  virtual void drawValues ( ViewBase * view ) const;

private:

  /** The style currently used.
   */
  hippodraw::Line::Style m_line_style;

  /** The Y error flag. If set @c true, error bars will be generated 
      along the Y axis. */
  bool m_y_flag;

  /** A flag that the cache has been cleared. */
  bool m_clear_flag;

public:

  /** The default constructor. */
  ColumnPointRep();
  
  /** The copy constructor. */
  ColumnPointRep( const ColumnPointRep & point_rep );

  /** The destructor. */
  ~ColumnPointRep();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual RepBase * clone();

  /** Sets the Color.  Sets the Color of both the representation and
      its ErrorBarRep if any. */
  virtual void setColor ( const Color & color );

  /** Sets the size of the line drawing and that of the error bar.
   */
  virtual void setSize ( float value );

  /** Sets the style of the line drawing.
   */
  virtual void setStyle ( unsigned int style );

  /** Returns the style of the line drawing.
   */
  virtual unsigned int getStyle ( ) const;

  /** Sets the error display on or off.  Sets the error display on the
      Y axis if @a axis is the Y axis, otherwise does nothing.
   */
  virtual void setErrorOn ( hippodraw::Axes::Type axis, bool yes = true );

  virtual bool yError ( ) const;

  virtual void drawProjectedValues ( const DataSource * ntuple,
				     TransformBase * transform,
				     ViewBase * view );

  /** Returns @c true as this class uses hippodraw::Line::Style.
   */
  virtual bool uses ( hippodraw::Line::Style ) const;


};

} // namespace hippodraw

#endif // _ColumnPointRep_H_
