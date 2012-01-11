/** @file

Python extension module for hippodraw::RepBase class.

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_RepBase.cxx,v 1.7 2006/03/31 22:33:20 pfkeb Exp $

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
#endif // _MSC_VER

// include first to avoid _POSIX_C_SOURCE warning.
#include <boost/python.hpp>

#include "reps/RepBase.h"

#include <vector>

using std::vector;
using namespace boost::python;

namespace hippodraw {
namespace Python {

void
export_RepBase()
{
  class_ < RepBase, 
    bases<>,
    RepBase, 
    boost::noncopyable >
    ( "RepBase",
      "The base class for  point and text representations.\n"
      "See PointRepFactory for creating objects of this type.\n",
      no_init )

    ;
}

} // namespace Python
} // namespace hippodraw
