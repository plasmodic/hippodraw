/** @file

ColorXML implementation

Copyright (C) 2002, 2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: ColorXML.cxx,v 1.6 2005/10/30 00:10:19 pfkeb Exp $
*/

// for dll interface warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "ColorXML.h"

#include "XmlElement.h"

#include "graphics/Color.h"

using std::string;

namespace hippodraw {

ColorXML::ColorXML ( XmlController * controller )
  : BaseXML ( "Color", controller ),
    m_red ( "red" ),
    m_green ( "green" ),
    m_blue ( "blue" )
{
}

XmlElement * ColorXML::createElement ( const Color & color )
{
  XmlElement * tag = BaseXML::createElement ();

  tag->setAttribute ( m_red,   color.getRed () );
  tag->setAttribute ( m_green, color.getGreen () );
  tag->setAttribute ( m_blue,  color.getBlue () );

  return tag;
}

Color * ColorXML::createObject ( const XmlElement * element )
{
  int red = 0;
  bool ok = element->attribute ( m_red, red );

  int green = 0;
  ok = element->attribute ( m_green, green );

  int blue = 0;
  ok = element->attribute ( m_blue, blue );

  Color * color = new Color ();
  color->setColor ( red, green, blue );

  return color;
}

} // namespace hippodraw

