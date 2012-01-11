/* -*- mode: c++ -*-  */

/** @file

hippodraw::CanvasViewProxy class interface.
 
Copyright (C) 2006  The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: CanvasViewProxy.h,v 1.8 2006/08/07 19:07:02 pfkeb Exp $

*/

#ifndef CanvasViewProxy_H
#define CanvasViewProxy_H

#include "libqthippo.h"

#include "qobject.h"

#include <string>

namespace hippodraw {

  class CanvasView;
  class PlotterBase;

/** A Proxy for the CanvasView class.  A Proxy for the CanvasView is
    responsible for invoking member functions of the CanvasView class
    in a thread safe manner.  For example, the CanvasView object is
    executed in the GUI thread, while a Python script or command is in
    a different thread, perhaps the main thread.  Thus Python should
    use the CanvasViewProxy to invoke member functions of the
    CanvasView object.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_QTHIPPOPLOT_API CanvasViewProxy : private QObject
{
Q_OBJECT

private:

  /** The CanvasView object managed by this proxy.
   */
  CanvasView * m_canvas_view;

  /** Private copy constructor to prevent copying.
   */
  CanvasViewProxy ( const CanvasViewProxy & );

public:

/** The constructor taking the CanvasView object to be used.
 */
 CanvasViewProxy ( CanvasView * view );

  /** The destructor.
   */
  virtual ~CanvasViewProxy();

  /** Adds a plotter display to the canvas.
   */
  void addDisplay ( PlotterBase * plotter );

  /** Clears the canvas of all displays.
   */
  void clear ();

  /** Saves the @a plotter as image file with name @a filename.
   */
  void saveAsImage ( const PlotterBase *, const std::string & filename );

  /** Swaps the orientation from portrait to landscape and vice verse.
   */
  void swapOrientation ();

  signals:

  /** Signal used to add a plotter display to the canvas.
   */
  void addDisplaySignal ( PlotterBase * plotter );

  /** Signal used to clear all displays from canvas.
   */
  void clearSignal ();

  /** Save the @a plotter as an image file.
   */
  void saveAsImageSignal ( const PlotterBase * plotter,
			   const std::string & filename );

  /** Swaps the orientation.
   */
  void swapOrientationSignal ();

};

} // namespace hippodraw

#endif // CanvasViewProxy_H
