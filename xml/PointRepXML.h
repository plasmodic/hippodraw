/* -*- mode: c++ -*- */

/** @file
 
PointRepXML class interface

Copyright (C)  2002, 2004, 2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: PointRepXML.h,v 1.27 2007/01/24 21:41:49 xiefang Exp $

*/

#ifndef _PointRepXML_H_
#define _PointRepXML_H_

#include "BaseXML.h"

namespace hippodraw {

class BinToColorXML;
class ColorXML;
class RepBase;

/** A singleton class that is does XML serialization and
    de-serialization of derived classes of RepBase.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API PointRepXML : public BaseXML
{

private:

  /** The ColorXML instance used by this object. */
  ColorXML * m_color_xml;

  /** The BinToColorXML instance used by this object.
   */
  BinToColorXML * m_bintocolor_xml;

  /** The size attribute name.
   */
  std::string a_size;

  /** The style attribute name.
   */
  std::string a_style;

  /** The x error attribute name.
   */
  std::string a_xerror;

  /** The y error attribute name.
   */
  std::string a_yerror;

  /** The text attribute name.
   */
  std::string a_text;

  /** The type of value transform.
   */
  std::string a_value_transform;

  /** Sets the attributes of a PointRep element @a tag. */
  void setAttributes ( XmlElement & tag, const RepBase & rep );

  /** A private copy constructor in order to avoid copying. */
  PointRepXML ( const PointRepXML & );

public:

  /** The only public constructor. */
  PointRepXML ( XmlController * );

  /** The destructor.
   */
  ~PointRepXML ();

  /** Returns a newly created XML element for the RepBase object. */
  XmlElement * createElement ( const RepBase & rep );

  /** Creates an object derived from RepBase from information in the
      XmlElement. Tries both the PointRepFactory and TextRepFactory to
      create the object. If neither is successful, returns a null
      pointer. */
  RepBase * createObject ( const XmlElement * element );

};

} // namespace hippodraw

#endif // _PointRepXML_H_
