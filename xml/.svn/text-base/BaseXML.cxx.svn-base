/** @file

BaseXML implementation

Copyright (C) 2002-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BaseXML.cxx,v 1.28 2005/10/30 00:10:18 pfkeb Exp $

*/

// for truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "BaseXML.h"

#include "XmlController.h"
#include "XmlDocument.h"
#include "XmlElement.h"

#include <cassert>

using std::string;
using std::swap;

namespace hippodraw {

BaseXML::
BaseXML ( const std::string & tagname, XmlController * controller )
  : m_tagname ( tagname ),
    m_type ( "type" ),
    m_id ( "id" ),
    m_controller ( controller )
{
}

BaseXML::~BaseXML ( )
{
}

XmlElement * BaseXML::createElement ( )
{
  return XmlController::m_xml_doc->createElement ( m_tagname );
}

const string & BaseXML::tagName () const
{
  return m_tagname;
}

const XmlElement * BaseXML::getNode ( const XmlElement * parent ) const
{
  return parent->getNode ( m_tagname );
}

void BaseXML::fillNodeList ( const XmlElement * element, 
			     std::list < XmlElement * > & nodelist )
{
  element->fillNodeList ( m_tagname, nodelist );
}

void BaseXML::setId ( XmlElement & tag, int id )
{
  tag.setAttribute ( m_id, id );
}

} // namespace hippodraw

