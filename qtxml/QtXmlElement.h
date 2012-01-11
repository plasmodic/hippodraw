/* -*- mode: c++ -*- */

/** @file

hippodraw::QtXmlElement class interface

Copyright (C) 2002-2006 The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtXmlElement.h,v 1.34 2006/02/07 00:34:20 pfkeb Exp $

*/

#ifndef _QtXmlElement_H_
#define _QtXmlElement_H_

#ifdef _MSC_VER
# pragma warning (disable : 4250) // inherits via dominance
#endif

// for clone defect
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "QtXmlNode.h"

#include "xml/XmlElement.h"

using std::list;

namespace hippodraw {

/** An XML element using the Qt XML module.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_QTHIPPOPLOT_API QtXmlElement : public XmlElement, public QtXmlNode
{

private:

  /** The DOM element node wrapped by this object.  

      @note When initialized, must also copy pointer value to
      inherited m_node data member so that base class can work
      correctly.
   */
  QDomElement m_element;

  /** A constructor used internally. */
  QtXmlElement ( const QDomElement & );

public:

  friend class QtXmlDocument;

  /** The copy constructor. */
  QtXmlElement ( const QtXmlElement & );

  /** The destructor.
   */
  virtual ~QtXmlElement ();

  virtual int getID () const;

  XmlElement * create () const;

  virtual std::string tagName () const;

#ifdef CLONE_DEFECT
  virtual XmlElement   * getNode ( const std::string & tagName ) const;
#else
  virtual QtXmlElement * getNode ( const std::string & tagName ) const;
#endif
  virtual void fillNodeList ( const std::string & tagName,
			      std::list < XmlElement * > & nodeList ) const;

  virtual void setAttribute ( const std::string & name, int value );
  virtual void setAttribute ( const std::string & name, bool value );
  virtual void setAttribute ( const std::string & name, unsigned int value );
  virtual void setAttribute ( const std::string & name, float value );
  virtual void setAttribute ( const std::string & name, double value );
  virtual void setAttribute ( const std::string & name, 
			      const std::string & value );

  virtual bool attribute ( const std::string & name, 
			   bool & value ) const;

  virtual bool attribute ( const std::string & name, 
			   int & value ) const;

  virtual bool attribute ( const std::string & name, 
			   unsigned int & value ) const;

  virtual bool attribute ( const std::string & name, 
			   float & value ) const;

  virtual bool attribute ( const std::string & name, 
			   double & value ) const;

  virtual bool attribute ( const std::string & name, 
			   std::string & value ) const;

  virtual const std::string & getText () const;

};

} // namespace hippodraw

#endif // _QtXmlElement_H_
