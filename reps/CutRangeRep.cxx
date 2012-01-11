/** @file

hippodraw::CutRangeRep class implementation

Copyright (C) 2002-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: CutRangeRep.cxx,v 1.32 2007/06/06 17:18:29 pfkeb Exp $

*/

#include "CutRangeRep.h"

#include "datasrcs/TupleCut.h"
#include "graphics/ViewBase.h"
#include "transforms/BinaryTransform.h"

#include <stdexcept>

using std::string;
using std::vector;

using namespace hippodraw;

CutRangeRep::CutRangeRep ( )
  : PointRepBase ( "CutRangeRep", 0.0 )
{
  const Color yellow ( Color::yellow );
  setColor ( yellow );
}

CutRangeRep::CutRangeRep ( const CutRangeRep & point_rep )
  : PointRepBase ( point_rep )
{
}

CutRangeRep * CutRangeRep::clone()
{
  return new CutRangeRep( *this );
}

void
CutRangeRep::
drawProjectedValues ( const DataSource *,
		      TransformBase *,
		      ViewBase * )
{
}

void 
CutRangeRep::
drawProjectedValues ( const Range & x_range,
		      const Range & y_range,
		      const TransformBase & tb,
		      ViewBase & view )
{
  int dim = m_cut_list.size();
  
  bool yes = m_cut_list[0].getInversion ();
  if ( dim == 2 ) yes |= m_cut_list[1].getInversion ();
  if ( yes ) {
    if ( dim == 1 ) {
      drawInvertedRange1D ( x_range, y_range, tb, view );
    }
    else {
      drawInvertedRange2D ( x_range, y_range, tb, view );
    }
  }
  else drawNormalRange ( x_range, y_range, tb, view );
}

void
CutRangeRep::
drawNormalRange ( const Range & x_range, const Range & y_range,
		  const TransformBase & tb,
		  ViewBase & view )
{
  const BinaryTransform & t 
    = dynamic_cast < const BinaryTransform & > ( tb );
  
  double x1 = x_range.low ();
  double y1 = y_range.low ();
  double x2 = x_range.high ();
  double y2 = y_range.high ();
  
  if ( m_cut_list.size () >= 1 ) // i.e. this section applies to 1D and 2D cuts
    {
      const Range & range = m_cut_list[0].getRange ();
      double cut_low = range.low ();
      double cut_high = range.high ();
      if ( cut_low > x1 ) x1 = cut_low;
      if ( cut_high < x2 ) x2 = cut_high;
    }
  
  if ( m_cut_list.size () >= 2 ) // i.e., this section applies only to 2D cuts.
    {
      const Range & range = m_cut_list[1].getRange ();
      double cut_low = range.low ();
      double cut_high = range.high ();
      if ( cut_low > y1 ) y1 = cut_low;
      if ( cut_high < y2 ) y2 = cut_high;
    }
  
  t.transform ( x1, y1 );
  t.transform ( x2, y2 );

  int red;
  int green;
  int blue;
  
  if ( m_desel == false ) {
    red   = m_color.getRed ();
    green = m_color.getGreen ();
    blue  = m_color.getBlue ();
  }
  else {
    red = s_desel_color.getRed ();
    green = s_desel_color.getGreen ();
    blue = s_desel_color.getBlue ();
  }
  view.drawSquare ( x1, y1, x2, y2, red, green, blue );
}

