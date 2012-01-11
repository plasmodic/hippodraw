/** @file

hippodraw::AxisRepBase class implementation

Copyright (C) 2000-2002, 2004-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: AxisRepBase.cxx,v 1.100 2007/07/02 18:17:16 pfkeb Exp $

*/

// for tex
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// for truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#define finite _finite
#endif

#include "AxisRepBase.h"

#include "axes/AxisModelBase.h"
#include "graphics/Color.h"
#include "graphics/DataView.h"
#include "pattern/string_convert.h"
#include "transforms/PeriodicBinaryTransform.h"

#include <cmath>
#include <cassert>
#include <cstdio>

using std::max;
using std::min;
using std::string;
using std::vector;
#ifdef __USE_ISOC99
using std::isfinite;
#endif

using namespace hippodraw;

AxisRepBase::AxisRepBase()
  : m_sci_note_x(false), 
    m_sci_note_y(false),
    m_axis_x_origin (0.),
    m_axis_y_origin ( 0.0),
    m_axis_width ( 0.0 ),
    m_axis_height ( 0.0 ),
    m_font_size( 1.0 ), 
    m_x_tick_font_size ( 1.0 ),
    m_y_tick_font_size ( 1.0 ),
    m_x_font_size( 1.0 ), 
    m_y_font_size( 1.0 ),
    m_xLabelFont( 0 ),
    m_yLabelFont( 0 ),
    m_zLabelFont( 0 ),
    m_titleFont( 0 ),
    m_draw_titles( true )
{
}

AxisRepBase::AxisRepBase( const AxisRepBase & axis_rep )
  : m_sci_note_x(axis_rep.m_sci_note_x),
    m_sci_note_y(axis_rep.m_sci_note_y),
    m_axis_x_origin( axis_rep.m_axis_x_origin ),
    m_axis_y_origin( axis_rep.m_axis_y_origin ),
    m_axis_width( axis_rep.m_axis_width ),
    m_axis_height( axis_rep.m_axis_height ),
    m_font_size( axis_rep.m_font_size ),
    m_x_font_size( axis_rep.m_x_font_size ),
    m_y_font_size( axis_rep.m_y_font_size ),
    m_xLabelFont( axis_rep.m_xLabelFont ),
    m_yLabelFont( axis_rep.m_yLabelFont ),
    m_zLabelFont( axis_rep.m_zLabelFont ),
    m_titleFont( axis_rep.m_titleFont ),
    m_draw_titles( axis_rep.m_draw_titles )
{
}

AxisRepBase::~AxisRepBase()
{
  delete m_xLabelFont;
  delete m_yLabelFont;
  delete m_zLabelFont;
  delete m_titleFont;
}

void
AxisRepBase::
initAxisRect( ViewBase & view )
{
  const Rect & rect = view.getUserRect();

  m_axis_x_origin = rect.getX();
  m_axis_y_origin = rect.getY();
  m_axis_width = rect.getWidth();
  m_axis_height = rect.getHeight();
}

void
AxisRepBase::
beginPlot( ViewBase & view )
{
  initAxisRect( view );
}

void AxisRepBase::setDrawTitles( bool set )
{
  m_draw_titles = set;
}

void
AxisRepBase::
drawTitle ( ViewBase & base, const std::string & title )
{

  DataView & view = dynamic_cast < DataView & > ( base );

  // Draw Latex at the top of the plotter if in Latex format
#ifdef HAVE_TEX_UTILS
  if (String::ci_find(title, "tex:")==0) {
    string tex_snippet = title.substr(4);
    view.drawLatex ( tex_snippet, 1 );
  }

  else {
#endif
    const Rect & marginRect = view.getMarginRect();
    float mx = marginRect.getX();
    Rect rect = view.getDrawRect ();
    float mw = rect.getWidth();
    
    double x_font_size = ( 1.3 * mw ) / title.size();
    m_font_size = min(x_font_size,12.0);
    
    double one = 1.0;
    m_font_size = max ( m_font_size, one );
    
    float x = mx + 0.5 * marginRect.getWidth ();
    float y = 2.0;
    
    if ( m_titleFont != 0 ) {
      view.drawText ( title, x, y, 0.0, 0.0, 'c', 't', false,
		      m_titleFont );
    } else {
      view.drawText ( title, x, y, m_font_size, 0.0, 'c', 't', false );
    }

#ifdef HAVE_TEX_UTILS
  }
#endif
}

void
AxisRepBase::
setXFontSize ( const AxisModelBase & axisModel,
	       ViewBase & view )
{
  Rect draw_rect = view.getDrawRect();
  m_x_tick_font_size = min( ( draw_rect.getWidth() ) * 0.040, 12.0 );
  double one = 1.0;
  m_x_tick_font_size = max ( m_x_tick_font_size, one );

  AxisLoc location = axisModel.getScaleLocation();
  
  if ( location == PLOTBOTTOM )
    {
      m_x_font_size = min( (double)m_x_tick_font_size,
			   ( m_axis_y_origin -
			     draw_rect.getY() ) / 2.0 );
    }
  else if ( location == PLOTTOP )
    {
      m_x_font_size = min( (double)m_x_tick_font_size,
			   ( draw_rect.getHeight() -
			     m_axis_height -
			     ( m_axis_y_origin
			       - draw_rect.getY() ) ) /2.0 );
    }
}

