/** @file 

hippodraw::PlotterEvent implementation

Copyright (C) 2003-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: PlotterEvent.cxx,v 1.6 2006/06/13 20:50:28 pfkeb Exp $

*/

#include "PlotterEvent.h"

using namespace hippodraw;

PlotterEvent::
PlotterEvent ( PlotterBase * plotter )
  : QEvent ( QEvent::User ),
    m_plotter(plotter)
{
}

PlotterEvent::
PlotterEvent ( const PlotterEvent & event )
  : QEvent ( event )
{
}

PlotterEvent::~PlotterEvent ()
{
}

PlotterBase *
PlotterEvent::
plotter ( ) const
{
  return m_plotter;
}
