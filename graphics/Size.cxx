/**

hippodraw::Size  class implementation

Copyright (C) 2000, 2001, 2003, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Size.cxx,v 1.11 2006/08/20 16:35:23 pfkeb Exp $

*/

#include "Size.h"

using namespace hippodraw;

Size::Size()
  : m_width( 0.0 ), m_height( 0.0 ), m_depth( 0.0 )
{

}

Size::Size( double x, double y )
  : m_width( x ), m_height( y )
{
}

Size::Size( double x, double y, double z )
  : m_width( x ), m_height( y ), m_depth( z )
{
}

void Size::setSize( double width, double height )
{
  m_width = width;
  m_height = height;
}

void Size::setSize( double width, double height, double depth )
{
  m_width = width;
  m_height = height;
  m_depth = depth;
}

void Size::setDepth ( double d )
{
  m_depth = d;
}
