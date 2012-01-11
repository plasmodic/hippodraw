/** @file

hippodraw::BinToGPink class implementation

Copyright (C) 2007   The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: BinToRainbow.cxx,v 1.2 2007/02/16 18:46:52 pfkeb Exp $

*/

#include "BinToRainbow.h"

using namespace hippodraw;

BinToRainbow::
BinToRainbow ( const char * name )
  : BinToColorMap ( name )
{
  m_reds.resize ( 256);
  m_greens.resize ( 256 );
  m_blues.resize ( 256 );

  for ( int i = 0; i < 256; i++ ) {
    if ( i < 64 ) {
      m_reds[i] = 0;
      m_greens[i] = 4*i;
      m_blues[i] = 255;
    }
    if ( i >= 64 && i < 192 ) {
      m_reds [i] = 2 * ( i - 64 );
      m_greens[i] = 255;
      m_blues[i] = 255 - m_reds[i];
    }      
    if ( i >= 192 ) {
      m_reds[i] = 255;
      m_greens[i] = 255 - 4*( i- 192);
      m_blues[i] = 0;
    }
  }
}

BinToRainbow::BinToRainbow ( const BinToRainbow & bin_to_color )
  : BinToColorMap ( bin_to_color )
{
}

BinToColor * BinToRainbow::clone () const
{
  return new BinToRainbow ( *this );
}

