/* -*- mode: c++ -*- */
/** @file

XmlTextNode class interface

Copyright (C) 2004  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: XmlTextNode.h,v 1.6 2005/10/30 00:10:19 pfkeb Exp $

*/

#ifndef _XmlTextNode_H_
#define _XmlTextNode_H_

#include "XmlNode.h"

#include <string>

namespace hippodraw {

/** A pure virtual base class of XML DOM Text node wrapper. A derived
    class must implement the pure virtual functions probably using an
    external XML package.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API XmlTextNode : public virtual XmlNode
{

private:

  /** A private copy constructor in order to avoid copying. */
  XmlTextNode ( const XmlTextNode & );

protected:

  /** The default constructor. */
  XmlTextNode ();

public:

  /** The destructor. */
  virtual ~XmlTextNode();

  /** Sets the data for the text node.
   */
  virtual void setData ( const std::string & ) = 0;
};

} // namespace hippodraw

#endif // _XmlTextNode_H_
