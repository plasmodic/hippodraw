/** @file

hippodraw::Color class implementation

Copyright (C) 2000, 2001, 2005, 2006  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Color.cxx,v 1.26 2006/09/29 21:33:41 xiefang Exp $

*/

#include "Color.h"

using std::string;

using namespace hippodraw;

// Used to generate a new color for composite plotter.
int Color::colorIndex=0;

Color::ColorMapByName_t Color::s_color_by_name;
std::vector < std::string > Color::s_color_names;

Color::Color ( int red, int green, int blue )
  : m_red( red ), m_green( green ), m_blue( blue )
{
}

Color::
Color ( Color::Value value )
{
  setColor ( value );
}

void
Color::
addColor ( const char * name, const Color & color )
{
  string s ( name );
  s_color_by_name [ s ] = color;
}

void
Color::
setColor ( Color::Value value )
{
  switch ( value ) {
  case Color::red :
    setColor ( "red" );
    break;
  case Color::green :
    setColor ( "green" );
    break;
  case Color::blue :
    setColor ( "blue" );
    break;
  case Color::cyan :
    setColor ( "cyan" );
    break;
  case Color::magenta :
    setColor ( "magenta" );
    break;
  case Color::yellow :
    setColor ( "yellow" );
    break;
  case Color::orange :
    setColor ( "orange" );
    break;
  case Color::black :
    setColor ( "black" );
    break;
  case Color::darkgray :
    setColor ( "darkgray" );
    break;
  case Color::lightgray :
    setColor ( "lightgray" );
    break;
  case Color::white :
    setColor ( "white" );
    break;
  default :
    setColor ( "black" );
    break;
  }
}

void
Color::
initColorByNameMap ()
{
  addColor ( "red",       Color ( 255,   0,   0 ) );
  addColor ( "green",     Color (   0, 255,   0 ) );
  addColor ( "blue",      Color (   0,   0, 255 ) );
  addColor ( "cyan",      Color (   0, 255, 255 ) );
  addColor ( "magenta",   Color ( 255,   0, 255 ) );
  addColor ( "yellow",    Color ( 255, 255,   0 ) );
  addColor ( "orange",    Color ( 255, 165,   0 ) );
  addColor ( "black",     Color (   0,   0,   0 ) );
  addColor ( "darkgray",  Color ( 152, 152, 152 ) );
  addColor ( "lightgray", Color ( 211, 211, 211 ) );
  addColor ( "white",     Color ( 255, 255, 255 ) );
}

Color::
Color ( const std::string & name )
{
  setColor ( name );
}

void
Color::
setColor ( const std::string & name )
{
  bool yes = isValid ( name );
  if ( yes ) {
    ColorMapByName_t :: const_iterator first = s_color_by_name.find ( name );
    const Color color = first -> second;
    m_red = color.m_red;
    m_green = color.m_green;
    m_blue = color.m_blue;
  }
  else { // black
    m_red = 0;
    m_green = 0;
    m_blue = 0; 
  }
}

bool
Color::
isValid ( const std::string & name )
{
  if ( s_color_by_name.empty () == true ) {
    initColorByNameMap ();
  }

  ColorMapByName_t::const_iterator first = s_color_by_name.find ( name );

  return first != s_color_by_name.end ();
}

const std::vector < std::string > &
Color::
colorNames ()
{
  // Make sure the list is up to date
  s_color_names.clear ();
  ColorMapByName_t::const_iterator it = s_color_by_name.begin();
  while ( it != s_color_by_name.end () ) {
    const string & name = (it++) -> first;
    s_color_names.push_back ( name );
  }

  return s_color_names;
}

void Color::setColor ( int red, int green, int blue )
{
  m_red = red;
  m_green = green;
  m_blue = blue;
}

int Color::getRed () const
{
  return m_red;
}

int Color::getGreen () const
{
  return m_green;
}

int Color::getBlue () const
{
  return m_blue;
}

// Get a color from red, green, blue, magenta and orange.
// Use to plot multiple curves in a plotter.
Color::Value Color::getColor()
{
  Color::Value retColor;

  switch (colorIndex)
    {
    case 0: retColor= red; break;
    case 1: retColor= green; break;
    case 2: retColor= blue; break;
    case 3: retColor= magenta; break;
    default: retColor= orange;
    }

  if (colorIndex==4) colorIndex=colorIndex-4;
  else colorIndex++;
  
  return retColor;
}


bool
Color::operator==(Color c)
{
  return ((m_red == c.getRed()) && 
	  (m_green == c.getGreen()) && 
	  (m_blue == c.getBlue()));
}

