/** @file

Profile2D class implementation

Copyright (C) 2000-2005  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Profile2D.cxx,v 1.24 2005/10/30 00:10:10 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "Profile2D.h"

#include "projectors/Profile2DProjector.h"
#include "reps/ColorBoxPointRep.h"


namespace hippodraw {

Profile2D::Profile2D ( )
  : DataRep ()
{
  m_name = "Profile 2D";
  
  m_projector = new Profile2DProjector();
  m_rep = new ColorBoxPointRep();
}

DataRep * Profile2D::clone ()
{
  return new Profile2D ( *this );
}

using namespace hippodraw;

bool
Profile2D::
hasAxis ( hippodraw::Axes::Type axis ) const
{
  return axis == Axes::X || axis == Axes::Y || axis ==Axes::Z;
}

} // namespace hippodraw

