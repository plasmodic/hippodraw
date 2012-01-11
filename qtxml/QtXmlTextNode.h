/* -*- mode: c++ -*- */
 
/** @file

hippodraw::QtXmlTextNode class interface

Copyright (C) 2004, 2005, 2006  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtXmlTextNode.h,v 1.9 2006/02/07 00:34:21 pfkeb Exp $

*/

#ifndef _QtXmlTextNode_H_
#define _QtXmlTextNode_H_

#ifdef _MSC_VER
# pragma warning (disable : 4250) // inherits via dominance
#endif

// for clone defect
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "QtXmlNode.h"

#include "xml/XmlTextNode.h"

#include <qdom.h>

namespace hippodraw {

/** An XML Dom text node using the Qt XML module.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_QTHIPPOPLOT_API QtXmlTextNode : public XmlTextNode, public QtXmlNode
{

private:

  /** The DOM text node wrapped by this object. */
  QDomText m_text_node;

  QtXmlTextNode ( const QDomText & );

 public:

  friend class QtXmlDocument;

  /** The copy constructor. */
  QtXmlTextNode ( const QtXmlTextNode & );

  /** The destructor.
   */
  ~QtXmlTextNode ();

  /** Sets the data for the node.
   */
  void setData ( const std::string & data );

};

} // namespace hippodraw

#endif // _QtXmlTextNode_H_
