/* -*- mode: c++ -*- */

/** @file
 
TupleCutXML class interface

Copyright (C)  2002, 2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: TupleCutXML.h,v 1.12 2005/10/30 00:10:19 pfkeb Exp $

*/

#ifndef _TupleCutXML_H_
#define _TupleCutXML_H_

#include "BaseXML.h"

#include "axes/AxesType.h"

#include <vector>

namespace hippodraw {

class TupleCut;

/** A class that is does XML serialization and de-serialization of
    derived classes of TupleCut.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API TupleCutXML : public BaseXML
{

private:

  /** The pointer to the singleton object. */
  static TupleCutXML * s_instance;

  /** The low end of Range attribute name. */
  std::string m_low;

  /** The high end of Range attribute name. */
  std::string m_high;

  /** The inversion attribute name. */
  std::string m_invert;

  /** The column index attribute name. */
  std::string m_column;

  /** The number of dimension attribute name.  This attribute is
      obsolete.  But it continues to be used with a negative value to
      indicate that all TupleCut objects are one dimensional.  The
      lack of this attribute was a bug, and 2D TupleCut objects
      weren't created from document.
   */
  std::string m_dim;

  /** The axis attribute name
   */
  std::string m_axis;

  /** A private copy constructor in order to avoid copying. */
  TupleCutXML ( const TupleCutXML & );

  /** Creates the children elements. */
  void createChildren ( XmlElement &, const TupleCut & );

  /** Sets the attributes for each axis of the TupleCut.
   */
  void setAxisAttributes ( TupleCut * cut,
			   hippodraw::Axes::Type axis,
			   const XmlElement * element ) const;

public:

  /** The only public constructor.  The parameter @a controller must
      object of class derived from XMLController. */
  TupleCutXML ( XmlController * controller );

  /** Returns a newly created XmlElement with attributes set for the
      TupleCut objects.
  */
  XmlElement * createElement ( unsigned int i, const TupleCut & cut );

  /** Returns @c true if element contains old style NTuple, i.e. one
      that had a non-negative dimension attribute or none.
  */
  bool hasMultiDimTupleCut ( const XmlElement * element ) const;

  /** Creates an TupleCut object and sets its properties from the
      XmlElement. */
  TupleCut * getObject ( const XmlElement & tag ) const;

  /** Creates TupleCut objects from old style multidimensional
      TupleCut.
   */
  void getObjects ( const XmlElement * element,
		    std::vector < TupleCut * > & cuts );

  /** Sets the attributes of the TupleCut object @a plotter from
      the information in the XmlElement @a element.
  */
  void setAttributes ( TupleCut * & cut, const XmlElement * element ) const;

};

} // namespace hippodraw

#endif // _TupleCutXML_H_