void
AxisRepBase::
setYFontSize ( const AxisModelBase & axisModel,
	       ViewBase & view )
{
  assert ( m_y_tick_font_size > 0 );

  Rect draw_rect = view.getDrawRect();
  double height = draw_rect.getHeight ();
  double size = height * 0.040;
  size = std::min ( size, 11.0 );
  double one = 1.0;
  m_y_tick_font_size = std::max ( size, one );
  
  assert ( m_y_tick_font_size > 0 );
  const vector<AxisTick> & labels = axisModel.getTicks ( );
  if ( labels.empty () == true ) return;

  if ( axisModel.getScaleLocation() & PLOTLEFT )
    {
      float yr;
      
      if ( m_draw_titles && 
	   axisModel.getLabelLocation() & PLOTLEFT ) {
	yr = 14;
      } 
      else {
	yr = 0;
      }
      const AxisTick & tick = labels.back ( );
      m_y_font_size = min ( m_y_tick_font_size, 
			   ( m_axis_x_origin -
			     draw_rect.getX() - yr )
			   / tick.content().size() );
    }
  else if ( axisModel.getScaleLocation() & PLOTRIGHT )
    {
      // Estimates the space needed for the Y axis label.
      float yr;
      if ( m_draw_titles && 
	   axisModel.getLabelLocation() & PLOTRIGHT ) {
	yr = 14;
      } else {
	yr = 0;
      }

      m_y_font_size = min( m_y_tick_font_size, 
      		   ( draw_rect.getWidth() - 
      		     m_axis_width - 
      		     ( m_axis_x_origin - 
      		       draw_rect.getX() ) - yr )
      		   / labels[labels.size() - 1].content().size() );
    }
}

void
AxisRepBase::
setZFontSize ( const AxisModelBase & axisModel,
	       ViewBase & view )
{
  AxisLoc location = axisModel.getScaleLocation();
  Rect draw_rect = view.getDrawRect();
  if ( location == PLOTBOTTOM )
    {
      m_z_font_size = min( (double)m_font_size,
			   ( m_axis_y_origin -
			     draw_rect.getY() ) / 2.0 );
    }
  else if ( location == PLOTTOP )
    {
      m_z_font_size = min( (double)m_font_size,
			   ( draw_rect.getHeight() -
			     m_axis_height -
			     ( m_axis_y_origin
			       - draw_rect.getY() ) ) /2.0 );
    }
}

