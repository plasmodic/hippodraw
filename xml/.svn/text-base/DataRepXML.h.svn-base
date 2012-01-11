/* -*- mode: c++ -*- */

/** @file

hippodraw::DataRepXML class interface

Copyright (C) 2002-2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

The library is distributed under the terms of the GNU Lesser General
Public License.

$Id: DataRepXML.h,v 1.47 2006/07/10 21:53:08 pfkeb Exp $

*/

#ifndef _DataRepXML_H_
#define _DataRepXML_H_

#include "BaseXML.h"

namespace hippodraw {

class BinningProjector;
class BinningProjectorXML;
class DataRep;
class FunctionProjector;
class FunctionProjectorXML;
class MapMatrixProjector;
class MapMatrixProjectorXML;
class NTupleProjector;
class NTupleProjectorXML;
class PointRepXML;
class TextDataRep;

/** A class that is does XML serialization and de-serialization of
    derived classes of DataRep.

    @request Check that histogram with equal bin contents can be archived.

    @request Check that minimum entries per bin can be archived.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket B. Malde <sanket@stanford.edu>
*/
class MDL_HIPPOPLOT_API DataRepXML : public BaseXML
{

private:

  /** The PointRepXML instance used by this class. */
  PointRepXML * m_pointrep;

  /** The NTupleProjectorXML instance used by this class. */
  NTupleProjectorXML * m_nTupleProjector;

  /** the BinningProjectorXML instance used by this class. */
  BinningProjectorXML * m_binningProjectorXML;

  /** The FunctionProjectorXML instance used by this class. */
  FunctionProjectorXML * m_functionProjector;

  /** The MapMatrixProjectorXML instance used by this class. 
   */
  MapMatrixProjectorXML * m_mapMatrixProjectorXML;

  /** The attribute name for number of contours. */
  std::string m_contours;

  /** Creates the children elements of this element. */
  void createChildren ( XmlElement & tag, const hippodraw::DataRep & rep );

  /** Creates the child elements in special case where the DataRep
      object contained a NTupleProjector object. */
  void createChildren ( XmlElement & tag, 
			const NTupleProjector * projector );

  /** Creates the child elements in special case where the DataRep
      object contains a BinningProjector object. */
  void createChildren ( XmlElement & tag,
			const BinningProjector * projector );

  /** Creates the child elements in special case where the DataRep
      object contains a MapMatrixProjector object. 
  */
  void createChildren ( XmlElement & tag,
			const MapMatrixProjector * projector );

  /** Creates the child elements in special case where the DataRep
      object contained a FunctionProjector object. */
  void createChildren ( XmlElement & tag, 
			const FunctionProjector * projector );

  /** Creates the child element for the function target. */
  void createFunctionTargetChild ( XmlElement & tag,
				   const hippodraw::DataRep & rep );

  /** Creates and returns a DataRep object that does not have NTuple
      bindings.
  */
  hippodraw::DataRep * createDataRep ( const XmlElement * element );

  /** Creates and returns a DataRep object that has NTuple
      bindings. These objects are the ones that can be created from
      the DataRepFactory. Uses the XML element @a dr_element for the
      %DataRep information and the XML element @a ntp_element for the
      %NTuple information.*/
  hippodraw::DataRep * createDataRep ( const XmlElement * dr_element,
				       const XmlElement * ntp_element );

  /** Creates and returns a DataRep object that is a TextDataRep.
      These objects are the ones that can be created from the
      TextRepFactory. */
  hippodraw::DataRep * getTextRepObject ( const XmlElement * );

public:

  /** A constructor taking an XML Controller as argument. */
  DataRepXML ( XmlController * );

  /** The destructor.
   */
  ~DataRepXML ();

  /** Returns a newly created XML element with its attributes set. */
  XmlElement * createElement ( const hippodraw::DataRep & rep );

  /** Returns a newly created object based on the XmlElement. */
  hippodraw::DataRep * createObject ( const XmlElement * element );

};

} // namespace hippodraw

#endif // _DataRepXML_H_
