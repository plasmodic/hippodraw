/** @file

BinaryTransform class implementation

Copyright (C) 2001-2003   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinaryTransform.cxx,v 1.20 2006/05/27 00:00:12 xiefang Exp $

*/

#include "BinaryTransform.h"

#include "UnaryTransform.h"

#include <cassert>

namespace hippodraw {

/** Should never be called.. Sets the function name to "nil".
    Should be overriden by derived class.  If function name
    is "nil", then that indicated error condition that default
    constructor was called directly. */
BinaryTransform::BinaryTransform ()
{
  m_name = "nil";

  m_needs_grid = false;
  m_needs_x_ticks = true;
  m_needs_y_ticks = true;
  m_is_periodic = false;
  
  m_z = 0;
}

BinaryTransform::BinaryTransform ( UnaryTransform * z,
				   bool is_periodic,
				   bool needs_grid,
				   bool needs_x_ticks,
				   bool needs_y_ticks )
{
  m_z = z;
  m_name = m_z->name();
  
  m_needs_grid = needs_grid;
  m_needs_x_ticks = needs_x_ticks;
  m_needs_y_ticks = needs_y_ticks;
  m_is_periodic = is_periodic;
  
}

BinaryTransform::BinaryTransform ( const BinaryTransform & t )
  : TransformBase ( t )
{
  
  if ( t.m_z == 0 ){
    m_z = 0;
    return;
  }

  assert ( t.m_z );

#ifdef CLONE_DEFECT
  m_z = dynamic_cast < UnaryTransform * > ( t.m_z->clone () );
#else
  m_z = t.m_z->clone ();
#endif
    
  m_needs_grid    = t.needsGrid();
  m_needs_x_ticks = t.needsXTicks();
  m_needs_y_ticks = t.needsYTicks();
  m_is_periodic   = t.isPeriodic();
  
}

BinaryTransform::~BinaryTransform ()
{
  delete m_z;
}

double BinaryTransform::aspectRatio () const
{
  return 0.0;
}

TransformBase * BinaryTransform::zTransform () const
{
  return m_z;
}

void
BinaryTransform::
setZTransform ( TransformBase * transform )
{
  UnaryTransform * t = dynamic_cast < UnaryTransform * > ( transform );

  m_z = t;
}

void BinaryTransform::transformZ ( double & z ) const
{
  assert ( m_z );
  m_z->transform ( z );
}

void BinaryTransform::inverseTransformZ ( double & z ) const
{
  assert ( m_z );
  m_z->inverseTransform ( z );
}

const Range & BinaryTransform::limitZ () const
{
  assert ( m_z );
  return m_z->limits();
}

bool  BinaryTransform::needsGrid() const
{
  return m_needs_grid;
}

void  BinaryTransform::setNeedsGrid( bool  needs_grid )
{
  m_needs_grid = needs_grid;
}

bool  BinaryTransform::needsXTicks() const
{
  return m_needs_x_ticks;
}

void  BinaryTransform::setNeedsXTicks( bool needs_x_ticks ) 
{
  m_needs_x_ticks = needs_x_ticks;
}

bool  BinaryTransform::needsYTicks() const
{
  return m_needs_y_ticks;
}

void  BinaryTransform::setNeedsYTicks( bool needs_y_ticks ) 
{
  m_needs_y_ticks = needs_y_ticks;
}

bool BinaryTransform::isPeriodic() const
{
  return m_is_periodic;
}

bool BinaryTransform::isLinearInZ() const
{
  return m_z->isLinear();
}


} // namespace hippodraw
