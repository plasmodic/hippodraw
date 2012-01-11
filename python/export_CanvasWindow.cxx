/** @file 

Part of Python extenstion module for CanvasWindow class.

Copyright (C) 2002-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_CanvasWindow.cxx,v 1.14 2005/11/24 00:30:23 pfkeb Exp $

*/

#ifdef _MSC_VER
# pragma warning(disable:4231)  // nonstandard extension used 'extern' before...
# pragma warning(disable:4251)  // needs to have dll-interface used by client
# pragma warning(disable:4275)  // non dll-interface struct
# pragma warning(disable:4800)  // 'int' : forcing value to bool 'true' or 'false' (performance warning)
#endif

// with Python 2.3, include before Qt headers to avoid conflict
// with symbol `slots'
#include <boost/python.hpp>

#include "qt/CanvasWindow.h"

using namespace boost::python;

namespace hippodraw {
namespace Python {

void
export_CanvasWindow()
{
  class_ < CanvasWindow >
    ( "CanvasWindow",
      "The CanvasWindow class contains the Canvas and some controls via\n"
      "menu items and dialogs.",
      no_init )

    .def ( "show", &CanvasWindow::show,
	   "show ( None ) -> None\n"
	   "\n"
	   "Brings the canvas window onto the screen." )

    .def ( "fileNew", &CanvasWindow::fileNew,
	   "fileNew ( None ) -> None\n"
	   "\n"
	   "Creates a new CanvasWindow." )
    ;
}

} // namespace Python
} // namespace hippodraw
