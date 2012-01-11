/** @file

MapMatrixProjectorXML class implementation

Copyright (C)  2003-2005  The Board of Trustees of The Leland Stanford
unior University.  All Rights Reserved.

$Id: MapMatrixProjectorXML.cxx,v 1.10 2007/04/23 21:57:22 pfkeb Exp $

*/

// for truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "MapMatrixProjectorXML.h"

#include "XmlController.h"
#include "XmlDocument.h"
#include "XmlElement.h"

#include "axes/Range.h"
#include "projectors/MapMatrixProjector.h"

namespace hippodraw {

MapMatrixProjectorXML::MapMatrixProjectorXML ( XmlController * controller )
  : BaseXML ( "MapMatrixProjector", controller ),
    m_x_bins ( "xbins" ),
    m_y_bins ( "ybins" ),
    m_x_step ( "xstep" ),
    m_y_step ( "ystep" ),
    m_x_orig ( "xorig" ),
    m_y_orig ( "yorig" )
{
}

XmlElement *
MapMatrixProjectorXML::
createElement ( const MapMatrixProjector & projector )
{
  XmlElement * tag = BaseXML::createElement (); // in base class

  const void * addr = reinterpret_cast < const void * > ( & projector );
  int id = m_controller -> getId ( addr );
  setId ( *tag, id );

  int number = projector.getNumberOfBins ( Axes::X );
  tag -> setAttribute ( m_x_bins, number );
  number = projector.getNumberOfBins ( Axes::Y );
  tag -> setAttribute ( m_y_bins, number );

  double step = projector.getBinWidth ( Axes::X );
  tag->setAttribute ( m_x_step, step );
  step = projector.getBinWidth ( Axes::Y );
  tag->setAttribute ( m_y_step, step );

  double orig = projector.getOffset ( Axes::X );
  tag->setAttribute ( m_x_orig, orig );
  orig = projector.getOffset ( Axes::Y );
  tag->setAttribute ( m_y_orig, orig );

  return tag;
}

void
MapMatrixProjectorXML::
setAttributes ( const XmlElement * element, ProjectorBase * projector )
{
  int number = 0;
  bool ok = element -> attribute ( m_x_bins, number );
  projector -> setNumberOfBins ( Axes::X, number );
  ok = element -> attribute ( m_y_bins, number );
  projector -> setNumberOfBins ( Axes::Y, number );

  double value = 0.;
  ok = element -> attribute ( m_x_step, value );
  projector -> setBinWidth ( Axes::X, value );
  ok = element -> attribute ( m_y_step, value );
  projector -> setBinWidth ( Axes::Y, value );

  ok = element -> attribute ( m_x_orig, value );
  projector -> setOffset ( Axes::X, value );
  ok = element -> attribute ( m_y_orig, value );
  projector -> setOffset ( Axes::Y, value );

  projector -> matrixTranspose(true);
}

} // namespace hippodraw