void
AxisRepBase::
drawXLabels ( const AxisModelBase & axisModel,
	      ViewBase & base, const std::string & x_label )
{
  DataView & view = dynamic_cast < DataView & > ( base );

  // Calculate the ratio of range(rmag) and value(pmag) to decide
  // where scientific notation is needed.
  const vector< AxisTick > & ticks = axisModel.getTicks();
  m_sci_note_x=false;
  int mid = ticks.size()/2;
  double ref = ticks[mid].value();
  double range = ticks[mid+1].value()-ref;
  if (fabs(ref/range)>1e6) m_sci_note_x=true;
  
  // Draw Latex at the bottom of the plotter if in Latex format
#ifdef HAVE_TEX_UTILS
  if (String::ci_find(x_label, "tex:")==0) {
    string tex_snippet = x_label.substr(4);
    if (m_sci_note_x) {
      tex_snippet+="-"+ticks[mid].content();
      if (axisModel.needPMag()) {
	// Use LaTeX to generate: label-mid*10^Pmag
	tex_snippet+="\\times 10^"+String::convert ( static_cast <int> (axisModel.getPMag()) );
      }
    }
    view.drawLatex ( tex_snippet, 2 );
  }

  else {    
#endif
    std::string label;
    if (m_sci_note_x) {
      label = x_label+" - "+ticks[mid].content();
    }
    else {
      label = x_label;
    }
    
    float x = 0., y = 0.;
    Rect draw_rect = view.getDrawRect ();
    float draw_h = draw_rect.getHeight ();
    float draw_w = draw_rect.getWidth ();
    
    const Rect & margin_rect = view.getMarginRect ();
    float margin_x = margin_rect.getX ();
    float margin_w = margin_rect.getWidth ();
    
    x = margin_x + 0.5 * margin_w;
    
    float tmp = 0.045 * draw_w;
    tmp = max ( tmp, 1.0f );
    m_x_font_size = min ( tmp, static_cast<float>( 18.0 ) );
    assert ( m_x_font_size > 0 );    
    
    double ratio = draw_w / ( label.size() ) * 2;
    m_x_font_size = std::min ( m_x_font_size, ratio );
    assert ( m_x_font_size > 0 );    
    double one = 2.0;
    m_x_font_size = std::max ( m_x_font_size, one );

    assert ( m_x_font_size > 0 );    
    if ( axisModel.getLabelLocation() & PLOTBOTTOM )
      {
	if ( m_xLabelFont != 0 ) {
	  y = draw_h - 1.5*m_xLabelFont->pointSize()-6.0; // experimental
	  view.drawText ( label, x, y, 0., 0.0, 'c', 't',
			  false, m_xLabelFont );
	} else {
	  y = draw_h - m_x_font_size - 6.0; // take into account descenders
	  view.drawText ( label, x, y, m_x_font_size, 0.0, 'c', 't', false );
	}
      }
    else if ( axisModel.getLabelLocation() & PLOTTOP )
      {
	y = 1.0;
	if ( m_xLabelFont != 0 ) {
	  
	  view.drawText ( label, x, y, 0.0, 0.0, 'c', 't',
			  false, m_xLabelFont );
	} else {
	  
	  view.drawText ( label, x, y, m_x_font_size, 0.0, 'c', 't', false );
	}
      }
    
    
    // Draw the mag when both pmag and small range sci_note are needed.
    // TODO: handle (location & PLOTTOP)
    if ( (m_sci_note_x) && (axisModel.needPMag() ) ){
      if ( m_xLabelFont == NULL ) {
	x = x+0.27 * m_x_font_size * label.size();
	y = y+0.2 * m_x_font_size;
	view.drawText ( " x10", x, y, m_x_font_size*0.8, 0.0, 'c', 't', false );
	
	double pmag = axisModel.getPMag();
	int i = static_cast < int > ( pmag );
	const string text = String::convert ( i );
	x += 1.0 * m_x_font_size ;
	y -= 0.4 * m_x_font_size;  
	view.drawText ( text, x, y, m_x_font_size*0.8, 0.0, 'c', 't', false );
      } else {
	x = x+0.4 * m_xLabelFont->pointSize() * label.size();
	view.drawText ( " x10", x, y, 0.0, 0.0, 'c', 't', false, m_xLabelFont );
	
	double pmag = axisModel.getPMag();
	int i = static_cast < int > ( pmag );
	const string text = String::convert ( i );
	x += 1.6 * m_xLabelFont->pointSize() ;
	y -= 0.4 * m_xLabelFont->pointSize();
	view.drawText ( text, x, y, 0.0, 0.0, 'c', 't', false, m_xLabelFont );
      }
    }
#ifdef HAVE_TEX_UTILS
  }
#endif
  
}
void
AxisRepBase::
drawYLabels ( const AxisModelBase & axisModel,
	      ViewBase & base, const std::string & y_label )
{
  DataView & view = dynamic_cast < DataView & > ( base );

  // Calculate the ratio of range(rmag) and value(pmag) to decide
  // where scientific notation is needed.
  const vector< AxisTick > & ticks = axisModel.getTicks();
  m_sci_note_y=false;
  int mid = ticks.size()/2;
  double ref = ticks[mid].value();
  double range = ticks[mid+1].value()-ref;
  if (fabs(ref/range)>1e6) m_sci_note_y=true;
  
  
#ifdef HAVE_TEX_UTILS
  // Draw Latex at the left of the plotter if in Latex format
  if (String::ci_find(y_label, "tex:")==0) {
    string tex_snippet = y_label.substr(4);
   
    if (m_sci_note_y) {
      tex_snippet+="-"+ticks[mid].content();
      if (axisModel.needPMag()) {
	// Use LaTeX to generate: label-mid*10^Pmag
	tex_snippet+="\\times 10^"+String::convert ( static_cast <int> (axisModel.getPMag()) );
      }
    }


    view.drawLatex ( tex_snippet, 3 );
  }

  else {
#endif

    
    std::string label;
    if (m_sci_note_y) {
      label = y_label+" - "+ticks[mid].content();
    }
    else {
      label = y_label;
    }
    
    Rect draw_rect = view.getDrawRect ();

    float x = 0., y = 0.;
    AxisLoc location = axisModel.getLabelLocation();
    
    //float tmp = min ( view.getDrawRect().getWidth() * 0.05,
    //	    view.getDrawRect().getHeight() * 0.05 );
    float tmp = draw_rect.getHeight() * 0.05;
    m_y_font_size = min( tmp, static_cast<float>( 18.0 ) );
    m_y_font_size = min( m_y_font_size, draw_rect.getHeight()
			 / ( label.size() ) * 2 );
    double one = 1.0;
    m_y_font_size = max ( m_y_font_size, one );
    y = view.getMarginRect().getY() + 
      view.getMarginRect().getHeight() *0.5;
    
    if ( location & PLOTLEFT )
      {
	x = 2.0;
	if ( m_yLabelFont != 0 ) {
	  view.drawText ( label, x, y, 0.0, 90.0, 'c', 't',
			  false, m_yLabelFont );
	} else {
	  view.drawText ( label, x, y, m_y_font_size, 90.0, 'c', 't', false );
	}
      }
    else if ( location & PLOTRIGHT )
      {
	x = draw_rect.getWidth() - 2.0;
	if ( m_yLabelFont != 0 ) {
	  view.drawText ( label, x, y, 0.0, -90.0, 'c', 't',
			  false, m_yLabelFont );
	} else {
	  view.drawText ( label, x, y, m_y_font_size, -90.0, 'c', 't', false );
	}
      }
    
    
    // Draw the mag when both pmag and small range sci_note are needed.
    // TODO: handle (location & PLOTTOP)
    if ( (m_sci_note_y) && (axisModel.needPMag() ) ){
      if ( m_yLabelFont == NULL ) {
	y = y-0.27 * m_y_font_size * label.size();
	x = x+0.2 * m_y_font_size;
	view.drawText ( " x10", x, y, m_y_font_size*0.8, 90.0, 'c', 't', false );
	
	double pmag = axisModel.getPMag();
	int i = static_cast < int > ( pmag );
	const string text = String::convert ( i );
	x -= 0.4 * m_y_font_size ;
	y -= 1.0 * m_y_font_size;  
	view.drawText ( text, x, y, m_y_font_size*0.8, 90.0, 'c', 't', false );
      } else {
	y = y-0.4 * m_yLabelFont->pointSize() * label.size();
	view.drawText ( " x10", x, y, 0.0, 90.0, 'c', 't', false, m_yLabelFont );
	
	double pmag = axisModel.getPMag();
	int i = static_cast < int > ( pmag );
	const string text = String::convert ( i );
	x -= 0.4 * m_yLabelFont->pointSize() ;
	y -= 1.6 * m_yLabelFont->pointSize();
	view.drawText ( text, x, y, 0.0, 90.0, 'c', 't', false, m_yLabelFont );
      }
    }
#ifdef HAVE_TEX_UTILS
  }
#endif
}

void
AxisRepBase::
drawReferencePoint( const AxisModelBase & axisModel, 
		    ViewBase & base,
		    const std::string & ref)
{
  if ( axisModel.getScaleLocation() & PLOTBOTTOM )
    {
      DataView & view = dynamic_cast < DataView & > ( base );
      
      const Rect & margin_rect = view.getMarginRect ();
      float x = margin_rect.getX() + margin_rect.getWidth ();
      
      x -= 1 * m_x_tick_font_size * ref.size();
      
      Rect view_rect = view.getDrawRect ();
      float y = + view_rect.getHeight ();
      
      view.drawText ( "X Ref:"+ref, x, y, m_x_tick_font_size, 0., 'l', 'b' );
    }
  else if (axisModel.getScaleLocation() & PLOTLEFT)
    {
      DataView & view = dynamic_cast < DataView & > ( base );

      float x = 2.;
      x += m_y_tick_font_size;
      
      const Rect & margin_rect = view.getMarginRect ();
      float y = margin_rect.getY() + 0.2 * m_y_tick_font_size;
      
      view.drawText ( "Y Ref:"+ref, x, y, m_y_tick_font_size, 0., 'l', 'b' );
    }
      
}


