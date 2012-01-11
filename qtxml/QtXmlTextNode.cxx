/** @file

hippodraw::QtXmlTextNode class implementation

Copyright (C) 2004-2006   The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: QtXmlTextNode.cxx,v 1.10 2006/02/07 00:34:21 pfkeb Exp $

*/

#include "QtXmlTextNode.h"

#include <qstring.h>

using namespace hippodraw;

QtXmlTextNode::
QtXmlTextNode ( const QtXmlTextNode & node )
  : XmlNode (),
    XmlTextNode (),
    QtXmlNode ()
{
  m_text_node = node.m_text_node;
  m_node = m_text_node;
}

QtXmlTextNode::
QtXmlTextNode ( const QDomText & node )
{
  m_text_node = node;
  m_node = m_text_node;
}

QtXmlTextNode::
~QtXmlTextNode ()
{
}

void
QtXmlTextNode::
setData ( const std::string & data )
{
  QString qstring ( data.c_str() );
  m_text_node.setData ( qstring );
}
