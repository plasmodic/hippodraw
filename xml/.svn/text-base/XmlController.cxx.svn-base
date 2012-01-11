/** @file

hippodraw::XmlController class implementation

Copyright (C) 2002-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: XmlController.cxx,v 1.125 2007/04/11 22:04:06 pfkeb Exp $

*/

// for truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "XmlController.h"

#include "DataRepXML.h"
#include "HiNTupleXML.h"
#include "TupleCutXML.h"
#include "ViewBaseXML.h"
#include "XmlDocument.h"
#include "XmlElement.h"

#include "controllers/CutController.h"
#include "datasrcs/DataSource.h"
#include "datasrcs/DataSourceController.h"
#include "graphics/ViewBase.h"
#include "graphics/ViewFactory.h"

#include <stdexcept>
#include <cassert>

using std::map;
using std::list;
using std::string;
using std::vector;

using namespace hippodraw;

XmlDocument * XmlController::m_xml_doc = 0;

XmlController::XmlController ( )
  : m_pasteboard ( 0 ),
    m_selectboard ( 0 ),
    m_id ( 0 )
{
  m_datarep_xml = new DataRepXML ( this );
  m_ntuple = new HiNTupleXML ( this );
  m_tuple_cut_xml = new TupleCutXML ( this );
  m_view = new ViewBaseXML ( this );
}

XmlController::~XmlController ( )
{
  delete m_pasteboard;
  delete m_datarep_xml;
  delete m_ntuple;
  delete m_tuple_cut_xml;
  delete m_view;
}

void
XmlController::
createDocument ( const std::string & doc_name )
{
  newDocument ( doc_name );
  XmlElement * root = m_xml_doc->createElement ( doc_name );

  m_xml_doc->appendChild ( *root );
  delete root;
}

XmlController::Status 
XmlController::
openDocument ( const std::string & filename )
{
  newDocument ( "HippoDraw" );

  XmlDocument::Status rc = m_xml_doc->setContent ( filename );
  if ( rc == XmlDocument::OpenError ) return OpenError;
  if ( rc == XmlDocument::ParseError ) return ParseError;

  return Success;
}

XmlController::Status 
XmlController::
openFile ( const std::string & filename )
{
  Status status = openDocument ( filename );
  if ( status != Success ) return status;

  m_views.clear();
  m_tuple_map.clear ();
  m_tuple_cut_map.clear ();

  return openNTuples ();
}

XmlController::Status 
XmlController::
openNTuples ( )
{
  m_missing_tuples.clear ();
  m_missing_ids.clear ();

  XmlElement * root = m_xml_doc->documentElement ();
  list < XmlElement * > nodelist;
  m_ntuple->fillNodeList ( root, nodelist );

  std::list < XmlElement * >::const_iterator first = nodelist.begin();

  while ( first != nodelist.end() ) {
    XmlElement * element = *first++;
    int id = element->getID ();
    try {
      const DataSource * ntuple = m_ntuple->getObject ( *element );
      m_tuple_map[id] = ntuple;
    }
    catch ( const std::runtime_error &  ) {
      const string name = m_ntuple->getName ( *element );
      m_missing_tuples.push_back ( name );
      m_missing_ids.push_back ( id );
    }
  }
  if ( m_missing_tuples.empty () == true ) return Success;

  return NTupleError;
}

XmlController::Status
XmlController::
openUpdatedNTuples ( )
{
  std::list < string >::iterator it;
  int i = 0;
  for ( it=m_missing_tuples.begin(); 
	it!=m_missing_tuples.end(); ++it, ++i )
    {
      const DataSource * ntuple = m_ntuple->getObject ( *it );
      if ( ntuple == 0 ) 
	{
	  return NTupleError;
	}
      else 
	{
	  int id = m_missing_ids[i];
	  m_tuple_map[id] = ntuple;
	}
    }
  return Success;
}


bool
XmlController::
isPasteboardEmpty ()
{
  return m_pasteboard == 0;
}

const list < ViewBase * > &
XmlController::
getFromPasteboard ( ViewFactory * factory,
		    const std::vector < const ViewBase * > & views )
{
  m_xml_doc = m_pasteboard;

  return getFromDocument ( factory, views );
}

const list < ViewBase * > &
XmlController::
getFromSelectionCopy ( ViewFactory * factory,
		       const std::vector < const ViewBase * > & views )
{
  m_xml_doc = m_selectboard;

  return getFromDocument ( factory, views );
}

