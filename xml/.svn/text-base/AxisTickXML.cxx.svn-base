/** @file

AxisTickXML class implementation

Copyright (C) 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: AxisTickXML.cxx,v 1.2 2005/10/30 00:10:18 pfkeb Exp $

*/

#include "AxisTickXML.h"

#include "XmlElement.h"

#include "axes/AxisTick.h"

using std::string;

namespace hippodraw {

AxisTickXML::
AxisTickXML ( XmlController * controller )
  : BaseXML ( "AxisTick", controller ),
    m_value ( "value" ),
    m_label ( "label" )
{
}

void
AxisTickXML::
setAttributes ( XmlElement & tag,
		const AxisTick & tick )
{
  double value = tick.value ();
  tag.setAttribute ( m_value, value );

  const string & label = tick.content ();
  tag.setAttribute ( m_label, label );
}

void
AxisTickXML::
setAttributes ( AxisTick * tick,
		const XmlElement * element )

{
  double value;
  bool ok = element -> attribute ( m_value, value );
  tick -> setValue ( value );

  string label;
  ok = element -> attribute ( m_label, label );
  tick -> setContent ( label );
}

} // namespace hippodraw

