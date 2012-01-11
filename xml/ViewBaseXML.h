/* -*- mode: c++ -*- */

/** @file
 
ViewBaseXML class interface

Copyright (C) 2002, 2003, 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: ViewBaseXML.h,v 1.31 2005/10/30 00:10:19 pfkeb Exp $

*/

#ifndef _ViewBaseXML_H_
#define _ViewBaseXML_H_

#include "BaseXML.h"

namespace hippodraw {

class PlotterBaseXML;
class ViewBase;

/** A class XML creation and parsing of XmlElement for ViewBase class.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API ViewBaseXML : public BaseXML
{

private:

  /** Attribute name for the X coordinate. */
  std::string m_x;

  /** Attribute name for the Y coordinate. */
  std::string m_y;

  /** Attribute name for the width. */
  std::string m_w;

  /** Attribute name for the height. */
  std::string m_h;

  /** A private copy constructor in order to avoid copying. */
  ViewBaseXML ( const ViewBaseXML & );

  /** The PlotterBaseXML instance used by this object. */
  PlotterBaseXML * m_plotter_xml;

  /** Create the child element, i.e. one of the PlotterBase. */
  void createChild ( XmlElement & tag, const ViewBase & view );

public:

  /** A default constructor for avoiding creation except by itself or
      with derived classes. */
  ViewBaseXML ( XmlController * );

  /** The destructor.
   */
  ~ViewBaseXML ();

  /** Returns a newly created XmlElement with attributes set for @a
      view. */
  XmlElement * createElement ( const ViewBase & view );

  /** Returns @c true if all the NTuple objects used by the view have
      been save to or read from a file. */
  bool areDataSourcesSaved ( const ViewBase & view );

  /** Creates an object derived from ViewBase.  Uses the information
      in the DOM element to instantiate the object. */
  virtual void getObjects ( const XmlElement * element );

  /** Connects the plotters that reference each other.
   */
  void connectPlotters ( const XmlElement * root );

};

} // namespace hippodraw

#endif // _ViewBaseXML_H_
