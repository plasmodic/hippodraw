/** @file

PointRepXML class implementation

Copyright (C) 2002-2005  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: PointRepXML.cxx,v 1.41 2007/01/24 21:41:49 xiefang Exp $

*/

// inconsistent dll linkage
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "PointRepXML.h"
#include "XmlController.h"

#include "BinToColorXML.h"
#include "ColorXML.h"
#include "XmlElement.h"

#include "colorreps/BinToColor.h"
#include "colorreps/BinToColorFactory.h"
#include "reps/RepBase.h"
#include "reps/PointRepFactory.h"
#include "reps/TextRepFactory.h"

#include <cassert>

using std::string;

namespace hippodraw {

PointRepXML::PointRepXML ( XmlController * controller )
  : BaseXML ( "PointRep", controller ),
    a_size ( "size" ),
    a_style ( "style" ),
    a_xerror ( "xerror" ),
    a_yerror ( "yerror" ),
    a_text ( "text" ),
    a_value_transform ( "value_transform" )
{
  m_color_xml = new ColorXML ( controller );
  m_bintocolor_xml = new BinToColorXML ( controller );
}

PointRepXML::
~PointRepXML ()
{
  delete m_color_xml;
  delete m_bintocolor_xml;
}

XmlElement * PointRepXML::createElement ( const RepBase & rep )
{
  XmlElement * tag = BaseXML::createElement ();

  const Color & color = rep.getColor ();
  XmlElement * element = m_color_xml->createElement ( color );
  tag->appendChild ( *element );
  delete element;

  const BinToColor * btc = rep.getValueTransform ();
  if ( btc != 0 ) {
    element = m_bintocolor_xml -> createElement ( *btc );
    tag -> appendChild ( *element );
    delete element;
  }

  setAttributes ( *tag, rep );

  return tag;
}

void PointRepXML::setAttributes ( XmlElement & tag,
				  const RepBase & rep )
{
  const void * addr = reinterpret_cast < const void * > ( & rep );
  int id = m_controller -> getId ( addr );
  setId ( tag, id );

  tag.setAttribute ( m_type, rep.name() );
  tag.setAttribute ( a_size,  rep.size () );
  tag.setAttribute ( a_style, rep.getStyle () );

  string value =  rep.xError() ? "yes" : "no";
  tag.setAttribute ( a_xerror,  value );
  value = rep.yError() ? "yes" : "no";
  tag.setAttribute ( a_yerror,  value );

  tag.setAttribute ( a_text, rep.getText () );

}

RepBase * PointRepXML::createObject ( const XmlElement * element )
{
  string type;
  bool ok = element->attribute ( m_type, type );
  assert ( ok );

  RepBase * rep = 0;
  PointRepFactory * factory = PointRepFactory::instance ();
  try { 
    rep = factory->create ( type );
  } catch ( const FactoryException & ) {
    TextRepFactory * factory = TextRepFactory::instance ();
      rep = factory->create ( type );
  }

  if ( rep == 0 ) return 0;

  const XmlElement * color_element = m_color_xml->getNode ( element );
  Color * color = m_color_xml->createObject ( color_element );
  rep->setColor ( *color );

  const XmlElement * btc_element = m_bintocolor_xml -> getNode ( element );
  if ( btc_element != 0 ) {
    BinToColor * btc = m_bintocolor_xml -> createObject ( btc_element );
    rep -> setValueTransform ( btc );
  }

  float size = 1.;
  ok = element->attribute ( a_size, size );
  rep->setSize ( size );

  unsigned int style = 0;
  ok = element -> attribute ( a_style, style );
  if ( ok ) rep -> setStyle ( style );

  string xerror = "no";
  ok = element->attribute ( a_xerror, xerror );
  if ( xerror ==  "yes" ) {
    rep->setErrorOn ( Axes::X, true );
  }

  string yerror = "no";
  ok = element->attribute ( a_yerror, yerror );
  if ( yerror == "yes" ) {
    rep->setErrorOn ( Axes::Y, true );
  }

  string text = "";
  ok = element->attribute ( a_text, text );
  if ( text != "" ) {
    rep->setText(text);
  }


  // The following for backward compatiblity
  string transform;
  ok = element -> attribute ( a_value_transform, transform );
  if ( ok ) {
    BinToColorFactory * factory = BinToColorFactory::instance ();
    BinToColor * btc = factory -> create ( transform );
    rep -> setValueTransform ( btc );
  }

  return rep;
}

} // namespace hippodraw

