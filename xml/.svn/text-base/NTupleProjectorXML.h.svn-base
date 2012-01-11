/* -*- mode: c++ -*- */

/** @file

hippodraw::NTupleProjectorXML class interface

Copyright (C) 2002, 2003, 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: NTupleProjectorXML.h,v 1.19 2005/12/01 19:23:41 pfkeb Exp $

*/

#ifndef _NTupleProjectorXML_H_
#define _NTupleProjectorXML_H_

#include "BaseXML.h"

#include <vector>

namespace hippodraw {

class NTupleProjector;

/** An class XML doing serialization and deserialization of derived
    classes of NTupleProjector.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API NTupleProjectorXML : public BaseXML
{

private:

  /** A private copy constructor in order to avoid copying. */
  NTupleProjectorXML ( const NTupleProjectorXML & );

public:

  /** The only public constructor. */
  NTupleProjectorXML ( XmlController * );

  /** Creates child elements of the element. */
  void createChildren ( XmlElement & tag, const NTupleProjector & proj );

  /** Returns a newly created XML element with its attributes set. */
  XmlElement * createElement ( const NTupleProjector & projector );

  /** Retrieves the reference to the DataSource from the XML @a
      elements's data. */
  int getNTupleReference ( const XmlElement * element ) const;

  /** Retreives the bindings to the HiNTuple from the @a 
      element's data by clearing the list then filling it. */
  void getBindings ( const XmlElement * element,
		     std::vector < std::string > & bindings ) const;

};

} // namespace hippodraw

#endif // _NTupleProjectorXML_H_
