/** @file

hippodraw::BoxTextRep class implementation

Copyright (C) 2003-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

 $Id: BoxTextRep.cxx,v 1.16 2007/04/23 21:57:21 pfkeb Exp $

*/

#ifdef _MSC_VER
// include max() and min() missing from MicroSoft Visual C++
#include "msdevstudio/MSconfig.h"
#endif //_MSC_VER

#include "BoxTextRep.h"

#include "pattern/string_convert.h"
#include "graphics/ViewBase.h"
#include "projectors/NTupleProjector.h"

using std::string;

using namespace hippodraw;

BoxTextRep::BoxTextRep ( )
  : TextRepBase ( "Text From Box" )
{
}

BoxTextRep::BoxTextRep ( const BoxTextRep & rep )
  : TextRepBase( rep )
{
}

RepBase * BoxTextRep::clone ()
{
  return new BoxTextRep ( *this );
}

/** @bug Should split the line if \n is in the string.
 */
void
BoxTextRep::
drawProjectedValues ( ProjectorBase &,
		      ViewBase & view )

{
  if (String::ci_find(m_text, "tex:")==0) {
    string tex_snippet = m_text.substr(4);
    view.drawLatex ( tex_snippet );
  }
    
  else {
    view.drawText ( m_text, 5, 10, m_size, 0, 'l', 't', true, 0, & m_color );
  }
}
