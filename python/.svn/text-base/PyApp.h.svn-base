/* -*- mode: c++ -*-  */

/** @file

hippodraw::PyApp class interface.

Copyright (C) 2002, 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: PyApp.h,v 1.30 2007/06/18 17:51:12 pfkeb Exp $

*/

#ifndef PyApp_H
#define PyApp_H

#include <string>

namespace hippodraw {

class HdThread;
class Inspector;
class PyCanvas;
class QtApp;

/** A class to interface Python to application running in separate
    thread. 

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Matthew Langston <langston@slac.stanford.edu>
*/
class PyApp
{
private:

  /** The instance of the application. */
  static PyApp * s_instance;

  /** The QApplication object. */
  static QtApp * s_app;

  /** The thread in which to run the GUI. */
  HdThread * m_thread;

public:

  /** The default Constructor. */
  PyApp ( );

  /** A constructor that created QtApp without threading */
  PyApp ( int );

  /** A constructor taking script name as argument. */
  PyApp ( char * script );

  /** The destructor. Kills the application object running in the
      thread. */
  ~PyApp ( );

  /** Returns the application instance. */
  static PyApp * instance ();

  /** Starts the QApplication event loop. */
  int exec();

  /** Terminates the application.
   */
  void quit ();

  /** Returns @c true if the application as terminated, otherwise
      returns @c false.
  */
  bool hasTerminated () const;

  /** Returns a pointer to the current CanvasWindow.  */
  PyCanvas * currentCanvas ();

  /** Opens a new document from file @a filename. */
  void openDocument ( const std::string & filename );

  /** Obtains a lock on the application's mutex.  For Qt 3, obtains a
      lock on the application's mutex, for Qt 4 does nothing.
   */
  static void lock();

  /** returns @c true if the Application mutex is locked by different thread.
   */
  static bool locked ();

  /** Releases the lock on the application's mutex.  For Qt 3,
      releases the lock on the application's mutex, for Qt 4 does
      nothing.
   */
  static void unlock();

  /** Returns @c true if the application object has pending events.
   */
  static bool hasPendingEvents ();

  /** If @a yes is @c true, allows the application to quit when the
      last CanvasWindow is closed by the user.
  */
  void quitOnLastWindowClose ( bool yes );

};

} // namespace hippodraw

#endif // PyApp_H
