/** @file 

Part of Python extension module for hippodraw::Color class

Copyright (C) 2005, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_Color.cxx,v 1.2 2006/03/31 22:33:19 pfkeb Exp $

*/

#ifdef _MSC_VER
// nonstandard extension used 'extern' before...
# pragma warning(disable:4231)  

// needs to have dll-interface used by client
# pragma warning(disable:4251)  

// non dll-interface struct
# pragma warning(disable:4275)  

// 'int' : forcing value to bool 'true' or 'false' (performance warning)
# pragma warning(disable:4800)  
#endif

// include first to avoid _POSIX_C_SOURCE warning.
#include <boost/python.hpp>

#include "graphics/Color.h"

using namespace boost::python;

namespace hippodraw {
namespace Python {

void
export_Color ()
{
  enum_ < Color::Value > ( "ColorValue" )
    .value ( "red",       Color::red )
    .value ( "green",     Color::green )
    .value ( "blue",      Color::blue )
    .value ( "yellow",    Color::yellow )
    .value ( "orange",    Color::orange )
    .value ( "cyan",      Color::cyan )
    .value ( "magenta",   Color::magenta )
    .value ( "black",     Color::black )
    .value ( "darkgray",  Color::darkgray )
    .value ( "lightgray", Color::lightgray )
    .value ( "white",     Color::white )
   ;
}

} // namespace Python
} // namespace hippodraw
