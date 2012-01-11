/** @file

hippodraw::BinningProjectorXML implementation

Copyright (C)  2003-2006  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinningProjectorXML.cxx,v 1.17 2007/04/23 21:57:22 pfkeb Exp $
*/


// for truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "BinningProjectorXML.h"

#include "BinsBaseXML.h"
#include "NTupleXML.h"

#include "XmlController.h"
#include "XmlDocument.h"
#include "XmlElement.h"

#include "axes/Range.h"
#include "binners/BinsBase.h"
#include "datasrcs/DataSource.h"
#include "projectors/BinningProjector.h"

#include <cassert>

using std::list;
using std::string;
using std::vector;

using namespace hippodraw;

BinningProjectorXML::BinningProjectorXML ( XmlController * controller )
  : BaseXML ( "BinningProjector", controller )
{
  m_binsbaseXML = new BinsBaseXML ( controller );
  m_ntupleXML = new NTupleXML ( controller );
}

BinningProjectorXML::
~BinningProjectorXML ()
{
  delete m_binsbaseXML;
  delete m_ntupleXML;
}

XmlElement *
BinningProjectorXML::
createElement ( const BinningProjector & projector )
{
  XmlElement * tag = BaseXML::createElement (); // in base class
  const void * addr = reinterpret_cast < const void * > ( & projector );
  int id = m_controller -> getId ( addr );
  setId ( *tag, id );

  createChildren ( tag, projector );

  return tag;
}
void
BinningProjectorXML::
createChildren ( XmlElement * tag, const BinningProjector & projector )
{
  const BinsBase * bins = projector.getBinner ();
  XmlElement * element = m_binsbaseXML->createElement ( *bins );

  tag->appendChild ( *element );
  delete element;

  BinningProjector & proj = const_cast < BinningProjector & > ( projector );
  if ( projector.hasDataSourceBindings () == false ) {
    proj.prepareValues ();
    const DataSource * ntuple = projector.getProjectedValues ();
    element = m_ntupleXML -> createElement ( *ntuple );

    tag -> appendChild ( *element );
    delete element;
  }
}

void
BinningProjectorXML::
getObject ( const XmlElement * parent, ProjectorBase * pbase )
{
  const XmlElement * bp_element = getNode ( parent );
  if ( bp_element == 0 ) return;

  const XmlElement * element = m_binsbaseXML->getNode ( bp_element );
  BinsBase * bins = m_binsbaseXML->createObject ( element );

  BinningProjector * projector = dynamic_cast < BinningProjector * > ( pbase );
  projector->setBinner ( bins );
}

void
BinningProjectorXML::
setBins ( const XmlElement * parent, ProjectorBase * pbase )
{
  const XmlElement * bp_element = getNode ( parent );
  if ( bp_element == 0 ) return;

  const XmlElement * nt_element = m_ntupleXML -> getNode ( bp_element );
  const DataSource * ntuple = m_ntupleXML -> getObject ( *nt_element );

  BinningProjector * projector = dynamic_cast < BinningProjector * > ( pbase );
  projector -> setBinContents ( ntuple );
}
