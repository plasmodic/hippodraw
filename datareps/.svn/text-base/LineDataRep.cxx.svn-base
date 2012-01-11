/** @file

LineDataRep class implementation

Copyright (C) 2004, 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: LineDataRep.cxx,v 1.10 2005/10/30 00:10:10 pfkeb Exp $

*/

#include "LineDataRep.h"

#include "projectors/LineProjector.h"
#include "reps/LinePointRep.h"

namespace hippodraw {

LineDataRep::
LineDataRep ( hippodraw::Axes::Type axis, double value )
{
  m_name = "Static Line";
  m_projector = new LineProjector ( axis, value );
  m_rep = new LinePointRep ( );
  // The above deleted in ~DataRep()
  const Color red ( Color::red );

  m_rep->setColor ( red );
}

LineDataRep::
LineDataRep ()
{
	m_name = "Static Line";
}

DataRep * LineDataRep::clone ()
{
  return new LineDataRep ( *this );
}

/* virtual */
bool LineDataRep::hasNTupleBindings () const
{
  return false;
}

void
LineDataRep::
setValue ( double value )
{
  LineProjector * projector 
    = dynamic_cast < LineProjector * > ( m_projector );
  projector -> setValue ( value );
}

bool
LineDataRep::
hasAxis ( hippodraw::Axes::Type axis ) const
{
  return axis == Axes::X || axis == Axes::Y;
}

} // namespace hippodraw

