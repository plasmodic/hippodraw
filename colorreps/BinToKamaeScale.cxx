/** @file

BinToKamaeScale class implementation

Copyright (C)  2004, 2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinToKamaeScale.cxx,v 1.6 2005/10/30 00:10:17 pfkeb Exp $

*/

#include "BinToKamaeScale.h"

#include <cmath>

#include <cassert>

namespace hippodraw {

BinToKamaeScale::
BinToKamaeScale ( const char * name )
  : BinToColor ( name )
{
}

BinToKamaeScale::BinToKamaeScale ( const BinToKamaeScale & bin_to_color )
  : BinToColor ( bin_to_color )
{
}

BinToColor * BinToKamaeScale:: clone () const
{
  return new BinToKamaeScale ( *this );
}

void
BinToKamaeScale::
doubleToColor ( double value, Color & color ) const
{
   double z = ( value - m_vmin ) / m_dv;
   z = std::max ( 0.0, z );
   z = std::min ( 1.0, z );

   double r = std::min ( 255., 
			 113.9 * sin ( 7.64 * pow ( z, 1.705 ) + 0.701 )
			 - 916.1 * pow ( ( z + 1.755), 1.862 )
			 + 3587.9 * z + 2563.4 );
   double g = std::min ( 255.,
			 70.0 * sin ( 8.7 * pow ( z, 1.26 ) - 2.418 )
			 + 151.7 * pow ( z, 0.5 )
			 + 70.0 );
   double b = std::min ( 255.,
			 194.5 * pow ( z, 2.88 )
			 + 99.72 * exp ( -77.24 *pow ( z - 0.742, 2 ) )
			 + 45.4 * pow ( z, 0.089 )
			 + 10.0 );

   int red   = static_cast < int > ( r );
   int green = static_cast < int > ( g );
   int blue  = static_cast < int > ( b );

   assert ( red < 256 && green < 256 && blue < 256 );
   color.setColor ( red, green, blue );
}

} // namespace hippodraw