void
CutRangeRep::
drawInvertedRange1D ( const Range & x_range, const Range & y_range,
		      const TransformBase & tb,
		      ViewBase & view )
{
  double xl = x_range.low ();
  double xr = x_range.high ();

  const Range range = m_cut_list[0].getRange ();
  double cut_low = range.low();
  double cut_high = range.high ();

  double x1 = std::max ( cut_low, xl );
  double x2 = std::min ( cut_high, xr );

  double y1 = y_range.low ();
  double y2 = y_range.high ();

  double yl = y1;
  double yr = y2;
  const BinaryTransform & t 
    = dynamic_cast < const BinaryTransform & > ( tb );
  
  t.transform ( x1, y1 ); // careful, they get modified
  t.transform ( x2, y2 );

  t.transform ( xl, yl );
  t.transform ( xr, yr );

  int red;
  int green;
  int blue;
  
  if ( m_desel == false ) {
    red   = m_color.getRed ();
    green = m_color.getGreen ();
    blue  = m_color.getBlue ();
  }
  else {
    red = s_desel_color.getRed ();
    green = s_desel_color.getGreen ();
    blue = s_desel_color.getBlue ();
  }
  
  view.drawSquare ( xl, y1, x1, y2, red, green, blue );  
  view.drawSquare ( x2, y1, xr, yr, red, green, blue );  
}

void
CutRangeRep::
drawInvertedRange2D ( const Range & x_range, const Range & y_range,
		      const TransformBase & tb,
		      ViewBase & view )
{
  const BinaryTransform & t 
    = dynamic_cast < const BinaryTransform & > ( tb );
  
  // loeer left hand corner of plot
  double xLow  = x_range.low ();
  double yLow  = y_range.low ();

  // upper right hand corner of plot
  double xHigh = x_range.high ();
  double yHigh = y_range.high ();

  // will be empty box in center
  double x1 = xLow;
  double x2 = xHigh;
  double y1 = yLow;
  double y2 = yHigh;
    
  if ( m_cut_list.size () >= 1 ) // i.e. this section applies to 1D and 2D cuts
    {
      const Range & range = m_cut_list[0].getRange ();
      double cut_low = range.low();
      double cut_high = range.high ();
      if ( cut_low > x1 ) x1 = cut_low;
      if ( cut_high < x2 ) x2 = cut_high;
    }
  
  if ( m_cut_list.size() >= 2 ) // i.e. applies only to 2D cuts
    {
      const Range & range = m_cut_list[1].getRange ();
      double cut_low = range.low();
      double cut_high = range.high();
      if ( cut_low > y1 ) y1 = cut_low;
      if ( cut_high < y2 ) y2 = cut_high;
    }
  
  t.transform ( x1, y1 );
  t.transform ( x2, y2 );

  t.transform ( xLow, yLow );
  t.transform ( xHigh, yHigh );
  
  int red;
  int green;
  int blue;
  
  if ( m_desel == false ) {
    red   = m_color.getRed ();
    green = m_color.getGreen ();
    blue  = m_color.getBlue ();
  }
  else {
    red = s_desel_color.getRed ();
    green = s_desel_color.getGreen ();
    blue = s_desel_color.getBlue ();
  }
  
  
  // left box
  view.drawSquare( xLow, yLow,    x1, yHigh, red, green, blue );

  // right box
  view.drawSquare(   x2, yLow, xHigh, yHigh, red, green, blue );

  // upper midle
  view.drawSquare(   x1,   y2,    x2, yHigh, red, green, blue );

  // lower midle
  view.drawSquare(   x1, yLow,    x2,    y1, red, green, blue );

}

const vector < TupleCut > &
CutRangeRep::
getCuts () const
{
  return m_cut_list;
}

void
CutRangeRep::
addCut ( const TupleCut & cut )
{
  m_cut_list.push_back ( cut );
}

void
CutRangeRep::
setCutRangeAt ( const Range & range, unsigned int i )
{
  if ( i >= m_cut_list.size () ) throwRangeException ();

  m_cut_list[i].setRange ( range );
}

void
CutRangeRep::
toggleInverted ( unsigned int i )
{
  if ( i >= m_cut_list.size () ) throwRangeException ();

  m_cut_list[i].toggleInverted ();
}

void
CutRangeRep::
setEnabled ( unsigned int i, bool yes )
{
  if ( i >= m_cut_list.size () ) throwRangeException ();

  m_cut_list[i].setEnabled ( yes );
}

void
CutRangeRep::
throwRangeException ()
{
  string what ( "CutRangeRep::setCutRange: index out of range." );
  throw std::out_of_range ( what );
}