void
AxisRepBase::
drawXMag ( const AxisModelBase & axisModel,
	   ViewBase & base , const std::string & mag )
{
  DataView & view = dynamic_cast < DataView & > ( base );

  const Rect & margin_rect = view.getMarginRect ();
  float x = margin_rect.getX() + margin_rect.getWidth ();

  double pmag = axisModel.getPMag();
  int i = static_cast<int> (pmag);
  const string text = String::convert ( i );
  x -= 0.8 * m_x_tick_font_size * text.size();

  Rect view_rect = view.getDrawRect ();
  float y = + view_rect.getHeight ();

  view.drawText ( "x10", x, y, 0.75 * m_x_tick_font_size, 0., 'l', 'b' );

  x += 1.25 * m_x_tick_font_size ;
  y -= 0.5 * m_x_tick_font_size;

  if (m_sci_note_x) {
    view.drawText( mag, x, y, 0.75 * m_x_tick_font_size, 0., 'l', 'b' );
  } else {
    view.drawText ( text, x, y, 0.75 * m_x_tick_font_size, 0., 'l', 'b' );
  }
}

void
AxisRepBase::
drawYMag ( const AxisModelBase & axisModel,
	   ViewBase & base, const std::string & mag )
{
  DataView & view = dynamic_cast < DataView & > ( base );

  float x = 2.;
  if ( axisModel.getScaleLocation() & PLOTLEFT ) {
    x += m_y_tick_font_size;
  }
  else {
    x -= m_y_tick_font_size;
  }

  const Rect & margin_rect = view.getMarginRect ();
  float y = margin_rect.getY() + 0.5 * m_y_tick_font_size;

  view.drawText ( "x10", x, y, m_y_tick_font_size, 0., 'l', 'b' );

  double pmag = axisModel.getPMag();
  int i = static_cast < int > ( pmag );
  const string text = String::convert ( i );
  x += 1.75 * m_y_tick_font_size ;
  y -= 0.5 * m_y_tick_font_size;

  if (m_sci_note_y) {
    view.drawText ( mag, x, y, m_y_tick_font_size, 0., 'l', 'b' );
  } else {
    view.drawText ( text, x, y, m_y_tick_font_size, 0., 'l', 'b' );
  }

}

void
AxisRepBase::
drawAxesLines ( TransformBase & transform, 
		ViewBase & view, 
		const Range & x_range,
		const Range & y_range)
{
  const BinaryTransform & t
    = dynamic_cast< const BinaryTransform & > ( transform );
  vector< double > xv ( 100 );
  vector< double > yv ( 100 );
  Color black ( 0, 0, 0 );

  xv[0] = x_range.low( );
  yv[0] = y_range.low( );
  for( int i = 1; i < 100; i++)
    {
      xv[i] = xv[i-1] + ( x_range.high() - x_range.low() )/99; 
      yv[i] = yv[i-1];
    }
  t.transform( xv, yv );
  view.drawPolyLine ( xv, yv, Line::Solid, black, 1 );

  
  xv[0] = x_range.high( );
  yv[0] = y_range.low( );
  for( int i = 1; i < 100; i++)
    {
      xv[i] = xv[i-1]; 
      yv[i] = yv[i-1] + ( y_range.high() - y_range.low() )/99; ;
    }
  t.transform( xv, yv );
  view.drawPolyLine ( xv, yv, Line::Solid, black, 1 );


  xv[0] = x_range.low( );
  yv[0] = y_range.high( );
  for( int i = 1; i < 100; i++)
    {
      xv[i] = xv[i-1] + ( x_range.high() - x_range.low() )/99;  
      yv[i] = yv[i-1];
    }
  
  t.transform( xv, yv );
  view.drawPolyLine ( xv, yv, Line::Solid, black, 1 );


  xv[0] = x_range.low( );
  yv[0] = y_range.low( );
  for( int i = 1; i < 100; i++)
    {
      xv[i] = xv[i-1];
      yv[i] = yv[i-1]  + ( y_range.high() - y_range.low() )/99;  
    }
  
  t.transform( xv, yv );
  view.drawPolyLine ( xv, yv, Line::Solid, black, 1 );

  
}



void AxisRepBase::drawGridLines( const AxisModelBase & axisModelX,
				 const AxisModelBase & axisModelY,
				 TransformBase & transform,
				 ViewBase & view )
{
  Range x_range = axisModelX.getRange( false );
  Range y_range = axisModelY.getRange( false );
  
    
  const vector< AxisTick > & x_ticks = axisModelX.getTicks();
  const vector< AxisTick > & y_ticks = axisModelY.getTicks();
  
  unsigned int nxpoints = 100;
  unsigned int nypoints = 100;
  vector< double > xv(100), yv(100);
  
  double user_x, user_y;
  Color grey( 180, 180, 180 );
    
  BinaryTransform & t
    = dynamic_cast< BinaryTransform & > ( transform );

  t.validate( x_range, y_range );

  if ( x_ticks.empty () == true || y_ticks.empty () == true ) 
    return;

  
  double dx = ( x_range.high() - x_range.low() ) / ( nxpoints - 1 );
  double dy = ( y_range.high() - y_range.low() ) / ( nypoints - 1 ) ;
  
    
  // Drawing the   coordinates of the aligned along y-Axis 
  // i.e.curves for which  x = const
  // Don't draw first tick if it will cover the left edge.
  for ( unsigned int i = ( x_range.low() == x_ticks[0].value() )?1:0; 
	i < x_ticks.size(); i++ )
    {
      user_x = x_ticks[ i ].value();
      
      for ( unsigned int j = 0; j < nypoints; j++ )
	{
	  user_y = y_range.low() + j * dy;
		  
	  xv[ j ] = user_x;
	  yv[ j ] = user_y;
	  
	}
      
      t.transform( xv, yv );
      view.drawPolyLine ( xv, yv, Line::Dot, grey, 0 );
      
    }
  
  
  // Drawing the   coordinates of the aligned along x-Axis 
  // i.e.curves for which  y = const
  // Don't draw first tick if it will cover the bottom edge.
  for ( unsigned int j = ( y_range.low() == y_ticks[0].value() )?1:0; 
	j < y_ticks.size(); j++ )
    {
      user_y = y_ticks[ j ].value();
      
      for ( unsigned int i = 0; i < nxpoints; i++ )
	{
	  user_x = x_range.low() + i * dx;
	
	  xv[ i ] = user_x ;
	  yv[ i ] = user_y ;
	}

      t.transform( xv, yv ); 
      view.drawPolyLine( xv, yv, Line::Dot, grey, 0 );
    }
   
   return;
}

