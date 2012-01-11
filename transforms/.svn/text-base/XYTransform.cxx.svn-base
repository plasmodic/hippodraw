/** @file

hippodraw::XYTransform class implementation

Copyright (C) 2001-2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: XYTransform.cxx,v 1.40 2006/12/19 19:12:40 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h" // for CLONE_DEFECT
#endif

#include "XYTransform.h"

#include "UnaryTransform.h"

#include "graphics/Rectangle.h"

using std::vector;

using namespace hippodraw;

XYTransform::XYTransform ( UnaryTransform * x,
			   UnaryTransform * y,
			   UnaryTransform * z )
  : BinaryTransform ( z )
{
  m_x = x;
  m_y = y;
// The following verboseness for VC++ in debug mode.
  const std::string xname = m_x->name ();
  const std::string yname = m_y->name ();
  const std::string zname = z->name ();
  m_name = xname;
  m_name += " ";
  m_name += yname;
  m_name += " ";
  m_name += zname;
}

XYTransform::~XYTransform ()
{
  delete m_x;
  delete m_y;
}

XYTransform::XYTransform ( const XYTransform & t )
  : BinaryTransform ( t )
{
#ifdef CLONE_DEFECT
  m_x = dynamic_cast < UnaryTransform * > ( t.m_x->clone () );
  m_y = dynamic_cast < UnaryTransform * > ( t.m_y->clone () );
#else
  m_x = t.m_x->clone ();
  m_y = t.m_y->clone ();
#endif
}

#ifdef CLONE_DEFECT
TransformBase * XYTransform::clone () const
#else
XYTransform   * XYTransform::clone () const
#endif
{
  return new XYTransform ( *this );
}

bool
XYTransform::
isLinearInXY ( ) const
{
  return m_x -> isLinear () && m_y -> isLinear ();
}

/* virtual */
void XYTransform::transform ( double & x,
			      double & y ) const
{
  m_x->transform ( x );
  m_y->transform ( y );
}

bool
XYTransform::
inverseTransform ( double & x, double & y ) const
{
  m_x->inverseTransform ( x );
  m_y->inverseTransform ( y );

  // Always return true because the return value only
  // make sense with WCSLIB.
  return true;
}

/* virtual */
void
XYTransform::
transform ( std::vector< double > & x,
	    std::vector< double > & y ) const
{
  m_x->transform ( x );
  m_y->transform ( y );
}

double XYTransform::aspectRatio () const
{
  return 0.0;
}

Rect XYTransform::calcRectangle ( const Range & x, 
					    const Range & y ) 
{
  double x_lo = x.low ();
  double x_hi = x.high ();
  double y_lo = y.low ();
  double y_hi = y.high ();

  transform ( x_lo, y_lo );
  transform ( x_hi, y_hi );

  return Rect ( x_lo, y_lo, x_hi - x_lo, y_hi - y_lo );
}

/* virtual */
void XYTransform::validate ( Range & x, Range & y ) const
{
  m_x->validate ( x );
  m_y->validate ( y );
}

/* virtual */
const Range & XYTransform::limitX () const
{
  return m_x->limits ();
}

const Range & XYTransform::limitY () const
{
  return m_y->limits ();
}

TransformBase * XYTransform::xTransform () const
{
  return m_x;
}

TransformBase * XYTransform::yTransform () const
{
  return m_y;
}

const vector < AxisTick > &
XYTransform::
setTicks ( AxisModelBase & model, hippodraw::Axes::Type axis ) 
{
  if ( axis == Axes::X ) {
    return m_x -> setTicks ( model );
  }
  else if ( axis == Axes::Y ) {
    return m_y -> setTicks ( model );
  }
  else {
    return m_z ->setTicks ( model );
  }
}

void
XYTransform::
adjustValues ( AxisModelBase & model,
	       hippodraw::Axes::Type  axes,
	       const Range & limit )
{
  if ( axes == Axes::X )
    m_x -> adjustValues ( model, limit );
  else  if ( axes == Axes::Y )
    m_y -> adjustValues ( model, limit );
  else if ( axes == Axes::Z )
    m_z -> adjustValues ( model, limit );
}
