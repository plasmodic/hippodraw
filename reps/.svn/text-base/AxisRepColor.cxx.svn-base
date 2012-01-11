/** @file

hippodraw::AxisRepColor class implementation

Copyright (C) 2000-2007  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: AxisRepColor.cxx,v 1.107 2007/08/20 17:31:15 pfkeb Exp $

*/

#ifdef _MSC_VER
// for min()
#include "msdevstudio/MSconfig.h"
#endif

#include "AxisRepColor.h"


#include "axes/AxisModelBase.h"
#include "axes/AxisTick.h"
#include "colorreps/BinToColor.h"
#include "graphics/DataView.h"
#include "pattern/string_convert.h"
#include "transforms/BinaryTransform.h"
#include "plotters/PlotterBase.h"


#include <algorithm>
#include <functional>

#include <cmath>

#include <cassert>

#include <iostream>

using std::min;
using std::string;
using std::vector;

using namespace hippodraw;

AxisRepColor::AxisRepColor ()
  : AxisRepBase (),
    m_axis_z_origin( 0.0 ),
    m_axis_depth( 0.0 )
{
}

AxisRepColor::AxisRepColor( const AxisRepColor & axis_rep )
  : AxisRepBase( axis_rep ),
    m_axis_z_origin( axis_rep.m_axis_z_origin ),
    m_axis_depth( axis_rep.m_axis_depth )
{
}

AxisRepBase * AxisRepColor::clone()
{
  return new AxisRepColor( *this );
}


inline float XPADDING( ViewBase & view )
{
  return ( view.getDrawRect().getWidth() * 0.01 );
}

inline float YPADDING( ViewBase & view )
{
  return ( view.getDrawRect().getHeight() * 0.01 );
}

inline float ZPADDING( ViewBase & view )
{
  return ( view.getDrawRect().getWidth() * 0.01 );
}

/** @request first argument is not used, why?
 */
void
AxisRepColor::
drawZLabels ( const AxisModelBase &, // axisModel,
	      ViewBase & base, 
	      const std::string & z_label )
{

  DataView & view = dynamic_cast < DataView & > ( base );

  // Draw Latex at the top of the plotter if in Latex format
  if (String::ci_find(z_label, "tex:")==0) {
    string tex_snippet = z_label.substr(4);
    view.drawLatex ( tex_snippet, 4 );
    return;
  }

  Rect draw_rect = view.getDrawRect ();
  float dh = draw_rect.getHeight ();

  const Rect & margin_rect = view.getMarginRect ();
  float mx = margin_rect.getX ();
  float mw = margin_rect.getWidth ();
  float my = margin_rect.getY ();
  float mh = margin_rect.getHeight ();

  float y_font_size = ( dh - mh ) / 2 * 0.20;
  float x_font_size = ( mw * 2 ) / z_label.size() * 2 / 3;

  m_z_font_size = min ( x_font_size, y_font_size );

  float x = mx + 0.5 * mw;
  float y = my - 37;

  if (m_titleFont != 0) {
    y = y + 1.2 * m_titleFont->pointSize() - 11.0; //experimental
  }

  if ( m_zLabelFont != 0 ) {
    y = y + 1.2 * m_zLabelFont->pointSize() - 8.0; // experimental
    view.drawText ( z_label, x, y, 0.0, 0.0, 'c', 'b',
		    false, m_zLabelFont );
  } else {
    view.drawText ( z_label, x, y, m_z_font_size, 0.0, 'c', 'b', false );
  }
}

void AxisRepColor::drawAllZTicks ( const AxisModelBase & axisModel,
				   const TransformBase & transform,
				   ViewBase & view )
{
  drawZTickLines ( axisModel, transform, view );
  drawZTickLabels ( axisModel, transform, view );
}

