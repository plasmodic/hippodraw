/* -*- mode: c++ -*- */

/** @file
 
FontXML class interface

Copyright (C) 2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FontXML.h,v 1.4 2005/10/30 00:10:19 pfkeb Exp $

*/

#ifndef _FontXML_H_
#define _FontXML_H_

#include "BaseXML.h"

#include "axes/AxesType.h"

namespace hippodraw {

class FontBase;

/** A class that is does XML serialization and deserialization of
    derived classes of FontBase.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API FontXML : virtual public BaseXML
{

private:

  /** The pointer to the singleton object. */
  static FontXML * s_instance;

  /** The family attribute name.
   */
  std::string m_family;

  /** The italic attribute name.
   */
  std::string m_italic;

  /** The point size attribute name.
   */
  std::string m_size;

  /** The font weight attribute name.
   */
  std::string m_weight;

  /** A private copy constructor in order to avoid copying. */
  FontXML ( const FontXML & );


public:

  /** The only public constructor.  The parameter @a controller must
      object of class derived from XMLController. */
  FontXML ( XmlController * controller );

  /** Sets the attributes of the element from FontBase object.
   */
  void setAttributes ( XmlElement &, const FontBase & );

  /** Returns the axis on which the FontBase object was used.
   */
  hippodraw::Axes::Type getAxis ( const XmlElement * element,
				  const std::string & tagname );

  /** Creates an objected derived from FontBase and sets it
      attributes.
  */
  void setAttributes ( const XmlElement * element, FontBase * font );

};

} // namespace hippodraw

#endif // _FontXML_H_
