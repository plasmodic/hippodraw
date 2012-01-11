/** @file 

Part of Python extenstion module for LineStyle

Copyright (C) 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_LineStyle.cxx,v 1.1 2005/07/03 23:32:10 pfkeb Exp $

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

#include "graphics/LineStyle.h"

#include <boost/python.hpp>

using namespace boost::python;

namespace hippodraw {
namespace Python {

void
export_LineStyle ()
{
  enum_ < Line::Style > ( "Line" )
    .value ( "solid",      Line::Solid )
    .value ( "dash",       Line::Dash )
    .value ( "dot",        Line::Dot )
    .value ( "dashdot",    Line::DashDot )
    .value ( "dashdotdot", Line::DashDotDot )
    .value ( "invisible",  Line::Invisible )
   ;
}

} // namespace Python
} // namespace hippodraw
