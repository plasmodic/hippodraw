/** @file

hippodraw::Rect  class implementation

The concept was strongly influenced by NeXTStep, circa 1989.

Copyright (C) 2000, 2001, 2003, 2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Rectangle.cxx,v 1.33 2007/01/29 17:30:37 pfkeb Exp $

*/

#ifdef _MSC_VER
// Include max() and min() missing from MicroSoft Visual C++.
#include "msdevstudio/MSconfig.h"
#endif //_MSC_VER

#include "Rectangle.h"

#ifdef _MSC_VER
#define isnan _isnan
#endif

//To have isnan.
#ifdef __APPLE__
#define _GLIBCPP_USE_C99 1
#endif

#include <algorithm>
#include <functional>
#include <iostream>

#include <cmath>
#include <cfloat>

#ifdef __APPLE__
using std::isnan;
#endif

using std::bind2nd;
using std::greater;
using std::less;
using std::max;
using std::min;
using std::replace_if;
using std::vector;

using namespace hippodraw;

Rect::Rect()
  : m_origin(), m_size()
{
}

Rect::
Rect( double x, double y, double width, double height )
  : m_origin(), m_size()
{
  setRect( x, y, width, height );
}

Rect::Rect( double x, double y, double z,
		      double width, double height, double depth )
  : m_origin(), m_size()
{
  setRect( x, y, z, width, height, depth );
}

void Rect::setRect( double x, double y, double width, double height )
{
  m_origin.setPoint( x, y );
  m_size.setSize( width, height );
}

void Rect::setRect( double x, double y, double z,
			 double width, double height, double depth )
{
  m_origin.setPoint( x, y, z );
  m_size.setSize( width, height, depth );
}

void Rect::moveBy ( double x, double y )
{
  m_origin.moveBy ( x, y );
}

const Point & Rect::getOrigin() const
{
  return m_origin;
}

const Size & Rect::getSize() const
{
  return m_size;
}

double Rect::getZ() const
{
  return getOrigin().getZ();
}

void Rect::setZ ( double z ) 
{
  m_origin.setZ ( z );
}

double Rect::getWidth() const
{
  return m_size.getWidth();
}

double Rect::getHeight() const
{
  return m_size.getHeight();
}

double Rect::getDepth() const
{
  return getSize().getDepth();
}


void Rect::setDepth ( double d )
{
  m_size.setDepth ( d );
}

/** @todo The multicative factor the implementation might be
    considered a kludge. Might there be a better way.

    @todo The final solution for the comparison works when the depth
    is a very small number compared the the values.  Is there a better
    numeric way.
*/
bool Rect::isInDepth ( double z1 ) const
{
  bool yes = false;
  double z_lo = m_origin.getZ ();

  if ( z1 >= z_lo && 
       ( z1 )  <= z_lo + 1.00001*m_size.getDepth() ) yes = true;

  return yes;
}

/** @todo Use if ( x!=1 ) to est for nan.
 */
bool Rect::isInBounds ( double x1, double y1 ) const
{
  if ( isnan ( x1 ) || isnan ( y1 ) ) return false;

  double x_lo = getX ();
  double y_lo = getY ();

  double x_hi = x_lo + getWidth();
  double y_hi = y_lo + getHeight();

  if( x1 < x_lo || x1 > x_hi ||
      y1 < y_lo || y1 > y_hi ) return false;

  return true;
}

bool Rect::isInBounds ( double x1, double y1, double z1 ) const
{
  double x_lo = getX ();
  double y_lo = getY ();
  double z_lo = getZ ();

  double x_hi = x_lo + getWidth();
  double y_hi = y_lo + getHeight();
  double z_hi = z_lo + getDepth();

  if( x1 < x_lo || x1 > x_hi ||
      y1 < y_lo || y1 > y_hi ||
      z1 < z_lo || z1 > z_hi ) return false;

  return true;
}

/* Note: the if statements were measured faster than the use of max()
   and min(). 
*/
void Rect::makeInBounds ( double & x1, double & y1 ) const
{
  double x_lo = m_origin.getX ();
  double y_lo = m_origin.getY ();

  double x_hi = x_lo + m_size.getWidth();
  double y_hi = y_lo + m_size.getHeight();

  if ( x1 < x_lo ) {
    x1 = x_lo;
  } 
  else if ( x1 > x_hi ) {
    x1 = x_hi;
  }
  if ( y1 < y_lo ) {
    y1 = y_lo;
  }
  else if ( y1 > y_hi ) {
    y1 = y_hi;
  }
}

void
Rect::
makeInBounds ( double & x1, double & y1, double & z1 ) const
{
  double x_lo = m_origin.getX ();
  double y_lo = m_origin.getY ();
  double z_lo = m_origin.getZ ();

  double x_hi = x_lo + m_size.getWidth();
  double y_hi = y_lo + m_size.getHeight();
  double z_hi = z_lo + m_size.getDepth();

  x1 = max ( x1, x_lo );
  x1 = min ( x1, x_hi );

  y1 = max ( y1, y_lo );
  y1 = min ( y1, y_hi );

  z1 = max ( z1, z_lo );
  z1 = min ( z1, z_hi );
}

void
Rect::
makeInBounds ( std::vector< double > & x, 
	       std::vector< double > & y  ) const
{
  double lo = getX ();
  double hi = lo + getWidth ();

  replace_if ( x.begin (), x.end (),
	       bind2nd ( less< double > (), lo ), lo );

  replace_if ( x.begin (), x.end (),
	       bind2nd ( greater< double > (), hi ), hi );

  lo = getY ();
  hi = lo + getHeight ();

  replace_if ( y.begin (), y.end (),
	       bind2nd ( less< double > (), lo ), lo );

  replace_if ( y.begin (), y.end (),
	       bind2nd ( greater< double > (), hi ), hi );

}

void
Rect::
makeInBounds ( std::vector< double > & x, 
	       std::vector< double > & y,
	       std::vector< double > & z  ) const
{
  double lo = getX ();
  double hi = lo + getWidth ();

  replace_if ( x.begin (), x.end (),
	       bind2nd ( less< double > (), lo ), lo );

  replace_if ( x.begin (), x.end (),
	       bind2nd ( greater< double > (), hi ), hi );

  lo = getY ();
  hi = lo + getHeight ();

  replace_if ( y.begin (), y.end (),
	       bind2nd ( less< double > (), lo ), lo );

  replace_if ( y.begin (), y.end (),
	       bind2nd ( greater< double > (), hi ), hi );

  lo = getZ ();
  hi = lo + getDepth ();

  replace_if ( z.begin (), z.end (),
	       bind2nd ( less< double > (), lo ), lo );

  replace_if ( z.begin (), z.end (),
	       bind2nd ( greater< double > (), hi ), hi );

}
