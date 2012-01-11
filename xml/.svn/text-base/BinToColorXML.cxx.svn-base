/** @file

BinToColorXML implementation

Copyright (C) 2004, 2005 The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: BinToColorXML.cxx,v 1.6 2007/01/21 21:10:55 pfkeb Exp $

*/

// for dll interface warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "BinToColorXML.h"

#include "XmlController.h"
#include "XmlDocument.h"
#include "XmlElement.h"

#include "colorreps/BinToColor.h"
#include "colorreps/BinToColorFactory.h"

#include <cassert>

using std::list;
using std::string;
using std::vector;

namespace hippodraw {

BinToColorXML::BinToColorXML ( XmlController * controller )
  : BaseXML ( "BinToColor", controller ),
    m_ctrlpt ( "CtrlPt" ),
    m_point ( "point" )
{
}

XmlElement * BinToColorXML::createElement ( const BinToColor & btc )
{
  XmlElement * tag = BaseXML::createElement ();
  tag -> setAttribute ( m_type, btc.name () );
  bool yes = btc.hasControlPoints ();
  if ( yes ) {
    const vector < double > & points = btc.getControlPoints ();
    unsigned int size = points.size ();
    for ( unsigned int i = 0; i < size; i++ ) {
      XmlElement * element 
	= XmlController::m_xml_doc -> createElement ( m_ctrlpt );
      element -> setAttribute ( m_point, points [ i ] );
      tag -> appendChild ( *element );
      delete element;
    }
  }

  return tag;
}

BinToColor * BinToColorXML::createObject ( const XmlElement * element )
{
  string type;
  bool ok = element -> attribute ( m_type, type );
  assert ( ok );
  BinToColor * btc = 0;
  BinToColorFactory * factory = BinToColorFactory::instance ();
  try {
    btc = factory -> create ( type );
  }
  catch ( const FactoryException & ) {
    btc = factory -> create ( "Rainbow" ); // default
  }

  ok = btc -> hasControlPoints ();
  if ( ok ) {
    vector < double > points;
    list < XmlElement * > nodelist;
    element -> fillNodeList ( m_ctrlpt, nodelist );
    list < XmlElement * > :: const_iterator first = nodelist.begin();
      while ( first != nodelist.end() ) {
	XmlElement * pt_element = *first++;
	double point;
	ok = pt_element -> attribute ( m_point, point );
	points.push_back ( point );
      }
    btc -> setControlPoints ( points );
  }

  return btc;
}

} // namespace hippodraw

