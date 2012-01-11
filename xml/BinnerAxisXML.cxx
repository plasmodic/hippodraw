/** @file

BinnerAxisXML implementation

Copyright (C) 2003, 2004   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinnerAxisXML.cxx,v 1.7 2005/10/30 00:10:18 pfkeb Exp $

*/

// for truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "BinnerAxisXML.h"

#include "XmlElement.h"

#include "binners/BinnerAxis.h"
#include "binners/BinnerAxisFactory.h"

#include <cassert>

using std::string;

namespace hippodraw {

BinnerAxisXML::BinnerAxisXML ( XmlController * controller )
  : BaseXML ( "BinnerBase", controller ),
    m_width ( "width" ),
    m_high ( "high" ),
    m_low ( "low" )
{
}

XmlElement * 
BinnerAxisXML::
createElement ( const BinnerAxis & binner )
{
  XmlElement * tag = BaseXML::createElement ();
  setAttributes ( tag, binner );

  return tag;
}

void
BinnerAxisXML::
setAttributes ( XmlElement * tag, const BinnerAxis & binner )
{
  const string & type = binner.name();
  tag->setAttribute ( m_type, type );

  const Range & range = binner.getRange ();
  double high = range.high();
  double low = range.low ();

  tag->setAttribute ( m_high, high );
  tag->setAttribute ( m_low, low );

  double width = binner.getBinWidth ();
  tag->setAttribute ( m_width, width );
}

BinnerAxis * 
BinnerAxisXML::
createObject ( const XmlElement * element )
{
  string type;
  bool ok = element->attribute ( m_type, type );
  assert ( ok );

  BinnerAxisFactory * factory = BinnerAxisFactory::instance ();
  BinnerAxis * binner = factory->create ( type );

  double high = 1.0;
  double low = 0.0;
  ok  = element->attribute ( m_high, high );
  ok &= element->attribute ( m_low, low );
  assert ( ok );
  Range range ( low, high );
  binner->setRange ( range, false );

  double width = -1.0;
  ok = element->attribute ( m_width, width );
  assert ( ok );
  binner->setBinWidth ( width );

  return binner;
}

} // namespace hippodraw

