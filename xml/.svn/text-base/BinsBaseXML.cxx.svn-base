/** @file

BinsBaseXML class implementation

Copyright (C) 2003, 2004   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinsBaseXML.cxx,v 1.11 2005/10/30 00:10:18 pfkeb Exp $

*/

// for truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "BinsBaseXML.h"

#include "BinnerAxisXML.h"
#include "XmlElement.h"

#include "binners/BinsBase.h"
#include "binners/BinsFactory.h"

#include <cassert>

using std::list;
using std::string;

namespace hippodraw {

BinsBaseXML::BinsBaseXML ( XmlController * controller )
  : BaseXML ( "BinsBase", controller ),
    m_axis ( "axis" )
{
  m_binnerXML = new BinnerAxisXML ( controller );
}

BinsBaseXML::
~BinsBaseXML ()
{
  delete m_binnerXML;
}

XmlElement * 
BinsBaseXML::
createElement ( const BinsBase & bins )
{
  XmlElement * tag = BaseXML::createElement ();
  const string & type = bins.name();
  tag->setAttribute ( m_type, type );

  createChildren ( *tag, bins );

  return tag;
}


void
BinsBaseXML::
createChildren ( XmlElement & tag, const BinsBase & bins )
{
  const BinnerAxis * binner = bins.getBinnerOn ( Axes::X );
  XmlElement * element = m_binnerXML->createElement ( *binner );
  string axis ( "X" );
  element->setAttribute ( m_axis, axis );
  tag.appendChild ( *element );
  delete element;

  int number = bins.getNumberOfAxes ();
  if ( number < 2 ) return;

  binner = bins.getBinnerOn ( Axes::Y );
  element = m_binnerXML->createElement ( *binner );
  axis = "Y";
  element->setAttribute ( m_axis, axis );
  tag.appendChild ( *element );
  delete element;
}

BinsBase * 
BinsBaseXML::
createObject ( const XmlElement * binsbase )
{
  string type;
  bool ok = binsbase->attribute ( m_type, type );
  assert ( ok );

  BinsFactory * factory = BinsFactory::instance ();
  BinsBase * bins = factory->create ( type );

  list < XmlElement * > nodelist;
  m_binnerXML->fillNodeList ( binsbase, nodelist );

  list < XmlElement * > :: const_iterator first = nodelist.begin();
  while ( first != nodelist.end() ) {
    XmlElement * element = *first++;
    BinnerAxis * binner = m_binnerXML->createObject ( element );
    string axis;
    bool ok = element->attribute ( m_axis, axis );
    assert ( ok );

    Axes::Type type = axis == "X" ? Axes::X : Axes::Y;
    bins->setBinnerOn ( binner, type );
  }

  return bins;
}

} // namespace hippodraw

