/** @file

hippodraw::DataRepXML class implementation

Copyright (C) 2002-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DataRepXML.cxx,v 1.90 2007/07/20 19:29:57 pfkeb Exp $

*/

// for truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "DataRepXML.h"

#include "BinningProjectorXML.h"
#include "NTupleProjectorXML.h"
#include "FunctionProjectorXML.h"
#include "MapMatrixProjectorXML.h"
#include "PointRepXML.h"
#include "XmlController.h"
#include "XmlDocument.h"
#include "XmlElement.h"

#include "controllers/DataRepController.h"
#include "controllers/FunctionController.h"

#include "datareps/DataRepException.h"
#include "datareps/FunctionRep.h"
#include "datareps/TextDataRep.h"

#include "datasrcs/DataSource.h"
#include "plotters/PlotterBase.h"

#include "projectors/BinningProjector.h"
#include "projectors/MapMatrixProjector.h"
#include "projectors/FunctionProjector.h"

#include "reps/RepBase.h"

#include <cassert>

using std::string;
using std::vector;


using namespace hippodraw;

DataRepXML::DataRepXML ( XmlController * controller )
  : BaseXML ( "DataRep", controller ),
    m_contours ( "contours" )
{
  m_pointrep  = new PointRepXML ( controller );
  m_nTupleProjector = new NTupleProjectorXML ( controller );
  m_binningProjectorXML = new BinningProjectorXML ( controller );
  m_functionProjector = new FunctionProjectorXML ( controller );
  m_mapMatrixProjectorXML = new MapMatrixProjectorXML ( controller );
}

DataRepXML::
~DataRepXML ()
{
  delete m_pointrep;
  delete m_nTupleProjector;
  delete m_binningProjectorXML;
  delete m_functionProjector;
  delete m_mapMatrixProjectorXML;
}

XmlElement * DataRepXML::createElement ( const hippodraw::DataRep & rep )
{
  XmlElement * tag = BaseXML::createElement ();

  const void * addr = reinterpret_cast < const void * > ( & rep );
  int id = m_controller -> getId ( addr );

  setId ( *tag, id );
  tag->setAttribute ( m_type, rep.name() );

  createChildren ( *tag, rep );

  return tag;
}


void DataRepXML::createChildren ( XmlElement & tag,
				  const NTupleProjector * projector )
{
  NTupleProjectorXML * qt_projector 
    = dynamic_cast < NTupleProjectorXML * > ( m_nTupleProjector );
  XmlElement * element = qt_projector->createElement ( *projector );

  tag.appendChild ( *element );
  delete element;
}

void 
DataRepXML::
createChildren ( XmlElement & tag, const BinningProjector * projector )
{
  XmlElement * element 
    = m_binningProjectorXML -> createElement ( *projector );
  tag.appendChild ( *element );
  delete element;
}

void 
DataRepXML::
createChildren ( XmlElement & tag, const MapMatrixProjector * projector )
{
  XmlElement * element 
    = m_mapMatrixProjectorXML -> createElement ( *projector );
  tag.appendChild ( *element );
  delete element;
}

void DataRepXML::createChildren ( XmlElement & tag, 
				  const FunctionProjector * projector )
{
  FunctionProjectorXML * qt_projector 
    = dynamic_cast < FunctionProjectorXML * > ( m_functionProjector );
  XmlElement * element = qt_projector->createElement ( *projector );

  tag.appendChild ( *element );
  delete element;
}

void DataRepXML::createChildren ( XmlElement & tag,
				  const hippodraw::DataRep & rep )
{
  RepBase * pointrep = rep.getRepresentation ();
  XmlElement * element = m_pointrep->createElement ( *pointrep );

  tag.appendChild ( *element );
  delete element;

  ProjectorBase * projbase = rep.getProjector ();

  if ( rep.hasNTupleBindings () ) 
    {
      const NTupleProjector * projector
	= dynamic_cast < const NTupleProjector * > ( projbase );
      createChildren ( tag, projector );
    }

  const BinningProjector * binProjector 
    = dynamic_cast < const BinningProjector * > ( projbase );
  if ( binProjector != 0 ) 
    {
      createChildren ( tag, binProjector );
    }

  const FunctionProjector * functionProjector
    = dynamic_cast < const FunctionProjector * > ( projbase );
  if ( functionProjector != 0  ) 
    {
      createFunctionTargetChild ( tag, rep );
      createChildren ( tag, functionProjector );
    }

  const MapMatrixProjector * mapMatrixProjector 
    = dynamic_cast < const MapMatrixProjector * > ( projbase );
  if ( mapMatrixProjector != 0 ) {
    createChildren ( tag, mapMatrixProjector );
  }

}

