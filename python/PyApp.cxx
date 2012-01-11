/** @file

hippodraw::PyApp class implementation and Python interface implementation.

Original taken from QTDIR/examples/canvas/main.cpp version 3.0 

Copyright (C) 2002-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: PyApp.cxx,v 1.82 2007/08/29 21:07:08 pfkeb Exp $

*/

#ifdef HAVE_CONFIG_H
// for have qt app
#include "config.h"
#endif

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#include <windows.h>
#endif

// with Python 2.3, include before Qt headers to avoid conflict
// with symbol `slots'
// include first to avoid _POSIX_C_SOURCE warning.
#include <boost/python.hpp>

#include "PyApp.h" 

#include "PyCanvas.h" 

#ifdef HAVE_QT_APP
#include "qt/CanvasWindow.h"
#include "qt/HdThread.h" 
#include "qt/QtApp.h"
#include "qt/WindowController.h"
#include "qapplication.h"
#include "qwaitcondition.h"
#endif

#ifndef _MSC_VER
#include <pthread.h>
#endif
using namespace boost::python;

namespace hippodraw {
namespace Python {

void 
export_HDApp()
{
	class_ < PyApp > ( "HDApp",
			   "The HippoDraw application class.\n" )

	  .def ( init < int >
		 ( "HDApp () -> HDApp\n"
		   "HDApp ( value ) -> HDApp\n"
		   "\n"
		   "Using the constructor with no arguments creates\n"
		   "a HippoDraw application that runs in a separate\n"
		   "thread.\n" 
		   "Using the constructor with one argument creates\n"
		   "a HippoDraw application in same thread. Use\n"
		   "app.exec_loop() to start it." ) )

	  .def ( init < char * >
		 ("HDapp ( string ) -> HDapp" ) )

	  .def ( "instance", &PyApp::instance,
		 return_value_policy < reference_existing_object > (),
		 "instance () -> HDApp\n"
		 "\n"
		 "Returns a singleton instance of the HippoDraw \n"
		 "application object" )

	  .staticmethod ( "instance" )

	  .def ( "canvas", &PyApp::currentCanvas, 
		 return_value_policy < reference_existing_object > (),
		 "canvas () -> Canvas\n"
		 "\n"
		 "Returns the current canvas window." )

	  .def ( "exec_loop", &PyApp::exec,
		 "exec_loop () -> value\n"
		 "\n"
		 "Starts the application object.  Returns the value returned\n"
		 "by the application object upon exiting." )

	  .def ( "quit", &PyApp::quit,
		 "quit () -> None\n"
		 "\n"
		 "Closes all Canvas windows and Terminates the application." )

	  .def ( "quitOnLastWindowClose", &PyApp::quitOnLastWindowClose,
		 "quitOnLastWindowClose ( Boolean ) -> None\n"
		 "\n"
		 "If argument is True, allows application to quit when last\n"
		 "Canvas is closed." )

	  .def ( "hasTerminated", &PyApp::hasTerminated,
		 "hasTerminated () -> Boolean\n"
		 "\n"
		 "Returns True if the application has terminated." )

	  .def ( "openDocument", &PyApp::openDocument,
		 "openDocument ( string ) -> None\n"
		 "\n"
		 "Opens new canvas window from saved document file." )

	  .def ( "lock", &PyApp::lock,
		 "lock () -> None\n"
		 "\n"
		 "Lock the Qt library Mutex.   Will not return until no\n"
		 "other thread has the lock." )

	  .def ( "unlock", &PyApp::unlock,
		 "unlock () -> None\n"
		 "\n"
		 "Unlocks the application thread." )

	  ;
}

} // namesapce Python
} // namespace hippodraw

using namespace hippodraw;

PyApp * PyApp::s_instance = 0;
QtApp * PyApp::s_app = 0;

void  *
run ( void * arg )
{
#ifdef HAVE_QT_APP
  static int argc = 1;
  static char * argv[1];
  argv[0] = const_cast< char * > ( "Python" );

  QtApp app ( argc, argv );
  app.setFirstWindow ();
  app.exec ();
#endif // have qt app
  return 0;
}


/** @request There's a better way to wait for application to be ready.
    Saw it in Qt related documentation.
 */
