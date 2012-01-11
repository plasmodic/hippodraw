/** @file

QtViewFactory implemenatation

Copyright (C) 2001, 2003, 2004   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: QtViewFactory.cxx,v 1.6 2005/10/30 00:10:16 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "QtViewFactory.h"

#include "QtFont.h"
#include "QtView.h"

#include "plotters/PlotterBase.h"

namespace hippodraw {

QtViewFactory::QtViewFactory( )
  : ViewFactory()
{
}

ViewFactory * QtViewFactory::instance ()
{
  if ( m_instance == 0 ) {
    m_instance = new QtViewFactory ();
  }

  return m_instance;
}

ViewBase * 
QtViewFactory::
createView ( PlotterBase * plotter ) const
{
  QtView * view = new QtView ( plotter );

  return view;
}

FontBase *
QtViewFactory::
createFont () const
{
  return new QtFont ();
}

} // namespace hippodraw

