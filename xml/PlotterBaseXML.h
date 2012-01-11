/* -*- mode: c++ -*- */

/** @file

PlotterBaseXML class interface

Copyright (C) 2002-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: PlotterBaseXML.h,v 1.62 2007/02/03 00:55:38 xiefang Exp $

*/

#ifndef _PlotterBaseXML_H_
#define _PlotterBaseXML_H_

#include "BaseXML.h"

#include "axes/AxesType.h"

namespace hippodraw {

class DataRep;
class AxisModelXML;
class CutPlotter;
class DataRepXML;
class FontXML;
class PlotterBase;
class PointRepXML;
class TextPlotter;
class TransformXML;
class TupleCutXML;
class XyPlotter;

/** A class that is does XML serialization and de-serialization of
    derived classes of PlotterBase.  

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API PlotterBaseXML : public BaseXML
{

private:

  /** The pointer to the singleton object. */
  static PlotterBaseXML * s_instance;

  /** The AxisModelXML instance used by this object. */
  AxisModelXML * m_axismodel;

  /** The DataRepXML instance used by this object. */
  DataRepXML * m_datarep;

  /** The PointRepXML instance used by this object. */
  PointRepXML * m_pointrep;

  /** The TransfromXML instance used by this object. */
  TransformXML * m_transform_xml;

  /** The TupleCutXML instance used by this object. */
  TupleCutXML * m_tuple_cut_xml;

  /** The FontXML instance used by this object. */
  FontXML * m_font;

  /** The attribute name of labeling an axis for the AxisModelBase
      elements.
  */
  std::string m_axis;

  /** The attribute name for the plot title.
   */
  std::string m_title;

  /** The attribute name for X axis label.
   */
  std::string m_x_label;

  /** The attribute name for Y axis label.
   */
  std::string m_y_label;

  /** The attribute name for Z axis label.
   */
  std::string m_z_label;

  /** The attribute name used to save and restore index of parent
      DataRep.
 */
  std::string m_pindex;

  /** A private copy constructor in order to avoid copying. */
  PlotterBaseXML ( const PlotterBaseXML & );

  /** Creates an axis model child element for the specified @a axis. */
  void createAxisModel ( XmlElement &, const PlotterBase &, 
			 hippodraw::Axes::Type axis );

  /** Re-creates AxisModelBase objects from the XML @a element. */
  void createAxisModels ( const XmlElement * element, PlotterBase * plotter );

  /** Creates all the font elements, if needed.
   */
  void createFontElements ( XmlElement & tag, const XyPlotter & plotter );

  /** Creates an element for label font.
   */
  void createFontElement ( XmlElement & tag,
			   const XyPlotter & plotter,
			   hippodraw::Axes::Type axis );

  /** Creates the children elements. */
  void createChildren ( XmlElement &, const PlotterBase & );

  /** Creates the child elements for the special case when the @a plotter
      is a derived class of CutPlotter.
  */
  void createCutChildren ( XmlElement &, const CutPlotter & plotter );

  /** Creates the child elements for the special case when the @a
      plotter is a TextPlotter.  It adds a element reference the
      target DataRep of its own TextDataRep object. */
  void createTextChildren ( XmlElement &, const TextPlotter & plotter );

  /** Creates the plotter object from the XML @a element. */
  PlotterBase * createPlotter ( const XmlElement * element );

  /** Handles the Cut attributes from the XML @a element.
   */
  void handleCutPlotter ( const XmlElement * element, 
			  CutPlotter * plotter );

  /** Handles the DataRep object if it is FunctionRep. */
  void handleFunction ( const XmlElement * element,
			hippodraw::DataRep * rep );

  /** Handles the TextPlotter special attributes from the XML @a
      element. Returns 1 if successful, 0 otherwise. */
  int handleTextPlotter ( const XmlElement * element, TextPlotter * plotter );

  /** Searches for and connects up the target DataRep objects of the
      @a plotter. */
  void getCutTargets ( const XmlElement * element, CutPlotter * plotter );

protected:

  /** A constructor used by derived class to override the node name. */
  PlotterBaseXML ( const std::string &, XmlController * controller );

  /** Creates the FontBase objects, if any.
   */
  void createFontObjects ( const XmlElement *, PlotterBase * plotter );

public:

  /** The only public constructor.  The parameter @a controller must
      object of class derived from XMLController. */
  PlotterBaseXML ( XmlController * controller );

  /** The destructor.
   */
  ~PlotterBaseXML ();

  /** Returns @c true if all the NTuple objects used by the plotter
      are save to or read from a file. */
  bool areDataSourcesSaved ( const PlotterBase & plotter );

  /** Returns a newly created XmlElement with attributes set for @a
      plotter. */
  XmlElement * createElement ( const PlotterBase & plotter );

  /** Returns the PlotterBase object represented by the @a element. */
  virtual PlotterBase *  getObject ( const XmlElement * element );

};

} // namespace hippodraw

#endif // _PlotterBaseXML_H_
