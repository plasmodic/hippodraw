/** @file

hippodraw::BinToGPink class implementation

Copyright (C) 2007   The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: BinToGPink.cxx,v 1.1 2007/01/22 20:14:00 pfkeb Exp $

*/

#include "BinToGPink.h"

using namespace hippodraw;

int BinToGPink::reds[256] =
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55,
60, 65, 70, 75, 80, 84, 88, 92, 96, 101, 106, 111, 116, 121, 126, 131,
136, 141, 146, 151, 156, 159, 162, 165, 168, 171, 174, 177, 180, 183,
186, 189, 192, 195, 198, 201, 204, 207, 210, 213, 216, 219, 222, 225,
228, 231, 234, 237, 240, 243, 246, 249, 252, 252, 252, 252, 252, 252,
252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252,
252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252,
252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252,
252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252,
  252, 252, 252, 253, 254, 255};
int BinToGPink::greens[256] =
  { 0, 0, 0, 0, 14, 28, 43, 57, 72, 81, 90, 99, 108, 117, 126,
135, 144, 153, 162, 171, 180, 179, 178, 177, 176, 175, 174, 173, 172,
171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158,
157, 156, 154, 152, 150, 148, 147, 146, 145, 144, 143, 142, 141, 140,
139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 126, 124,
122, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108,
107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 94, 92, 90, 88,
87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70,
69, 68, 66, 64, 62, 60, 59, 58, 57, 56, 54, 52, 50, 48, 47, 46, 45, 44,
42, 40, 38, 36, 34, 32, 30, 28, 26, 24, 22, 20, 19, 18, 17, 16, 14, 12,
10, 8, 6, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44,
48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112,
116, 120, 124, 128, 132, 136, 140, 144, 147, 150, 153, 156, 160, 164,
168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220,
    224, 228, 232, 236, 240, 244, 248, 252, 253, 254, 255};
  int BinToGPink::blues[256]=
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 1, 3, 4, 6, 8, 10, 12, 14, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33,
35, 37, 38, 40, 42, 44, 46, 48, 50, 53, 54, 56, 58, 60, 62, 64, 66, 68,
69, 71, 73, 75, 77, 79, 81, 83, 84, 86, 88, 90, 92, 94, 96, 99, 100,
102, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 123, 125, 127,
129, 131, 133, 135, 137, 138, 140, 142, 144, 146, 148, 150, 153, 154,
156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 177, 179, 181,
183, 185, 187, 189, 191, 193, 195, 197, 199, 199, 198, 197, 196, 196,
196, 196, 195, 195, 194, 193, 192, 192, 191, 191, 190, 190, 190, 190,
190, 190, 190, 190, 189, 189, 188, 187, 186, 186, 185, 185, 184, 184,
185, 186, 187, 187, 187, 187, 187, 187, 187, 187, 188, 188, 188, 188,
189, 189, 189, 189, 189, 189, 189, 189, 190, 190, 190, 190, 191, 191,
191, 191, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202,
203, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 214,
215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228,
229, 230, 231, 232, 233, 234, 235, 236, 237, 237, 238, 239, 240, 241,
      242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};
BinToGPink::
BinToGPink ( const char * name )
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

BinToGPink::BinToGPink ( const BinToGPink & bin_to_color )
  : BinToColorMap ( bin_to_color )
{
}

BinToColor * BinToGPink::clone () const
{
  return new BinToGPink ( *this );
}

