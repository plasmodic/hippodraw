/** @file

hippodraw::NTupleXML class implementation

Copyright (C) 2002, 2004-2006 The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: NTupleXML.cxx,v 1.45 2006/05/17 19:47:13 pfkeb Exp $

*/

// for truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "NTupleXML.h"

#include "XmlController.h"
#include "XmlDocument.h"
#include "XmlElement.h"
#include "XmlTextNode.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/NTupleController.h"
#include "datasrcs/NTuple.h"
#include "pattern/string_convert.h"

#include <iterator>


#include <sstream>
using std::istringstream;

using std::istream_iterator;
using std::list;
using std::string;
using std::vector;

using namespace hippodraw;

NTupleXML::NTupleXML ( XmlController * controller )
  : BaseXML ( "NTuple", controller ),
    m_name ( "name" ),
    m_col_name ( "column" ),
    m_data ( "NTupleData" ),
    m_label ( "label" )
{
}

XmlElement * NTupleXML::createElement ( const DataSource & ntuple )
{
  XmlElement * tag = BaseXML::createElement ();

  setAttributes ( tag, ntuple );

  createChildren ( tag, ntuple);

  return tag;
}


void
NTupleXML::
setAttributes ( XmlElement * tag, const DataSource & ntuple )
{
  const void * addr = reinterpret_cast < const void * > ( & ntuple );
  int id = m_controller -> getId ( addr );
  setId ( *tag, id );

  const string & full_name = ntuple.getName ();
  tag -> setAttribute ( m_name, full_name );
}

namespace dp = hippodraw::DataPoint2DTuple;

void
NTupleXML::
createChildren ( XmlElement * tag, const DataSource & ntuple )
{
  unsigned int columns = ntuple.columns ();
  if ( columns == dp::SIZE ) createChildren2D ( tag, ntuple );
  else createChildren2D ( tag, ntuple );
}

void
NTupleXML::
createChildren2D ( XmlElement * tag, const DataSource & ntuple )
{
  static unsigned int cols [] 
    = { dp::X, dp::Y, dp::XERR, dp::YERR };

  for ( unsigned int i = 0; i < dp::SIZE; i++ ) {
    XmlElement * element
      = XmlController::m_xml_doc -> createElement ( m_data );
    element -> setAttribute ( m_col_name, i );
    const string & label = ntuple.getLabelAt ( cols[i] );
    element -> setAttribute ( m_label, label );

    const vector < double > & data = ntuple.getColumn ( cols[i] );
    unsigned int size = ntuple.rows ();

    string text;
    for ( unsigned int j = 0; j < size; j++ ) {
      text += String::convert ( data[j] );
      text += " ";
    }

    XmlTextNode * node 
      = XmlController::m_xml_doc -> createTextNode ( text );
    element -> appendChild ( *node );
    delete node;

    tag -> appendChild ( *element );
    delete element;
  }
}

/** @todo Why is this called with a DataSource argument.
 */
void
NTupleXML::
createChildren3D ( XmlElement * tag, const DataSource & )
{
  XmlTextNode * node 
    = XmlController::m_xml_doc -> createTextNode ( "XData" );
  tag -> appendChild ( *node );
  delete node;
}

/** @todo Implement M4 macro to detect missing string::clear(). Gcc
    2.95.3 is missing this function, while gcc 3.2.1 has it. */
string NTupleXML::getName ( const XmlElement & tag ) const
{
  string name;
  bool ok = tag.attribute ( m_name, name );
#ifndef STRING_CLEAR_DEFECT // change to ifdef when M4 macro is implemented
  if ( ! ok ) name.erase ( name.begin(), name.end() );
#else
  if ( ! ok ) name.clear();
#endif

  return name;
}

const DataSource * NTupleXML::getObject ( const XmlElement & tag )
{
  list < XmlElement * > nodelist;
  tag.fillNodeList ( m_data, nodelist );
  unsigned int size = nodelist.size ();

  // tempoary holding area as no guarantee data comes back in order
  vector < vector < double > > columns ( size );
  vector < string > labels ( size );

  list < XmlElement * >::const_iterator first = nodelist.begin ();
    while ( first != nodelist.end() ) {
      XmlElement * elem = *first++;
    int column;
    bool ok = elem -> attribute ( m_col_name, column );

    string label;
    ok = elem -> attribute ( m_label, label );
    labels[column] = label;

    const string & text = elem -> getText ();
    istringstream stream ( text );

    copy ( istream_iterator<double> ( stream ),
	   istream_iterator<double> (), back_inserter ( columns[column] ) );
  }

  NTuple * ntuple = new NTuple ();
  for ( unsigned int i = 0; i < size; i++ ) {
    ntuple -> addColumn ( labels[i], columns[i] );
  }

  return ntuple;
}
