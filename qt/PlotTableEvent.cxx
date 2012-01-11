/** @file 

PlotTableEvent implementation

Copyright (C) 2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: PlotTableEvent.cxx,v 1.4 2006/05/05 23:42:22 pfkeb Exp $

*/

#include "PlotTableEvent.h"

#include <QtCore/QEvent>
#include <QtCore/QEvent>

namespace hippodraw {

PlotTableEvent::
PlotTableEvent ( PlotTableEvent::Type type )
  : QEvent ( QEvent::Type(type) )
{
}

PlotTableEvent::
PlotTableEvent ( const PlotTableEvent & event )
  : QEvent ( event )
{
}

PlotTableEvent::~PlotTableEvent ()
{
}

} // namespace hippodraw

