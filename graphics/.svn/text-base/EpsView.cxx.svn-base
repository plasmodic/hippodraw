/** @file

hippodaraw::EpsView class implementation

Copyright (C) 2001-2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: EpsView.cxx,v 1.89 2007/04/23 21:57:20 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "EpsView.h"

#include "FontBase.h"
#include "Color.h"

#include "plotters/PlotterBase.h"


#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::endl;
using std::ofstream;
using std::string;
#endif

using namespace hippodraw;

EpsView::
EpsView ( const std::string filename, double x, double y, double w, double h )
  : DataView(),
    m_boundingRect ( x, y, w, h )
{
  initPlot ( filename, x, y, w, h );
}

EpsView::
~EpsView()
{
}

void
EpsView::
lineTo ( double x, double y )
{
  m_outfile << x << " " << y << " lineto\n";
}

void
EpsView::
moveTo ( double x, double y )
{
  m_outfile << x << " " << y << " moveto\n";
}

void
EpsView::
setDash ( Line::Style style )
{
  switch (style)
    {
    case Line::Solid:
      m_outfile << "[] 0 setdash\n";
      break;
    case Line::Dot:
      m_outfile << "[3 5] 0 setdash\n";
      break;
    case Line::Dash:
      m_outfile << "[5 3] 0 setdash\n";
      break;
    case Line::DashDot:
      m_outfile << "[5 3 1 3] 0 setdash\n";
      break;
    default:
      break;
    }
}

void
EpsView::
setLineWidth ( double size )
{
  m_outfile << size << " setlinewidth\n";
}

void
EpsView::
setRgbColor ( int red, int green, int blue )
{
  double inv = 1. / 255.;
  m_outfile << red   * inv << " "
	    << green * inv << " "
	    << blue  * inv << " setrgbcolor\n";
}

void
EpsView::
setRgbColor ( const Color & color )
{
  setRgbColor ( color.getRed (), color.getGreen (), color.getBlue () );
}
/** @todo Using std::endl; add '\n' and flushes the buffer.  We don't
    need to flush the buffer so often, so maybe just '\n' should be
    used.
*/
void EpsView::initPlot ( const std::string & fname, 
			 double, double, double w, double h )
{

  const char * fn = fname.c_str();
  m_outfile.open (fn, std::ios::out);

  m_outfile << "%%!PS-Adobe-3.0 EPSF-3.0" << endl;
  m_outfile << "%%Creator: HippoDraw" << endl;

  double x1 = 0;
  double y1 = 0;
  double x2 = x1 + w;
  double y2 = y1 + h;

  m_outfile << "%%BoundingBox: " 
	  << x1 << " " << y1 << " " 
	  << x2 << " " << y2 << endl; 
  
  m_outfile << "%%EndComments" << endl;
  m_outfile << endl << endl;

  m_outfile << "%% Add emulation of selectfont if needed" << endl;
  m_outfile << "%%   taken from PS Lang. Ref. Manual, Appendix D.4" << endl;
  m_outfile << "/*SF {" << endl;
  m_outfile << "  exch findfont exch" << endl;
  m_outfile << "  dup type /arraytype eq {makefont}{scalefont} ifelse setfont" 
	    << endl;
  m_outfile << "} bind def" << endl;
  m_outfile << endl;
  m_outfile << "/languagelevel where" << endl;
  m_outfile << " {pop languagelevel} {1} ifelse" << endl;
  m_outfile << "2 lt {/SF /*SF load def}{/SF /selectfont load def} ifelse"
	  << endl;

  m_outfile << "%%" << endl << "%%" << endl;
}

void EpsView::closeFile ()
{
  m_outfile << "%%EOF" << endl;
  m_outfile.close();
}

