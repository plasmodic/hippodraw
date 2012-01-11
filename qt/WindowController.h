/* -*- mode: c++ -*- */

/** @file

hippodraw::WindowController class interface

Copyright (C) 2001-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: WindowController.h,v 1.30 2006/10/24 19:48:56 pfkeb Exp $

*/

#ifndef WindowController_H
#define WindowController_H

#include "qt/libqthippo.h"

#ifdef _MSC_VER
#include <msdevstudio/MSconfig.h>
#endif

#include <qglobal.h> // for version

#include <list>
#include <string>

class QAssistantClient;
class QRect;

namespace hippodraw {

class CanvasWindow;
class Inspector;

/** A singleton class for keeping track the window components of
    HippoDraw.  It can be used as a helper function to an
    application. 

    @request @@@@@@ Add a flag, set-able from Python, that would
    disallow the application to be exited via closing the last
    CanvasWindow.

    @request @@@@ no need to terminate the application on last window
    close on the mac OS.

    @todo There are circular references between WindowController and
    CanvasWindow. Probably, WindowController should be an observer of
    CanvasWindow or the methods of the WindowController should be
    static methods of the CanvasWindow.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_QTHIPPOPLOT_API WindowController
{

private:

  /** The instance of the application. */
  static WindowController * s_instance;

  /** The list of CanvasWindow objects created by the application. */
  std::list < CanvasWindow * > m_window_list;

  /** The CanvasWindow object. */
  CanvasWindow * m_active_window;

  /** The Inspector. */
  Inspector * m_inspector;

  /** The QtAssistant client.
   */
  QAssistantClient * m_assistant;

  /** The version number of HippoDraw.  

      @request @@ Windows only, Need to get the version number into
      the Windows installer file.
  */
  static std::string s_version;

  /** A flag that when set @c true will terminate the application when
      the last canvas window closes.
  */
  bool m_quit;

  /** Returns a rectangle of the screen. */
  QRect getScreenRect ();

  /** Positions and sets the size of the window taking into account
      the size of the screen. */
  void positionCanvas ( CanvasWindow * window );

  /** Resizes the CanvasWindow so that it doesn't overlap with the
      Inspector window.
  */
  void resizeCanvas ( CanvasWindow * window );

  /** Positions the Inspector as far right as possible based on size
      of screen and width of Inspector.
  */
  void positionInspector ();

  /** Move the Inspector next to the edge of the Window.
   */
  void moveInspector ( CanvasWindow * window );

public:

  /** Private Constructor.  @todo Had to make this public for
      boost.python.  Maybe there is a better way. */
  WindowController ( );

  /** The destructor.  */
  ~WindowController();

  /** Returns the application instance. */
  static WindowController * instance ();

  /** Returns the version number of HippoDraw. */
  static const std::string & version ();

  /** Creates the Inspector object and positions it on the desktop. */
  void createInspector ( );

  /** Registers the new CanvasWindow and finishes its initialization. */
  void newWindow ( CanvasWindow * );

  /** Removes the @a window from the window list.  @todo If only one
      window remains, informs that window to disable the close
      button. */
  void aboutToClose ( CanvasWindow * window );

  /** Sets the flag that determines if the application will be
      terminated when the last window is closed.
  */
  void quitOnLastWindowClose ( bool yes = true );

  /** Responds to hide event from a document window.  If all document
      windows are hidden, then hides the Inspector window as well.
      Otherwise, does nothing.
  */
  void hasBeenHidden ();

  /** Responds to document window being shown.  Shows the Inspector
      window if it had been hidden. */
  void unHide ( CanvasWindow * window );

  /** Queries application if it is OK to quit.  The application
      queries each document window for unsaved changes.  If any window
      refuses to close, the application returns @c false Otherwise,
      returns @c true.  @sa CanvasWindow::allowClose(). */
  bool okToQuit ();

  /** Returns a pointer to the current CanvasWindow.  */
  CanvasWindow * currentCanvas ();

  /** Sets the current canvas.  This method does not change the
      canvas' window active state which is control by the windowing
      system.  The intent of this method is to allow CanvasWindow
      object to inform the application that it has become active. */
  void setCurrentCanvas ( CanvasWindow * canvas );

  /** returns the Inspector. */
  Inspector * getInspector();

  /** Update the actions in all CanvasWindow objects.  Update the
      enabled status of all actions, such as menu items and tool bar
      icons, of all windows. Sends CanvasWindow::updateActions to all
      windows. 
  */
  void updateActions ();

  /** Closes all Windows.  Closes all Document windows as well as the
      Inspector window.  Effectively this will terminate the
      application. */
  void closeAllWindows ( bool alsoDelete );

  /** Sets up the first CanvasWindow object.
   */
  void setFirstWindow ();

  /** Opens the QAssistant object.
   */
  void openAssistant ();

};

} // namespace hippodraw

#endif // WindowController_H
