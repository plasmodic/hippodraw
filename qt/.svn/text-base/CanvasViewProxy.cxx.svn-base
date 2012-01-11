/** @file

hippodraw::CanvasViewProxy class implementation

Copyright (C) 2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: CanvasViewProxy.cxx,v 1.9 2006/08/07 19:07:02 pfkeb Exp $

*/

#include "CanvasViewProxy.h"

#include "CanvasView.h"

#if _MSC_VER
#include "PlotterEvent.h"
#endif

#include "qapplication.h"

using namespace hippodraw;

CanvasViewProxy::
CanvasViewProxy ( CanvasView * view )
  : m_canvas_view ( view )
{
  connect ( this, SIGNAL ( addDisplaySignal ( PlotterBase * ) ),
	    m_canvas_view, SLOT ( addPlotDisplay ( PlotterBase * ) ) );

  connect ( this, SIGNAL ( saveAsImageSignal ( const PlotterBase *,
					       const std::string & ) ),
	    m_canvas_view,  SLOT ( savePlotAsImage ( const PlotterBase *,
						     const std::string & ) ) );

  connect ( this, SIGNAL ( swapOrientationSignal ( ) ),
	    m_canvas_view,  SLOT ( swapOrientation ( ) ) );

  connect ( this, SIGNAL ( clearSignal () ),
	     m_canvas_view, SLOT ( clear () ) );

}

CanvasViewProxy::
~CanvasViewProxy ()
{
}

void
CanvasViewProxy::
addDisplay ( PlotterBase * plotter )
{
  emit addDisplaySignal ( plotter );
}

void
CanvasViewProxy::
clear ()
{
  emit clearSignal ();
}

void
CanvasViewProxy::
saveAsImage ( const PlotterBase * plotter, const std::string & filename )
{
  emit saveAsImageSignal ( plotter, filename );
}

void
CanvasViewProxy::
swapOrientation ()
{
  emit swapOrientationSignal ();
}
