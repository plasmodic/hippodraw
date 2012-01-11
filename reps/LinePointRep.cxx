/** @file

hippodraw::LinePointRep class implementation

Copyright (C) 2000, 2001, 2003-2006  The Board of Trustees of The
Leland Stanford Junior University.  All Rights Reserved.

$Id: LinePointRep.cxx,v 1.81 2006/09/22 13:31:37 pfkeb Exp $

*/

#ifdef _MSC_VER
// include max() and min() missing from MicroSoft Visual C++
#include "msdevstudio/MSconfig.h"
#endif //_MSC_VER

#include "LinePointRep.h"

#include "axes/Range.h"
#include "datasrcs/DataPointTuple.h"
#include "datasrcs/DataSource.h"
#include "graphics/ViewBase.h"
#include "transforms/BinaryTransform.h"

#include <cmath>

using std::vector;

using namespace hippodraw;

LinePointRep::LinePointRep ()
  : PointRepBase ( "Line", 1.0 ),
    m_line_style ( Line::Solid )
{
}

LinePointRep::
LinePointRep ( const char * name, float size )
  : PointRepBase ( name, size ),
    m_line_style ( Line::Solid )
{
}

LinePointRep::LinePointRep ( float size )
  : PointRepBase ( "Line", size ),
    m_line_style ( Line::Solid )
{
}

LinePointRep::LinePointRep ( const LinePointRep & point_rep )
  : PointRepBase ( point_rep ), 
    m_line_style ( point_rep.m_line_style )
{
}

LinePointRep::~LinePointRep ()
{
}

RepBase * LinePointRep::clone()
{
  return new LinePointRep( *this );
}

void
LinePointRep::
setStyle ( unsigned int style )
{
  m_line_style = Line::convert ( style );
}

unsigned int
LinePointRep::
getStyle ( ) const
{
  return m_line_style;
}

namespace dp = hippodraw::DataPoint2DTuple;

void
LinePointRep::
transformValues ( const DataSource * ntuple, TransformBase * transform )
{
  unsigned int size = ntuple -> rows ();
  if ( size == 0 ) return;

  m_x.clear();
  m_y.clear();

  m_x.reserve ( size );
  m_y.reserve ( size );

  unsigned int i = 0; // for VC++ 7.1
  for ( ; i < size; i++ ) {
    const vector < double > & row = ntuple -> getRow ( i );
    double x = row [ dp::X ];    
    double y = row [ dp::Y ];
    m_x.push_back ( x );
    m_y.push_back ( y );
  }
  const BinaryTransform * t
    = dynamic_cast < const BinaryTransform * > ( transform );

  t -> transform ( m_x, m_y );
}

void
LinePointRep::
drawProjectedValues ( const DataSource * ntuple,
		      TransformBase * transform,
		      ViewBase * view )
{
  transformValues ( ntuple, transform );
  drawValues ( view );
}

void
LinePointRep::
drawValues ( ViewBase * view )
{
  unsigned int size = m_x.size ();
  if ( size == 0 ) return;

  m_user_rect = &(view -> getUserRect ());

  vector < double > x;
  vector < double > y;
  x.reserve ( size );
  y.reserve ( size );

  const Color & cur_color = color();

  m_xmin = m_user_rect->getX ();
  m_xmax = m_xmin + m_user_rect->getWidth ();
  m_ymin = m_user_rect->getY ();
  m_ymax = m_ymin + m_user_rect->getHeight ();

  // Plot each interval and truncate at the boundaries.
  unsigned int i = 0;
  std::vector<double>::const_iterator ix = m_x.begin();
  std::vector<double>::const_iterator iy = m_y.begin();
  bool isCorner(false);
  std::vector<double> xcorner;
  std::vector<double> ycorner;

  while (i < size-1) {
     if (outside_box(ix, iy)) {
        if (!x.empty()) {
           view -> drawPolyLine ( x, y, m_line_style, cur_color, m_size );
        }
        x.clear();
        y.clear();
        if (isCorner) {
           view -> drawPolyLine ( xcorner, ycorner, m_line_style,
                                  cur_color, m_size );
        }
     } else {
        isCorner = cornerCase(ix, iy, xcorner, ycorner);
        if (!isCorner) {
           addEndPoints(ix, iy, x, y);
        }
     }
     ++i;
     ++ix;
     ++iy;
  }
  if (!x.empty()) {
     view -> drawPolyLine ( x, y, m_line_style, cur_color, m_size );
  }
}

