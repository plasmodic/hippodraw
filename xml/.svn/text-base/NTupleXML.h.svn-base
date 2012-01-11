/* -*- mode: c++ -*- */

/** @file

NTupleXML class interface

Copyright (C)  2002, 2004  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: NTupleXML.h,v 1.28 2005/10/30 00:10:19 pfkeb Exp $


*/

#ifndef _NTupleXML_H_
#define _NTupleXML_H_

#include "BaseXML.h"

#include <vector>

namespace hippodraw {

class DataSource;

/** A singleton class that is does XML serialization and
    deserialization of derived classes of NTuple.

    @request Need to save shape of NTuple in order to implement saving
    St2DHistogram.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API NTupleXML : virtual public BaseXML
{

private:

  /** A private copy constructor in order to avoid copying. */
  NTupleXML ( const NTupleXML & );

  /** The tag name for the name attribute. */
  std::string m_name;

  /** the attribute name for the column index.
   */
  std::string m_col_name;

  /** The tag name of the column data.
   */
  std::string m_data;

  /** The attribute name of the column label.
   */
  std::string m_label;

  /** Sets the attributes.
   */
  void setAttributes ( XmlElement * tag, const DataSource & ntuple );

  /** Creates the Dom text node children.
   */
  void createChildren ( XmlElement * tag, const DataSource & ntuple );

  /** Creates the Dom text node children for 1D histograms.
   */
  void createChildren2D ( XmlElement * tag, const DataSource & ntuple );

  /** Creates the Dom text node children for 2D histograms.
   */
  void createChildren3D ( XmlElement * tag, const DataSource & ntuple );

public:

  /** The only public constructor. */
  NTupleXML ( XmlController * );

  /** Returns a newly created XmlElement for @a DataSource. 
   */
  XmlElement * createElement ( const DataSource & ntuple );

  /** Returns the DataSource  corresponding to XmlElement.
   */
  const DataSource * getObject ( const XmlElement & tag );

  /** Returns the name of the NTuple object.
   */
  std::string getName ( const XmlElement & tag ) const;

};

} // namespace hippodraw

#endif // _NTupleXML_H_
