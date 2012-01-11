/** @file

hippodraw::BinToGreyScale class implementation

Copyright (C)  2000, 2004-2006  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinToGreyScale.cxx,v 1.24 2007/01/21 21:10:54 pfkeb Exp $

*/

#include "BinToGreyScale.h"

using namespace hippodraw;

BinToGreyScale::
BinToGreyScale ( const char * name )
  : BinToColor ( name )
{
}

BinToGreyScale::BinToGreyScale ( const BinToGreyScale & bin_to_color )
  : BinToColor ( bin_to_color )
{
}

BinToColor * BinToGreyScale::clone () const
{
  return new BinToGreyScale ( *this );
}

void
BinToGreyScale::
doubleToColor ( double value, Color & color ) const
{ 
  int red=color.getRed();
  int green=color.getGreen();
  int blue=color.getBlue();

  double tmp = ( value - m_vmin ) / m_dv ;
  double red_tmp   = 255. - tmp * ( 255. - static_cast < double > ( red) );
  double green_tmp = 255. - tmp * ( 255. - static_cast < double > ( green ) );
  double blue_tmp  = 255. - tmp * ( 255. - static_cast < double > ( blue ) ); 

  int ired_tmp = static_cast< int> ( red_tmp );
  int igreen_tmp = static_cast< int> ( green_tmp );
  int iblue_tmp = static_cast< int> ( blue_tmp );

  color.setColor ( ired_tmp, igreen_tmp, iblue_tmp );
}

bool
BinToGreyScale::
acceptChangeColor () const
{ 
	return true;
}
