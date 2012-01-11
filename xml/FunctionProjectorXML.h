/* -*- mode: c++ -*- */

/** @file

FunctionProjectorXML class interface

Copyright (C) 2002, 2003, 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FunctionProjectorXML.h,v 1.18 2005/10/30 00:10:19 pfkeb Exp $

*/

#ifndef _FunctionProjectorXML_H_
#define _FunctionProjectorXML_H_

#include "BaseXML.h"

namespace hippodraw {

class DataRep;
class FunctionBase;
class FunctionProjector;

/** An class XML doing serialization and deserialization of derived
    classes of FunctionProjector.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket B. Malde <sanket@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API FunctionProjectorXML : public BaseXML
{

private:

  /** A private copy constructor in order to avoid copying. */
  FunctionProjectorXML ( const FunctionProjectorXML & );

public:

  /** The only public constructor. */
  FunctionProjectorXML ( XmlController * );

  /** Returns a newly created XmlElement with its attributes set for
      @a projector. */
  XmlElement * createElement ( const FunctionProjector & projector );

  /** Get the function name from the XML @a element. */
  std::string getFunctionName ( const XmlElement * element );

  /** Set the values for the function from the XML @a element. */
  void setAttributes ( const XmlElement * element, FunctionBase * function );

  /** Creates a FunctionRep from @a element with @a target DataRep.
   */
  hippodraw::DataRep *
  createFunctionRep ( const XmlElement * element,
		      hippodraw::DataRep * target );

};

} // namespace hippodraw

#endif // _FunctionProjectorXML_H_
