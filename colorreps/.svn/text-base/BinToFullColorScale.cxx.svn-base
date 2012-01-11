/** @file

BinToFullColorScale class implementation

Copyright (C)  2000, 2003-2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinToFullColorScale.cxx,v 1.4 2005/10/30 00:10:17 pfkeb Exp $

*/

#include "BinToFullColorScale.h"

#include <cassert>

namespace hippodraw {

BinToFullColorScale::
BinToFullColorScale ( const char * name )
  : BinToColor ( name )
{
}

BinToFullColorScale::
BinToFullColorScale ( const BinToFullColorScale & bin_to_color )
  : BinToColor ( bin_to_color )
{
}

BinToColor * BinToFullColorScale:: clone () const
{
  return new BinToFullColorScale ( *this );
}

void
BinToFullColorScale::
doubleToColor ( double value, Color & color ) const
{
   double r=1;
   double g=1;
   double b=1;
   double v = value;
   
   if (v < (m_vmin + ( 0.1667 ) * m_dv)) {
     r = 0;
     g = 6 * (v - m_vmin) / m_dv;

   } else if (v < (m_vmin + (0.3333) * m_dv)) {
     r = 0;
     b = 1 + 6 * (m_vmin + (0.1667) * m_dv - v) / m_dv;

   } else if (v < (m_vmin + (0.5) * m_dv)) {
     r = 6 * (v - m_vmin - (0.3333) * m_dv) / m_dv;
     b = 0;

   } else if (v < (m_vmin + (0.6667) * m_dv) ) {
     g = 1 + 6 * (m_vmin + (0.5) * m_dv - v) / m_dv;
     b = 0;

   } else if (v < (m_vmin + (0.8333) * m_dv) ) {
     g = 0;
     b = 6 * (v- m_vmin - (0.6667)*m_dv) /m_dv;

   } else if (v < (m_vmin + m_dv )){
     g=0; 
     r = 1 + 6 * ( m_vmin + (0.8333)*m_dv - v) / m_dv;
   }  else{ //this else is for guarding against numerical errors
     g = 0;
     r = 0;
   }
   
  
   int red   = static_cast < int > ( r * 255. );
   int green = static_cast < int > ( g * 255. );
   int blue  = static_cast < int > ( b * 255. );

   assert ( red < 256 && green < 256 && blue < 256 );
   color.setColor ( red, green, blue );
}

} // namespace hippodraw

