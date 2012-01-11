/* -*- mode: c++ -*- */

/** @file
 
AxisTickXML class interface

Copyright (C) 2002-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: AxisTickXML.h,v 1.2 2005/10/30 00:10:18 pfkeb Exp $

*/

#ifndef _AxisTickXML_H_
#define _AxisTickXML_H_

#include "BaseXML.h"

namespace hippodraw {

class AxisTick;

/** A class that is does XML serialization and de-serialization of
    AxisTick with XML.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API AxisTickXML : virtual public BaseXML
{

private:

  /** The pointer to the singleton object.
   */
  static AxisTickXML * s_instance;

  /** The attribute name for tick values. 
   */
  std::string m_value;

  /** The attribute name for tick labels.
   */
  std::string m_label;

  /** A private copy constructor in order to avoid copying. */
  AxisTickXML ( const AxisTickXML & );

public:

  /** The only public constructor.  The parameter @a controller must
      object of class derived from XMLController. */
  AxisTickXML ( XmlController * controller );

  /** Sets the attributes of the XmlElement @a tag from information in
      the AxisTick object @a tick.
  */
  void setAttributes ( XmlElement & tag, const AxisTick & tick );

  /** Sets the attributes of the AxisTick object @a tick from
      the information in the XmlElement @a element.
  */
  void setAttributes ( AxisTick * tick, const XmlElement * element );

};

} // namespace hippodraw

#endif // _AxisTickXML_H_
