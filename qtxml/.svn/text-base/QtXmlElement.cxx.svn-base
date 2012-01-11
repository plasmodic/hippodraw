/** @file

hippodraw::QtXmlElement class implementation

Copyright (C) 2002-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtXmlElement.cxx,v 1.49 2006/05/17 19:47:13 pfkeb Exp $

*/

#include "QtXmlElement.h"

#include "pattern/string_convert.h"

#include <qstring.h>

#include <cassert>

using std::list;
using std::string;

using namespace hippodraw;

QtXmlElement::QtXmlElement ( const QtXmlElement & element )
  : XmlNode (),
    XmlElement (), 
    QtXmlNode ( element )
{
  const QDomElement & elem 
    = reinterpret_cast < const QDomElement & > ( element.m_node );

  // make copy of pointer so we don't have to cast in the implementation.
  m_element = elem;
  m_node = m_element;
}

/** @todo Remove this method.  It was only temporarily here to aid in
    the transition from the old scheme.
*/
QtXmlElement::QtXmlElement ( const QDomElement & element )
  : QtXmlNode ( )
{
  // make copy of pointer so we don't have to cast in the implementation.
  m_element = element;
  m_node = m_element;
}

QtXmlElement::
~QtXmlElement()
{
}

string 
QtXmlElement::tagName () const
{
  QString tagname = m_element.tagName ();

  return tagname.toStdString ();
}

int QtXmlElement::getID () const
{
  QString value = m_element.attribute ( "id" );
  if ( value == QString::null ) return 0;

  bool ok;
  int id = value.toInt ( &ok );
  if ( ok == false ) id = 0;

  return id;
}

#ifdef CLONE_DEFECT
XmlElement *
#else
QtXmlElement *
#endif 
QtXmlElement::
getNode ( const std::string & tagName ) const
{

  QString tagname ( tagName.c_str () );
  QDomNode node = m_element.namedItem ( tagname );
  if ( node.isNull () ) return 0;

  QDomElement element = node.toElement ();
  assert ( element.isNull () == false );
  return new QtXmlElement ( element );
}

/** @todo This needs to return only immediate children with given
    name.  Doesn't hurt us yet. */
void
QtXmlElement::
fillNodeList ( const std::string & tagName, 
	       std::list < XmlElement * > & nodeList ) const
{
  QString tag_name ( tagName.c_str() );


  QDomNodeList nodelist = m_element.elementsByTagName ( tag_name );
  unsigned int size ( nodelist.count () );
  for ( unsigned int i = 0; i < size; i++ ) {
    QDomNode node = nodelist.item ( i );
    QDomElement element = node.toElement ();
    nodeList.push_back ( new QtXmlElement ( element ) );
  }

}

void
QtXmlElement::
setAttribute ( const std::string & name, int value )
{
  assert ( ! name.empty () );
  m_element.setAttribute ( name.c_str(), value );
}


void
QtXmlElement::
setAttribute ( const std::string & name, bool yes )
{
  assert ( ! name.empty () );

  unsigned int value = yes ? 1 : 0;
  m_element.setAttribute ( name.c_str(), value );
}

void
QtXmlElement::
setAttribute ( const std::string & name, unsigned int value )
{
  assert ( ! name.empty () );
  m_element.setAttribute ( name.c_str(), value );
}

void
QtXmlElement::
setAttribute ( const std::string & name, float value )
{
  assert ( ! name.empty () );
  double v = value; // Qt only supports double converstion to string

  setAttribute ( name, v );
}

void
QtXmlElement::
setAttribute ( const std::string & name, double value )
{
  assert ( ! name.empty () );
  // The following lead to a double 36. to be converted to "35.:" for
  // a ViewBase X coordinate.
  //    m_element.setAttribute ( name.c_str(), value );
 string vtext = String::convert ( value );
 const QString qname ( name.c_str() );
 const string text ( vtext.c_str() );
 const QString qtext ( text.c_str() );
 m_element.setAttribute ( qname, qtext );
}

void
QtXmlElement::
setAttribute ( const std::string & name,
	       const std::string & value )
{
  assert ( ! name.empty () );
  m_element.setAttribute ( name.c_str(), value.c_str() );
}

bool
QtXmlElement::
attribute ( const std::string & name, int & value ) const
{
  QString rstring = m_element.attribute ( name.c_str() );
  if (  rstring == QString::null ) return false;

  bool ok;
  int val = rstring.toInt ( & ok );
  if ( ! ok ) return false;

  value = val;

  return true;
}

bool
QtXmlElement::
attribute ( const std::string & name, bool & value ) const
{
  QString rstring = m_element.attribute ( name.c_str() );
  if (  rstring == QString::null ) return false;

  bool ok;
  int val = rstring.toInt ( & ok );
  if ( ! ok ) return false;

  value = ( val = 1 ) ? true : false;

  return true;
}

bool
QtXmlElement::
attribute ( const std::string & name, unsigned int & value ) const
{
  QString rstring = m_element.attribute ( name.c_str() );
  if (  rstring == QString::null ) return false;

  bool ok;
  int val = rstring.toUInt ( & ok );
  if ( ! ok ) return false;

  value = val;

  return true;
}

bool
QtXmlElement::
attribute ( const std::string & name, float & value ) const
{
  QString rstring = m_element.attribute ( name.c_str() );
  if (  rstring == QString::null ) return false;

  bool ok;
  float val = rstring.toFloat ( & ok );
  if ( ! ok ) return false;

  value = val;

  return true;
}

bool
QtXmlElement::
attribute ( const std::string & name, double & value ) const
{
  QString rstring = m_element.attribute ( name.c_str() );
  if (  rstring == QString::null ) return false;

  bool ok;
  float val = rstring.toDouble ( & ok );
  if ( ! ok ) return false;

  value = val;

  return true;
}

bool
QtXmlElement::
attribute ( const std::string & name, std::string & value ) const
{
  QString val = m_element.attribute ( name.c_str() );
  if ( val == QString::null ) return false;

  value = val.toStdString ();
  return true;
}

const string &
QtXmlElement::
getText () const
{
  static string text; // don't initialize it.
  QString t = m_element.text ();
  text = t.toStdString ();

  return text;
}
