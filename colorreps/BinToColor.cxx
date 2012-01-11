/** @file

BinToColor class implementation

Copyright (C) 2000, 2003-2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinToColor.cxx,v 1.23 2007/01/21 21:10:54 pfkeb Exp $

*/

#include "BinToColor.h"
using std :: string;

namespace hippodraw {

BinToColor::BinToColor ( const char * name )
  : m_name ( name )
{
}

// BinToColor::
// BinToColor ( const std::string & name )
//   : m_name ( name )
// {
// }

BinToColor::BinToColor( const BinToColor & bin_to_color )
  : m_name ( bin_to_color.m_name ),
    m_dv ( bin_to_color.m_dv ),
    m_vmin ( bin_to_color.m_vmin )
{
  m_control_points = bin_to_color.m_control_points;
}

BinToColor::~BinToColor()
{
}

const std::string &
BinToColor::
name () const
{
  return m_name;
}

void BinToColor::setRange( const Range & r )
{
  m_dv = r.length();
  m_vmin = r.low ();
}

Range
BinToColor::
getRange () const
{
  Range range ( m_vmin, m_vmin + m_dv );

  return range;
}

bool
BinToColor::
hasControlPoints () const
{
  return false;
}

const std::vector < double > &
BinToColor::
getControlPoints () const
{
  return m_control_points;
}

void
BinToColor::
setControlPoints ( const std::vector < double > & points )
{
  m_control_points = points;
}

bool
BinToColor::
isUserDefined () const
{
  return false;
}

// added by Tao Aug 10,2006
// return False in the base class BinToColor, reimplemented in BinToBlack and BinToGreyScale
bool
BinToColor::
acceptChangeColor () const
{ 
	return false;
}

} // namespace hippodraw

