/* -*- mode:c++ -*- */

/** @file

AxesType conversion functions

Copyright (C) 2005 The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

The library is distributed under the terms of the GNU Lesser General
Public License.

$Id: AxesType.cxx,v 1.5 2006/07/10 21:53:08 pfkeb Exp $ 

*/

#include "AxesType.h"

#include <cassert>

namespace hippodraw {

  namespace Axes {

/** An array to allow conversion of integer to enumeration.

    @note Use static as recommended by KDE developers because no long
    symbol will be exported.
 */
    static Type types [] = { X, Y, Z, W, XE, YE, UNKNOWN };

    Type convert ( const std::string & axis )
    {
      hippodraw::Axes::Type type = UNKNOWN;
      if ( axis == "x" || axis == "X" ) type = X;
      else if ( axis == "y" || axis == "Y" ) type = Y;
      else if ( axis == "z" || axis == "Z" ) type = Z;
      else if ( axis == "t" || axis == "T" ) type = T;

      return type;
    }

    Type convert ( unsigned int i )
    {
      assert ( i <= UNKNOWN );

      return types [ i ];
    }

  } // end namespace Axes
} // end namespace hippodraw
