/* -*- mode: c++ -*- */

/** @file
 
AxisModelXML class interface

Copyright (C) 2002-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: AxisModelXML.h,v 1.13 2005/10/30 00:10:18 pfkeb Exp $

*/

#ifndef _AxisModelXML_H_
#define _AxisModelXML_H_

#include "BaseXML.h"

#include "axes/AxesType.h"

namespace hippodraw {

class AxisModelBase;
class AxisTickXML;

/** A class that is does XML serialization and de-serialization of
    derived classes of AxisModelBase with XML.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API AxisModelXML : virtual public BaseXML
{

private:

  /** The pointer to the singleton object. */
  static AxisModelXML * s_instance;

  /** The AxisTick XML handler.
   */
  AxisTickXML * m_axistick_xml;

  /** The auto ranging attribute name. */
  std::string m_autorange;

  /** The low end of Range attribute name. */
  std::string m_low;

  /** The high end of Range attribute name. */
  std::string m_high;

  /** the scale factor attribute name. */
  std::string m_scale_factor;

  /** The attribute name for axis model that is logarithmic. */
  std::string m_log;

  /** The attribute name for axis model that wants auto tick
      generation.
  */
  std::string m_auto_tick;

  /** A private copy constructor in order to avoid copying. */
  AxisModelXML ( const AxisModelXML & );

  /** Creates the children elements.
   */
  void createChildren ( XmlElement &, const AxisModelBase & );

  /** Creates the children objects.
   */
  void createChildren ( const XmlElement * element, AxisModelBase * model );

public:

  /** The only public constructor.  The parameter @a controller must
      object of class derived from XMLController. */
  AxisModelXML ( XmlController * controller );

  /** The destructor.
   */
  ~AxisModelXML ();

  /** Sets the attributes of the XmlElement @a tag from information in
      the AxisModel object @a plotter */
  void setAttributes ( XmlElement & tag, const AxisModelBase & plotter );

  /** Sets the attributes of the AxisModel object @a plotter from
      the information in the XmlElement @a element */
  void setAttributes ( AxisModelBase * model, const XmlElement * element );

  /** Returns the Axis attribute value. */
  hippodraw::Axes::Type getAxis ( const XmlElement * element, 
				  const std::string & tagname );

  /** Returns @c true if the element represents an axis model that is
      on a logarithmic scale. */
  bool isLog ( const XmlElement * element );

};

} // namespace hippodraw

#endif // _AxisModelXML_H_
