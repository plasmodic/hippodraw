/* -*- mode: c++ -*- */

/** @file 

hippodraw::QtApp class interface

Copyright (C)  2001-2006  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: QtApp.h,v 1.57 2006/09/12 13:43:30 pfkeb Exp $

*/

#ifndef QtApp_H
#define QtApp_H

#include "qt/libqthippo.h"

#include <QApplication>
#include <QString>

namespace hippodraw {

class CanvasWindow;

/** A derived class of QApplication that instantiates the components
    of HippoDraw.  The class can be instantiates either from a main
    program, or from a script such as Python. 

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_QTHIPPOPLOT_API QtApp : public QApplication
{
private:

  /** The instance of the application. */
  static QtApp * s_instance;

  /** Method called by constructors to initialize the application.
   */
  void init ();

  /** Takes the string as a file name and opens the file if it is
      recognized.  If file was document, returns @c true, otherwise
      returns @c false. */
  bool tryOpenFile ( const QString & name );

 protected:

#ifdef Q_OS_MACX
  /** Overriding implementation to catch certain QEvent events.
      @note This member function is only used on Mac OS X
   */
  virtual void customEvent ( QEvent * event );
#endif

public:

  /** A Constructor that always enables the GUI.
   */
  QtApp ( int &argc,  char ** argv );

  /** A Constructor that enables the GUI optionally.
   */
  QtApp ( int &argc,  char ** argv, bool gui );

  /** The destructor.  */
  ~QtApp();

  /** Returns the application instance. */
  static QtApp * instance ();

  /** Sets up the first application main window. When the application
      is started, at least one main window must appear in order to
      control the application.  If no arguments were given to the
      command line start-up, then a CanvasWindow with an empty
      CanvasView is created.  If arguments were given, then they are
      assumed to be a filename.  If the filename suffix matches the
      document suffix as returned by CanvasWindow::docSuffix, then an
      existing document is opened.  If the filename suffix matches the
      text NTuple suffix as returned by CanvasWindow::dataSuffix, then
      an empty canvas window is opened and the %NTuple file imported.
  */
  void setFirstWindow ( );

  /** Returns a pointer to the current CanvasWindow.  */
  CanvasWindow * currentCanvas ();

};

} // namespace hippodraw

#endif // QtApp_H
