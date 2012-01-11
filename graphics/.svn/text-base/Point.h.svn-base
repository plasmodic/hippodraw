/* -*- mode: c++ -*- */

/** @file

Point class interface

Copyright (C) 2000, 2001, 2003, 2005  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Point.h,v 1.19 2005/10/30 00:10:12 pfkeb Exp $

*/

#ifndef _POINT_H_
#define _POINT_H_

#include "pattern/libhippo.h"

namespace hippodraw {

/** Class representing a two or three dimension point.  @attention The
    in line functions were measured to have high profile when plotting
    large images.
*/
class MDL_HIPPOPLOT_API Point
{
 private:
  double m_x;
  double m_y;
  double m_z;

 public:
  //Point( double x = 0.0, double y = 0.0 );
  Point( double x = 0.0, double y = 0.0, double z = 0.0 );
 
  void setPoint( double x, double y );
  void setPoint( double x, double y, double z );

  /** Moves the point by specified amount. */
  void moveBy ( double x, double y );

  /** The following function returns the X coordinate. */
  inline double getX() const;

  /** The following function returns the Y coordinate. */
  inline double getY() const;

  /** The following function returns the Z coordinate. */
  inline double getZ() const;

  /** Sets the Z component of the Rectangle. */
  void setZ ( double );

};

inline
double 
Point::
getX() const 
{
  return m_x;
}

inline 
double
Point::
getY() const 
{
  return m_y;
}

inline double 
Point::
getZ() const 
{
  return m_z;
}

} // namespace hippodraw

#endif // _POINT_H_
