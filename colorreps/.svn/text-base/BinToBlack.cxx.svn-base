/** @file

BinToBlack class implementation

Copyright (C)  2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinToBlack.cxx,v 1.40 2006/09/25 16:19:25 pfkeb Exp $

*/

#include "BinToBlack.h"

namespace hippodraw {

BinToBlack::
BinToBlack ( const char * name )
  : BinToColor ( name )
{
}

BinToBlack::BinToBlack ( const BinToBlack & bin_to_color )
  : BinToColor ( bin_to_color )
{
}

BinToColor * BinToBlack:: clone () const
{
  return new BinToBlack ( *this );
}

void
BinToBlack::
doubleToColor ( double, Color & color ) const
{ 
 // PlotterBase * plotter = Inspector:: getPlotter ();
 // if ( !plotter ) return;

  //const Color & rep_color = plotter->repColor();
  //QColor color_temp ( rep_color.getRed(),
//		              rep_color.getGreen(),
//		              rep_color.getBlue() );
 // color_temp = QColorDialog::getColor ( color_temp );
  //if ( color_temp.isValid() == false ) return;

  //int red = color_temp.red();
  //int green = color_temp.green();
  //int blue = color_temp.blue();
  
  int red=color.getRed();
  int green=color.getGreen();
  int blue=color.getBlue();
  
  color.setColor ( red, green, blue );
}

bool
BinToBlack::
acceptChangeColor () const
{ 
	return true;
}

} // namespace hippodraw

