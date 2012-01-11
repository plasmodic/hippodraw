/** @file

hippodraw::HdThread class implementation

Original taken from QTDIR/examples/canvas/main.cpp version 3.0 

Copyright (C) 2001-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: HdThread.cxx,v 1.26 2007/05/17 17:57:10 pfkeb Exp $

*/

// inconsistent dll linkage
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "HdThread.h"

#include "QtApp.h"

#include <iostream>
using std::cout;
using std::endl;

using namespace hippodraw;

HdThread::HdThread ( )
  : QThread ()
{
}

HdThread::~HdThread ()
{
}

void
HdThread::
run ()
{
  static int argc = 1;
  static char * argv[1];
  argv[0] = const_cast < char * > ( "/HippoDraw");// fake abs path for Mac OS X

  QtApp app ( argc, argv );
  app.setFirstWindow();

  try {
    app.exec ();
  }
  catch ( std::exception & e ) {
    std::cout << e.what()
	      << std::endl;
  }
}
