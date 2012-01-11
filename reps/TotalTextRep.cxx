/** @file

hippodraw::TotalTextRep class implementation

Copyright (C) 2001-2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: TotalTextRep.cxx,v 1.30 2006/10/20 23:47:55 xiefang Exp $

*/

#include "TotalTextRep.h"

#include "graphics/ViewBase.h"
#include "pattern/string_convert.h"
#include "projectors/NTupleProjector.h"

using std::string;

using namespace hippodraw;

TotalTextRep::TotalTextRep ( )
  : TextRepBase ( "Total Entries" )
{
}

TotalTextRep::TotalTextRep ( const TotalTextRep & rep )
  : TextRepBase( rep )
{
}

RepBase * TotalTextRep::clone ()
{
  return new TotalTextRep ( *this );
}

void
TotalTextRep::
drawProjectedValues ( ProjectorBase & proj,
		      ViewBase & view )
{
  int total = proj.getNumberOfEntries ();
  string text ( "Total_entries = " );
  if ( total < 0 ) {
    text += "meaningless";
  }
  else {
    text += String::convert ( total );
  }

  view.drawText ( text, 5, 10, m_size, 0, 'l', 't', true, 0, & m_color );
}
