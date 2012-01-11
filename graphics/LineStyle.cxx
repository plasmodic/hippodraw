/* -*- mode:c++ -*- */

/** @file

LineStyle conversion functon.


Copyright (C) 2000, 2004 The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: LineStyle.cxx,v 1.6 2005/07/12 20:45:56 pfkeb Exp $ 

*/

#include "LineStyle.h"

namespace hippodraw {

  namespace Line {

/** An array to allow conversion of integer to enumeration.

    @note Use static as recommended by KDE developers because no long
    symbol will be exported.
 */
    static Style styles[] = { Solid, Dash, Dot, DashDot, DashDotDot,
			      Invisible };

    Style convert ( unsigned int style )
    {
      if ( style > Invisible ) style = Invisible;

      return styles [ style  ];
    }

  } // end namespace Line
} // end namespace 