void EpsView::drawLines ( const std::vector< double > & x,
			  const std::vector< double > & y,
			  hippodraw::Line::Style style,
			  const Color & color,
			  double size )
{
  if ( style == Line::Invisible ) return;

  m_outfile << "%% drawLines" << endl;

  m_outfile << "gsave" << endl;

  setRgbColor ( color );

  setLineWidth ( size );
  setDash ( style );
  
  for ( unsigned int i = 0; i < x.size(); i = i+2 )
    {
      
      m_outfile << "gsave" << endl << "newpath systemdict begin" << endl;
      moveTo ( toViewX (x[i]), toViewY (y[i]) );
      lineTo ( toViewX (x[i+1]), toViewY (y[i+1]) );
      m_outfile << "end" << endl;
      m_outfile << "stroke grestore" << endl;
      
    }

  m_outfile << "grestore" << endl;

}

void EpsView::drawColorLines ( const std::vector< double > & x,
			       const std::vector< double > & y,
			       hippodraw::Line::Style style,
			       const std::vector < Color > & colors,
			       double size )
{
  if ( style == Line::Invisible ) return;

  m_outfile << "%% drawLines" << endl;
  m_outfile << "gsave" << endl;

  for (unsigned int i = 0; i < x.size(); i+=2 ){
    
    const Color & color = colors[i];
    setRgbColor ( color );
    setLineWidth ( size );
    setDash ( style );
  
    m_outfile << "gsave" << endl << "newpath systemdict begin" << endl;
    moveTo ( toViewX (x[i]),   toViewY (y[i]) );
    lineTo ( toViewX (x[i+1]), toViewY (y[i+1]) );
    m_outfile << "end" << endl;
    m_outfile << "stroke grestore" << endl;
    
  }
  
  m_outfile << "grestore" << endl;

}

/** @bug Does not use the color parameter
 */
void EpsView::drawViewLines ( const std::vector< double > & x,
			      const std::vector< double > & y,
			      hippodraw::Line::Style style,
			      bool, // color,
			      double size )
{
  if ( style == Line::Invisible ) return;

  m_outfile << "%% drawViewLines1" << endl;
  
  m_outfile << "gsave" << endl;

  setLineWidth ( size );
  setDash ( style );

    for ( unsigned int i = 0; i < x.size(); i = i+2 )
    {
    
      m_outfile << "gsave" << endl << "newpath systemdict begin" << endl;
      moveTo ( toX(x[i]),   toY(y[i])   );
      lineTo ( toX(x[i+1]), toY(y[i+1]) );
      m_outfile << "end" << endl;
      m_outfile << "stroke grestore" << endl;
      
    }

    m_outfile << "grestore" << endl;

}
 
void EpsView::drawViewLines ( const std::vector< double > & x,
			      const std::vector< double > & y,
			      hippodraw::Line::Style style,
			      const Color & color,
			      double size )
{
  if ( style == Line::Invisible ) return;

  m_outfile << "%% drawViewLines2" << endl;

  m_outfile << "gsave" << endl;

  setRgbColor ( color );
  setLineWidth ( size );
  setDash ( style );
  
  for ( unsigned int i = 0; i < x.size(); i = i+2 )
    {
      
      m_outfile << "gsave" << endl << "newpath systemdict begin" << endl;
      moveTo ( toX(x[i]),   toY(y[i])   );
      lineTo ( toX(x[i+1]), toY(y[i+1]) );
      m_outfile << "end" << endl;
      m_outfile << "stroke grestore" << endl;
      
    }
  
  m_outfile << "grestore" << endl;
  
}
 
