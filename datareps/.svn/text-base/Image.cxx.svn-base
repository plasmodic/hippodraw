/** @file

hippodraw::Image class implementation

Copyright (C) 2003, 2005, 2006 The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: Image.cxx,v 1.13 2006/07/29 22:25:47 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "Image.h"

#include "projectors/MapMatrixProjector.h"
#include "reps/ColorBoxPointRep.h"

using namespace hippodraw;

Image::Image ( )
  : DataRep()
{
  m_name = "Image";

  MapMatrixProjector * projector = new MapMatrixProjector();
  m_projector = projector;
  m_rep = new ColorBoxPointRep();
}

DataRep * Image::clone ()
{
  return new Image ( *this );
}

bool Image::acceptFunction ( int num )
{
  return num == 2;
}

using namespace hippodraw;

bool
Image::
hasAxis ( hippodraw::Axes::Type axis ) const
{
  return axis == Axes::X || axis == Axes::Y || axis ==Axes::Z;
}

bool
Image::
needsMatrixSet () const
{
  return true;
}
