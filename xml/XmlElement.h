/* -*- mode: c++ -*- */

/** @file

XmlElement class interface

Copyright (C) 2002-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: XmlElement.h,v 1.31 2005/10/30 00:10:19 pfkeb Exp $

*/

#ifndef _XmlElement_H_
#define _XmlElement_H_

#include "XmlNode.h"

#include <list>
#include <string>

namespace hippodraw {

/** A pure virtual base class of XML element wrapper. A derived class
    must implement the pure virtual functions probably using an
    external XML package.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API XmlElement : public virtual XmlNode
{

private:

  /** A private copy constructor in order to avoid copying. */
  XmlElement ( const XmlElement & );

protected:

  /** The default constructor. */
  XmlElement ();

public:

  /** The type of container used to store XmlElement nodes.
   */
  typedef std::list < XmlElement * > NodeList_t;

  /** The destructor. */
  virtual ~XmlElement();

  /** Returns the tag name of the element. This function is not needed
      for XML processing.  It is only here as an aid in debugging. */
  virtual std::string tagName () const = 0;

  /** Returns the single direct child element node with name @a
      tagName.  If a child of that name doesn't exist returns a null
      pointer. if child exists but is not an element node, an
      assertion is made. */
  virtual XmlElement * getNode ( const std::string & tagName ) const = 0;

  /** Fills the @a nodeList with elements with tag name @a tagName. */
  virtual void 
  fillNodeList ( const std::string & tagName,
		 std::list < XmlElement * > & nodeList ) const = 0;

  /** Returns the ID of the element, if it has one. */
  virtual int getID () const = 0;

  /** Sets attribute named @a name to the int value @a value.
   */
  virtual void setAttribute ( const std::string & name, bool value ) = 0;

  /** Sets attribute named @a name to the int value @a value. */
  virtual void setAttribute ( const std::string & name, int value ) = 0;

  /** Sets attribute named @a name to the unsigned int value @a
      value. */
  virtual void setAttribute ( const std::string & name, 
			      unsigned int value ) = 0;

  /** Sets attribute named @a name to the float value @a value. */
  virtual void setAttribute ( const std::string & name, 
			      float value ) = 0;

  /** Sets attribute named @a name to the double value @a value. */
  virtual void setAttribute ( const std::string & name, 
			      double value ) = 0;

  /** Sets attribute named @a name to the string value @a value. */
  virtual void setAttribute ( const std::string & name,
			      const std::string & value ) = 0;

  /** Sets @a value to the attribute @a name's value.  If attribute of
      name @a name exists returns @c true, otherwise does not change @a
      value and returns @c false.
  */
  virtual bool attribute ( const std::string & name, 
			   bool & value ) const = 0;

  /** Sets @a value to the attribute @a name's value.  If attribute of
      name @a name exists returns @c true, otherwise does not change @a
      value and returns @c false. */
  virtual bool attribute ( const std::string & name, 
			   int & value ) const = 0;

  /** Sets @a value to the attribute @a name's value.  If attribute of
      name @a name exists returns @c true, otherwise does not change @a
      value and returns @c false. */
  virtual bool attribute ( const std::string & name, 
			   unsigned int & value ) const = 0;

  /** Sets @a value to the attribute @a name's value.  If attribute of
      name @a name exists returns @c true, otherwise does not change @a
      value and returns @c false. */
  virtual bool attribute ( const std::string & name, float & value ) const = 0;

  /** Sets @a value to the attribute @a name's value.  If attribute of
      name @a name exists returns @c true, otherwise does not change @a
      value and returns @c false. */
  virtual bool attribute ( const std::string & name, 
			   double & value ) const = 0;

  /** Sets @a value to the attribute @a name's value.  If attribute of
      name @a name exists returns @c true, otherwise does not change @a
      value and returns @c false. */
  virtual bool attribute ( const std::string & name, 
			   std::string & value ) const = 0;

  /** Gets the text node from the element.
   */
  virtual const std::string & getText () const = 0;

};

} // namespace hippodraw

#endif // _XmlElement_H_
