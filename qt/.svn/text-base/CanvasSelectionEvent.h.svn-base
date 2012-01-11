/* -*- mode: c++ -*- */

/** @file

CanvasSelectionEvent  class interface

Copyright (C) 2004, 2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: CanvasSelectionEvent.h,v 1.6 2006/05/05 23:42:22 pfkeb Exp $

*/

#ifndef _CanvasSelectionEvent_H_
#define _CanvasSelectionEvent_H_

#include "libqthippo.h"

#include <qevent.h>
#include <vector>

namespace hippodraw {

class PlotterBase;

/** A derived class of QEvent with which the CanvasView
    notifiies clients of changes in selections of objects on the
    canvas.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_QTHIPPOPLOT_API CanvasSelectionEvent : public QEvent
{

  std::vector < PlotterBase * > m_plotters;

  /** The copy constructor. */
  CanvasSelectionEvent ( const CanvasSelectionEvent & );

public:

  CanvasSelectionEvent ( std::vector < PlotterBase * > );

  /** The required virtual destructor. */
  virtual ~CanvasSelectionEvent ();

  /** Returns the PlotterBase objects that created the update event. 
   */
  const std::vector < PlotterBase * > & getPlotters () const;

};

} // namespace hippodraw

#endif // _CanvasSelectionEvent_H_