void EpsView::drawPolyLine ( const std::vector< double > & xpoints,
			     const std::vector< double > & ypoints, 
			     hippodraw::Line::Style style,
			     const Color & color,
			     double size )
{
  if ( style == Line::Invisible ) return;

  m_outfile << "%% drawPolyLine" << endl;

  m_outfile << "gsave" << endl;

  setRgbColor ( color );
  setLineWidth ( size );
  setDash ( style );
  
  m_outfile << "gsave" << endl << "newpath systemdict begin" << endl;
  moveTo ( toViewX ( xpoints[0] ), toViewY ( ypoints[0] ) );

  for ( unsigned int i = 1; i < xpoints.size(); i++ )
    {
      double view_x = toViewX ( xpoints[i] );
      double view_y = toViewY ( ypoints[i] );
      lineTo ( view_x, view_y );
    }
     
  m_outfile << "end" << endl;
  
  m_outfile << "stroke grestore" << endl;

  m_outfile << "grestore" << endl;
}

void EpsView::drawPolygon ( const std::vector< double > & xpoints,
			     const std::vector< double > & ypoints, 
			     const Color & color,
			     const Color & edge )
{
  m_outfile << "%% drawPolygon" << endl;

  m_outfile << "gsave" << endl;

  setRgbColor ( color );
  
  m_outfile << "newpath" << endl;
  moveTo ( toViewX ( xpoints[0] ), toViewY ( ypoints[0] ) );

  for ( unsigned int i = 1; i < xpoints.size(); i++ )
    {
      double view_x = toViewX ( xpoints[i] );
      double view_y = toViewY ( ypoints[i] );
      lineTo ( view_x, view_y );
    }
     
  m_outfile << "closepath" << endl;

  m_outfile << "fill" << endl;
  
  m_outfile << "grestore" << endl;
}

void
EpsView::
drawSquare ( double x1, double y1, double x2, double y2,
	     int red, int green, int blue )
{
  m_outfile << "%% drawSquareRGB" << endl;

  m_outfile << "gsave" << endl;
  
  setRgbColor ( red, green, blue );
  m_outfile << "newpath" << endl; 
  moveTo ( toViewX ((double)(x1)), toViewY ((double)(y1)) );
  lineTo ( toViewX ((double)(x2)), toViewY ((double)(y1)) );
  lineTo ( toViewX ((double)(x2)), toViewY ((double)(y2)) );
  lineTo ( toViewX ((double)(x1)), toViewY ((double)(y2)) );
  m_outfile << "closepath" << endl;

  m_outfile << "fill" << endl;

  m_outfile << "grestore" << endl;

}

void EpsView::drawViewSquare ( double x1, double y1, double x2, double y2,
			       int red, int green, int blue )
{
  m_outfile << "%% drawSquareRGB" << endl;

  m_outfile << "gsave" << endl;
  
  setRgbColor ( red, green, blue );  
  m_outfile << "newpath" << endl; 
  moveTo ( toX ((double)(x1)), toY ((double)(y1)) );
  lineTo ( toX ((double)(x2)), toY ((double)(y1)) );
  lineTo ( toX ((double)(x2)), toY ((double)(y2)) );
  lineTo ( toX ((double)(x1)), toY ((double)(y2)) );
  m_outfile << "closepath" << endl;

  m_outfile << "fill" << endl;

  m_outfile << "grestore" << endl;

}

