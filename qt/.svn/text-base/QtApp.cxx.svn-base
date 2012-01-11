/** @file

hippodraw::QtApp class implementation.

Copyright (C) 2002, 2003, 2005, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtApp.cxx,v 1.98 2007/03/14 23:28:28 xiefang Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "QtApp.h"

#include "CanvasWindow.h"
#include <QtGui/QFileOpenEvent>
#include "QtFileDialog.h"
#include "WindowController.h"

#include "qdir.h"
#include <cassert>

using std::string;

using namespace hippodraw;

QtApp * QtApp::s_instance = 0;

QtApp::QtApp ( int &argc, char** argv)
  : QApplication ( argc, argv )
{
  init ();
}

QtApp::QtApp ( int &argc, char** argv, bool gui )
  : QApplication ( argc, argv, gui )
{
  init ();

}

void
QtApp::
init ()
{

  /* Create temp directory. Now it is used for only PNG files generated
     from LaTex equation. It will be deleted when the application exit. 
  */
  QDir current_dir = QDir();
  current_dir.mkdir("temp_latex");


  // Needed in order to use std::string as argument in signal/slot connection.
  qRegisterMetaType < std::string > ( "std::string" );

  s_instance = this;
}

QtApp::~QtApp ()
{
  /* It's not session safe here. Consider two copy of hippo are running. */
  QDir current_dir = QDir();
  system("rm -f temp_latex/*.*"); 
  current_dir.rmdir("temp_latex");


  WindowController * controller = WindowController::instance ();
  controller -> closeAllWindows ( true );
  delete controller;

  s_instance = 0;
}

QtApp * QtApp::instance ()
{
  return s_instance;
}

#ifdef Q_OS_MAC
/** @bug Verifty that this is working on Mac OS X
 */
void
QtApp::
customEvent ( QEvent * event )
{
  QFileOpenEvent * oe = dynamic_cast < QFileOpenEvent * > ( event );
  if ( oe != 0 )
    tryOpenFile ( oe->file() );
}
#endif

void QtApp::setFirstWindow ()
{
  bool hasWindow = false;
  
  WindowController * wc = WindowController::instance ();
  QStringList args = QCoreApplication::arguments();
   int count = args.count();
  // No argument.
  if ( count == 1 ) {
    wc -> setFirstWindow();
    return;
  }

  wc -> createInspector();

  QString qarg;
  string arg;
  // Process each argument
  for ( int i = 1; i < count; i++ ) {
//     const string arg ( args[i] );
    qarg =  args[i];

    hasWindow |= tryOpenFile ( qarg.toLatin1().data() );
  }
  // No window created ( no .hpo argument ), create the first window.
  if ( !hasWindow ) wc->setFirstWindow();
}

bool
QtApp::
tryOpenFile ( const QString & arg )
{
  int pos = arg.lastIndexOf ( '.' );
  if ( pos == -1 ) return false;

  QString suffix = arg.mid ( pos );

  if ( QtFileDialog::isDocSuffix ( suffix ) ) {
    CanvasWindow * window = new CanvasWindow ();
    try {
      window -> initFromFile ( arg );
    }
    catch ( ... ) {
    }
    return true;
  }

  if ( QtFileDialog::isTextSuffix ( suffix ) ) {
    QtFileDialog::openTextTuple ( arg );
    return false;
  }

  // Use a QtFileDialog object to call non-static methods.
  QtFileDialog * qd = new QtFileDialog ();

  if ( QtFileDialog::isFitsSuffix ( suffix ) ) {
    qd->openFitsTuple ( arg, 0 );
    delete qd;
    return false;
  }

  if ( QtFileDialog::isRootSuffix ( suffix ) ) {
    qd->openRootTuple ( arg, 0 );
    delete qd;
    return false;
  }

  delete qd;
  return false;
}

CanvasWindow *
QtApp::
currentCanvas ()
{
  return WindowController::instance() -> currentCanvas();
}
