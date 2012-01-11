/** @file

hippodraw::QtXmlNode class implementation

Copyright (C) 2004, 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtXmlNode.cxx,v 1.8 2006/02/07 00:34:20 pfkeb Exp $

*/

#include "QtXmlNode.h"

#include <cassert>

using namespace hippodraw;

QtXmlNode::
QtXmlNode ( const QtXmlNode & )
  : XmlNode ()
{
}

QtXmlNode::
QtXmlNode ()
{
}

QtXmlNode::
~QtXmlNode ()
{
}

void QtXmlNode::appendChild ( const XmlNode & child )
{
  try {
    const QtXmlNode & qtelem 
      = dynamic_cast < const QtXmlNode & > ( child );
    m_node.appendChild ( qtelem.m_node );
  }
  catch ( ... ) {
    assert ( false );
  }
}