void DataRepXML::
createFunctionTargetChild ( XmlElement & tag,
			    const hippodraw::DataRep & rep )
{
  const FunctionRep & frep = dynamic_cast < const FunctionRep & > ( rep );
  hippodraw::DataRep * target = frep.getTarget ();
  const void * addr = reinterpret_cast < const void * > ( target );
  int id = m_controller -> getId ( addr );
  XmlElement * element 
    = XmlController::m_xml_doc->createElement ( "FunctionTarget" );
  element->setAttribute ( "id", id );
  tag.appendChild ( *element );
  delete element;
}

hippodraw::DataRep *
DataRepXML::
createDataRep ( const XmlElement * element )
{
  DataRep * rep = 0;

  string type;
  bool ok = element -> attribute ( m_type, type );
  assert ( ok );

  if ( type == "Z Plot" ) {
    type = "Image";  // name has changed.
  }

  DataRepController * controller = DataRepController::instance ();
  try {
    rep = controller -> createDataRep ( type );
  }
  catch ( const DataRepException & ) {
    // do nothing
  }

  return rep;
}

DataRep * 
DataRepXML::
createDataRep ( const XmlElement * element, const XmlElement * ntp_element )
{
  string  type;
  bool ok = element->attribute ( m_type, type );
  assert ( ok );

  int ref = m_nTupleProjector->getNTupleReference ( ntp_element );
  const DataSource * ntuple = m_controller->getDataSource ( ref );
  assert ( ntuple != 0 );

  vector < string > bindings;
  m_nTupleProjector->getBindings ( ntp_element, bindings );
      
  DataRepController * controller = DataRepController::instance();
  DataRep * rep 
    = controller->createDataRep ( type, ntuple, bindings );

  return rep;
}

/** @todo Why is this call with an argument.
 */
DataRep *
DataRepXML::
getTextRepObject ( const XmlElement * )
{
  TextDataRep * rep = new TextDataRep ();

  return rep;
}

DataRep * DataRepXML::createObject ( const XmlElement * dr_element )
{
  DataRep * rep = 0;

  const XmlElement * element
    = m_nTupleProjector->getNode ( dr_element );

  if ( element != 0 ) { // bound to ntuple
      rep = createDataRep ( dr_element, element );
      ProjectorBase * projector = rep->getProjector ();
      m_binningProjectorXML->getObject ( dr_element, projector );
  }

  element = m_mapMatrixProjectorXML -> getNode ( dr_element );
  if ( element != 0 ) {
    ProjectorBase * projector = rep -> getProjector ();
    m_mapMatrixProjectorXML -> setAttributes ( element, projector );
  }

  element = m_functionProjector->getNode ( dr_element );
  if ( element != 0 ) {
    XmlElement * targetelement = dr_element->getNode ( "FunctionTarget" );
    assert ( targetelement );
    int id = targetelement->getID ();
    DataRep * target = m_controller->getDataRep ( id );
    assert ( target );
    rep = m_functionProjector -> createFunctionRep ( element, target );
  }

  if ( element == 0 ) { // not any of above
    const XmlElement * element
      = m_nTupleProjector->getNode ( dr_element );
    if ( element == 0 ) {
      rep = createDataRep ( dr_element );
      if ( rep != 0 ) {
	ProjectorBase * projector = rep -> getProjector ();
	m_binningProjectorXML -> getObject ( dr_element, projector );
	m_binningProjectorXML -> setBins ( dr_element, projector );
      }
    }
  }

  string type;
  bool ok = dr_element->attribute ( m_type, type );
  assert ( ok );

  if ( type == "TextDataRep" ) {
    rep = getTextRepObject ( dr_element );
  }

  if ( rep == 0 ) return 0;
  
  const XmlElement * pr_element = m_pointrep->getNode ( dr_element );
  assert ( pr_element );
  RepBase * pr = m_pointrep->createObject ( pr_element );
  rep->setPointRep ( pr );
  
  return rep;
}