void
EpsView::
drawSymbol ( hippodraw::Symbol::Type type, 
	     double x, double  y, double sym_size )
{
  switch ( type )
    {
    case Symbol::SQUARE:
    case Symbol::SOLIDSQUARE:
	  
      moveTo ( toViewX (x)-(sym_size/2), toViewY (y)-(sym_size/2) );
      lineTo ( toViewX (x)+(sym_size/2), toViewY (y)-(sym_size/2) );
      lineTo ( toViewX (x)+(sym_size/2), toViewY (y)+(sym_size/2) );
      lineTo ( toViewX (x)-(sym_size/2), toViewY (y)+(sym_size/2) );

      m_outfile << "closepath" << endl;
	  
      break;
	  
    case Symbol::TRIANGLE:
    case Symbol::FILLED_TRIANGLE:
      moveTo ( toViewX (x),                toViewY (y) + (sym_size/2) );
      lineTo ( toViewX (x) + (sym_size/2), toViewY (y)-(sym_size/2) );
      lineTo ( toViewX (x) - (sym_size/2), toViewY (y)-(sym_size/2) );
      m_outfile << "closepath" << endl;
      break;
	  
    case Symbol::CIRCLE:
    case Symbol::FILLED_CIRCLE:
      moveTo ( toViewX (x) + (sym_size/2), toViewY (y) );
      m_outfile << toViewX (x) << " "
		<< toViewY (y) << " "
		<< sym_size/2 << " "
		<< "0.0 360 arc" << endl;
      break;

    case Symbol::PLUS:
      moveTo ( toViewX (x)-(sym_size/2), toViewY (y) );
      lineTo ( toViewX (x)+(sym_size/2), toViewY (y) );
      moveTo ( toViewX (x),              toViewY (y)-(sym_size/2) );
      lineTo ( toViewX (x),              toViewY (y)+(sym_size/2) );
      break;
	  
    case Symbol::TIMES:
      moveTo ( toViewX (x)-(sym_size/2), toViewY (y)-(sym_size/2) );
      lineTo ( toViewX (x)+(sym_size/2), toViewY (y)+(sym_size/2) );
      lineTo ( toViewX (x)+(sym_size/2), toViewY (y)-(sym_size/2) );
      lineTo ( toViewX (x)-(sym_size/2), toViewY (y)+(sym_size/2) );
      break;

    default:
      break;
    }
}

void EpsView::drawPoints ( const std::vector<double> & x,
			   const std::vector<double> & y,
			   hippodraw::Symbol::Type type, 
			   double sym_size, 
			   const Color & color )
{
  m_outfile << "%% drawPoints" << endl;

  m_outfile << "gsave" << endl;

  setRgbColor ( color );

  bool filled = false;

  if ( type == Symbol::SOLIDSQUARE ||
       type == Symbol::FILLED_TRIANGLE ||
       type == Symbol::FILLED_CIRCLE ) filled = true;

  m_outfile << "gsave" << endl;
  m_outfile << "newpath systemdict begin" << endl;
  
  for (unsigned int i = 0; i < x.size(); i++) {
      drawSymbol ( type, x[i], y[i], sym_size );
  }

  m_outfile << "end" << endl;
  
  if(filled)
    {
      m_outfile << "fill grestore" << endl;
    }
  else
    {
      m_outfile << "stroke grestore" << endl;
    }

  m_outfile << "grestore" << endl;
  
}

void
EpsView::
drawPoints ( const std::vector< double > & x,
	     const std::vector< double > & y, 
	     const std::vector< Color > & colors,
	     hippodraw::Symbol::Type type, 
	     double sym_size )
{
  m_outfile << "%% drawPoints2" << endl;

  m_outfile << "gsave" << endl;
  
  bool filled = false;

  if ( type == Symbol::SOLIDSQUARE ||
       type == Symbol::FILLED_TRIANGLE ||
       type == Symbol::FILLED_CIRCLE ) filled = true;

  for (unsigned int i = 0; i < x.size(); i++)
    {
      m_outfile << "gsave" << endl;

      const Color & color = colors[i];
      setRgbColor ( color );

      m_outfile << "newpath systemdict begin" << endl;
      drawSymbol ( type, x[i], y[i], sym_size );

      m_outfile << "end" << endl;
      
      if ( filled ) {
	m_outfile << "fill grestore" << endl;
      }
      else {
	m_outfile << "stroke grestore" << endl;
      }
    }
  
  m_outfile << "grestore" << endl;

}

/** @bug Text size doesn't come out the same as on screen.
 */
