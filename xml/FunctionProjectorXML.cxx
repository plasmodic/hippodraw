/** @file

hippodraw::FunctionProjectorXML class implementation

Copyright (C) 2002, 2003, 2005, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FunctionProjectorXML.cxx,v 1.22 2007/04/23 21:57:22 pfkeb Exp $

*/

// for truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "FunctionProjectorXML.h"

#include "XmlController.h"
#include "XmlElement.h"

#include "controllers/FunctionController.h"
#include "datareps/FunctionRep.h"
#include "functions/FunctionBase.h"
#include "pattern/string_convert.h"
#include "projectors/FunctionProjector.h"

using std::string;
using std::vector;

namespace hippodraw {

FunctionProjectorXML::FunctionProjectorXML ( XmlController * controller )
  : BaseXML ( "FunctionProjector", controller )
{
}

XmlElement * 
FunctionProjectorXML::createElement ( const FunctionProjector & projector )
{
  XmlElement * tag = BaseXML::createElement ();

  const void * addr = reinterpret_cast < const void * > ( & projector );
  int id = m_controller -> getId ( addr );
  setId ( *tag, id );
  FunctionBase * function = projector.function ();
  tag->setAttribute ( "name", function->name() );

  const vector < double > & parms = function->getParameters ();
  unsigned int size = parms.size();

  for ( unsigned int i = 0; i < size; i++ ) {
    string parm ( "Parm" );
    parm += String::convert ( i );
    tag -> setAttribute ( parm, parms[i] );
  }

  return tag;
}

string 
FunctionProjectorXML::
getFunctionName ( const XmlElement * element )
{
  string fname;
  bool ok = element->attribute ( "name", fname );
  if ( ! ok ) return 0;

  return fname;
}

void 
FunctionProjectorXML::
setAttributes ( const XmlElement * element, FunctionBase * function )
{
  int size = function->size();
  vector < double > parms ( size );
  for ( int i = 0; i < size; i++ ) {
    string parm ( "Parm" );
    parm += String::convert ( i );
    element -> attribute ( parm, parms[i] );
  }

  function->setParameters ( parms );
}

DataRep *
FunctionProjectorXML::
createFunctionRep ( const XmlElement * fun_element,
		    DataRep * drep )
{
  FunctionRep * rep = 0;

  std::string fname = getFunctionName ( fun_element );
  FunctionController * fc = FunctionController::instance();
  if ( fc -> functionExists ( fname ) ) {
    rep = fc->createFunctionRep ( fname, drep );
    FunctionBase * function = rep->getFunction ();
    setAttributes ( fun_element, function );
    rep->saveParameters ();
  }
  return rep;
}

} // namespace hippodraw

