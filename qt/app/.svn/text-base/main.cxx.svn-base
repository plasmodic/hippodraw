/** @file

Main program for stand-a-lone application.

Copyright (C) 2001-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: main.cxx,v 1.38 2007/07/02 18:17:16 pfkeb Exp $

*/

#ifdef HAVE_CONFIG_H
// for VERSION
#include "config.h"
#else
#include "msdevstudio/MSconfig.h"
#endif

// for dll interface warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "qt/QtApp.h"

#include <iostream>

using std::cout;
using std::endl;

/** Main program for stand-a-lone application.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
using hippodraw::QtApp;

int main ( int argc, char** argv)
{
   if ( argc > 1 ) {
    std::string arg1 ( argv[1] );
    if ( arg1 == "--version" ) {
      std::cout << "HippoDraw version " << VERSION
	   << std::endl;
      return 0;
    }
  }

  QtApp app ( argc, argv );
  app.setFirstWindow();
   return app.exec();
}
