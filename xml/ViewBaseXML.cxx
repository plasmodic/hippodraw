/** @file

hippodraw::ViewBaseXML implementation

Copyright (C) 2002-2006  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: ViewBaseXML.cxx,v 1.47 2006/02/07 00:34:23 pfkeb Exp $

*/

// for truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "ViewBaseXML.h"

#include "PlotterBaseXML.h"
#include "XmlController.h"
#include "XmlElement.h"

#include "graphics/ViewBase.h"
#include "plotters/PlotterBase.h"

using std::list;
using std::string;

using namespace hippodraw;

ViewBaseXML::ViewBaseXML ( XmlController * controller )
  : BaseXML ( "ViewBase", controller ),
    m_x ( "x" ),
    m_y ( "y" ),
    m_w ( "w" ),
    m_h ( "h" )
{
  m_plotter_xml = new PlotterBaseXML ( controller );
}

ViewBaseXML::
~ViewBaseXML ()
{
  delete m_plotter_xml;
}

bool ViewBaseXML::areDataSourcesSaved ( const ViewBase & view )
{
  const PlotterBase * plotter = view.getPlotter ();

  return m_plotter_xml->areDataSourcesSaved ( *plotter );
}

void ViewBaseXML::createChild ( XmlElement & tag, const ViewBase & view )
{
  const PlotterBase * plotter = view.getPlotter ();
  if ( plotter != 0 ) {
    XmlElement * element =  m_plotter_xml->createElement ( *plotter );
    tag.appendChild ( *element );
  }
}

XmlElement * ViewBaseXML::createElement ( const ViewBase & view )
{
  XmlElement * tag = BaseXML::createElement ();

  const void * addr = reinterpret_cast < const void * > ( & view );
  int id = m_controller -> getId ( addr );
  setId ( *tag, id );

  Rect rect = view.getDrawRect ();
  tag->setAttribute ( m_x, rect.getX() );
  tag->setAttribute ( m_y, rect.getY() );
  tag->setAttribute ( m_w, rect.getWidth() );
  tag->setAttribute ( m_h, rect.getHeight() );

  createChild ( *tag, view );

  return tag;
}

void ViewBaseXML::getObjects ( const XmlElement * view_element )
{
  const XmlElement * element = m_plotter_xml->getNode ( view_element );
  PlotterBase * plotter = m_plotter_xml->getObject ( element );

  if ( plotter != 0 ) {
    ViewBase * view = m_controller->createView ( plotter );

    double x;
    bool ok = view_element->attribute ( m_x, x );

    double y;
    ok &= view_element->attribute ( m_y, y );

    double w;
    ok &= view_element->attribute ( m_w, w );

    double h;
    ok &= view_element->attribute ( m_h, h );

    view->setDrawRect ( x, y, w, h );
  }
}

void
ViewBaseXML::
connectPlotters ( const XmlElement * root )
{
  list < XmlElement * > nodelist;
  m_plotter_xml -> fillNodeList ( root, nodelist );

  std::list < XmlElement * > ::const_iterator first = nodelist.begin ();
  while ( first != nodelist.end () ) {
    XmlElement * element = *first++;
    int ref;
    bool ok = element -> attribute ( "ref", ref );
    if ( ok ) {
      int id = element -> getID ();
      PlotterBase * source = m_controller -> getPlotter ( id );
      PlotterBase * target = m_controller -> getPlotter ( ref );
      source -> setParentPlotter ( target );
    }
  }
}