void EpsView::draw_Text ( const std::string &s, double x, double y,
			 double fontsize, double angle,
			 char xp, char yp, const FontBase * font )
{
  double xStep = 0.0, yStep = 0.0;
     
  switch (yp)
    {
    case 'c':
      yStep = 0.4f;
      break;
	  
    case 't':
      yStep = 0.8f;
      break;

    default:
      break;
    }
  
  switch (xp)
    {

    case 'c':
      xStep = 0.5f;
      break;

    case 'r':
      xStep = 1.0f;
      break;

    default:
      break;
    }
     
  m_outfile << "gsave" << endl;

  int size = 0;
  if ( font == 0 ) {
    m_outfile << "(Helvetica) " << fontsize << " SF" << endl;
    size = static_cast < int > ( fontsize );
  }
  else {
    string family ( "(" );
    family += font -> family ();
    family += ") ";
    size = font -> pointSize ();
    m_outfile << family << size << " SF" << endl;
  }
  m_outfile << x << " " << y << " translate " << angle << " rotate" << endl;
     
  m_outfile << "(" << s << ") stringwidth pop" << endl;
  m_outfile << xStep << " neg mul " << -size*yStep << " moveto" << endl;
  m_outfile << "(" << s << ") show" << endl;
  m_outfile << "grestore" << endl;

}

/** @bug @@@@@@ Does not use color value.
    @bug Does not use the resize parameter.
 */
void EpsView::drawText ( const std::string &s, double x, double y,
			 double fontsize, double angle,
			 char xp, char yp, bool, // resize,
			 const FontBase * font,
			 const Color * color )
{
  // Resize ignored as the view size is always just bounding the contents
  m_outfile << "%% drawText" << endl;
     
  draw_Text ( s, toX(x), toY(y), fontsize, angle, xp, yp, font );
}

void EpsView:: update ( const Observable * )
{
}

/** @todo Could eliminated userToMarginX.
 */
double EpsView::userToDrawX ( double x ) const
{
  return userToMarginX ( x );
}

double EpsView::userToDrawXAutoInv ( double x ) const
{
  if (m_plotter -> isReverse())
    return userToInvertedMarginX( x );
  else 
    return userToMarginX( x );
}

/** @todo Could eliminate userToInvertedMarginY.
 */
double EpsView::userToDrawY ( double y ) const
{
  return userToInvertedMarginY( y );
}

/** @todo Could eliminate userToMargincolor.
 */
double EpsView::userToDrawColor ( double c ) const
{
  return userToMarginColor( c );
}

Rect EpsView::getDrawRect() const
{
  return m_draw_rect;
}

void EpsView::setDrawRect ( double x, double y, double w, double h )
{
  m_draw_rect.setRect ( x, y, w, h );
}

double EpsView::toViewX ( double datX ) const
{
  double view_x =  ( m_draw_rect.getX() 
	   + userToMarginX ( datX )
	   - m_boundingRect.getX() );

  return view_x;
}

double EpsView::toViewY ( double datY ) const
{
  /* If you remove the s1 and the s2 from the return statements, you get
     the plots that look OK, but a plot on top in the canvas appears at the
     bottom in the EPSF file. Subtracting from s2 vertically inverts each
     plot, and then subtracting from s1 vertically inverts the entire EPSF
     file. The result is what is desired - Sanket. */

  double s1 = m_boundingRect.getY() + m_boundingRect.getHeight();

  double s2 = m_draw_rect.getY() + m_draw_rect.getHeight();

  const Rect & margin_rect = getMarginRect ();

  double mar_y = userToMarginY ( datY );
  return ( s1 - ( s2 -  ( mar_y
			  - 2 * margin_rect.getY()
			  + m_draw_rect.getHeight()
			  - margin_rect.getHeight()
			  )
		  )
	   );
}

double EpsView::toX ( double x ) const
{  
  return static_cast < double > ( m_draw_rect.getX() 
				 + x
				 - m_boundingRect.getX() );
}

double EpsView::toY ( double y ) const
{
  double s1 = m_boundingRect.getY() + m_boundingRect.getHeight();
  
  return static_cast < double > ( s1 - ( m_draw_rect.getY() + y ) );
}
