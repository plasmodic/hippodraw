/** @file

QtXMLController class implementation

Copyright (C) 2002, 2003   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtXMLController.cxx,v 1.80 2005/10/30 00:10:16 pfkeb Exp $
*/

#include "QtXMLController.h"

#include "QtXmlDocument.h"

#include <qfile.h>

using std::string;

namespace hippodraw {

QtXMLController * QtXMLController::s_instance = 0;

QtXMLController::QtXMLController ( )
{
}

QtXMLController * QtXMLController::instance ( )
{
  if ( s_instance == 0 ) {
    s_instance = new QtXMLController ();
  }
  return s_instance;
}

bool
QtXMLController::
fileExists ( const std::string & filename ) const
{
  QFile file ( filename.c_str() );

  return file.exists ();
}

void
QtXMLController::
newDocument ( const std::string & name )
{
//   if ( m_xml_doc != 0 ) delete m_xml_doc;
  m_xml_doc = new QtXmlDocument ( name );
}

} // namespace hippodraw

