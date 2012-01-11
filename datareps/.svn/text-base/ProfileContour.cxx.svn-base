/** @file

ProfileContour class implementation

Copyright (C) 2000-2005  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: ProfileContour.cxx,v 1.10 2005/10/30 00:10:10 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "ProfileContour.h"

#include "projectors/Profile2DProjector.h"
#include "reps/ContourPointRep.h"

namespace hippodraw {

ProfileContour::ProfileContour ( )
  : DataRep ()
{
  m_name = "Profile Contour";
  
  m_projector = new Profile2DProjector();
  m_rep = new ContourPointRep();
}

DataRep * ProfileContour::clone ()
{
  return new ProfileContour ( *this );
}

using namespace hippodraw;

bool
ProfileContour::
hasAxis ( hippodraw::Axes::Type axis ) const
{
  return axis == Axes::X || axis == Axes::Y || axis ==Axes::Z;
}

} // namespace hippodraw

