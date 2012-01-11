/* -*- mode: c++ -*- */

/** @file

XmlNode class interface

Copyright (C) 2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: XmlNode.h,v 1.3 2005/10/30 00:10:19 pfkeb Exp $

*/

#ifndef _XmlNode_H_
#define _XmlNode_H_

#include "pattern/libhippo.h"

namespace hippodraw {

/** A pure virtual base class of XML node wrappers. A derived class
    must implement the pure virtual functions probably using an
    external XML package.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API XmlNode
{

private:

  /** A private copy constructor in order to avoid copying. */
  XmlNode ( const XmlNode & );

protected:

  /** The default constructor. */
  XmlNode ();

public:

  /** The destructor. */
  virtual ~XmlNode();

  /** Appends a child element to the element. */
  virtual void appendChild ( const XmlNode & child ) = 0;

};

} // namespace hippodraw

#endif // _XmlNode_H_
