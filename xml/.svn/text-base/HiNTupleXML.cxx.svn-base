/** @file

hippodraw::HiNTupleXML class implementation

Copyright (C)  2002, 2004, 2006  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: HiNTupleXML.cxx,v 1.43 2007/07/02 18:17:16 pfkeb Exp $

*/
// for have root cfitsio
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// for truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "HiNTupleXML.h"

#include "XmlController.h"
#include "XmlElement.h"

#include "datasrcs/NTupleController.h"
#include "datasrcs/NTuple.h"

#ifdef HAVE_CFITSIO
#include "fits/FitsController.h"
#endif

#ifdef HAVE_ROOT
#include "root/RootController.h"
#endif

#include <fstream>
#include <stdexcept>

using std::ifstream;
using std::runtime_error;
using std::string;

using namespace hippodraw;

HiNTupleXML::HiNTupleXML ( XmlController * controller )
  : BaseXML ( "HiNTuple", controller ),
    m_name ( "name" )
{
}

XmlElement * HiNTupleXML::createElement ( const DataSource & ntuple )
{
  XmlElement * tag = BaseXML::createElement ();

  const void * addr = reinterpret_cast < const void * > ( & ntuple );
  int id = m_controller -> getId ( addr );
  setId ( *tag, id );

  const string & full_name = ntuple.getName ();
  tag -> setAttribute ( m_name, full_name );

  return tag;
}

/** @todo Implement M4 macro to detect missing string::clear(). Gcc
    2.95.3 is missing this function, while gcc 3.2.1 has it. */
string HiNTupleXML::getName ( const XmlElement & tag ) const
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



const DataSource *
HiNTupleXML::
getObject ( const std::string fullname ) const
{

  const DataSource * ntuple = 0;

  string filename;
  string name;
  string::size_type pos = fullname.find_last_of ( ":" );
  if ( pos != string::npos  && pos != 1 ) { // for Windows "C:"
	filename = fullname.substr ( 0, pos );
	name = fullname.substr ( pos + 2 );
  }
  else {
    filename = fullname;
  }

  string suffix;
  pos = filename.find_last_of ( "." );
  if ( pos != string::npos ) {
    suffix = filename.substr ( pos + 1 );
  }

  if ( suffix.empty () == false ) {
    if ( suffix == "fits" || suffix == "gz" ) {
#ifdef HAVE_CFITSIO
      FitsController * controller = FitsController::instance ();
      ntuple = controller -> createNTuple ( filename, name );
#else
      string what ("Can not open file\n" );
      what += filename + "\n";
      what += "because application was not built with FITS support";
      throw runtime_error ( what );
#endif
    }
    else if ( suffix == "root" ) {
#ifdef HAVE_ROOT
      RootController * controller = RootController::instance ();
      ntuple = controller -> createNTuple ( filename, name );
#else
      string what ( "Can not open file\n" );
      what += filename + "\n";
      what += "because application was not built with ROOT support";
      throw runtime_error ( what );
#endif
    }
    else {
      NTupleController * controller = NTupleController::instance ();
      ntuple = controller->createNTuple ( filename );
    }
  } else {
      NTupleController * controller = NTupleController::instance ();
      ntuple = controller->createNTuple ( filename );
  }

  return ntuple;
}
  

const DataSource * 
HiNTupleXML::
getObject ( const XmlElement & tag ) const
{
  const string fullname = getName ( tag );
  if ( fullname.empty() ) return 0;

  return getObject ( fullname );
}
