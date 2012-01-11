/** @file

hippodraw::AverageTextRep class implementation

Copyright (C) 2001-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: AverageTextRep.cxx,v 1.28 2006/10/20 23:47:55 xiefang Exp $

*/

#include "AverageTextRep.h"

#include "graphics/ViewBase.h"
#include "pattern/string_convert.h"
#include "projectors/NTupleProjector.h"

using namespace hippodraw;

using std::string;

AverageTextRep::AverageTextRep ( )
  : TextRepBase ( "averagex" )
{
  m_axis = Axes::X;
}

AverageTextRep::
AverageTextRep ( hippodraw::Axes::Type axis )
  : TextRepBase ( "averagex" )
{
  m_axis = axis;
  if ( axis == Axes::Y ) m_name = "averagey" ;
}

AverageTextRep::AverageTextRep ( const AverageTextRep & rep )
  : TextRepBase( rep )
{
  m_axis = rep.m_axis;
}

/* virtual */
RepBase * AverageTextRep::clone ()
{
  return new AverageTextRep ( *this );
}

void
AverageTextRep::
drawProjectedValues ( ProjectorBase & projector,
		      ViewBase & view )

{
  double average = projector.getAverage ( m_axis );

  string text ( "average " );
 
  if (m_axis == Axes::X ) {
    text += "X"; 
  }
  if (m_axis == Axes::Y ) {
    text += "Y";
  }
  text += "-value ";
  text += String::convert ( average );

  view.drawText ( text, 5, 10, m_size, 0, 'l', 't', true, 0, &m_color );
}
