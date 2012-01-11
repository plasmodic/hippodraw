/* -*- mode: c++ -*- */

/** @file

TransformXML class interface

Copyright (C) 2002, 2003, 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: TransformXML.h,v 1.9 2005/10/30 00:10:19 pfkeb Exp $

*/

#ifndef _TransformXML_H_
#define _TransformXML_H_

#include "BaseXML.h"

namespace hippodraw {

class TransformBase;

/** A class XML creation and parsing of XmlElement for Transform class.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API TransformXML : public BaseXML
{

private:

  /** A private copy constructor in order to avoid copying. */
  TransformXML ( const TransformXML & );

public:

  /** A default constructor for avoiding creation except by itself or
      with derived classes. */
  TransformXML ( XmlController * );

  /** Returns a newly created XmlElement with attributes set for @a
      view. */
  XmlElement * createElement ( const TransformBase & view );

  /** Creates an object derived from TransformBase.  Uses the
      information in it to instantiate and return the object. */
  virtual TransformBase * createObject ( const XmlElement * element );

};

} // namespace hippodraw

#endif // _TransformXML_H_
