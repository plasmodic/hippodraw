/** @file

ProfileHist class implementation.

Copyright (C) 2000-2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: ProfileHist.cxx,v 1.67 2005/10/30 00:10:10 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "ProfileHist.h"

#include "projectors/ProfileProjector.h"
#include "reps/SymbolPointRep.h"

namespace hippodraw {

ProfileHist::ProfileHist ( )
{
  m_name = "Profile";

  m_projector = new ProfileProjector();
  m_rep = new SymbolPointRep ( Symbol::SOLIDSQUARE, 4.0 );
  setErrorDisplay ( Axes::Y, true );
}

DataRep * ProfileHist::clone ()
{
  return new ProfileHist ( *this );
}

bool ProfileHist::acceptFunction ( int num )
{
  return num == 1;
}

bool
ProfileHist::
hasErrorDisplay () const
{
  return true;
}

bool
ProfileHist::
hasAxis ( hippodraw::Axes::Type axis ) const
{
  return axis == Axes::X || axis == Axes::Y;
}

} // namespace hippodraw