PyApp::PyApp ( )
{
#if QT_VERSION < 0x040200

#if HAVE_QT_APP
  m_thread = new HdThread ( );
  m_thread->start ();

  // Wait for application to initialize itself.  It is not sufficient
  // to wait just for the thread to run as it may cause segmentation
  // fault when we try to use the application.
  while ( QtApp::startingUp () == true  ) {
    m_thread->wait ( 100 ); // wait for thread to run
  }
  s_app = QtApp::instance ();

#ifdef _MSC_VER
  // The following works under Windows, but under Linux leads to
  // occasional X Windows async errors
  while ( s_app->currentCanvas () == 0 ) {
    m_thread->wait ( 100 ); // wait for thread to create first window
  }
#else
  // The following seems to work better for Linux but not at all for
  // Windows ...
  while ( s_app->hasPendingEvents () == true ) {
    m_thread->wait ( 100 );
  }
#endif

  while ( s_app->currentCanvas () == 0 ) {
    m_thread->wait ( 100 ); // wait for thread to create first window
  }

#else // no qt app
  s_app = 0;
#endif // have qt app
#else // qt 4.2 or later
  pthread_t * thread = ( pthread_t * ) malloc ( sizeof ( pthread_t ) );
  if ( !thread ) {
    perror ( "Malloc of thread failed" );
  }
  if ( pthread_create ( thread, NULL, run, ( void * ) 0 ) ) {
    perror ( "Failed to create thread");
  }
#ifdef HAVE_QT_APP
  while ( QtApp::startingUp() == true ) {
    sleep ( 1 );
  }
  s_app = QtApp::instance ();
#endif // have qt app
#endif // qt 4.2

  s_instance = this;
}


PyApp::PyApp ( char * script )
{
#ifndef _MSC_VER
  pthread_t * thread = ( pthread_t * ) malloc ( sizeof ( pthread_t ) );
  if ( !thread ) {
    perror ( "Malloc of thread failed" );
  }
  if ( pthread_create ( thread, NULL, run, ( void * ) 0 ) ) {
    perror ( "Failed to create thread");
  }
#ifdef HAVE_QT_APP
  while ( QtApp::startingUp() == true ) {
    sleep ( 1 );
  }
  s_app = QtApp::instance ();
#endif
#endif
}

 
PyApp::PyApp ( int ) // parameter only used to make unique function
{
#ifdef HAVE_QT_APP
  static int argc = 1;
  static char * argv[1];
  argv[0] = const_cast < char * > ( "/HippoDraw" );

  s_app = new QtApp ( argc, argv, true );
#else
  s_app = 0;
#endif
}

PyApp::~PyApp ( )
{
  // Nothing to delete
  s_instance = 0;
}

PyApp * PyApp::instance ()
{
  if ( s_instance == 0 ) {
    s_instance = new PyApp ();
  }

  return s_instance;
}

int PyApp::exec ()
{
#ifdef HAVE_QT_APP
  return s_app->exec();
#else
  return 0;
#endif
}

bool
PyApp::
hasTerminated () const
{
#ifdef HAVE_QT_APP
  return QApplication::closingDown ();
#else
  return true;
#endif
}

void
PyApp::
quit ()
{
#ifdef HAVE_QT_APP
  s_app -> closeAllWindows ();
#endif
}

PyCanvas * PyApp::currentCanvas ()
{
#ifdef HAVE_QT_APP
  PyCanvas * pycanvas = 0;

  CanvasWindow * canvas = s_app->currentCanvas();
  while ( canvas == 0 ) {
#ifdef _MSC_VER
      Sleep ( 1000000 ); // milliseconds
#else 
      sleep (1);
#endif
      canvas = s_app-> currentCanvas();
  }

  pycanvas = new PyCanvas ( canvas );
  return pycanvas;
#else
  return 0;
#endif
}

void PyApp::openDocument ( const std::string & filename )
{
#ifdef HAVE_QT_APP
  PyApp::lock(); // definitely need it else documents.py crashes

  CanvasWindow * window = new CanvasWindow ();
  window -> initFromFile ( filename.c_str() );

  PyApp::unlock ();
#endif
}

void PyApp::lock ()
{
}

bool PyApp::locked ()
{
  return false;
}

void PyApp::unlock ()
{
}

bool
PyApp::
hasPendingEvents ()
{
  bool yes = false;
#ifdef HAVE_QT_APP
  if ( s_app != 0 && QApplication::closingDown () == false ) {
    yes = s_app -> hasPendingEvents ();
  }
#endif
  return yes;
}

void
PyApp::
quitOnLastWindowClose ( bool yes )
{
#ifdef HAVE_QT_APP
  WindowController * controller = WindowController::instance ();

  controller -> quitOnLastWindowClose ( yes );
#endif
}
