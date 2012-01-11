/** @file

hippodraw::TupleCutXML implementation

Copyright (C)  2002, 2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: TupleCutXML.cxx,v 1.21 2006/10/18 17:22:26 pfkeb Exp $

*/

// for iterator member defect
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "TupleCutXML.h"

#include "XmlController.h"
#include "XmlDocument.h"
#include "XmlElement.h"

#include "datasrcs//TupleCut.h"

#include <cassert>

using namespace hippodraw;

TupleCutXML::TupleCutXML ( XmlController * controller )
  : BaseXML ( "TupleCut", controller ),
    m_low ( "low" ),
    m_high ( "high" ),
    m_invert ( "invert" ),
    m_column ( "column" ),
    m_dim ( "dimension" ),
    m_axis ( "axis" )
{
}

XmlElement *
TupleCutXML::
createElement ( unsigned int i, const TupleCut & cut )
{
  XmlElement * tag = BaseXML::createElement ();

  const void * addr = reinterpret_cast < const void * > ( & cut );
  int id = m_controller -> getId ( addr );
  setId ( *tag, id );

  tag -> setAttribute ( m_dim, -1 ); // to flag not obsolete TupleCut
  XmlElement * element 
    = XmlController::m_xml_doc -> createElement ( "TupleCutAxis" );
  element -> setAttribute ( m_axis, i );

  int tmp = cut.getInversion () ? 1 : 0;
  element -> setAttribute ( m_invert, tmp );

  const Range & range = cut.getRange ();
  element -> setAttribute ( m_low,  range.low()  );
  element -> setAttribute ( m_high, range.high() );

  int col = cut.getColumn ( );
  element -> setAttribute ( m_column, col );

  tag -> appendChild ( *element );
  delete element;

  return tag;
}

TupleCut * TupleCutXML::getObject ( const XmlElement & tag ) const
{
  TupleCut * cut = 0; // will create one of correct dim in function below
  setAttributes ( cut, &tag );

  return cut; 
}

/** @bug The axis argument is not used.
 */
void
TupleCutXML::
setAxisAttributes ( TupleCut * cut,
		    hippodraw::Axes::Type,
		    const XmlElement * element ) const
{
  int value;
  bool ok = element->attribute ( m_invert, value );
  if ( ok && ( value == 1 ) )  cut -> setInversion( true );

  double low = 0.0;
  ok = element->attribute( m_low, low );
  double high = 0.0;
  ok = element->attribute( m_high, high );

  Range range ( low, high );

  cut->setRange( range );

  int col = -2;
  ok = element->attribute ( m_column, col );
  cut->setColumn( col );
}

bool
TupleCutXML::
hasMultiDimTupleCut ( const XmlElement * element ) const
{
  bool yes = true;
  int dim;
  bool ok = element -> attribute ( m_dim, dim );
  if ( ok && dim < 0 ) yes = false;

  return yes;
}

void
TupleCutXML::
getObjects ( const XmlElement * element, std::vector <TupleCut * > & cuts )
{
  int dim;
  bool ok = element -> attribute ( m_dim, dim );
  if ( ! ok ) { // very old documents forgot the Y axis 
    TupleCut * cut = new TupleCut ();
    setAxisAttributes ( cut, Axes::X, element );
    cuts.push_back ( cut );
  }
  else { // multidemension style
    XmlElement::NodeList_t nodelist;
    element -> fillNodeList ( "TupleCutAxis", nodelist );
    assert ( nodelist.empty () == false );
    XmlElement::NodeList_t::const_iterator first = nodelist.begin();

    while ( first != nodelist.end() ) {
      XmlElement * node = *first++;
      int axis;
      bool ok = node -> attribute ( m_axis, axis );
      assert ( ok );
      Axes::Type axis_t = axis == 0 ? Axes::X : Axes::Y;
      TupleCut * cut = new TupleCut ();
      setAxisAttributes ( cut, axis_t, node );
      cuts.push_back ( cut );
    }
  }
}

void TupleCutXML::setAttributes ( TupleCut * & cut,
				  const XmlElement * element ) const
{
  int dim;
  bool ok = element->attribute ( m_dim, dim );

  if ( ! ok ) { // older documents forgot Y axis
    cut = new TupleCut ();
    setAxisAttributes ( cut, Axes::X, element );
  }
  else { // new style
    cut = new TupleCut ( );
    XmlElement::NodeList_t nodelist;
    element -> fillNodeList ( "TupleCutAxis", nodelist );
    assert ( nodelist.empty () == false );
    XmlElement::NodeList_t::const_iterator first = nodelist.begin();

    while ( first != nodelist.end() ) {
      XmlElement * node = *first++;
      int axis;
      bool ok = node -> attribute ( m_axis, axis );
      assert ( ok );
      Axes::Type axis_t = axis == 0 ? Axes::X : Axes::Y;
      setAxisAttributes ( cut, axis_t, node );
    }
  }
}