bool LinePointRep::
outside_box(std::vector<double>::const_iterator ix,
            std::vector<double>::const_iterator iy) const {
   return ( (*ix <= m_xmin && *(ix+1) <= m_xmin) || 
            (*ix >= m_xmax && *(ix+1) >= m_xmax) ||
            (*iy <= m_ymin && *(iy+1) <= m_ymin) || 
            (*iy >= m_ymax && *(iy+1) >= m_ymax) );
}

bool LinePointRep::
straddles_x_boundary(std::vector<double>::const_iterator ix,
                     std::vector<double>::const_iterator iy,
                     Point & pt1, Point & pt2,
                     double & distance) const {
   if ( outside_box(ix, iy) || 
        (*ix >= m_xmin && *(ix+1) >= m_xmin && 
         *ix <= m_xmax && *(ix+1) <= m_xmax) ) {
      return false;
   }
   // Treat case if interval straddles both boundaries.
   if (*ix < m_xmin && *(ix+1) > m_xmax) { 
      pt1.setPoint(m_xmin, interpolate(m_xmin, ix, iy));
      pt2.setPoint(m_xmax, interpolate(m_xmax, ix, iy));
   } else if (*ix > m_xmax && *(ix+1) < m_xmin) {
      pt1.setPoint(m_xmax, interpolate(m_xmax, ix, iy));
      pt2.setPoint(m_xmin, interpolate(m_xmin, ix, iy));
   }
   // Interval straddles lower boundary.
   else if (*ix < m_xmin && *(ix+1) > m_xmin) {
      pt1.setPoint(m_xmin, interpolate(m_xmin, ix, iy));
      pt2.setPoint(*(ix+1), *(iy+1));
   } else if (*ix > m_xmin && *(ix+1) < m_xmin) {
      pt1.setPoint(*ix, *iy);
      pt2.setPoint(m_xmin, interpolate(m_xmin, ix, iy));
   }
   // Interval straddles upper boundary.
   else if (*ix < m_xmax && *(ix+1) > m_xmax) {
      pt1.setPoint(*ix, *iy);
      pt2.setPoint(m_xmax, interpolate(m_xmax, ix, iy));
   } else if (*ix > m_xmax && *(ix+1) < m_xmax) {
      pt1.setPoint(m_xmax, interpolate(m_xmax, ix, iy));
      pt2.setPoint(*(ix+1), *(iy+1));
   }
   distance = separation(pt1, pt2);
   return true;
}

bool LinePointRep::
straddles_y_boundary(std::vector<double>::const_iterator ix,
                     std::vector<double>::const_iterator iy,
                     Point & pt1, Point & pt2,
                     double & distance) const {
   if ( outside_box(ix, iy) || 
        (*iy >= m_ymin && *(iy+1) >= m_ymin && 
         *iy <= m_ymax && *(iy+1) <= m_ymax) ) {
      return false;
   }
   // Treat case if interval straddles both boundaries.
   if (*iy < m_ymin && *(iy+1) > m_ymax) { 
      pt1.setPoint(interpolate(m_ymin, iy, ix), m_ymin);
      pt2.setPoint(interpolate(m_ymax, iy, ix), m_ymax);
   } else if (*iy > m_ymax && *(iy+1) < m_ymin) {
      pt1.setPoint(interpolate(m_ymax, iy, ix), m_ymax);
      pt2.setPoint(interpolate(m_ymin, iy, ix), m_ymin);
   } 
   // Interval straddles lower boundary.
   else if (*iy < m_ymin && *(iy+1) > m_ymin) {
      pt1.setPoint(interpolate(m_ymin, iy, ix), m_ymin );
      pt2.setPoint(*(ix+1), *(iy+1));
   } else if (*iy > m_ymin && *(iy+1) < m_ymin) {
      pt1.setPoint(*ix, *iy);
      pt2.setPoint(interpolate(m_ymin, iy, ix), m_ymin);
   }
   // Interval straddles upper boundary.
   else if (*iy < m_ymax && *(iy+1) > m_ymax) {
      pt1.setPoint(*ix, *iy);
      pt2.setPoint(interpolate(m_ymax, iy, ix), m_ymax);
   } else if (*iy > m_ymax && *(iy+1) < m_ymax) {
      pt1.setPoint(interpolate(m_ymax, iy, ix), m_ymax);
      pt2.setPoint(*(ix+1), *(iy+1));
   }
   distance = separation(pt1, pt2);
   return true;
}

