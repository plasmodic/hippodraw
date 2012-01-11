/* -*- mode: c++ -*- */

/** @file

SymbolType enumeration conversion functions

Copyright (C) 2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: SymbolType.cxx,v 1.3 2005/08/01 20:28:18 pfkeb Exp $

*/

#include "SymbolType.h"

namespace hippodraw {

  namespace Symbol {

/** An array to allow conversion of integer to enumeration.

    @note Use static as recommended by KDE developers because no long
    symbol will be exported.
 */
    static Type types [] = { SQUARE, SOLIDSQUARE,
			     PLUS, TIMES,
			     TRIANGLE, FILLED_TRIANGLE,
			     CIRCLE, FILLED_CIRCLE,
			     INVISIBLE };

    Type convert ( unsigned int type )
    {
      type = type > INVISIBLE ? INVISIBLE : type;

      return types [ type ];
    }

  }
}
