/** @file

hippodraw::FilledColumnPointRep class implementation

Copyright (C) 2006   The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: FilledColumnPointRep.cxx,v 1.5 2006/06/25 16:54:48 pfkeb Exp $

*/

// For truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "FilledColumnPointRep.h"

#include "axes/Range.h"
#include "graphics/ViewBase.h"

using std::vector;

using namespace hippodraw;

FilledColumnPointRep::
FilledColumnPointRep ()
  : ColumnPointRep ( ),
    m_fill_color ( Color::red )
{
  m_name = "FilledColumn";
}

FilledColumnPointRep::
FilledColumnPointRep ( const FilledColumnPointRep & point_rep )
  : ColumnPointRep ( point_rep ),
    m_fill_color ( point_rep.m_fill_color )
{
}

FilledColumnPointRep::
~FilledColumnPointRep ()
{
  // Does nothing
}

RepBase *
FilledColumnPointRep::
clone ()
{
  return new FilledColumnPointRep ( *this );
}


void
FilledColumnPointRep::
setColor ( const Color & color )
{
  m_fill_color = color;
}

const Color &
FilledColumnPointRep::getColor () const
{
  return m_fill_color;
}

void
FilledColumnPointRep::
drawValues ( ViewBase * view ) const
{
  int red = m_fill_color.getRed ();
  int green = m_fill_color.getGreen ();
  int blue = m_fill_color.getBlue ();

  const Range & range = view -> getRange ( Axes::Y );

  unsigned int size = m_x.size () -1;
  for ( unsigned int i = 1; i < size; i += 2 ) {
    double y2 = m_y [ i+1 ];
    double y1 = range.low ();
    double x1 = m_x [ i ];
    double x2 = m_x [ i+1 ];

      view -> drawSquare ( x1, y1, x2, y2, red, green, blue );
  }

  ColumnPointRep::drawValues ( view );
}
