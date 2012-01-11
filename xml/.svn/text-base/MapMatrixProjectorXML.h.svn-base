/* -*- mode: c++ -*- */

/** @file

MapMatrixProjectorXML class interface

Copyright (C) 2003, 2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: MapMatrixProjectorXML.h,v 1.5 2005/10/30 00:10:19 pfkeb Exp $

*/

#ifndef _MapMatrixProjectorXML_H_
#define _MapMatrixProjectorXML_H_

#include "BaseXML.h"

namespace hippodraw {

class MapMatrixProjector;
class ProjectorBase;

/** An class XML doing serialization and de-serialization of derived
    classes of MapMatrixProjector.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API MapMatrixProjectorXML : public BaseXML
{

private:

  /** A private copy constructor in order to avoid copying. */
  MapMatrixProjectorXML ( const MapMatrixProjectorXML & );

  /** Name of attribute for number X bins. 
   */
  std::string m_x_bins;

  /** Name of attribute for number Y bins. 
   */
  std::string m_y_bins;

  /** Name of attribute for number step size in X direction. 
   */
  std::string m_x_step;

  /** Name of attribute for number step size in Y direction. 
   */
  std::string m_y_step;

  /** Name of attribute for origin along X direction. 
   */
  std::string m_x_orig;

  /** Name of attribute for origin along Y direction. 
   */
  std::string m_y_orig;

public:

  /** The only public constructor. */
  MapMatrixProjectorXML ( XmlController * );

  /** Returns a newly created XML element with attributes set. 
   */
  XmlElement * createElement ( const MapMatrixProjector & projector );

  /** Sets the MapMatrixProjector attributes of the @a projector from
      child nodes of the @a parent XML element. 
  */
  void setAttributes ( const XmlElement * element,
		       ProjectorBase * projector );

};

} // namespace hippodraw

#endif // _MapMatrixProjectorXML_H_
