/* -*- mode: c++ -*- */

/** @file
 
BinsBaseXML class interface

Copyright (C) 2003, 2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinsBaseXML.h,v 1.9 2005/10/30 00:10:18 pfkeb Exp $

*/

#ifndef _BinsBaseXML_H_
#define _BinsBaseXML_H_

#include "BaseXML.h"

namespace hippodraw {

class BinsBase;
class BinnerAxisXML;

/** A class XML creation and parsing of XmlElement for BinsBase class.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API BinsBaseXML : public BaseXML
{

private:

  /** The attribute name for the axis of the BinnerAxisXML element. */
  std::string m_axis;

  /** The BinsBaseXML object used by this object. */
  BinnerAxisXML * m_binnerXML;

public: // member functions

  /** A default constructor for avoiding creation except by itself or
      with derived classes. */
  BinsBaseXML ( XmlController * );

  /** The destructor.
   */
  ~BinsBaseXML ();

  /** Returns a newly created XmlElement with attributes set for @a
      view. */
  XmlElement * createElement ( const BinsBase & bins );

  /** Creates an object derived from BinsBaseBase.  Uses the
      information in it to instantiate and return the object. */
  virtual BinsBase * createObject ( const XmlElement * element );

private: // member functions

  /** A private copy constructor in order to avoid copying. */
  BinsBaseXML ( const BinsBaseXML & );

  /** Creates the child XML elements of this element. */
  void createChildren ( XmlElement & tag, const BinsBase & bins );

};

} // namespace hippodraw

#endif // _BinsBaseXML_H_
