/* -*- mode: c++ -*- */

/** @file
 
BinnerAxisXML class interface

Copyright (C) 2003, 2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinnerAxisXML.h,v 1.6 2005/10/30 00:10:18 pfkeb Exp $

*/

#ifndef _BinnerAxisXML_H_
#define _BinnerAxisXML_H_

#include "BaseXML.h"

namespace hippodraw {

class BinnerAxis;

/** A class XML creation and parsing of XmlElement for BinnerAxis classes.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API BinnerAxisXML : public BaseXML
{

private:

  /** A private copy constructor in order to avoid copying. */
  BinnerAxisXML ( const BinnerAxisXML & );

  /** The attribute name for the width parameter of the binner. */
  std::string m_width;

  /** The attribute name for the high end of the binner's Range. */
  std::string m_high;

  /** The attribute name for the low end of the binner's Range. */
  std::string m_low;

  /** Sets the attributes of the elment.
   */
  void setAttributes ( XmlElement * tag, const BinnerAxis & binner );

public:

  /** A default constructor for avoiding creation except by itself or
      with derived classes. */
  BinnerAxisXML ( XmlController * );

  /** Returns a newly created XmlElement with attributes set for @a
      binner. */
  XmlElement * createElement ( const BinnerAxis & binner );

  /** Creates an object derived from BinnerAxis.  Uses the
      information in it to instantiate and return the object. */
  virtual BinnerAxis * createObject ( const XmlElement * element );

};

} // namespace hippodraw

#endif // _BinnerAxisXML_H_
