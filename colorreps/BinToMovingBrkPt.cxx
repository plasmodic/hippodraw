/** @file

BinToMovingBrkPt class implementation

Copyright (C)  2004, 2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinToMovingBrkPt.cxx,v 1.11 2005/10/30 00:10:17 pfkeb Exp $

*/

#include "BinToMovingBrkPt.h"

#include <cassert>

namespace hippodraw {

BinToMovingBrkPt::
BinToMovingBrkPt ( const char * name )
  : BinToColor ( name )
{
  m_control_points.push_back(0.5);
  m_control_points.push_back(0.5);
  m_control_points.push_back(0.5);
}

BinToMovingBrkPt::BinToMovingBrkPt ( const BinToMovingBrkPt & bin_to_color )
  : BinToColor ( bin_to_color )
{
 
  if(m_control_points.size()==0)
    {
      m_control_points.push_back(0.5);
      m_control_points.push_back(0.5);
      m_control_points.push_back(0.5);
    }
}

BinToColor * BinToMovingBrkPt:: clone () const
{
  return new BinToMovingBrkPt ( *this );
}

void BinToMovingBrkPt::doubleToColor ( double value, Color & color ) const
{
   double r=1;double g = 1;double b =1;   
//    double dv = m_range.length ();
//    double vmin = m_range.low ();
   double v = value;
   assert(m_control_points.size()!=0);

   double brk_pt = m_control_points[0];
   double flat = m_control_points[1];

   if ( brk_pt < (1 - brk_pt))
     flat = flat * ( brk_pt);
   else flat = flat * ( 1 - brk_pt);

   if ( v < (m_vmin + brk_pt * m_dv)) 
      r = 0;
   else r = ( v - m_vmin - brk_pt*m_dv ) / ( ( 1.05 - brk_pt ) * m_dv );
 
 
   if ( v < (m_vmin + (brk_pt - flat) * m_dv ))
      g = (v - m_vmin) / ( (0.05+brk_pt - flat) * m_dv );
   else if ( v > ( m_vmin + (brk_pt + flat) * m_dv ))      
   g =  1 - ( v - m_vmin - ( brk_pt + flat )*m_dv) / 
     ( (1.05 - brk_pt - flat) * m_dv );
   else g = 1;


   if ( v < ( m_vmin + brk_pt *m_dv))
     b = ( m_vmin + brk_pt*m_dv  - v ) / ( (brk_pt+0.05) * m_dv );
   else b = 0;


   int red   = static_cast < int > ( r * 255. );
   int green = static_cast < int > ( g * 255. );
   int blue  = static_cast < int > ( b * 255. );
   

   assert ( red < 256 && green < 256 && blue < 256 );
   color.setColor ( red, green, blue );
}

bool
BinToMovingBrkPt::
hasControlPoints () const
{
  return true;
}

} // namespace hippodraw