void
AxisRepBase::
drawXTickLines( const AxisModelBase & axisModelX,
		const AxisModelBase & axisModelY,
		const TransformBase & transform,
		ViewBase & base )
{
  DataView & view = dynamic_cast < DataView & > ( base );
  AxisLoc loc = axisModelX.getLabelLocation();
  assert( loc == PLOTBOTTOM || PLOTTOP );
  
  const vector< AxisTick > & ticks = axisModelX.getTicks();
  if ( ticks.empty() == true ) return;
  
  vector< double > xv;
  vector< double > yv;

  unsigned int size = 4 * ticks.size();
  xv.reserve( size );
  yv.reserve( size );

  const BinaryTransform & t
    = dynamic_cast< const BinaryTransform & > ( transform );

  const Rect & view_rect = view.getMarginRect();
  double tick_length = 0.05 * view_rect.getHeight();
  tick_length = min ( tick_length, 8. );  
  Range yrange = axisModelY.getRange( false );
  
  for ( unsigned int i = 0; i < ticks.size(); i++ )
    {
      double user_x_start = ticks[i].value ();
      double user_x_temp = user_x_start;

      double user_by_start = yrange.low();
      double user_ty_start = yrange.high();
      
      // transform will change the x value, so we need the temp
      t.transform ( user_x_start, user_by_start );
      t.transform ( user_x_temp, user_ty_start );
      
      double view_x_start = view.userToDrawXAutoInv ( user_x_start );
      double view_by_start = view.userToDrawY ( user_by_start );
      double view_ty_start = view.userToDrawY ( user_ty_start );
      
      double view_x_end = view_x_start;
      double view_by_end = view_by_start - tick_length;
      double view_ty_end = view_ty_start + tick_length;

#ifdef __USE_ISOC99
      if( isfinite( view_x_start ) &&
	  isfinite( view_by_start ) &&
	  isfinite( view_ty_start ) &&
	  isfinite( view_x_start ) &&
	  isfinite( view_by_end   ) )
#else
      if( finite( view_x_start ) &&
	  finite( view_by_start ) &&
	  finite( view_ty_start ) &&
	  finite( view_x_start ) &&
	  finite( view_by_end   ) )
#endif
	{
	  xv.push_back( view_x_start );
	  yv.push_back( view_by_start );
	  xv.push_back( view_x_end );
	  yv.push_back( view_by_end );

	  xv.push_back( view_x_start );
	  yv.push_back( view_ty_start );
	  xv.push_back( view_x_end );
	  yv.push_back( view_ty_end );
	}
    }
  
  view.drawViewLines ( xv, yv, Line::Solid, false, 1 );
}

void 
AxisRepBase::
drawYTickLines ( const AxisModelBase & axisModelX,
		 const AxisModelBase & axisModelY,
		 const TransformBase & transform,
		 ViewBase & base )
{
  AxisLoc loc = axisModelY.getLabelLocation ();
  assert ( loc == PLOTLEFT || loc == PLOTRIGHT );

  DataView & view = dynamic_cast < DataView & > ( base );
  const Rect & draw_rect = view.getMarginRect ();

  double tick_length = 0.05 * draw_rect.getWidth ();
  tick_length = min ( tick_length, 8. );

  vector< double > xv;
  vector< double > yv; 

  const vector< AxisTick > & ticks = axisModelY.getTicks ();
  unsigned int size = ticks.size ();

  if ( size == 0 ) return;

  size *= 4;

  xv.reserve ( size );
  yv.reserve ( size );

  const BinaryTransform & t 
    = dynamic_cast< const BinaryTransform & > ( transform );

  Range xrange = axisModelX.getRange( false );
  
  for ( unsigned int i = 0; i < ticks.size(); i++ )
    {
      double user_lx_start = xrange.low();
      double user_rx_start = xrange.high();
      double user_y_start = ticks[i].value();
      double user_y_temp = user_y_start; // for second call to transform

      // transform modifies its argument, thus we need 2 separate y's
      t.transform( user_lx_start, user_y_start );
      t.transform( user_rx_start, user_y_temp );
      
      double view_lx_start = view.userToDrawX( user_lx_start );
      double view_rx_start = view.userToDrawX( user_rx_start );
      double view_y_start = view.userToDrawY( user_y_start );

      double view_lx_end = view_lx_start + tick_length;
      double view_rx_end = view_rx_start - tick_length;
      double view_y_end = view_y_start;

#ifdef __USE_ISOC99
      if( isfinite( view_lx_start ) &&
	  isfinite( view_y_start ) &&
	  isfinite( view_lx_start ) &&
	  isfinite( view_y_end   ) )
#else
      if( finite( view_lx_start ) &&
	  finite( view_y_start ) &&
	  finite( view_lx_start ) &&
	  finite( view_y_end   ) )
#endif
	{
	  xv.push_back( view_lx_start );
	  yv.push_back( view_y_start );
	  xv.push_back( view_lx_end );
	  yv.push_back( view_y_end );

	  xv.push_back( view_rx_start );
	  yv.push_back( view_y_start );
	  xv.push_back( view_rx_end );
	  yv.push_back( view_y_end );
	}
    }
  
  view.drawViewLines ( xv, yv, Line::Solid, false, 1 );
}

