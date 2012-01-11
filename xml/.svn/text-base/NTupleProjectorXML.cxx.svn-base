/** @file

hippodraw::NTupleProjectorXML class implementation

Copyright (C) 2002, 2003, 2005, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: NTupleProjectorXML.cxx,v 1.45 2007/04/23 21:57:22 pfkeb Exp $

*/

// for truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "NTupleProjectorXML.h"

#include "XmlController.h"
#include "XmlDocument.h"
#include "XmlElement.h"

#include "datasrcs/TupleCut.h"
#include "pattern/string_convert.h"
#include "projectors/NTupleProjector.h"

#include <cassert>

using std::list;
using std::string;
using std::vector;

using namespace hippodraw;

NTupleProjectorXML::NTupleProjectorXML ( XmlController * controller )
  : BaseXML ( "NTupleProjector", controller )
{
}

void
NTupleProjectorXML::
createChildren ( XmlElement & tag, 
		 const NTupleProjector & projector )
{
  const vector < const TupleCut * > & cutlist = projector.getCutList ();
  if ( cutlist.empty() ) return;

#ifdef ITERATOR_MEMBER_DEFECT
  std:: 
#endif
  vector < const TupleCut * >::const_iterator first = cutlist.begin();
  for ( ; first != cutlist.end(); ++first ) {
    const TupleCut * cut = *first;
    const void * addr = reinterpret_cast < const void * > ( cut );
    int ref = m_controller -> getId ( addr );
    XmlElement * element 
      = XmlController::m_xml_doc->createElement ( "TupleCutRef" );
    element->setAttribute ( "ref", ref );
    tag.appendChild ( *element );
    delete element;
  }

}

XmlElement * 
NTupleProjectorXML::createElement ( const NTupleProjector & projector )
{
  XmlElement * tag = BaseXML::createElement (); // in base class
  const void * addr = reinterpret_cast < const void * > ( & projector );
  int id = m_controller -> getId ( addr );
  setId ( *tag, id );

  const DataSource * ntuple = projector.getNTuple ();

  XmlElement * element = m_controller->addDataSource ( ntuple );

  int ref = element->getID ();
  tag->setAttribute ( "ref", ref );

  const vector < string > & bindings = projector.getAxisBindings();
  unsigned int size = bindings.size();

  for ( unsigned int i = 0; i < size; i++ ) {
    string name ( "Axis" );
    name += String::convert ( i );
    const string & label = bindings[i];
    tag -> setAttribute ( name, label );
  }

  createChildren ( *tag, projector );

  return tag;
}

int NTupleProjectorXML::getNTupleReference ( const XmlElement * element ) const
{
  int ref;
  bool ok = element->attribute ( "ref", ref );
  if ( ! ok ) return 0;

  return ref;
}

void 
NTupleProjectorXML::
getBindings ( const XmlElement * element, 
	      std::vector < std::string > & bindings ) const
{
  bindings.clear();

  for ( int i = 0; ; i++ ) {
    string label;
    string name ( "Axis" );
    name += String::convert ( i );
    bool ok = element -> attribute ( name, label );
    if ( ! ok ) break;

    bindings.push_back ( label );
  }

}
