/** @file

hippodraw::BinToGPink class implementation

Copyright (C) 2007   The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: BinToPastels.cxx,v 1.1 2007/01/23 19:24:49 pfkeb Exp $

*/

#include "BinToPastels.h"

using namespace hippodraw;

int BinToPastels::reds[256] =
{ 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
253, 251, 249, 247, 245, 243, 241, 239, 237, 235, 233, 231, 229, 227,
225, 223, 221, 219, 217, 216, 214, 212, 210, 208, 206, 204, 202, 200,
198, 196, 194, 192, 190, 188, 186, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 2, 8, 13, 19, 25, 30, 36, 42, 48, 53, 59, 65, 71, 76, 82,
88, 93, 99, 105, 111, 116, 122, 128, 133, 139, 145, 151, 156, 162, 168,
173, 179, 185, 191, 196, 202, 208, 214, 219, 225, 231, 236, 242, 248,
254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255};
int BinToPastels::greens[256] =
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 140, 146, 152, 157, 163,
169, 174, 180, 186, 192, 197, 203, 209, 214, 220, 226, 232, 237, 243,
249, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 250, 244, 238, 233, 227, 221, 215,
  210, 204, 198, 193, 187, 181, 175, 170, 170};
  int BinToPastels::blues[256]=
{ 72, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 91, 93, 95,
97, 99, 101, 103, 105, 107, 109, 111, 113, 115, 117, 119, 121, 123, 125,
127, 129, 131, 133, 135, 137, 139, 141, 142, 144, 146, 148, 150, 152,
154, 156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180,
182, 184, 186, 188, 190, 191, 193, 195, 197, 199, 201, 203, 205, 207,
209, 211, 213, 215, 217, 219, 221, 223, 225, 227, 229, 231, 233, 235,
237, 239, 241, 242, 244, 246, 248, 250, 252, 254, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 249, 243, 237, 232,
226, 220, 214, 209, 203, 197, 192, 186, 180, 174, 169, 163, 157, 152,
146, 140, 134, 129, 123, 117, 112, 106, 100, 94, 89, 83, 77, 71, 66, 60,
54, 49, 43, 37, 31, 26, 20, 14, 9, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0};
BinToPastels::
BinToPastels ( const char * name )
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

BinToPastels::BinToPastels ( const BinToPastels & bin_to_color )
  : BinToColorMap ( bin_to_color )
{
}

BinToColor * BinToPastels::clone () const
{
  return new BinToPastels ( *this );
}