void
AxisRepBase::
setFontSize( const AxisModelBase * xAxisModel,
	     const AxisModelBase * yAxisModel,
	     const AxisModelBase * zAxisModel,
	     ViewBase & view )
{
  Rect draw_rect = view.getDrawRect();

  m_font_size = min( ( draw_rect.getWidth() ) * 0.040, 12.0 );
  m_font_size = min (m_font_size, 12.0);
  double one = 1.0;
  m_font_size = max ( m_font_size, one );
  setXFontSize ( *xAxisModel, view );
  setYFontSize ( *yAxisModel, view );
  if ( zAxisModel != 0 ) setZFontSize ( *zAxisModel, view );
}

void
AxisRepBase::
drawXTickLabels ( const AxisModelBase & axisModelX,
		  const AxisModelBase & axisModelY,
		  const TransformBase & transform, 
		  ViewBase & base )
{
  double padding  = 1.0;
  
  vector < double > xv;  
  vector < double > yv;  
  
  const vector < AxisTick > & ticks = axisModelX.getTicks ();
  unsigned int size = ticks.size ();
  if ( size == 0 ) return;
  
  xv.reserve ( size );
  yv.reserve ( size );
  
  Range yrange = axisModelY.getRange( false );

  for ( unsigned int i = 0; i < size; i++ ){
    xv.push_back ( ticks[i].value () );
    yv.push_back ( yrange.low() ); 
  }
  
  const BinaryTransform & t 
    = dynamic_cast< const BinaryTransform & > ( transform );
  t.transform ( xv, yv );
  
  // Calculate the Y position in view coordinates.
  DataView & view = dynamic_cast < DataView & > ( base );
  const Rect & margin = view.getMarginRect ();
  float y = margin.getY ();
  float x;
  char yp = 'b';
  
  if ( axisModelX.getScaleLocation() & PLOTBOTTOM ) {
    y = margin.getY () + margin.getHeight ();
    yp = 't';
  }

  // If scientific notation is needed, calculate mag.
  string mag="";
  double ref = 0.;
  if (m_sci_note_x) {
    int mid = ticks.size()/2;
    ref = ticks[mid].value();
    double range = ticks[mid+1].value()-ref;
    char cstr[8]; 
    sprintf(cstr, "%.0e", range);
    string str(cstr);
    string::size_type pos = str.find ( 'e' );
    mag=string( str, pos + 1 ); // exponent
  }

  for ( unsigned int i = 0; i < ticks.size(); i++ )
    {
      // In first stage we calculate the tick. This shall help us get
      // the direction of the tick and using it we shall locate where
      // to draw the tick labels.
      double user_x_start = ticks[i].value();
      double user_y_start = yrange.low();
            
      t.transform( user_x_start, user_y_start );
      
      double view_x_start = view.userToDrawXAutoInv( user_x_start );
      double view_y_start = view.userToDrawY( user_y_start );
      
      double user_x_end = ticks[i].value ();
      double user_y_end = 
	yrange.low() + .05 * ( yrange.high() - yrange.low() );
      
      t.transform( user_x_end, user_y_end );
      
      double view_x_end = view.userToDrawXAutoInv( user_x_end );
      double view_y_end = view.userToDrawY( user_y_end );
      
      // Calculating the unit vector (ux,uy) in the direction of the tick
      double dx = view_x_end - view_x_start;
      double dy = view_y_end - view_y_start;
      
      double ux = dx / sqrt( dx * dx + dy * dy );
      double uy = dy / sqrt ( dx * dx + dy * dy );
      
      // Now calculate the x coord of the end point of the tick by 
      // moving a distance "tick_length" starting from view_x_start 
     // in the direction of ux. Similarly for y coordinate.
      x = view_x_start - ux * padding;
      y = view_y_start - uy * padding;
      
      // Plot the labels now. ( Handle two mode. )
      if (m_sci_note_x){
	// Draw the difference from reference tick value.
	double diff=ticks[i].value()-ref;
	// 6 byte should be enough, as small as -1e-99
	char pstr[8];    
	sprintf(pstr, "%.0e", diff);
	const std::string str(pstr);
	string::size_type pos = str.find ( 'e' );
	string m ( str, 0, pos ); // mantessa
	string e ( str, pos + 1 ); // exponent
	// Is this safe ?
	if (e!=mag && m!="0") m+="0";
	drawXTickLabel ( m, x, y,  view );
      }
      else {
	// Draw the content of the ticks.
	drawXTickLabel ( ticks[i].content(), x, y, view );
      }
    }
  
  // Draw sct_note reference point or pmag.
  if (m_sci_note_x) {
    drawXMag( axisModelX, view, mag );
    //drawXLabels(axisModelX, view, "X="+ticks[mid].content());
    //drawReferencePoint( axisModelX, view, ticks[mid].content());
  }
    
    else if ( axisModelX.needPMag () )
    {
      // Logarithmic graphs do not need the magnitude written.
      if ( axisModelX.isLog () == false ) {
	drawXMag ( axisModelX, view );
      }
    }
}

