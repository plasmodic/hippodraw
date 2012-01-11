/** @file

hippodraw::BinToGPink class implementation

Copyright (C) 2007   The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: BinToHueSatLight2.cxx,v 1.1 2007/01/23 19:24:49 pfkeb Exp $

*/

#include "BinToHueSatLight2.h"

using namespace hippodraw;

int BinToHueSatLight2::reds[256] =
{ 253, 253, 251, 250, 250, 249, 249, 248, 248, 247, 247,
246, 246, 246, 245, 245, 244, 244, 244, 243, 243, 242, 242, 242, 241,
241, 240, 240, 240, 239, 239, 239, 238, 238, 238, 237, 237, 236, 236,
236, 235, 235, 235, 235, 234, 233, 233, 232, 232, 231, 231, 230, 229,
229, 228, 227, 226, 226, 225, 224, 223, 223, 222, 221, 220, 219, 218,
217, 216, 215, 214, 213, 212, 211, 210, 208, 207, 206, 205, 204, 202,
200, 199, 198, 196, 195, 194, 192, 192, 191, 191, 190, 189, 188, 187,
185, 185, 184, 183, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174,
173, 172, 172, 171, 170, 169, 168, 167, 166, 165, 165, 164, 163, 162,
161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 149, 148, 147,
146, 145, 144, 143, 142, 140, 139, 138, 138, 137, 136, 135, 134, 132,
131, 130, 129, 128, 127, 126, 125, 124, 122, 121, 120, 119, 118, 118,
116, 114, 113, 112, 111, 111, 109, 108, 109, 111, 114, 115, 117, 118,
120, 123, 125, 127, 129, 131, 134, 137, 139, 141, 144, 147, 150, 152,
155, 158, 161, 164, 167, 171, 174, 177, 181, 184, 188, 192, 195, 199,
203, 207, 211, 216, 220, 224, 228, 233, 237, 238, 239, 239, 240, 240,
240, 241, 241, 241, 242, 242, 243, 243, 243, 244, 244, 245, 245, 245,
246, 246, 247, 247, 247, 248, 248, 249, 249, 250, 250, 251, 251, 252,
  252, 253, 253, 254, 254, 254, 254};
int BinToHueSatLight2::greens[256] =
{ 252, 252, 250, 250, 249, 249, 248, 248, 247, 247, 246,
246, 245, 245, 244, 244, 243, 242, 242, 241, 241, 240, 239, 239, 238,
238, 237, 236, 236, 235, 235, 234, 233, 233, 232, 232, 231, 230, 230,
229, 228, 228, 227, 226, 226, 225, 224, 223, 223, 222, 221, 221, 220,
219, 218, 218, 217, 217, 216, 215, 214, 214, 213, 212, 211, 211, 209,
209, 208, 208, 206, 206, 205, 205, 204, 202, 202, 201, 201, 200, 199,
197, 197, 196, 196, 195, 195, 194, 194, 194, 194, 194, 194, 194, 195,
194, 195, 195, 195, 196, 196, 196, 197, 197, 198, 198, 199, 199, 200,
200, 201, 202, 203, 204, 204, 205, 206, 207, 208, 210, 211, 212, 213,
215, 216, 217, 219, 220, 222, 222, 222, 222, 222, 222, 223, 223, 223,
223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 224,
224, 224, 224, 224, 224, 224, 224, 224, 225, 225, 225, 225, 225, 225,
225, 226, 226, 226, 226, 226, 227, 227, 227, 227, 227, 227, 228, 228,
228, 228, 229, 229, 229, 229, 229, 230, 230, 230, 230, 231, 231, 231,
231, 232, 232, 232, 232, 233, 233, 233, 234, 234, 234, 234, 235, 235,
235, 236, 236, 236, 237, 237, 237, 238, 238, 235, 231, 227, 223, 218,
214, 210, 205, 201, 196, 191, 187, 182, 177, 172, 167, 162, 157, 151,
146, 140, 135, 129, 123, 117, 111, 106, 99, 93, 85, 78, 74, 67, 61, 54,
  47, 40, 33, 27, 27};
  int BinToHueSatLight2::blues[256]=
{ 252, 252, 250, 250, 249, 249, 248, 248, 247, 247, 246,
246, 245, 245, 244, 244, 243, 243, 243, 242, 242, 241, 241, 241, 240,
240, 239, 239, 239, 238, 238, 238, 237, 237, 237, 237, 236, 236, 236,
236, 235, 235, 235, 235, 234, 234, 234, 234, 233, 233, 233, 232, 232,
232, 232, 231, 231, 231, 230, 230, 230, 230, 229, 229, 229, 229, 229,
228, 228, 228, 228, 227, 227, 227, 227, 227, 226, 226, 226, 226, 226,
226, 225, 226, 225, 225, 225, 225, 224, 225, 224, 224, 224, 224, 224,
224, 224, 224, 224, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223,
223, 223, 222, 222, 222, 222, 222, 222, 222, 222, 222, 222, 222, 222,
222, 222, 222, 222, 222, 222, 222, 220, 218, 217, 215, 213, 211, 210,
208, 206, 204, 202, 200, 197, 195, 193, 191, 188, 186, 183, 181, 178,
176, 173, 170, 168, 165, 162, 159, 156, 153, 150, 146, 143, 140, 137,
133, 129, 126, 122, 118, 116, 112, 107, 106, 105, 105, 104, 102, 100,
99, 99, 97, 96, 94, 93, 93, 91, 90, 88, 88, 86, 85, 83, 83, 81, 80, 78,
78, 76, 75, 73, 72, 71, 70, 69, 67, 66, 65, 63, 62, 62, 59, 58, 58, 55,
54, 54, 51, 50, 49, 47, 46, 45, 43, 42, 41, 39, 38, 36, 35, 34, 32, 31,
30, 28, 27, 25, 24, 23, 22, 20, 18, 17, 15, 14, 13, 8, 13, 8, 7, 5, 4,
  2, 1, 0, 0};
BinToHueSatLight2::
BinToHueSatLight2 ( const char * name )
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

BinToHueSatLight2::BinToHueSatLight2 ( const BinToHueSatLight2 & bin_to_color )
  : BinToColorMap ( bin_to_color )
{
}

BinToColor * BinToHueSatLight2::clone () const
{
  return new BinToHueSatLight2 ( *this );
}

