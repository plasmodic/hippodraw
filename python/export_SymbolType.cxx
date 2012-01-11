/** @file 

Part of Python extenstion module for SymbolType

Copyright (C) 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_SymbolType.cxx,v 1.1 2005/07/03 17:58:04 pfkeb Exp $

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

#include "graphics/SymbolType.h"

#include <boost/python.hpp>

using namespace boost::python;

namespace hippodraw {
namespace Python {

void
export_SymbolType ()
{
  enum_ < Symbol::Type > ( "Symbol" )
    .value ( "opensquare",     Symbol::SQUARE )
    .value ( "filledsquare",   Symbol::SOLIDSQUARE )
    .value ( "plus",           Symbol::PLUS )
    .value ( "times",          Symbol::TIMES )
    .value ( "opentriangle",   Symbol::TRIANGLE )
    .value ( "filledtriangle", Symbol::FILLED_TRIANGLE )
    .value ( "opencircle",     Symbol::CIRCLE )
    .value ( "filledcircle",   Symbol::FILLED_CIRCLE )
    .value ( "invisible",      Symbol::INVISIBLE )
   ;
}

} // namespace Python
} // namespace hippodraw
