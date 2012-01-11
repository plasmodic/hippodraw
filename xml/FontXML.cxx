/** @file

FontXML class implementation

Copyright (C) 2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FontXML.cxx,v 1.4 2005/10/30 00:10:19 pfkeb Exp $

*/

#include "FontXML.h"

#include "XmlElement.h"

#include "graphics/FontBase.h"

#include <cassert>

using std::string;

namespace hippodraw {

FontXML::FontXML ( XmlController * controller )
  : BaseXML ( "Font", controller ),
    m_family ( "family" ),
    m_italic ( "italic" ),
    m_size ( "size" ),
    m_weight ( "weight" )
{
}

void
FontXML::
setAttributes ( XmlElement & tag, const FontBase & font )
{
  string family = font.family ();
  tag.setAttribute ( m_family, family );

  bool italic = font.italic ();
  tag.setAttribute ( m_italic, italic );

  int size = font.pointSize ();
  tag.setAttribute ( m_size, size );

  int weight = font.weight ();
  tag.setAttribute ( m_weight, weight );
}

void
FontXML::
setAttributes ( const XmlElement * element, FontBase * font )
{
  string family;
  bool ok = element -> attribute ( m_family, family );
  font -> setFamily ( family );

  bool italic;
  ok = element -> attribute ( m_italic, italic );
  assert ( ok );
  font -> setItalic ( italic );

  int size;
  ok = element -> attribute ( m_size, size );
  font -> setPointSize ( size );

  int weight;
  ok = element -> attribute ( m_weight, weight );
  font -> setWeight ( weight );

}

Axes::Type
FontXML::
getAxis ( const XmlElement * element, const std::string & tagname )
{
  string value;
  bool ok = element -> attribute ( tagname, value );
  assert ( ok );

  return Axes::convert ( value );
}

} // namespace hippodraw

