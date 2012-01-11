/* -*- mode: c++ -*- */

/** @file
 
BinToColorXML class interface

Copyright (C)  2004  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinToColorXML.h,v 1.3 2005/10/30 00:10:18 pfkeb Exp $

*/

#ifndef _BinToColorXML_H_
#define _BinToColorXML_H_

#include "BaseXML.h"


namespace hippodraw {

class BinToColor;

/** A singleton class that is does XML serialization and
    deserialization of derived classes of BinToColor with XML.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API BinToColorXML : public BaseXML
{

private:

  /** The element name for control points.
   */
  std:: string m_ctrlpt;

  /** The attribute name of a control point.
   */
  std::string m_point;

  /** A private copy constructor in order to avoid copying. */
  BinToColorXML ( const BinToColorXML & );

public:

  /** The only public constructor. */
  BinToColorXML ( XmlController * );

  /** Create a Color XML element.  Sets the attribute to represent the
      color of @a color and returns the newly created element. */
  XmlElement * createElement ( const BinToColor & btc );

  /** Sets the attributes of the @a object from the element values. */
  BinToColor * createObject ( const XmlElement * element );

};

} // namespace hippodraw

#endif // _BinToColorXML_H_
