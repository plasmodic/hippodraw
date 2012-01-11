/* -*- mode: c++ -*- */

/** @file

XmlDocument class interface

Copyright (C) 2002, 2003, 2005  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: XmlDocument.h,v 1.15 2005/10/30 00:10:19 pfkeb Exp $

*/

#ifndef _XmlDocument_H_
#define _XmlDocument_H_

#include "pattern/libhippo.h"

#include <string>

namespace hippodraw {

class XmlElement;
class XmlTextNode;

/** An abstract base class of XML document element wrapper. A derived
    class must implement the pure virtual functions probably using an
    extern XML package.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API XmlDocument
{

public:
  /** Status codes for opening an XML document. */
  enum Status { Success, OpenError, ParseError, WriteError };

private:

  /** A private copy constructor in order to avoid copying. */
  XmlDocument ( const XmlDocument & );

protected:

  /** The default constructor. */
  XmlDocument ();

public:

  /** The destructor. */
  virtual ~XmlDocument();

  /** Returns the root document element. */
  virtual XmlElement * documentElement ( ) const = 0;

  /** Creates a new DOM element wrapper object and returns a pointer
      to it. The receiver takes procession of the object and is
      responsible for its deletion. */
  virtual XmlElement * createElement ( const std::string & tagName ) = 0;

  /** Creates a new DOM Text node.*/
  virtual XmlTextNode * createTextNode ( const std::string & tag ) = 0;

  /** Appends the root element to the document. */
  virtual void appendChild ( XmlElement & ) = 0;

  /** Saves the document to the file.  @return XmlDocument::Success if
      successful, otherwise XmlDocument::WriteError. */
  virtual Status saveToFile ( const std::string & filename ) = 0;

  /** Sets the content of the XML document from the text in the file
      @a filename.  Also sets the application's current working
      directory to the path to the file.  This is done so that files
      referenced by the XML document can be found relative to the
      document itself.  
      @return XmlDocument::Success if successful.
      XmlDocument::OpenError if the file could not be opened
      or XmlDocument::ParseError if the file was not successfully
      parsed. */
  virtual Status setContent ( const std::string & filename ) = 0;

};

} // namespace hippodraw

#endif // _XmlDocument_H_
