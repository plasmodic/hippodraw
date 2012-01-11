/** @file

BinToLineStyle class implementation

Copyright (C)  2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinToLineStyle.cxx,v 1.1 2006/10/02 23:00:29 xiefang Exp $

*/

#include "BinToLineStyle.h"

namespace hippodraw {

BinToLineStyle::
BinToLineStyle ( const char * name )
  : BinToColor ( name )
{
}

BinToLineStyle::BinToLineStyle ( const BinToLineStyle & bin_to_color )
  : BinToColor ( bin_to_color )
{
}

BinToColor * BinToLineStyle:: clone () const
{
  return new BinToLineStyle ( *this );
}

void
BinToLineStyle::
doubleToColor ( double, Color & color ) const
{ 
  int red=color.getRed();
  int green=color.getGreen();
  int blue=color.getBlue();
  
  color.setColor ( red, green, blue );
}

bool
BinToLineStyle::
acceptChangeColor () const
{ 
	return true;
}

} // namespace hippodraw

