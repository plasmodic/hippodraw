/** @file

BinToColorMap class implementation

Copyright (C) 2000, 2003-2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinToColorMap.cxx,v 1.3 2007/01/30 22:49:58 pfkeb Exp $

*/

#include "BinToColorMap.h"

#include <cmath>
using std :: string;

#include <cassert>

namespace hippodraw {

BinToColorMap::BinToColorMap ( const char * name )
  : BinToColor ( name )
{
  m_control_points.push_back ( .5 );
  calcGamma ( );
}

bool
BinToColorMap::
hasControlPoints () const
{
  return true;
}

void
BinToColorMap::
setControlPoints ( const std::vector < double > & points )
{
  m_control_points[0] = points[0];
  calcGamma ( );
}

void
BinToColorMap::
calcGamma ( )
{
  double f = m_control_points[0];
  m_gamma = std::pow ( 100., ( f - 0.5) );
}

BinToColorMap::BinToColorMap( const BinToColorMap & bin_to_color )
  : BinToColor ( bin_to_color )

{
  m_reds = bin_to_color.m_reds;
  m_greens = bin_to_color.m_greens;
  m_blues = bin_to_color.m_blues;
  m_control_points = bin_to_color.m_control_points;
  calcGamma ();
}



BinToColorMap::~BinToColorMap()
{
}

void
BinToColorMap::
doubleToColor ( double value, Color & color ) const
{
  double tmp = ( value - m_vmin ) / m_dv;
  int index = static_cast < int > (255. * std::pow( tmp, m_gamma) );
  assert ( index >= 0 &&  index < 256 );
  color.setColor ( m_reds[index], m_greens[index], m_blues[index] );
}

} // namespace hippodraw