double LinePointRep::
interpolate(double x, std::vector<double>::const_iterator ix,
            std::vector<double>::const_iterator iy) const {
   return (x - *ix)/(*(ix+1) - *ix)*(*(iy+1) - *iy) + *iy;
}

double LinePointRep::
separation(const Point & pt1, const Point & pt2) const {
   return sqrt( (pt1.getX() - pt2.getX())*(pt1.getX() - pt2.getX()) + 
                (pt1.getY() - pt2.getY())*(pt1.getY() - pt2.getY()) );
}

void LinePointRep::
addEndPoints(std::vector<double>::const_iterator ix,
             std::vector<double>::const_iterator iy,
             std::vector<double> & x, std::vector<double> & y) const {
   Point pt1, pt2;
   findEndPoints(ix, iy, pt1, pt2);
   if (m_user_rect->isInBounds((pt1.getX() + pt2.getX())/2.,
                               (pt1.getY() + pt2.getY())/2.) ) {
      x.push_back(pt1.getX());
      y.push_back(pt1.getY());
      x.push_back(pt2.getX());
      y.push_back(pt2.getY());
   }
}

void LinePointRep::
findEndPoints(std::vector<double>::const_iterator ix,
              std::vector<double>::const_iterator iy,
              Point & pt1, Point & pt2) const {
   Point xpt1, xpt2, ypt1, ypt2;
   double xdist, ydist;
   bool xstrad = straddles_x_boundary(ix, iy, xpt1, xpt2, xdist);
   bool ystrad = straddles_y_boundary(ix, iy, ypt1, ypt2, ydist);
   if (xstrad || ystrad) {
      if ( (xstrad && ystrad && xdist < ydist) || xstrad ) {
         pt1 = xpt1;
         pt2 = xpt2;
      } else if ( (xstrad && ystrad && xdist > ydist) || ystrad ) {
         pt1 = ypt1;
         pt2 = ypt2;
      }
   } else {
      pt1.setPoint(*ix, *iy);
      pt2.setPoint(*(ix+1), *(iy+1));
   }
}

bool LinePointRep::
cornerCase(std::vector<double>::const_iterator ix,
           std::vector<double>::const_iterator iy, 
           std::vector<double> & x, std::vector<double> & y) const {
   Point xpt1, xpt2, ypt1, ypt2;
   double dist;
   bool xstrad = straddles_x_boundary(ix, iy, xpt1, xpt2, dist);
   bool ystrad = straddles_y_boundary(ix, iy, ypt1, ypt2, dist);
   if (!m_user_rect->isInBounds(*ix, *iy) &&
       !m_user_rect->isInBounds(*(ix+1), *(iy+1)) &&
       xstrad && ystrad) {
      x.clear();
      y.clear();
      if (xpt1.getX() == m_xmin || xpt1.getX() == m_xmax) {
         x.push_back(xpt1.getX());
         y.push_back(xpt1.getY());
      } else {
         x.push_back(xpt2.getX());
         y.push_back(xpt2.getY());
      }
      if (ypt1.getY() == m_ymin || ypt1.getY() == m_ymax) {
         x.push_back(ypt1.getX());
         y.push_back(ypt1.getY());
      } else {
         x.push_back(ypt2.getX());
         y.push_back(ypt2.getY());
      }
      if ( m_user_rect->isInBounds(x[0], y[0]) &&
           m_user_rect->isInBounds(x[1], y[1]) ) {
         return true;
      } else {
         return false;
      }
   }
   return false;
}

bool
LinePointRep::
uses ( Line::Style ) const
{
  return true;
}