const list < ViewBase * > &
XmlController::
getFromDocument ( ViewFactory * factory, 
		  const std::vector < const ViewBase * > & views )
{
  fillNTupleMap ();
  fillTupleCutMap ( views );
  m_view_factory = factory;

  getViews ();

  CutController * controller = CutController::instance ();
  controller->connectDataRep ( m_views, views );

  return m_views;
}

const list < ViewBase * > & 
XmlController::getViews ( ViewFactory * factory )
{
  m_view_factory = factory;
  getViews ();
  delete m_xml_doc; // done with it.

  return m_views;
}

bool 
XmlController::
areDataSourcesSaved ( const std::vector < const ViewBase * > & views )
{
  bool yes = true;
#ifdef ITERATOR_MEMBER_DEFECT
  std::
#endif
  vector < const ViewBase * > ::const_iterator first = views.begin ();
  while ( first != views.end () ) {
    const ViewBase * view = *first++;
    yes = m_view->areDataSourcesSaved ( *view );
    if ( yes == false ) break;
  }

  return yes;
}

bool 
XmlController::
serializeViews ( const std::vector < const ViewBase * > & views )
{
  createDocument ( "HippoDraw" );

  if ( m_xml_doc == 0 ) return false;

  addViews ( views );

  return true;
}

void
XmlController::
clearDataSourceMap ()
{
  map < const DataSource *, XmlElement * > ::iterator first 
    = m_data.begin();
  for ( ; first != m_data.end (); ++ first ) {
    delete first->second;
  }

  m_data.clear ();
}

void
XmlController::
saveToDocument ( const std::vector < const ViewBase * > & views )
{
  clearDataSourceMap ();
  m_addr_map.clear();
  serializeViews ( views );
}

void
XmlController::
saveToPasteboard ( const std::vector < const ViewBase * > & selviews )
{
  saveToDocument ( selviews );

  if ( m_pasteboard != 0 ) delete m_pasteboard;
  m_pasteboard = m_xml_doc;
}

void
XmlController::
saveToSelectionCopy ( const std::vector < const ViewBase * > & views )
{
  saveToDocument ( views );

  if ( m_selectboard != 0 ) delete m_selectboard;
  m_selectboard = m_xml_doc;
}

XmlController::Status
XmlController::
saveToFile ( const std::vector < const ViewBase * > & views, 
	     const std::string & filename )
{
  clearDataSourceMap ();
  m_addr_map.clear();
  serializeViews ( views );
  XmlDocument::Status status = m_xml_doc->saveToFile ( filename );
  delete m_xml_doc;

  if ( status != XmlDocument::Success ) return OpenError;

  return Success;
}

void 
XmlController::
addViews ( const std::vector < const ViewBase * > & views )
{
  XmlElement * root = m_xml_doc->documentElement ();

  std::vector < const ViewBase * >::const_iterator it = views.begin();
  for ( ; it != views.end(); ++it ) {
    XmlElement * elem = m_view->createElement ( **it );

    root->appendChild ( *elem );
    delete elem;
  }
  delete root;
}

ViewBase * XmlController::createView ( PlotterBase * plotter )
{
  ViewBase * view = m_view_factory->createView ( plotter );
  m_views.push_back ( view );

  return view;
}

FontBase *
XmlController::
createFont () const
{
  return m_view_factory -> createFont ();
}

XmlElement * XmlController::addDataSource ( const DataSource * ntuple )
{
  std::map < const DataSource *, XmlElement * >::const_iterator it
    = m_data.find ( ntuple );
  if ( it != m_data.end () ) {
    return it->second;
  }
  XmlElement * element = m_ntuple->createElement ( *ntuple );

  m_data[ntuple] = element;

  XmlElement * root = m_xml_doc->documentElement ();
  root->appendChild ( *element );
  delete root;
  return element;
}

const DataSource * XmlController::getDataSource ( int ref )
{
  std::map < int, const DataSource * >::const_iterator it
    = m_tuple_map.find ( ref );
  if ( it == m_tuple_map.end () ) {
    return 0;
  }

  return it->second;
}

const TupleCut * XmlController::getTupleCut ( int ref )
{
  const TupleCut * cut = 0;

  std::map < int, const TupleCut * >::iterator it
    = m_tuple_cut_map.find ( ref );
  if ( it == m_tuple_cut_map.end() &&
       ref >=0  ) {
    it = m_tuple_cut_map.find ( -ref ); // old multiDim TupleCut
  }
  if ( it != m_tuple_cut_map.end() ) {
    cut = it->second;
  }

  return cut;
}

