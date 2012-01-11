/* -*- mode:c++ -*- */

/** @file 

SIP interface definition for WindowController

Copyright (C)  2002, 2004  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

This file is part of sihippo

$Id: WindowController.sip,v 1.21 2007/02/24 22:24:38 pfkeb Exp $

@author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

*/


class WindowController
{
%TypeHeaderCode
#include "qt/WindowController.h"
using hippodraw::WindowController;
using hippodraw::CanvasWindow;
using hippodraw::Inspector;
%End


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
  void aboutToClose ( CanvasWindow * );

  /** Sets the flag that determines if the applicaiton will be
      terminated when the last window is closed.
  */
  void quitOnLastWindowClose ( bool = true );

  /** Responds to hide event from a document window.  If all document
      windows are hidden, then hides the Inspector window as well.
      Otherwise, does nothing. */
  void hasBeenHidden ( );

  /** Reponds to document window being shown.  Shows the Inspector
      window if it had been hidden. */
  void unHide ( CanvasWindow * );

  /** Queries application if it is Ok to quit.  The application
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
  void setCurrentCanvas ( CanvasWindow * );

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
      application.  If argument has value @c true, then also destroy
      the windows.
  */
  void closeAllWindows ( bool );

  /** Sets up the first CanvasWindow object.
   */
  void setFirstWindow ();

};

char * __facility__;
char * __abstract__;
char * __author__;
char * __date__;
char * __version__;
char * __credits__;

%ModuleCode
static char * __facility__ = "HippoDraw";
static char * __abstract__ = "Data display package";
static char * __author__   = "Paul F. Kunz and others";
static char * __date__     = "$Date: 2007/02/24 22:24:38 $";
static char * __version__  =  
    const_cast < char * > (WindowController::version().c_str());
static char * __credits__  = "Stanford Linear Accelerator Center";
%End
