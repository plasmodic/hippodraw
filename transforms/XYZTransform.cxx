/** @file

XYZTransform class implementation

Copyright (C)  2001, 2002  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: XYZTransform.cxx,v 1.13 2006/05/27 00:00:12 xiefang Exp $

*/

#include "XYZTransform.h"

#include "UnaryTransform.h"

using std::vector;

namespace hippodraw {

XYZTransform::XYZTransform ( UnaryTransform * x,
			     UnaryTransform * y,
			     UnaryTransform * z )
{
  m_name = "XYZTransform";
  m_x = x;
  m_y = y;
  m_z = z;
}

XYZTransform::~XYZTransform ()
{
  delete m_x;
  delete m_y;
  delete m_z;
}

XYZTransform::XYZTransform ( const XYZTransform & t )
  : XYZTransformBase ( t )
{
#ifdef CLONE_DEFECT
  m_x = dynamic_cast < UnaryTransform * > ( t.m_x->clone () );
  m_y = dynamic_cast < UnaryTransform * > ( t.m_y->clone () );
  m_z = dynamic_cast < UnaryTransform * > ( t.m_z->clone () );
#else
  m_x = t.m_x->clone ();
  m_y = t.m_y->clone ();
  m_z = t.m_z->clone ();
#endif
}

#ifdef CLONE_DEFECT
TransformBase * XYZTransform::clone () const
#else
XYZTransform  * XYZTransform::clone () const
#endif
{
  return new XYZTransform ( *this );
}

/* virtual */
void XYZTransform::transform ( double & x,
			       double & y,
			       double & z ) const
{
  m_x->transform ( x );
  m_y->transform ( y );
  m_z->transform ( z );
}

/* virtual */
void
XYZTransform::
transform ( std::vector< double > & x,
	    std::vector< double > & y,
	    std::vector< double > & z ) const
{
  m_x->transform ( x );
  m_y->transform ( y );
  m_z->transform ( z );
}

/* virtual */
void XYZTransform::validate ( Range & x, Range & y, Range & z ) const
{
  m_x->validate ( x );
  m_y->validate ( y );
  m_z->validate ( z );
}

/* virtual */
const Range & XYZTransform::limitX () const
{
  return m_x->limits ();
}

/* virtual */
const Range & XYZTransform::limitY () const
{
  return m_y->limits ();
}

/* virtual */
const Range & XYZTransform::limitZ () const
{
  return m_z->limits ();
}

TransformBase * XYZTransform::xTransform () const
{
  return m_x;
}

TransformBase * XYZTransform::yTransform () const
{
  return m_y;
}

TransformBase * XYZTransform::zTransform () const
{
  return m_z;
}

bool XYZTransform::isLinearInZ () const
{
  return m_z->isLinear();
}


} // namespace hippodraw

