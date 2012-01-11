/* -*- mode: c++ -*- */

/** @file

QtXmlDocument class interface

Copyright (C) 2002, 2003, 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtXmlDocument.h,v 1.15 2005/10/30 00:10:16 pfkeb Exp $

*/

#ifndef _QtXmlDocument_H_
#define _QtXmlDocument_H_

#include "qt/libqthippo.h"

#include "xml/XmlDocument.h"

#include <qdom.h>

namespace hippodraw {

/** An XML DOM  Document handler  using the Qt XML module.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_QTHIPPOPLOT_API QtXmlDocument : public XmlDocument
{

private:

  /** The DOM element wrapped by this object. */
  QDomDocument m_document;

  /** A private copy constructor in order to avoid copying. */
  QtXmlDocument ( const QtXmlDocument & );

public:

  /** The public constructor taking a Qt XML module QDocument as argument. */
  QtXmlDocument ( QDomDocument document );

  QtXmlDocument ( const std:: string & name );

  /** The dstructor.
   */
  ~QtXmlDocument ();

  virtual XmlElement * documentElement ( ) const;
  virtual XmlElement * createElement ( const std::string & tagName );
  virtual XmlTextNode * createTextNode ( const std::string & tag );
  virtual void appendChild ( XmlElement & );
  virtual Status saveToFile ( const std::string & filename );
  virtual Status setContent ( const std::string & filename );

};

} // namespace hippodraw

#endif // _QtXmlDocument_H_
