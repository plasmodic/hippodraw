/** @file

hippodraw::BinToGPink class implementation

Copyright (C) 2007   The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: BinToEosB.cxx,v 1.1 2007/01/28 19:04:26 pfkeb Exp $

*/

#include "BinToEosB.h"

using namespace hippodraw;

int BinToEosB::reds[256] =
{ 255, 255, 127, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
9, 18, 27, 36, 45, 49, 57, 72, 81, 91, 100, 109, 118, 127, 136, 131,
139, 163, 173, 182, 191, 200, 209, 218, 227, 213, 221, 255, 255, 255,
255, 255, 255, 255, 255, 229, 229, 255, 255, 255, 255, 255, 255, 255,
255, 229, 229, 255, 255, 255, 255, 255, 255, 255, 255, 229, 229, 255,
255, 255, 255, 255, 255, 255, 255, 229, 229, 255, 255, 255, 255, 255,
255, 255, 255, 229, 229, 255, 255, 255, 255, 255, 255, 255, 255, 229,
229, 255, 255, 255, 255, 255, 255, 255, 255, 229, 229, 255, 255, 255,
255, 255, 255, 255, 255, 229, 229, 255, 255, 255, 255, 255, 255, 255,
255, 229, 229, 255, 253, 251, 249, 247, 245, 243, 241, 215, 214, 235,
234, 232, 230, 228, 226, 224, 222, 198, 196, 216, 215, 213, 211, 209,
207, 205, 203, 181, 179, 197, 196, 194, 192, 190, 188, 186, 184, 164,
162, 178, 176, 175, 173, 171, 169, 167, 165, 147, 145, 159, 157, 156,
154, 152, 150, 148, 146, 130, 128, 140, 138, 137, 135, 133, 131, 129,
  127, 113, 111, 121, 119, 117, 117};
int BinToEosB::greens[256] =
{ 255, 255, 127, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 15, 23, 31, 39, 47,
55, 57, 64, 79, 87, 95, 103, 111, 119, 127, 135, 129, 136, 159, 167,
175, 183, 191, 199, 207, 215, 200, 207, 239, 247, 255, 255, 255, 255,
255, 255, 229, 229, 255, 255, 255, 255, 255, 255, 255, 255, 229, 229,
255, 255, 255, 255, 255, 255, 255, 255, 229, 229, 255, 250, 246, 242,
238, 233, 229, 225, 198, 195, 212, 208, 204, 199, 195, 191, 187, 182,
160, 156, 169, 165, 161, 157, 153, 148, 144, 140, 122, 118, 127, 125,
123, 121, 119, 116, 114, 112, 99, 97, 106, 104, 102, 99, 97, 95, 93, 91,
80, 78, 84, 82, 80, 78, 76, 74, 72, 70, 61, 59, 63, 61, 59, 57, 55, 53,
50, 48, 42, 40, 42, 40, 38, 36, 33, 31, 29, 27, 22, 21, 21, 19, 16, 14,
12, 13, 8, 6, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0};
  int BinToEosB::blues[256]=
{ 255, 255, 127, 131, 136, 140, 144, 148, 153, 157, 145,
149, 170, 174, 178, 182, 187, 191, 195, 199, 183, 187, 212, 216, 221,
225, 229, 233, 238, 242, 221, 225, 255, 247, 239, 231, 223, 215, 207,
199, 172, 164, 175, 167, 159, 151, 143, 135, 127, 119, 100, 93, 95, 87,
79, 71, 63, 55, 47, 39, 28, 21, 15, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
BinToEosB::
BinToEosB ( const char * name )
  : BinToColorMap ( name )
{
  m_reds.resize ( 256);
  m_greens.resize ( 256 );
  m_blues.resize ( 256 );

  for ( int i = 0; i < 256; i++ ) {
    m_reds  [i] = reds[i];
    m_greens[i] = greens[i];
    m_blues [i] = blues[i];
  }
}

BinToEosB::BinToEosB ( const BinToEosB & bin_to_color )
  : BinToColorMap ( bin_to_color )
{
}

BinToColor * BinToEosB::clone () const
{
  return new BinToEosB ( *this );
}

