/** @file

BinTo3ColorScale class implementation

Copyright (C)  2000, 200-2005 The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinTo3ColorScale.cxx,v 1.4 2005/10/30 00:10:17 pfkeb Exp $

*/

#include "BinTo3ColorScale.h"

#include <cassert>

namespace hippodraw {

BinTo3ColorScale::
BinTo3ColorScale ( const char * name )
  : BinToColor ( name )
{
}

BinTo3ColorScale::BinTo3ColorScale ( const BinTo3ColorScale & bin_to_color )
  : BinToColor ( bin_to_color )
{
}

BinToColor * BinTo3ColorScale:: clone () const
{
  return new BinTo3ColorScale ( *this );
}

void BinTo3ColorScale::doubleToColor ( double value, Color & color ) const
{
   double r,g,b;   
   double v = value;
   
   if (v < (m_vmin + 0.25 * m_dv)) {
      r = 0;
      g = 2* (v - m_vmin) / m_dv;
      b = 1 -  2*(v -m_vmin)/m_dv ; 

   } else if (v < (m_vmin + 0.5 * m_dv)) {
      r = 0;
      g = 2*(v - m_vmin) / m_dv;
      b = 1 - 2 * (v - m_vmin) / m_dv;
   } else if (v < (m_vmin + 0.75 * m_dv)) {
      r = 2 * (v - m_vmin - 0.5 * m_dv) / m_dv;
      g = 1 - 2* (v - m_vmin - 0.5*m_dv) / m_dv;
      b = 0;
   } else {
      r = 2 * (v - m_vmin - 0.5 * m_dv) / m_dv;
      g = 1  - 2 * (v - m_vmin - 0.5 * m_dv ) / m_dv;
      b = 0;
   }
   int red   = static_cast < int > ( r * 255. );
   int green = static_cast < int > ( g * 255. );
   int blue  = static_cast < int > ( b * 255. );
   

   assert ( red < 256 && green < 256 && blue < 256 );
   color.setColor ( red, green, blue );
}

} // namespace hippodraw

