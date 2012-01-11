/** @file

Python extenstion module for HippoDraw classes.

Copyright (C) 2003-2005  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: hippomodule.cxx,v 1.1 2005/03/16 15:34:19 pfkeb Exp $

*/

#ifdef _MSC_VER
# pragma warning(disable:4231)  // nonstandard extension used 'extern'
				// before...
# pragma warning(disable:4251)  // needs to have dll-interface used by client
# pragma warning(disable:4275)  // non dll-interface struct
# pragma warning(disable:4800)  // 'int' : forcing value to bool
				// 'true' or 'false' (performance
				// warning)
#endif

#include "exportToPython.h"

#include <boost/python.hpp>

using namespace hippodraw::Python;
using namespace boost::python;

/** Creates a Python extension module for HippoDraw.  Creates the
    Python extension module @em hippo. Requires version 2 of
    Boost.Python, which was initially distributed with boost_1_29_0
    obtained from <a href="http://www.boost.org"> the Boost Web
    site. </a>

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
 */
BOOST_PYTHON_MODULE(hippo)
{
  exportToPython ();
}
