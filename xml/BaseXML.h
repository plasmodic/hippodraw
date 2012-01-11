/* -*- mode: c++ -*- */

/** @file

BaseXML class interface

Copyright (C) 2002, 2003, 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BaseXML.h,v 1.28 2005/10/30 00:10:18 pfkeb Exp $

*/

#ifndef _BaseXML_H_
#define _BaseXML_H_

#include "pattern/libhippo.h"

#include <list>
#include <string>

namespace hippodraw {

class XmlController;
class XmlDocument;
class XmlElement;

/** A base class of XML element controllers.  Its purpose is to
    provide consistency in naming convention of the generated XML
    files across possible concrete XML controllers.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API BaseXML
{

private:

  /** A private copy constructor in order to avoid copying. */
  BaseXML ( const BaseXML & );

protected:

  /** The container type for element nodes.
   */
  typedef std::list < XmlElement * > NodeList_t;

  /** The tag name used for elements created by this class. */
  const std::string m_tagname;

  /** The attribute name for the type of object.  Used to create
      object from one of the classes derived from Factory. */
  const std::string m_type;

  /** The attribute name for the identification of the object. */
  const std::string m_id;

  /** The singleton XML controller object. */
  XmlController * m_controller;

  /** A constructor that takes the tag name and XML controller object
      as arguments.  Since a default constructor does not exist,
      derived class must use this constructor therefore guaranteeing
      that the tag name is always set by derived classes. */
  BaseXML ( const std::string  & tagname, XmlController * );

  /** Sets the unique identification of the object. */
  void setId ( XmlElement & tag, int id );

public:

  /** The destructor. */
  virtual ~BaseXML();

  /** Returns the tag name used by this class. */
  virtual const std::string & tagName () const;

  /** Creates a new element node. Uses the current document as
      determined by the XMLController. */
  XmlElement * createElement ( );

  /** Returns the single child node of @a element of the type that can
      be handled by this object.  If no such node exists, returns a
      null pointer. */
  const XmlElement * getNode ( const XmlElement * element ) const;

  /** Fills the @a nodelist with immediate child nodes of @a element
      with nodes that can be handled by this object. */
  void fillNodeList ( const XmlElement * element, 
		      std::list < XmlElement * > & nodelist );

};

} // namespace hippodraw

#endif // _BaseXML_H_
