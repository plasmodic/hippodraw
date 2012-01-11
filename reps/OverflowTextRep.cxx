/** @file

hippodraw::OverflowTextRep class implementation

Copyright (C) 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: OverflowTextRep.cxx,v 1.4 2006/10/20 23:47:55 xiefang Exp $

*/

#include "OverflowTextRep.h"

#include "graphics/ViewBase.h"
#include "pattern/string_convert.h"
#include "projectors/NTupleProjector.h"

using std::string;

using namespace hippodraw;

OverflowTextRep::OverflowTextRep ( )
  : TextRepBase ( "Overflow" )
{
}

OverflowTextRep::OverflowTextRep ( const OverflowTextRep & rep )
  : TextRepBase( rep )
{
}

RepBase * OverflowTextRep::clone ()
{
  return new OverflowTextRep ( *this );
}

void
OverflowTextRep::
drawProjectedValues ( ProjectorBase & proj,
		      ViewBase & view )
{
  int overflow = proj.getOverflow ();
  string text ( "Overflow_entries = " );
  if ( overflow < 0 ) {
    text += "meaningless";
  }
  else {
    text += String::convert ( overflow );
  }

  view.drawText ( text, 5, 10, m_size, 0, 'l', 't', true, 0, & m_color );
}
