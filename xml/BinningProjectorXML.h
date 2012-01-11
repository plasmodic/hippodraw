/* -*- mode: c++ -*- */

/** @file
 
BinningProjectorXML class interface

Copyright (C) 2003, 2004   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinningProjectorXML.h,v 1.7 2005/10/30 00:10:18 pfkeb Exp $

*/

#ifndef _BinningProjectorXML_H_
#define _BinningProjectorXML_H_

#include "BaseXML.h"

namespace hippodraw {

class BinsBaseXML;
class BinningProjector;
class NTupleXML;
class ProjectorBase;

/** An class XML doing serialization and deserialization of derived
    classes of BinningProjector.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API BinningProjectorXML : public BaseXML
{

private:

  /** A private copy constructor in order to avoid copying. */
  BinningProjectorXML ( const BinningProjectorXML & );

  /** The BinsBaseXML object used by this object. */
  BinsBaseXML * m_binsbaseXML;

  /** The creator of the NTuple element node.
   */
  NTupleXML * m_ntupleXML;

  /** Create the child element nodes.
   */
  void createChildren ( XmlElement * tag, const BinningProjector & projector );

public:

  /** The only public constructor. */
  BinningProjectorXML ( XmlController * );

  /** The destructor.
   */
  ~BinningProjectorXML ();

  /** Returns a newly created XML element with attributes set for
      specified axis. */
  XmlElement * createElement ( const BinningProjector & projector );

  /** Sets the binning attributes of the @a projector from child nodes
      of the @a parent XML element. */
  void getObject ( const XmlElement * parent, ProjectorBase * projector );

  /** Sets the contents of the bins.
   */
  void setBins ( const XmlElement * parent, ProjectorBase * projector );

};

} // namespace hippodraw

#endif // _BinningProjectorXML_H_
