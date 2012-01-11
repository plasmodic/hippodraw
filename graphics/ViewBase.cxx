/** @file

hippodraw::ViewBase class implementation

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: ViewBase.cxx,v 1.71 2007/03/23 22:41:33 xiefang Exp $

*/

// inconsistent dll linkage
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "ViewBase.h"

#include "plotters/PlotterBase.h"

#include <cassert>

using std::list;

using namespace hippodraw;

ViewBase::ViewBase( )
  : m_plotter ( 0 )
{
}

ViewBase::ViewBase ( PlotterBase * plotter )
  : m_plotter ( plotter )
{
}

ViewBase::~ViewBase()
{
  // Do not delete plotter here, some derived classes, such as
  // EPSView, are only temporary and don't own the plotter.
}

void
ViewBase::
setPlotter ( PlotterBase * plotter )
{
  m_plotter = plotter;
}

PlotterBase * ViewBase::getPlotter () const
{
  return m_plotter;
}

PlotterBase * ViewBase::getPlotter ()
{
  return m_plotter;
}

void ViewBase::drawSelf ()
{
  assert ( m_plotter );
  m_plotter -> prepareToDraw ();
  m_plotter->drawIn ( this );
}

const Range &
ViewBase::
getRange ( hippodraw::Axes::Type axis ) const
{
  return m_plotter -> getRange ( axis, false );
}

const Rect &
ViewBase::
getUserRect() const
{
  return m_plotter->getUserRect();
}

const Rect &
ViewBase::
getRawRect() const
{
  return m_plotter->getRawRect();
}

double ViewBase::getAspectRatio () const
{
  assert ( m_plotter );
  return m_plotter -> getAspectRatio ();
}

void
ViewBase::
willDelete ( const Observable * observee )
{
  if ( observee == m_plotter ) {
    m_plotter = 0;
  }
}

void
ViewBase::
updateDrawRect ()
{
  // Do nothing in base class.
}

void 
ViewBase::
drawImage ( const std::string &filename, int position )
{
  // Do nothing in base class.
}

void
ViewBase::
drawLatex ( const std::string &eq, int position )
{
  // Do nothing in base class.
}

void 
ViewBase::
drawPolygon ( const std::vector <double > & xpoints,
	      const std::vector <double > & ypoints,
	      const Color & color,
	      const Color & edge )
{
  // Do nothing in base class.
}
