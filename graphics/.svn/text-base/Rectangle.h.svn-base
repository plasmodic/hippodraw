/* -*- mode: c++ -*- */

/** @file

hippodrw::Rect  class interface

Copyright (C) 2000, 2002, 2005, 2006   The Board of Trustees of The
Leland Stanford Junior University.  All Rights Reserved.

$Id: Rectangle.h,v 1.31 2006/01/08 21:52:27 pfkeb Exp $

*/

#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

#include "Point.h"
#include "Size.h"

#include <vector>

namespace hippodraw {

/** Class representing a rectangle.  @attention Some methods that show
    hide usage in a profile of displaying large image have been
    optimized by directly accessing member functions of data members
    rather than using the public interface.  Also the in line functions
    had a high profile when plotting large images.

    @note Would like to have this class named Rectangle, but it
    conflicts with an apparent typedef under Windows.

*/
class MDL_HIPPOPLOT_API Rect
{

private:

  /** The origin of the Rectangle */
  Point m_origin;

  /** The size of the Rectangle. */
  Size  m_size;

  /** Returns the origin. */
  const Point & getOrigin() const;

  /** Returns the size. */
  const Size & getSize() const;

public:

  /** Default constructor.
   */
  Rect ();

  /** Constructor for 2D Rectangle.
   */
  Rect ( double x, double y, double width, double height );

  /** constructor for 3D Rectangle.
   */
  Rect ( double x, double y, double z,
	 double width, double height, double depth );

  /** Set the parameters of 2D Rectangle. */
  void setRect ( double x, double y, double width, double height );

  /** Set the parameters of 3D Rectangle. */
  void setRect ( double x, double y, double z,
		 double width, double height, double depth );

  /** Moves the Rectangle by specified amount.  The size remains
      unchanged. */
  void moveBy ( double x, double y );

  /** A shortcut to get origin.X. */
  inline double getX() const;

  /** A shortcut to get origin.Y. */
  inline double getY() const;
  
  /** A shortcut to get origin.Z. */
  double getZ() const;

  /** Sets the Z dimension. */
  void setZ ( double );

  /** A shortcut to get size.width. */
  double getWidth() const;

  /** A shortcut to get size.height. */
  double getHeight() const;

  /** A shortcut to get size.depth. */
  double getDepth() const;


  /** Sets the depth of the rectangle.
   */
  void setDepth ( double );

  /** Checks the value against the depth to see if it is in bounds. */
  bool isInDepth ( double z1 ) const;

  /** Checks the coordinates to see if they are inside or on the boundary
      of the rectangle.
  */
  bool isInBounds ( double x1, double y1 ) const;

  /** Checks the coordinates to see if they are inside or on the boundary
      of the rectangle.
  */
  bool isInBounds  ( double x1, double y1, double z1 ) const;

  /** Changes the coordinates so that they are inside or on the
      boundary of the rectangle.  That is, for @a x or @a y, leave it
      unchanged if it is within the rectangle; otherwise, set it to
      the boundary on which it exceeds.
  */
  void makeInBounds ( double & x, double & y ) const;

  /** Changes the coordinates so that they are inside or on the
      boundary of the rectangle.  That is, for @a x or @a y or @a z,
      leave it unchanged if it is within the rectangle; otherwise, set
      it to the boundary on which it exceeds.
  */
  void makeInBounds ( double & x, double & y, double & z ) const;

  /** Changes the coordinates so that they are inside or on the
      boundary of the rectangle.  That is, for each element of the
      vector @a x or @a y, leave it unchanged if it is within the
      rectangle; otherwise, set it to the boundary on which it
      exceeds.
  */
  void makeInBounds ( std::vector< double > & x, 
		      std::vector< double > & y ) const;

  /** Changes the coordinates so that they are inside or on the
      boundary of the rectangle.  That is, for each element of the
      vector @a x or @a y or @a z, leave it unchanged if it is within
      the rectangle; otherwise, set it to the boundary on which it
      exceeds.
  */
  void makeInBounds ( std::vector< double > & x, 
		      std::vector< double > & y,
		      std::vector< double > & z ) const;

};

inline
double
Rect::
getX() const
{
  return m_origin.getX();
}

inline
double
Rect::
getY() const
{
  return m_origin.getY();
}

} // namespace hippodraw

#endif // _RECTANGLE_H_
