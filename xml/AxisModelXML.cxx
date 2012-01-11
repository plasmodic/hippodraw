/** @file

AxisModelXML class implementation

Copyright (C) 2002-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: AxisModelXML.cxx,v 1.18 2005/10/30 00:10:18 pfkeb Exp $

*/

// for iterator member defect
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "AxisModelXML.h"

#include "AxisTickXML.h"
#include "XmlElement.h"

#include "axes//AxisModelBase.h"
#include "axes//AxisTick.h"

#include <cassert>

using std::string;
using std::vector;

namespace hippodraw {

AxisModelXML::AxisModelXML ( XmlController * controller )
  : BaseXML ( "AxisModel", controller ),
    m_autorange ( "autorange" ),
    m_low ( "low" ),
    m_high ( "high" ),
    m_scale_factor ( "scale_factor" ),
    m_log ( "log" ),
    m_auto_tick ( "autotick" )
{
  m_axistick_xml = new AxisTickXML ( controller );
}

AxisModelXML::
~AxisModelXML ()
{
  delete m_axistick_xml;
}

void AxisModelXML::setAttributes ( XmlElement & tag,
				   const AxisModelBase & model )
{
  bool yes = model.isAutoRanging ();
  if ( yes ) {
    tag.setAttribute ( m_autorange, 1 );
  }
  else {
    tag.setAttribute ( m_autorange, 0 );
  }
  const Range & range = model.getRange ( false );
  tag.setAttribute ( m_low,  range.low()  );
  tag.setAttribute ( m_high, range.high() );

  tag.setAttribute ( m_scale_factor, model.getScaleFactor () );

  if ( model.isLog () == true ) {
    tag.setAttribute ( m_log, 1 );
  }

  yes = model.isAutoTicks ();
  if ( yes ) {
    tag.setAttribute ( m_auto_tick, 1 );
  }
  else {
    tag.setAttribute ( m_auto_tick, 0 );
    createChildren ( tag, model );
  }
}

void
AxisModelXML::
createChildren ( XmlElement & tag, const AxisModelBase & model )
{
  const vector < AxisTick > & ticks = model.getTicks ();
  unsigned int size = ticks.size ();
  for ( unsigned int i = 0; i < size; i++ ) {
    const AxisTick & tick = ticks [ i ];
    XmlElement * element = m_axistick_xml -> createElement ();
    m_axistick_xml -> setAttributes ( *element, tick );
    tag.appendChild ( *element );
    delete element;
  }
}

Axes::Type 
AxisModelXML::
getAxis ( const XmlElement * element, 
	  const std::string & tagname )
{
  string value;
  bool ok = element->attribute ( tagname, value );
  assert ( ok );

  return Axes::convert ( value );
}

bool AxisModelXML::isLog ( const XmlElement * element )
{
  int value;
  bool ok = element->attribute ( m_log, value );
  if ( ok && value != 0 ) return true;

  return false;
}

void AxisModelXML::setAttributes ( AxisModelBase * model,
				   const XmlElement * element )

{
  int value;
  bool ok = element->attribute ( m_autorange, value );
  if ( ok && ( value == 0 ) )  model->setAutoRanging  ( false );

  double low = 0.0;
  ok = element->attribute ( m_low, low );
  double high = 0.0;
  ok = element->attribute ( m_high, high );

  Range range ( low, high );

  model->setRange ( range, false );

  double scale_factor;
  ok = element->attribute ( m_scale_factor, scale_factor );
  if ( ok ) model->setScaleFactor ( scale_factor );

  ok = element -> attribute ( m_auto_tick, value );
  if ( ok ) {
    bool yes = value != 0;
    model -> setAutoTicks ( yes );
    if ( ! yes ) {
      createChildren ( element, model );
    }
  }
}

void
AxisModelXML::
createChildren ( const XmlElement * element, AxisModelBase * model )
{
  vector < AxisTick > ticks;
  AxisTick tick;

  NodeList_t nodelist;
  m_axistick_xml -> fillNodeList ( element, nodelist );
  if ( nodelist.empty () == false ) {
    NodeList_t :: const_iterator first = nodelist.begin ();
    while ( first != nodelist.end() ) {
      XmlElement * element = *first++;
      m_axistick_xml -> setAttributes ( & tick, element );
      ticks.push_back ( tick );
    }
    model -> setTicks ( ticks );
  }
}

} // namespace hippodraw

