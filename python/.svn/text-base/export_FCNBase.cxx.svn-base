/** @file 

Part of Python extension module for hippodraw::FCNBase class.

Copyright (C) 2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_FCNBase.cxx,v 1.5 2006/03/31 22:33:19 pfkeb Exp $

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

#include "functions/FunctionBase.h"

// import derived class to get correct logic for Minuit and/or Minuit2
#include "minimizers/StatedFCN.h"

using std::vector;
using namespace boost::python;

namespace hippodraw {
namespace Python {

void
export_FCNBase()
{
#ifdef HAVE_MINUIT2
  class_ < ROOT::Minuit2::FCNBase, bases<>,
    ROOT::Minuit2::FCNBase, boost::noncopyable >
#else
  class_ < FCNBase, bases<>,
    FCNBase, boost::noncopyable >
#endif

    ( "FCNBase",
      "The base class for the objective function used in fitting.",
      no_init )

   ;
}

} // namespace Python
} // namespace hippodraw
