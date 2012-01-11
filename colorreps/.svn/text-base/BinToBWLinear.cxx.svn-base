/** @file

hippodraw::BinToBWLinear class implementation

Copyright (C)  2000, 2004-2006  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinToBWLinear.cxx,v 1.1 2007/01/21 21:10:54 pfkeb Exp $

*/

#include "BinToBWLinear.h"

using namespace hippodraw;

BinToBWLinear::
BinToBWLinear ( const char * name )
  : BinToColorMap ( name )
{
  m_reds.resize ( 256);
  m_greens.resize ( 256 );
  m_blues.resize ( 256 );

  for ( int i = 0; i < 256; i++ ) {
    m_reds  [i] =  i;
    m_greens[i] =  i;
    m_blues [i] =  i;
  }
}

BinToBWLinear::BinToBWLinear ( const BinToBWLinear & bin_to_color )
  : BinToColorMap ( bin_to_color )
{
}

BinToColor * BinToBWLinear::clone () const
{
  return new BinToBWLinear ( *this );
}

