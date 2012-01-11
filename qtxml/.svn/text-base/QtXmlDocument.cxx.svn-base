/** @file

QtXmlDocument class implementation

Copyright (C) 2002-2005  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtXmlDocument.cxx,v 1.21 2005/10/30 00:10:16 pfkeb Exp $

*/

#include "QtXmlDocument.h"
#include "QtXmlElement.h"
#include "QtXmlTextNode.h"

#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtextstream.h>

using std::string;

namespace hippodraw {

QtXmlDocument::QtXmlDocument ( QDomDocument document )
  : m_document ( document )
{
}

QtXmlDocument::
QtXmlDocument ( const std::string & name )
{
  m_document = QDomDocument ( name.c_str () );
}

QtXmlDocument::
~QtXmlDocument ()
{
}

XmlElement * QtXmlDocument::documentElement ( ) const
{
  QDomElement root = m_document.documentElement ();

  return new QtXmlElement ( root );
}

XmlElement * 
QtXmlDocument::
createElement ( const std::string & tagName )
{
  QDomElement element = m_document.createElement ( tagName.c_str() );

  return new QtXmlElement ( element );
}

XmlTextNode *
QtXmlDocument::
createTextNode ( const std::string & tag )
{
  QDomText node = m_document.createTextNode ( tag.c_str () );

  return new QtXmlTextNode ( node );
}

/* virtual */
void QtXmlDocument::appendChild ( XmlElement & child )
{
  const QtXmlElement & qtelem 
    = dynamic_cast < const QtXmlElement & > ( child );

 m_document.appendChild ( qtelem.m_node );
}

XmlDocument::Status
QtXmlDocument::
saveToFile ( const std::string & filename )
{
  QFile filedev ( filename.c_str() );

  bool ok = filedev.open ( QIODevice::WriteOnly );

  if ( ! ok ) {
    return WriteError;
  }

  QTextStream ts ( &filedev );
  m_document.save ( ts, 2 );
  filedev.close ();

  return Success;
}

XmlDocument::Status 
QtXmlDocument::
setContent ( const std::string & filename )
{
  QFile file ( filename.c_str() );
  bool ok = file.open ( QIODevice::ReadOnly );

  if ( ! ok ) {
    file.close ();
    return OpenError; //
  }
  ok = m_document.setContent ( &file );
  if ( ! ok ) {
    file.close ();
    return ParseError;
  }

  QFileInfo info ( file );
  QString dir = info.path();
  QDir::setCurrent ( dir );

  file.close ();

  return Success;
}

} // namespace hippodraw