void AxisRepColor::drawZTickLines ( const AxisModelBase & axisModel,
				    const TransformBase & transform,
				    ViewBase & base )
{
  AxisLoc loc = axisModel.getLabelLocation ();
  assert ( loc == PLOTBOTTOM || PLOTTOP );

  const vector< AxisTick > & ticks = axisModel.getTicks ();
  if ( ticks.empty() ) return;  

  vector< double > xv;
  vector< double > yv;
  
  unsigned int size = 2 * ticks.size ();
  xv.reserve ( size );
  yv.reserve ( size );

  const BinaryTransform & t 
    = dynamic_cast< const BinaryTransform & > ( transform );
  DataView & view = dynamic_cast < DataView & > ( base );
  const Rect & view_rect = view.getMarginRect ();
  double tick_length = 5;
  double y_base = view_rect.getY() - 24;

  for ( unsigned int i = 0; i < ticks.size(); i++ ) {
    double user_z = ticks[i].value ();
    t.transformZ ( user_z );
    // Reinstate the following line when XYZ transform exists.
    //t.transform ( user_z, y ); // Some valid value on Y.
  
    double view_x = view.userToDrawColor ( user_z );

    xv.push_back ( view_x );
    yv.push_back ( y_base );
    xv.push_back ( view_x );
    yv.push_back ( y_base + tick_length );
  
  }

  view.drawViewLines ( xv, yv, Line::Solid, false, 0 );  

}

void AxisRepColor::drawZTickLabels ( const AxisModelBase & axisModel,
				     const TransformBase & transform, 
				     ViewBase & base )
{
  vector < double > xv;  
  vector < double > yv;  
  
  const vector< AxisTick > & ticks = axisModel.getTicks ();
  unsigned int size = ticks.size ();
  if ( size == 0 ) return;

  xv.reserve ( size );
  yv.reserve ( size );

  const BinaryTransform & t 
    = dynamic_cast< const BinaryTransform & > ( transform );

  unsigned int i = 0; // For MS VC++.
  for ( ; i < size; i++ ) {
    double value = ticks[i].value ();
    t.transformZ ( value );
    xv.push_back ( value );
    yv.push_back ( 1.0 ); // Use a valid Y value.
  }


  // Calculate the Y position in view coordinates.
  DataView & view = dynamic_cast < DataView & > ( base );  
  const Rect & margin = view.getMarginRect ();
  float y = margin.getY() - 42;

  i = 0;  // For MS VC++.
  for ( ; i < size; i++ ) {
    float x = view.userToDrawColor ( xv[i] );
    drawXTickLabel ( ticks[i].content (), x, y, view );
  }	  
  
  if ( axisModel.needPMag () ) {
    // Logarithmic graphs do not need the magnitude written.
    if ( !(axisModel.isLog() ) ) {
      const Rect & margin_rect = view.getMarginRect ();
      float xx = margin_rect.getX() 
	+ margin_rect.getWidth ()
	- m_y_font_size;
      float yy = margin_rect.getY() - 30. - m_y_tick_font_size;
      view.drawText ( "x10",  xx, yy, m_y_tick_font_size, 0., 'l', 'b' );
      double mag = axisModel.getPMag ();
      int i = static_cast < int > ( mag );
      const string text = String::convert ( i );
      xx += 1.75* m_y_tick_font_size;
      yy -= 0.5 * m_y_tick_font_size;
      view.drawText ( text, xx, yy, m_y_tick_font_size, 0., 'l', 'b' );
    }
  }

}

void
AxisRepColor::
drawColorScale ( const BinToColor & bin_to_color, ViewBase & base )
{
  DataView & view = dynamic_cast < DataView & > ( base );
  PlotterBase * plotter = view.getPlotter ();

  const Rect & margin = view.getMarginRect(); 

  double y_base = margin.getY() - 22;

  const Range range = bin_to_color.getRange ();
  double value = range.low ();
  double delta_v = range.length () / margin.getWidth ();
  if (delta_v == 0) {
     return;
  }
  for ( float i = 0; i <= margin.getWidth() ; i++ ) {
    
    const Color & rep_color = plotter->repColor();
	
	Color color = rep_color;

    bin_to_color.doubleToColor ( value, color );
    view.drawViewSquare ( margin.getX() + i,
			  y_base,
			  margin.getX() + i + 1.0,
			  y_base + 6,
			  color.getRed(),
			  color.getGreen(),
			  color.getBlue () );
    value  += delta_v;
  }
  
  view.drawViewSquare ( margin.getX(),
			y_base,
			margin.getX(),
			y_base + 8, 
			0, 0, 0 );
}
