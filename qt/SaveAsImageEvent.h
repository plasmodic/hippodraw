/* -*- mode: c++ -*- */

/** @file

SaveAsImageEvent  class interface

Copyright (C) 2004,2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: SaveAsImageEvent.h,v 1.5 2006/05/05 23:42:22 pfkeb Exp $

*/

#ifndef _SaveAsImageEvent_H_
#define _SaveAsImageEvent_H_

#include "libqthippo.h"

#include <qevent.h>
#include <string>

namespace hippodraw {

class PlotterBase;

/** A derived class of QEvent to handle saving a PlotterBase object as
    image file events. A derived class of QEvent to handle
    saving a PlotterBase object as image file.

    @todo This class can go away.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_QTHIPPOPLOT_API SaveAsImageEvent : public QEvent
{

private:

  /** The filename for the image.
   */
  std::string m_filename;

  /** The relevant plotter.
   */
  const PlotterBase *m_plotter;

public:

  /** The constructor taking the @a plotter object and image @a
      filename as arguments.
  */
  SaveAsImageEvent ( const PlotterBase * plotter,
		     const std::string & filename );

  /** The copy constructor. */
  SaveAsImageEvent ( const SaveAsImageEvent & );

  /** The required virtual destructor. */
  virtual ~SaveAsImageEvent ();

  /** Returns the PlotterBase object that created the update event.
   */
  const PlotterBase * plotter () const;

  /** Returns the filename for the image.
   */
  const std::string & filename () const;

};

} // namespace hippodraw

#endif // _SaveAsImageEvent_H_
