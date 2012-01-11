/* -*- mode: c++ -*- */

/** @file

ColorXML class interface

Copyright (C)  2002, 2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: ColorXML.h,v 1.7 2005/10/30 00:10:19 pfkeb Exp $

*/

#ifndef _ColorXML_H_
#define _ColorXML_H_

#include "BaseXML.h"

namespace hippodraw {

class Color;

/** A singleton class that is does XML serialization and
    deserialization of derived classes of Color with XML.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API ColorXML : public BaseXML
{

private:

  /** A private copy constructor in order to avoid copying. */
  ColorXML ( const ColorXML & );

  /** Attribute name for red. */
  std::string m_red;

  /** Attribute name for green. */
  std::string m_green;

  /** Attribute name for blue. */
  std::string m_blue;

public:

  /** The only public constructor. */
  ColorXML ( XmlController * );

  /** Create a Color XML element.  Sets the attribute to represent the
      color of @a color and returns the newly created element. */
  XmlElement * createElement ( const Color & color );

  /** Sets the attributes of the @a object from the element values. */
  Color * createObject ( const XmlElement * element );

};

} // namespace hippodraw

#endif // _ColorXML_H_
