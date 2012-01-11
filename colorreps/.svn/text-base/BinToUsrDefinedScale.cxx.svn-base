/** @file

hippodraw::BinToUsrDefinedScale class implementation

Copyright (C) 2005, 2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinToUsrDefinedScale.cxx,v 1.8 2007/02/08 19:19:24 pfkeb Exp $

*/

#include "BinToUsrDefinedScale.h"

#include <cassert>

using namespace hippodraw;

BinToUsrDefinedScale::
BinToUsrDefinedScale ( const char * name )
  : BinToColor ( name )
{
  m_control_points.push_back(0.5);
  m_control_points.push_back(0.25);
}

BinToUsrDefinedScale::
BinToUsrDefinedScale ( const BinToUsrDefinedScale & bin_to_color )
  : BinToColor ( bin_to_color )
{
}

BinToColor * BinToUsrDefinedScale:: clone () const
{
  return new BinToUsrDefinedScale ( *this );
}


/** @todo This is copy of member function of same name in
    BinToColorSaleMovingBrkPt. */
void
BinToUsrDefinedScale::
doubleToColor ( double value, Color & color ) const
{
   double r = 1; double g = 1; double b = 1;   
   double v = value;

   assert(m_control_points.size()!=0);
   double brk_pt = m_control_points[0];
   double flat = m_control_points[1];
   double col = m_control_points[2];
   

   if ( brk_pt <= (1 - brk_pt))
     flat = flat * ( brk_pt);
   else flat = flat * ( 1 - brk_pt);

    if ( v < ( m_vmin + col*m_dv ) )
      v = v + 1.5 * m_dv;

    //Red
    if ( v < ( m_vmin + ( 1.25 * brk_pt - 0.5 * flat  + col) * m_dv ) ) 
      r = 0;

    else if ( v > ( m_vmin + ( 1.25 * brk_pt - 0.5 * flat  + col) * m_dv ) &&
	      v < ( m_vmin + ( 1.25 * brk_pt + 0.5 * flat + col) * m_dv ) )
      r = ( v - m_vmin - ( 1.25 * brk_pt - 0.5 * flat + col) * m_dv ) / 
	  ( ( (flat+0.001) ) * m_dv );

    else if( v > ( m_vmin + ( 1.25 * brk_pt + 0.5 * flat + col) * m_dv ) &&  
	     v < ( m_vmin + ( 1.25 + col ) * m_dv ) ) 
      r = 1;  

    else r = 1 -  4 * ( v - m_vmin - ( 1.25 + col ) * m_dv ) / m_dv;


    //Green
    if ( v < (m_vmin + (brk_pt - flat + col) * m_dv ))
      g = (v - m_vmin - col*m_dv ) / ( (0.001+brk_pt - flat ) * m_dv );

    else if ( v > (m_vmin + (brk_pt - flat + col) * m_dv ) &&
	      v < ( m_vmin + (brk_pt + flat + col) * m_dv ) )      
      g = 1;

    else if ( v > ( m_vmin + (brk_pt + flat + col) * m_dv ) &&
	      v < ( m_vmin + ( 1 + col ) * m_dv ) )       

      g =  1 - ( v - m_vmin - ( brk_pt + flat + col )*m_dv)/
       ( (1.001 - brk_pt - flat ) * m_dv );
  
    else  g = 0;
   

    //Blue
    if ( v < (m_vmin + ( 0.75 * brk_pt -  0.5 * flat + col ) * m_dv ) ) 
      b = 1;

    else if ( v > (m_vmin + ( 0.75 * brk_pt -  0.5 * flat + col ) * m_dv ) &&
	      v < (m_vmin + ( 0.75 * brk_pt  +  0.5 * flat  + col) * m_dv ) ) 
     
      b = 1 - ( v - m_vmin - ( 0.75 * brk_pt - 0.5 * flat + col )*m_dv ) / 
	( ( (flat + 0.001) ) * m_dv );
 
    else if ( v > (m_vmin + ( 0.75 * brk_pt  +  0.5 * flat  + col) * m_dv ) && 
	       v < ( m_vmin + ( 1 + col ) * m_dv ) ) 
      b = 0;  

    else if ( v > ( m_vmin + ( 1 + col ) * m_dv ) &&  
	      v < ( m_vmin + ( 1.25 + col ) * m_dv ) )
     
      b = 4 * ( v - m_vmin - ( 1 + col ) * m_dv ) / m_dv ; 

    else b = 1;

   int red   = static_cast < int > ( r * 255. );
   int green = static_cast < int > ( g * 255. );
   int blue  = static_cast < int > ( b * 255. );
   
 
   assert ( red < 256 && green < 256 && blue < 256 );
   color.setColor ( red, green, blue );
//    double r = 1; double g = 1; double b = 1;   
//    double v = value;
 
//    assert(m_control_points.size()!=0);
//    double brk_pt = m_control_points[0];
//    double flat = m_control_points[1];
 
//    if ( brk_pt <= (1 - brk_pt))
//      flat = flat * ( brk_pt);
//    else flat = flat * ( 1 - brk_pt);



//    if ( v < ( m_vmin + ( 1.25 * brk_pt - 0.5 * flat ) * m_dv ) ) 
//       r = 0;

//    else if( v > ( m_vmin + ( 1.25 * brk_pt + 0.5 * flat ) * m_dv ) ) 
//       r = 1;  

//    else r = ( v - m_vmin - ( 1.25 * brk_pt - 0.5 * flat ) * m_dv ) / 
// 	  ( ( (flat+0.001) ) * m_dv );

 


//    if ( v < (m_vmin + (brk_pt - flat) * m_dv ))
//       g = (v - m_vmin) / ( (0.001+brk_pt - flat) * m_dv );

//    else if ( v > ( m_vmin + (brk_pt + flat) * m_dv ))      

//      g =  1 - ( v - m_vmin - ( brk_pt + flat )*m_dv)/
//        ( (1.001 - brk_pt - flat) * m_dv );
   
//    else g = 1;




//    if ( v < (m_vmin + ( 0.75 * brk_pt -  0.5 * flat ) * m_dv ) ) 
//       b = 1;

//    else if( v > (m_vmin + ( 0.75 * brk_pt  +  0.5 * flat ) * m_dv ) ) 
//       b = 0;  

//    else b = 1 - ( v - m_vmin - ( 0.75 * brk_pt - 0.5 * flat)*m_dv ) / 
// 	  ( ( (flat + 0.001) ) * m_dv );

 


//    int red   = static_cast < int > ( r * 255. );
//    int green = static_cast < int > ( g * 255. );
//    int blue  = static_cast < int > ( b * 255. );
   

//    assert ( red < 256 && green < 256 && blue < 256 );
//    color.setColor ( red, green, blue );
}

bool
BinToUsrDefinedScale::
hasControlPoints () const
{
  return true;
}

bool
BinToUsrDefinedScale::
isUserDefined () const
{
  return true;
}
