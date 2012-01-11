/*
 * HippoPlot Point  class implementation
 *
 * Copyright (C)  2000, 2001  The Board of Trustees of The Leland Stanford
 * Junior University.  All Rights Reserved.
 *
 * $Id: Point.cxx,v 1.18 2005/10/30 00:10:12 pfkeb Exp $
 */

#include "Point.h"

namespace hippodraw {

Point::Point( double x, double y, double z )
  : m_x(x), m_y(y), m_z(z)
{
}

void Point::setPoint( double x, double y ) {
  m_x = x;
  m_y = y;
}

void Point::setPoint( double x, double y, double z ) {
  m_x = x;
  m_y = y;
  m_z = z;
}

void Point::moveBy ( double x, double y )
{
  m_x += x;
  m_y += y;
}

void Point::setZ ( double z )
{
  m_z = z;
}

} // namespace hippodraw

