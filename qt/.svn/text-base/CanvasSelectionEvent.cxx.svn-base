/** @file

CanvasSelectionEvent implementation

Copyright (C) 2004, 2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: CanvasSelectionEvent.cxx,v 1.5 2006/05/05 23:42:22 pfkeb Exp $
*/

#include "CanvasSelectionEvent.h"

#include <QtCore/QEvent>
#include <QtCore/QEvent>

namespace hippodraw {

CanvasSelectionEvent::
CanvasSelectionEvent ( std::vector < PlotterBase * > plotters )
  : QEvent ( QEvent::User ),
    m_plotters ( plotters )
{
}

CanvasSelectionEvent::
CanvasSelectionEvent ( const CanvasSelectionEvent & event )
  : QEvent ( event )
{
}

CanvasSelectionEvent::~CanvasSelectionEvent ()
{
}

const std::vector < PlotterBase * > &
CanvasSelectionEvent::
getPlotters ( ) const
{
  return m_plotters;
}

} // namespace hippodraw

