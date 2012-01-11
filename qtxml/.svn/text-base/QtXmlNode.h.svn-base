/* -*- mode:c++ -*- */

/** @file

hippodraw::QtXmlNode class interface

Copyright (C) 2004-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: QtXmlNode.h,v 1.6 2006/02/07 00:34:20 pfkeb Exp $

*/

#ifndef _QtXmlNode_H_
#define _QtXmlNode_H_

#include "qt/libqthippo.h"


// for clone defect
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "xml/XmlNode.h"

#include <qdom.h>

namespace hippodraw {

/** An XML element using the Qt XML module.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_QTHIPPOPLOT_API QtXmlNode : virtual public XmlNode
{

 protected:

  /** The DOM node wrapped by this object. */
  QDomNode m_node;

  /** The default constructor.
   */
  QtXmlNode ();

  /** The destructor.
   */
  virtual ~QtXmlNode ();

public:

  friend class QtXmlDocument;

  /** The copy constructor. */
  QtXmlNode ( const QtXmlNode & );

  virtual void appendChild ( const XmlNode & child );


};

} // namespace hippodraw

#endif // _QtXmlNode_H_