void
AxisRepBase::
drawYTickLabels ( const AxisModelBase & axisModelX,
		  const AxisModelBase & axisModelY,
		  const TransformBase & transform,
		  ViewBase & view )
{
  double padding  = 2.0;

  const vector< AxisTick > & ticks = axisModelY.getTicks ();
  unsigned int size = ticks.size ();
  if ( size == 0 ) return;

  const BinaryTransform & t 
    = dynamic_cast< const BinaryTransform & > ( transform );

  Range xrange = axisModelX.getRange( false );

  // If scientific notation is needed, calculate mag.
  string mag="";
  double ref = 0.;
  if (m_sci_note_y) {
    int mid = ticks.size()/2;
    ref = ticks[mid].value();
    double range = ticks[mid+1].value()-ref;
    char cstr[8]; 
    sprintf(cstr, "%.0e", range);
    string str(cstr);
    string::size_type pos = str.find ( 'e' );
    mag=string( str, pos + 1 ); // exponent
  }

  for ( unsigned int i = 0; i < ticks.size(); i++ )
    {
      // In first stage we calculate the tick. This shall help us get
      // the direction of the tick and using it we shall locate where
      // to draw the tick labels.
      double user_x_start = xrange.low();
      double user_y_start = ticks[i].value();
            
      t.transform( user_x_start, user_y_start );
      
      double view_x_start = view.userToDrawX( user_x_start );
      double view_y_start = view.userToDrawY( user_y_start );
      
      double user_x_end 
	= xrange.low() + .05 * ( xrange.high() - xrange.low() );
      double user_y_end = ticks[i].value ();
      
      t.transform( user_x_end, user_y_end );
      
      double view_x_end = view.userToDrawX( user_x_end );
      double view_y_end = view.userToDrawY( user_y_end );

      // Calculating the unit vector (ux,uy) in the direction of the tick
      double dx = view_x_end - view_x_start;
      double dy = view_y_end - view_y_start;
      
      double ux = dx / sqrt( dx * dx + dy * dy );
      double uy = dy / sqrt ( dx * dx + dy * dy );

      // Now calculate the x coord of the end point of the tick by 
      // moving a distance "tick_length" starting from view_x_start 
      // in the direction of ux. Similarly for y coordinate.
      float x = view_x_start - ux * padding;
      float y = view_y_start - uy * 1.5;

      // Plot the labels now. ( Handle two mode. )
      if (m_sci_note_y){
	// Draw the difference from reference tick value.
	double diff=ticks[i].value()-ref;
	// 6 byte should be enough, as small as -1e-99
	char pstr[8];    
	sprintf(pstr, "%.0e", diff);
	const std::string str(pstr);
	string::size_type pos = str.find ( 'e' );
	string m ( str, 0, pos ); // mantessa
	string e ( str, pos + 1 ); // exponent
	// Is this safe?
	if (e!=mag && m!="0") m+="0";
	drawYTickLabel ( m, x, y,  view );
      }
      else {
	// Draw the content of the ticks.
	drawYTickLabel ( ticks[i].content(), x, y, view );
      }
    } 

  
  // Draw pmag.
  if (m_sci_note_y) {
    drawYMag( axisModelY, view, mag );
  }
    
  else if ( axisModelY.needPMag () ){
    // Logarithmic graphs do not need the magnitude written.
    if ( axisModelY.isLog () == false ) {
      drawYMag ( axisModelY, view );
    }
  }
}

void
AxisRepBase::
drawXTickLabel ( const std::string & label, float x, float y, ViewBase & view )
{
  char xalign = 'c';
  char yalign = 't';
  float angle = 0.;
  double & font_size = m_x_tick_font_size;

  string::size_type pos = label.find ( 'e' );
  if ( pos == string::npos ) {
    view.drawText ( label, x, y, m_x_tick_font_size, angle, xalign, yalign );
  }
  else {  // need superscript
    double ss_size = 0.9 * font_size;
    string m ( label, 0, pos ); // mantessa
    string e ( label, pos + 1 ); // exponent
    if ( m == "1" ) { 
      m += "0";
      x += 0.25 * font_size; // don't know why this is needed.
      y += 0.25 * ss_size;
      view.drawText ( m, x, y, font_size, angle, 'r', yalign );

      if ( e[0] == '-' ) {
	x -= 0.10 * font_size;
      }
      y -= 0.5 * ss_size;
      view.drawText ( e, x, y, ss_size, angle, 'l', yalign );
    }
    else { // m != 1
      m += "x10";
      x += 0.25 * font_size;
      y += 0.25 * ss_size;
      view.drawText ( m, x, y, font_size, angle, 'r', yalign );

      if ( e[0] == '-' ) {
	x -= 0.10 * font_size;
      }
      y -= 0.5 * ss_size;
      view.drawText ( e, x, y, ss_size, angle, 'l', yalign );
    }
  }
}


void 
AxisRepBase::
drawYTickLabel ( const std::string & label,
		 float x, float y,
		 ViewBase & view )
{
  char xalign = 'r';
  char yalign = 'c';
  float angle = 0.;
  double font_size = m_y_tick_font_size;

  string::size_type pos = label.find ( 'e' );  
  if ( pos == string::npos ) {
      view.drawText ( label, x, y, font_size, angle, xalign, yalign );
    }

  else {  // need superscript
    string m ( label, 0, pos ); // mantessa
    string e ( label, pos + 1 ); // exponent
 
    if ( m == "1" ) { 
      m += "0";
      x = x - 0.40 * font_size * e.size();
      view.drawText ( m, x, y, font_size, angle, xalign, yalign );
      
      if ( e[0] == '-' ) {
	x -= 0.20 * font_size;
      }
      y = y - 0.625 * font_size;
      view.drawText ( e, x, y, 0.9 * font_size, angle, 'l', yalign );
    }
    else { // m != '1' and need to leave room for label
      m += "x10";
      x = x - 0.40 * font_size * e.size();
      view.drawText (m, x, y, font_size, angle, xalign, yalign );
      
      if ( e[0] == '-' ) {
	x -= 0.20 * font_size;
      }
      y = y - 0.625 * font_size;
      view.drawText ( e, x, y, 0.9 * font_size, angle, 'l', yalign );
    } 
  }
}


