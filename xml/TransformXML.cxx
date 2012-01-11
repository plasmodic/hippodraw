/** @file

TransformXML implementation

Copyright (C) 2002, 2003, 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: TransformXML.cxx,v 1.9 2005/10/30 00:10:19 pfkeb Exp $

*/

// for truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "TransformXML.h"

#include "XmlElement.h"

#include "transforms/TransformBase.h"
#include "transforms/TransformFactory.h"

#include <cassert>

using std::string;

namespace hippodraw {

TransformXML::TransformXML ( XmlController * controller )
  : BaseXML ( "Transform", controller )
{
}

XmlElement * TransformXML::createElement ( const TransformBase & transform )
{
  XmlElement * tag = BaseXML::createElement ();
  const string & type = transform.name();
  tag->setAttribute ( m_type, type );

  return tag;
}

TransformBase * TransformXML::createObject ( const XmlElement * element )
{
  string type;
  bool ok = element->attribute ( m_type, type );
  assert ( ok );

  TransformFactory * factory = TransformFactory::instance ();
  TransformBase * transform = factory->createTransform ( type );

  return transform;
}

} // namespace hippodraw