DataRep * XmlController::getDataRep ( int ref )
{
  std::map < int, DataRep * >::iterator it
    = m_datarep_map.find ( ref );

  if ( it == m_datarep_map.end() ) return 0;

  return it->second;
}

PlotterBase *
XmlController::
getPlotter ( int ref )
{
  std::map < int, PlotterBase * >::iterator it
    = m_plotter_map.find ( ref );

  if ( it == m_plotter_map.end() ) return 0;

  return it->second;
}

void
XmlController::
registerPlotter ( int id, PlotterBase * plotter )
{
  m_plotter_map [ id ] = plotter;
}

void
XmlController::
fillNTupleMap ()
{
  m_tuple_map.clear();
  DataSourceController * controller = DataSourceController::instance ();
  vector < DataSource * > tuples;
  controller->getDataSources ( tuples ); // get all
#ifdef ITERATOR_MEMBER_DEFECT
  std::
#endif
  vector < DataSource * > ::const_iterator first = tuples.begin();
  for ( ; first != tuples.end(); ++first ) {
    const void * addr = reinterpret_cast < const void * > ( *first );
    int ref = getId ( addr );
    m_tuple_map[ref] = *first;
  }
}

void XmlController::
fillTupleCutMap ( const std::vector < const ViewBase * > & views )
{
  m_tuple_cut_map.clear();
  vector < const TupleCut * > cut_list;
  CutController * controller = CutController::instance ();

  controller->fillTupleCutList ( views, cut_list );

  vector < const TupleCut * > :: iterator first = cut_list.begin ();
  while ( first != cut_list.end () ) {
    const TupleCut * cut = *first++;
    void * addr = reinterpret_cast < void * > ( &cut );
    int id = getId ( addr );
    m_tuple_cut_map [ id ] = cut;
  }
}

void XmlController::getDataReps ()
{
  XmlElement * root = m_xml_doc->documentElement ();
  list < XmlElement * > nodelist;
  m_datarep_xml->fillNodeList ( root, nodelist );

#ifdef ITERATOR_MEMBER_DEFECT
  std::
#endif
  list < XmlElement * > ::const_iterator first = nodelist.begin();

  for ( ; first != nodelist.end(); ++first ) {
    XmlElement * element = *first;
    int id = element->getID ();
    DataRep * rep = m_datarep_xml->createObject ( element );
    m_datarep_map[id] = rep;
  }

}

void XmlController::getTupleCuts ()
{
  XmlElement * root = m_xml_doc->documentElement ();
  list < XmlElement * > nodelist;
  m_tuple_cut_xml->fillNodeList ( root, nodelist );

  // std:: for MS VC++ 6.0
  std::list < XmlElement * >::const_iterator first = nodelist.begin();

  for ( ; first != nodelist.end(); ++first ) {
    XmlElement * element = *first;
    if ( m_tuple_cut_xml -> hasMultiDimTupleCut ( element ) ) {
      vector < TupleCut * > cuts;
      m_tuple_cut_xml -> getObjects ( element, cuts );
      for ( unsigned int i = 0; i < cuts.size (); i++ ) {
	int id = element -> getID ();
	if ( i == 1 ) id = -id;
	m_tuple_cut_map[id] = cuts[i];
      }
    }
    else {
      TupleCut * cut = m_tuple_cut_xml->getObject ( *element );
      int id = element->getID ();
      m_tuple_cut_map[id] = cut;
    }
  }
}

void XmlController::getViews ( )
{
  m_views.clear();
  if ( m_xml_doc != 0 ) {
    getTupleCuts ();
    getDataReps ();

    XmlElement * root = m_xml_doc->documentElement ();
    list < XmlElement * > nodelist;
    m_view->fillNodeList ( root, nodelist );

    if ( nodelist.empty () ) return;

    // std:: for MS VC++ 6.0
    std::list < XmlElement * >::const_iterator first = nodelist.begin ();
    for ( ; first != nodelist.end(); ++ first ) {
      m_view->getObjects ( *first );
    }

    m_view -> connectPlotters ( root );
  }
}

int
XmlController::
getId ( const void * addr )
{
  int id = 0;
  AddrMapIterator_t it = m_addr_map.find ( addr );
  if ( it == m_addr_map.end () ) { // not found
    id = m_id;
    m_addr_map [ addr ] = m_id++;
  }
  else { // found
    id = it -> second;
  }

  return id;
}

std::list < std::string > &
XmlController::
getMissingTuples ( )
{
  return m_missing_tuples;
}
