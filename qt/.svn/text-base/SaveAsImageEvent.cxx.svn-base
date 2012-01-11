/** @file 

SaveAsImageEvent implementation

Copyright (C) 2004   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: SaveAsImageEvent.cxx,v 1.4 2006/05/05 23:42:22 pfkeb Exp $
*/

#include "SaveAsImageEvent.h"

namespace hippodraw {

SaveAsImageEvent::
SaveAsImageEvent ( const PlotterBase * plotter, 
		   const std::string & filename )
  : QEvent(QEvent::User),
    m_filename(filename),
    m_plotter(plotter)
{
}

SaveAsImageEvent::
SaveAsImageEvent ( const SaveAsImageEvent & event )
  : QEvent ( event ),
    m_filename ( event.m_filename )
{
}

SaveAsImageEvent::~SaveAsImageEvent ()
{
}

const PlotterBase *
SaveAsImageEvent::
plotter ( ) const
{
  return m_plotter;
}

const std::string & 
SaveAsImageEvent::
filename () const
{
  return m_filename;
}

} // namespace hippodraw