void
AxisRepBase::
drawYTickLabels ( const std::vector < AxisTick > & ticks,
		  const std::vector < float > & xv,
		  const std::vector < float> & yv,
		  ViewBase & view)
{
  char xalign = 'r';
  char yalign = 'c';
  float angle = 0.;
  double font_size = m_y_tick_font_size;

  const string & alabel = ticks[0].content();
  string::size_type pos = alabel.find ( 'e' );
  bool sci_notate = pos != string::npos;

  if ( sci_notate == false ) {
    for ( unsigned int i = 0; i < ticks.size(); i++ ) {
      view.drawText ( ticks[i].content(), xv[i], yv[i],
		      font_size, angle, xalign, yalign );
    }
  }
  else {  // need superscript
    bool mixed_m = false;
    for ( unsigned int i = 0; i < ticks.size (); i++ ) {
      const string & label = ticks[i].content ();
      string m ( label, 0, pos ); // mantessa
      string e ( label, pos + 1 ); // exponent
      mixed_m |= m != "1";
    }
    if ( mixed_m == true ) {
      font_size *= 0.90;
    }
    for ( unsigned int i = 0; i < ticks.size (); i++ ) {
      const string & label = ticks[i].content();
      string::size_type pos = alabel.find ( 'e' );
      string m ( label, 0, pos );
      string e ( label, pos + 1 );
      if ( m == "1" ) { 
	m += "0";
	float x = xv[i] - 0.40 * font_size * e.size();
	view.drawText ( m, x, yv[i], font_size, angle, xalign, yalign );

	if ( e[0] == '-' ) {
	  x -= 0.20 * font_size;
	}
	float y = yv[i] - 0.625 * font_size;
	view.drawText ( e, x, y, 0.9 * font_size, angle, 'l', yalign );
      }
      else { // m != '1' and need to leave room for label
	m += "x10";
	float x = xv[i] - 0.40 * font_size * e.size();
	view.drawText (m, x, yv[i], font_size, angle, xalign, yalign );

	if ( e[0] == '-' ) {
	  x -= 0.20 * font_size;
	}
	float y = yv[i] - 0.625 * font_size;
	view.drawText ( e, x, y, 0.9 * font_size, angle, 'l', yalign );
      }
    }
  }
}

void
AxisRepBase::
drawCrossHairs ( double x, double y,
		 TransformBase & tb,
		 ViewBase & view )
{

  vector < double > xv ( 4 );
  vector < double > yv ( 4 );
  

   const BinaryTransform & tf
    = dynamic_cast< const BinaryTransform & > ( tb );

  if ( tf.isPeriodic() )
    {
      const PeriodicBinaryTransform & tp 
	= dynamic_cast < const PeriodicBinaryTransform & > ( tb );
      
      double xoffset = tp.rotation(Axes::X);
      double yoffset = tp.rotation(Axes::Y);
      
      x   = tp.moduloSubX( x, xoffset/UNITS_RADIANS * tp.units(Axes::X) );
      y   = tp.moduloSubY( y, yoffset/UNITS_RADIANS * tp.units(Axes::Y) );
    }
  
  tf.transform ( x,y );
  
  xv[0] = m_axis_x_origin;
  yv[0] = y;
  
  xv[1] = m_axis_width + m_axis_x_origin;
  yv[1] = y;

  xv[2] = x;
  yv[2] = m_axis_height + m_axis_y_origin;

  xv[3] = x;
  yv[3] = m_axis_y_origin;

  // Note: m_axis_* is already in the transformed system.
  Color grey ( 180, 180, 180 );
  view.drawLines ( xv, yv, Line::Solid, grey, 0 );

}

/** @todo Draw lines connecting ticks top and bottom.  Line should
    under go transform which might be curved, thus should use
    something like the function projector.
*/
void
AxisRepBase::
drawAllXTicks ( const AxisModelBase & axisModelX,
		const AxisModelBase & axisModelY,
		const TransformBase & transform,
		ViewBase & view )
{
  drawXTickLines ( axisModelX, axisModelY, transform, view );
  drawXTickLabels ( axisModelX, axisModelY, transform, view );
}

void
AxisRepBase::
drawAllYTicks ( const AxisModelBase & axisModelX,
		const AxisModelBase & axisModelY,
		const TransformBase & transform,
				   ViewBase & view )
{
  drawYTickLines ( axisModelX, axisModelY, transform, view );
  drawYTickLabels ( axisModelX, axisModelY, transform, view );
}


void AxisRepBase::setXLabelFont( FontBase* font )
{
  assert( font != 0 );
  if( m_xLabelFont == NULL )
    m_xLabelFont = font;
  else
    {
      delete m_xLabelFont;
      m_xLabelFont = font;
    }
}

FontBase* AxisRepBase::xLabelFont( )
{
  return m_xLabelFont;
}


void AxisRepBase::setYLabelFont( FontBase* font )
{
  assert( font != 0 );
  if( m_yLabelFont == NULL )
    m_yLabelFont = font;
  else
    {
      delete m_yLabelFont;
      m_yLabelFont = font;
    }
}

FontBase* AxisRepBase::yLabelFont( )
{
  return m_yLabelFont;
}


void AxisRepBase::setZLabelFont( FontBase* font )
{
  assert( font != 0 );
  if( m_zLabelFont == NULL )
    m_zLabelFont = font;
  else
    {
      delete m_zLabelFont;
      m_zLabelFont = font;
    }
}

FontBase* AxisRepBase::zLabelFont( )
{
  return m_zLabelFont;
}

void AxisRepBase::setTitleFont( FontBase* font )
{
  assert( font != 0 );
  if( m_titleFont == NULL )
    m_titleFont = font;
  else
    {
      delete m_titleFont;
      m_titleFont = font;
    }
}

FontBase* AxisRepBase::titleFont( )
{
  return m_titleFont;
}

void
AxisRepBase::drawColorScale ( const BinToColor &, ViewBase & )
{
  assert ( false );
}
