// -*- mode:c++ -*-

/** @file

This is the SIP interface definition for Color

Copyright (C)  2002, 2004, 2005  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

$Id: Color.sip,v 1.11 2006/10/07 17:58:09 pfkeb Exp $

@author Paul_Kunz@slac.stanford.edu

*/

class Color
{
%TypeHeaderCode
#include "graphics/Color.h"
using hippodraw::Color;
%End

public:

/** The available fixed colors.
 */
enum Value { red, green, blue, yellow, orange, cyan, magenta,
	     black, darkgray, lightgray, white };

//   /** The following constructor takes red, green, and blue components as
//   arguments.  The range of the values is 0 to 255. If the default
//   values are taken, then the color will be black.*/
//   Color ( int = 0, int = 0, int = 0 );

    /** The following constructor takes red, green, and blue components as
  arguments.  The range of the values is 0 to 255. If the default
  values are taken, then the color will be black.*/
  /* explicit */
  Color ( int  =  0, int  = 0, int = 0 );

  /** Sets the color. */
  void setColor ( int =  0, int  = 0, int = 0 );

  int getRed () const;

  int getGreen () const;

  int getBlue () const;

//   friend std::ostream & operator << ( std::ostream &, const Color & );

};

// %ModuleCode
//   static Color blue   = Color (   0,   0, 255 );
//   static Color red    = Color ( 255,   0,   0 );
//   static Color yellow = Color ( 255, 255,   0 );
// %End

