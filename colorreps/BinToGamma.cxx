/** @file

hippodraw::BinToGamma class implementation

Copyright (C) 2005, 2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinToGamma.cxx,v 1.2 2007/02/06 22:09:45 pfkeb Exp $

*/

#include "BinToGamma.h"

#include <cmath>
#include <cassert>

using std::pow;

using namespace hippodraw;

BinToGamma::
BinToGamma ( const char * name )
  : BinToColor ( name )
{
  m_control_points.push_back(0.5);
}

BinToGamma::
BinToGamma ( const BinToGamma & bin_to_color )
  : BinToColor ( bin_to_color )
{
}

BinToColor * BinToGamma:: clone () const
{
  return new BinToGamma ( *this );
}


void
BinToGamma::
doubleToColor ( double value, Color & color ) const
{
   assert(m_control_points.size()!=0);

   // brk_pt which represents gamma ranges from [0,1]  
   double brk_pt = m_control_points[0];

   // convert gamma from [0,1] to [0,5]
   double gam= brk_pt * 8;

   int red=color.getRed();
   int green=color.getGreen();
   int blue=color.getBlue();

   double red_tem= 255. - (pow(((value-m_vmin)/m_dv), gam)) * (255.-(double) red);
   double green_tem= 255. - (pow(((value-m_vmin)/m_dv), gam)) * (255.-(double) green);
   double blue_tem= 255. - (pow(((value-m_vmin)/m_dv), gam)) * (255.-(double) blue);

   int ired_tmp = static_cast< int> ( red_tem );
   int igreen_tmp = static_cast< int> ( green_tem );
   int iblue_tmp = static_cast< int> ( blue_tem );

   //color.setColor ( red, green, blue );
   color.setColor ( ired_tmp, igreen_tmp, iblue_tmp );

}

bool
BinToGamma::
hasControlPoints () const
{
  return true;
}

bool
BinToGamma::
isUserDefined () const
{
  return true;
}

bool
BinToGamma::
acceptChangeColor () const
{ 
	return true;
}
