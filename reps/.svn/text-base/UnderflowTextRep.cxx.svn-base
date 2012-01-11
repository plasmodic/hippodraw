/** @file

hippodra::UnderflowTextRep class implementation

Copyright (C) 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: UnderflowTextRep.cxx,v 1.4 2006/10/20 23:47:55 xiefang Exp $

*/


#include "UnderflowTextRep.h"

#include "graphics/ViewBase.h"
#include "pattern/string_convert.h"
#include "projectors/NTupleProjector.h"

using std::string;

using namespace hippodraw;

UnderflowTextRep::UnderflowTextRep ( )
  : TextRepBase ( "Underflow" )
{
}

UnderflowTextRep::UnderflowTextRep ( const UnderflowTextRep & rep )
  : TextRepBase( rep )
{
}

/* virtual */
RepBase * UnderflowTextRep::clone ()
{
  return new UnderflowTextRep ( *this );
}

void
UnderflowTextRep::
drawProjectedValues ( ProjectorBase & proj,
		      ViewBase & view )
{
  int underflow = proj.getUnderflow ();
  string text ( "Underflow_entries = " );
  if ( underflow < 0 ) {
    text += "meaningless";
  }
  else {
    text += String::convert ( underflow );
  }

  view.drawText ( text, 5, 10, m_size, 0, 'l', 't', true, 0, & m_color );
}
