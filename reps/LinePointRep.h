/* -*- mode: c++ -*- */

/** @file

hippodraw::LinePointRep class interface

Copyright (C) 2000, 2001, 2003, 2005-2006   The Board of Trustees of The
Leland Stanford Junior University.  All Rights Reserved.

$Id: LinePointRep.h,v 1.48 2006/08/18 15:32:54 pfkeb Exp $

*/

#ifndef _LinePointRep_H_
#define _LinePointRep_H_

#include "PointRepBase.h"

namespace hippodraw {

class Rect;
class Point;

/** A PointRep class that draws a column to represent the value.  The
    variety of styles available are enumerated in the @c enum @c
    hippodraw::Line::Style.

    @bug Qt 4 Only.   The extra colored lines do not show up in axisBug.py

    @author Jim Chiang <jchiang@slac.stanford.edu>
    @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/
class MDL_HIPPOPLOT_API LinePointRep : public PointRepBase
{

private:

   // The drawing rectangle boundaries.
   double m_xmin, m_xmax, m_ymin, m_ymax;

   const Rect * m_user_rect;

  /** Transform the projected values in @a source with @a transform.
   */
  void transformValues ( const DataSource * source,
			 TransformBase * transform );

   bool outside_box(std::vector<double>::const_iterator ix,
                    std::vector<double>::const_iterator iy) const;

   bool straddles_x_boundary(std::vector<double>::const_iterator ix,
                             std::vector<double>::const_iterator iy,
                             Point & pt1, Point & pt2, 
                             double & distance) const;

   bool straddles_y_boundary(std::vector<double>::const_iterator ix,
                             std::vector<double>::const_iterator iy,
                             Point & pt1, Point & pt2, 
                             double & distance) const;

   bool cornerCase(std::vector<double>::const_iterator ix,
                   std::vector<double>::const_iterator iy,
                   std::vector<double> & x, std::vector<double> & y) const;

   double interpolate(double x, std::vector<double>::const_iterator ix,
                      std::vector<double>::const_iterator iy) const;

   double separation(const Point & pt1, const Point & pt2) const;

   void findEndPoints(std::vector<double>::const_iterator ix,
                      std::vector<double>::const_iterator iy,
                      Point & pt1, Point & pt2) const;
 
   void addEndPoints(std::vector<double>::const_iterator ix,
                     std::vector<double>::const_iterator iy,
                     std::vector<double> & x, std::vector<double> & y) const;

protected:

  /** A cache of the points along the X axis to be drawn.  For
      efficiency, all line segments are sent once to the view.
      However, the x and y points are kept separate so that they can
      be transformed separately.  LinePointRep builds a cache for
      efficiency. 
  */
  std::vector< double > m_x;

  /** A cache of the points along the Y axis to be drawn.  For
      efficiency, all line segments are sent once to the view.
      However, the x and y points are kept separate so that they can
      be transformed separately.  LinePointRep builds a cache for
      efficiency.  
  */
  std::vector< double > m_y;

  /** The style currently used.
   */
  hippodraw::Line::Style m_line_style;

  /** Constructor for use by derived classes.
   */
  LinePointRep ( const char * name, float size );

  /** Draws the projected values from protected transformed copy to @a
      view.
   */
  void drawValues ( ViewBase * view );

public:
  /** The default constructor. */
  LinePointRep();

  /** Constructor that takes size as argument. */
  LinePointRep ( float size );

  /** The copy constructor. */
  LinePointRep( const LinePointRep & point_rep );

  /** The destructor. */
  virtual ~LinePointRep();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual RepBase * clone();

  /** Sets the style of the line drawing.
   */
  virtual void setStyle ( unsigned int style );

  /** Returns the style of the line drawing.
   */
  virtual unsigned int getStyle ( ) const;

  virtual void drawProjectedValues ( const DataSource * ntuple,
				     TransformBase * transform,
				     ViewBase * previews );

  /** Returns @c true as this class uses hippodraw::Line::Style.
   */
  virtual bool uses ( hippodraw::Line::Style ) const;

};

} // namespace hippodraw

#endif // _LinePointRep_H_
