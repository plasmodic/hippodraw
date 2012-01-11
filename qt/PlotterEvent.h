/* -*- mode: c++ -*- */

/** @file

hippodraw::PlotterEvent  class interface

Copyright (C) 2003, 2005, 2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: PlotterEvent.h,v 1.6 2006/06/13 20:50:28 pfkeb Exp $

*/

#ifndef _PlotterEvent_H_
#define _PlotterEvent_H_

#include "libqthippo.h"

#include <qevent.h>

namespace hippodraw {

class PlotterBase;

/** A derived class of QEvent to handle updates from
    PlotterBase.  A derived class of QEvent to handle updates
    from PlotterBase.  It adds a member function to preserve the type,
    PlotterBase, for the receiving object.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_QTHIPPOPLOT_API PlotterEvent : public QEvent
{

public:

  PlotterEvent ( PlotterBase * plotter );

  /** The copy constructor. */
  PlotterEvent ( const PlotterEvent & );

  /** The required virtual destructor. */
  virtual ~PlotterEvent ();

  /** Returns the PlotterBase object that created the update event. 
   */
  PlotterBase * plotter () const;

private:
  PlotterBase *m_plotter;

};

} // namespace hippodraw

#endif // _PlotterEvent_H_
