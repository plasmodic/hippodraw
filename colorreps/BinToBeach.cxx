/** @file

hippodraw::BinToGPink class implementation

Copyright (C) 2007   The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: BinToBeach.cxx,v 1.1 2007/01/24 00:04:15 pfkeb Exp $

*/

#include "BinToBeach.h"

using namespace hippodraw;

int BinToBeach::reds[256] =
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 4, 8, 12, 17, 21, 25, 29, 34, 38, 42, 46, 51, 55, 59, 64, 68,
72, 76, 81, 85, 89, 93, 98, 102, 106, 110, 115, 119, 123, 128, 132, 136,
140, 76, 76, 76, 81, 85, 89, 93, 98, 102, 106, 110, 115, 119, 123, 128,
132, 136, 140, 145, 149, 153, 157, 157, 161, 164, 167, 170, 173, 177,
180, 183, 186, 189, 193, 196, 199, 202, 205, 209, 212, 215, 218, 218,
219, 220, 222, 223, 224, 225, 226, 227, 229, 230, 231, 232, 233, 234,
236, 237, 238, 239, 240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0};
int BinToBeach::greens[256] =
{ 251, 251, 249, 247, 245, 243, 241, 238, 236, 234, 232,
230, 228, 226, 224, 221, 219, 217, 215, 213, 211, 209, 206, 204, 202,
200, 198, 196, 194, 192, 189, 187, 185, 183, 181, 179, 177, 174, 172,
170, 168, 166, 164, 162, 160, 157, 155, 153, 151, 149, 147, 145, 142,
140, 138, 136, 134, 132, 130, 128, 128, 129, 131, 133, 134, 136, 138,
139, 141, 143, 145, 146, 148, 150, 151, 153, 155, 157, 158, 160, 162,
163, 165, 167, 168, 170, 172, 174, 175, 177, 179, 180, 182, 182, 177,
171, 165, 160, 154, 149, 143, 138, 132, 126, 121, 115, 110, 104, 99, 93,
88, 82, 76, 71, 65, 60, 54, 49, 43, 37, 32, 26, 21, 15, 13, 4, 0, 0, 0,
4, 8, 12, 17, 21, 25, 29, 34, 38, 42, 46, 51, 55, 59, 64, 68, 72, 76,
81, 81, 85, 89, 93, 98, 102, 106, 110, 115, 119, 123, 128, 132, 136,
140, 145, 149, 153, 157, 162, 162, 166, 170, 174, 179, 183, 187, 192,
196, 200, 204, 209, 213, 217, 221, 226, 230, 234, 238, 243, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int BinToBeach::blues[256]=
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 8, 12, 17, 21, 25,
29, 34, 38, 42, 46, 51, 55, 59, 64, 68, 72, 76, 81, 85, 89, 93, 98, 102,
106, 110, 115, 119, 123, 128, 132, 136, 136, 136, 136, 136, 136, 136,
136, 136, 136, 136, 136, 136, 136, 136, 136, 136, 136, 136, 136, 136,
136, 136, 136, 136, 136, 136, 136, 136, 136, 136, 136, 136, 136, 76, 76,
76, 72, 68, 64, 60, 56, 52, 48, 44, 40, 36, 32, 28, 23, 19, 15, 11, 7,
3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 8, 12, 17,
21, 25, 29, 34, 38, 42, 46, 51, 55, 59, 64, 68, 72, 76, 81, 81, 84, 88,
91, 95, 98, 102, 105, 109, 113, 116, 120, 123, 127, 130, 134, 137, 141,
145, 148, 152, 155, 159, 159, 162, 166, 170, 173, 177, 181, 184, 188,
192, 195, 199, 203, 206, 210, 214, 217, 221, 225, 228, 232, 236, 239, 0,
  0, 0};
BinToBeach::
BinToBeach ( const char * name )
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

BinToBeach::BinToBeach ( const BinToBeach & bin_to_color )
  : BinToColorMap ( bin_to_color )
{
}

BinToColor * BinToBeach::clone () const
{
  return new BinToBeach ( *this );
}

