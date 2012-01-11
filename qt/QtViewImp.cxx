/** @file

hippodraw::QtViewImp implementation

Copyright (C) 2001-2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: QtViewImp.cxx,v 1.193 2007/06/28 20:03:02 pfkeb Exp $

*/


// inconsistent dll linkage
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "QtViewImp.h"

#include "DrawBorder.h"
#include "PlotterEvent.h"
#include "QtFont.h"

#include "axes/Range.h"
#include "graphics/Color.h"
#include "plotters/PlotterBase.h"

#include <qapplication.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qwmatrix.h>

#include <qfile.h>
#include <qdir.h>
#include <qdatetime.h>
#include <QtGui/QPolygon>
#include <QtCore/QTextStream>

#include <cassert>
#include <cmath>

using namespace hippodraw;
using std::vector;

enum Location { none, invalid,
  upper_left,  upper_middle, upper_right,
  middle_left, middle_right,
  lower_left,  lower_middle, lower_right };

std::map < Line::Style, Qt::PenStyle > QtViewImp::s_line_style;

QtViewImp::
QtViewImp ()
  :DataView (),
   m_dragging( none ),
   m_inspector ( 0 ),
   m_painter ( 0 ),
   m_font_default ( "helvetica" )
{
  if ( s_line_style.empty () ) {
    s_line_style [ Line::Solid ]      = Qt::SolidLine;
    s_line_style [ Line::Dash ]       = Qt::DashLine;
    s_line_style [ Line::Dot ]        = Qt::DotLine;
    s_line_style [ Line::DashDot ]    = Qt::DashDotLine;
    s_line_style [ Line::DashDotDot ] = Qt::DashDotDotLine;
    s_line_style [ Line::Invisible ]  = Qt::NoPen;
  }
  m_eq_png.clear();
}
   

QtViewImp::
QtViewImp ( PlotterBase * plotter )
  : DataView ( plotter ),
    m_dragging( none ),
    m_inspector ( 0 ),
    m_painter ( 0 ),
    m_font_default ( "helvetica" )
{
  if ( s_line_style.empty () ) {
    s_line_style [ Line::Solid ]      = Qt::SolidLine;
    s_line_style [ Line::Dash ]       = Qt::DashLine;
    s_line_style [ Line::Dot ]        = Qt::DotLine;
    s_line_style [ Line::DashDot ]    = Qt::DashDotLine;
    s_line_style [ Line::DashDotDot ] = Qt::DashDotDotLine;
    s_line_style [ Line::Invisible ]  = Qt::NoPen;
  }
  m_eq_png.clear();
}

void
QtViewImp::
setInspector ( QObject * inspector )
{
  m_inspector = inspector;
}

void
QtViewImp::
update ( const Observable * display )
{
  if ( display == NULL ) return;

  if (  m_inspector != NULL ) {
    Observable * o = const_cast < Observable * > ( display );
    PlotterBase * plotter = dynamic_cast < PlotterBase * > ( o );
    PlotterEvent * event = new PlotterEvent ( plotter );
    QApplication::removePostedEvents ( m_inspector );
    QApplication::postEvent ( m_inspector, event );
  }

}

double QtViewImp::userToDrawXAutoInv ( double xx ) const
{
  if (m_plotter -> isReverse())
    return userToInvertedMarginX( xx );
  else 
    return userToMarginX( xx );
}

double QtViewImp::userToDrawX ( double xx ) const
{
  return userToMarginX( xx );
}

double QtViewImp::userToDrawY ( double yy ) const
{
  return  userToInvertedMarginY( yy );
}

double QtViewImp::userToDrawColor ( double c ) const
{
  return userToMarginColor( c );
}

void
QtViewImp::
transformAndFill ( QPolygonF & array,
		   const std::vector< double > & x,
		   const std::vector< double > & y,
		   double (QtViewImp::* xfunc) ( double ) const,
		   double (QtViewImp::* yfunc) ( double ) const )
{
  unsigned int size = x.size();
  assert ( size == y.size() );

  for ( unsigned int i = 0; i < size; i++ )
    array[i] = QPointF((this->*xfunc)(x[i]), (this->*yfunc)(y[i]));

}

/** @todo Should style and color arguments be used?
 */
void 
QtViewImp::
drawViewMethod ( const std::vector< double > & x,
		 const std::vector< double > & y,
		 int, int )
{

  unsigned int size = x.size();
  assert ( size == y.size() );

  QPolygonF array ( size );
  transformAndFill ( array, x, y,
		     &QtViewImp::toCanvasX, &QtViewImp::toCanvasY );

  m_painter->drawLines ( array );

}


/** @bug Should style and color arguments be used?
 */
void 
QtViewImp::
drawMethod ( const std::vector < double > & x,
	     const std::vector < double > & y,
	     int style, int color)
{

  unsigned int size = x.size();
  assert ( size == y.size() );

  QPolygonF array ( size );
  transformAndFill ( array, x, y,
		     &QtViewImp::toViewX, &QtViewImp::toViewY );

  m_painter->drawPolyline ( array );
}


void 
QtViewImp::
drawPoints ( const std::vector<double> & x,
	     const std::vector<double> & y,
	     Symbol::Type type, 
	     double sym_size, 
	     const Color & color )
{
  m_painter->save();
  
  int i_sym_size = (int)(sym_size/2);

  int rgb[3];
  rgb[0] = color.getRed();
  rgb[1] = color.getGreen();
  rgb[2] = color.getBlue();
  QColor qcolor ( rgb[0], rgb[1], rgb[2] );

  m_painter->setPen ( qcolor );

  QPolygon triangleArray ( 3 );

  for (unsigned int i = 0; i < x.size(); i++)
    {
      double originX = toViewX ( x[i] );
      double originY = toViewY ( y[i] );
      
      switch ( type )
	{



	case Symbol::SOLIDSQUARE:
	  
	  m_painter->fillRect ( ( toViewX (x[i]) - (i_sym_size) ), 
				( toViewY (y[i]) - (i_sym_size) ),
				(i_sym_size*2 + 1 ),
				(i_sym_size*2 + 1 ),
				( qcolor ) );
	  
	  break;



	case Symbol::SQUARE:  
	  m_painter -> drawRect ( toViewX ( x[i] ) - i_sym_size,
				  toViewY ( y[i] ) - i_sym_size,
				  2*i_sym_size + 1,
				  2*i_sym_size + 1 );
 	  break;

 	case Symbol::TRIANGLE:
	  triangleArray.setPoint ( 0, ( toViewX (x[i]) ), 
			      ( toViewY (y[i]) + (i_sym_size) ) );

	  triangleArray.setPoint ( 1, ( toViewX (x[i]) + (i_sym_size) ),
			      ( toViewY (y[i]) - (i_sym_size) ) );


	  triangleArray.setPoint ( 2, ( toViewX (x[i]) - (i_sym_size) ),
			      ( toViewY (y[i]) - (i_sym_size) ) );
	  
	  m_painter -> setBrush ( Qt::NoBrush );
	  m_painter->drawPolygon (triangleArray);
 	  break;

 	case Symbol::FILLED_TRIANGLE:
	  
	  triangleArray.setPoint ( 0, ( toViewX (x[i]) ), 
			      ( toViewY (y[i]) + (i_sym_size) ) );

	  triangleArray.setPoint ( 1, ( toViewX (x[i]) + (i_sym_size) ),
			      ( toViewY (y[i]) - (i_sym_size) ) );


	  triangleArray.setPoint ( 2, ( toViewX (x[i]) - (i_sym_size) ),
			      ( toViewY (y[i]) - (i_sym_size) ) );
	  
	  m_painter->setBrush (qcolor);

	  m_painter->drawPolygon (triangleArray);
	  
	  break;


 	case Symbol::CIRCLE:

	  m_painter->drawEllipse ( ( toViewX (x[i]) - (i_sym_size) ), 
				   ( toViewY (y[i]) - (i_sym_size) ),
				   ( i_sym_size*2 + 1 ),
				   ( i_sym_size*2 + 1 ) );
 	  
	  break;


 	case Symbol::FILLED_CIRCLE:

	  m_painter->setBrush (qcolor);

	  m_painter->drawPie ( ( toViewX (x[i]) - (i_sym_size) ), 
			       ( toViewY (y[i]) - (i_sym_size) ),
			       ( i_sym_size*2 + 1 ),
			       ( i_sym_size*2 + 1 ),
			       ( 16 * 0 ),
			       ( 16 * 360 ) );

	  break;

 	case Symbol::PLUS:
	  m_painter -> drawLine ( originX - i_sym_size, originY,
				  originX + i_sym_size, originY );
	  m_painter -> drawLine ( originX, originY - i_sym_size,
				  originX, originY + i_sym_size );
 	  break;
	  
 	case Symbol::TIMES:
	  m_painter -> drawLine ( originX - i_sym_size, originY - i_sym_size,
				  originX + i_sym_size, originY + i_sym_size );
	  m_painter -> drawLine ( originX + i_sym_size, originY - i_sym_size,
				  originX - i_sym_size, originY + i_sym_size );
 	  break;
	  
 	default:
 	  break;
 	
	}
    }
  
  m_painter->restore();
  
}

/** @todo Determine if this member function is no longer needed. 
 */
void 
QtViewImp::
drawPoints ( const std::vector< double > &,
	     const std::vector< double > &,
	     Symbol::Type, 
	     double )
{

}

/** @todo This method has a lot of duplicated code from the one with
    the same name but only one color.  One should remove the
    duplication.
    
    @todo The draw triangle methods have duplicated code which could
    be removed.
*/
void 
QtViewImp::
drawPoints ( const std::vector< double > & x,
	     const std::vector< double > & y,
	     const std::vector< Color > & colors,
	     Symbol::Type type, 
	     double sym_size )
{
  assert ( x.size() == colors.size() );
  m_painter->save();
  
  int i_sym_size = (int)(sym_size/2);

  if ( i_sym_size == 0 &&
       type == Symbol::SOLIDSQUARE ) {
    type = Symbol::SQUARE;
  }

  QPolygon triangleArray ( 3 );

  for (unsigned int i = 0; i < x.size(); i++)
    {
      double o_x = toViewX ( x[i] );
      double o_y = toViewY ( y[i] );

      const Color & c = colors[i];
      QColor qcolor ( c.getRed (), c.getGreen(), c.getBlue () );
      m_painter->setPen ( qcolor );
      
      switch ( type )
	{
	case Symbol::SOLIDSQUARE:
	  m_painter->fillRect ( ( toViewX (x[i]) - (i_sym_size) ), 
				( toViewY (y[i]) - (i_sym_size) ),
				(i_sym_size*2 ),
				(i_sym_size*2 ),
				( qcolor ) );
	  
	  break;

	case Symbol::SQUARE:  
	  m_painter -> drawRect ( o_x -i_sym_size, o_y - i_sym_size,
				  2 * i_sym_size, 2 * i_sym_size );
 	  break;
	  


 	case Symbol::TRIANGLE:
	  triangleArray.setPoint ( 0, ( toViewX (x[i]) ), 
			      ( toViewY (y[i]) + (i_sym_size) ) );

	  triangleArray.setPoint ( 1, ( toViewX (x[i]) + (i_sym_size) ),
			      ( toViewY (y[i]) - (i_sym_size) ) );


	  triangleArray.setPoint ( 2, ( toViewX (x[i]) - (i_sym_size) ),
			      ( toViewY (y[i]) - (i_sym_size) ) );
	  
	  m_painter->setBrush ( Qt::NoBrush );
	  m_painter->drawPolygon (triangleArray);
	  
 	  break;



 	case Symbol::FILLED_TRIANGLE:
	  
	  triangleArray.setPoint ( 0, ( toViewX (x[i]) ), 
			      ( toViewY (y[i]) + (i_sym_size) ) );

	  triangleArray.setPoint ( 1, ( toViewX (x[i]) + (i_sym_size) ),
			      ( toViewY (y[i]) - (i_sym_size) ) );


	  triangleArray.setPoint ( 2, ( toViewX (x[i]) - (i_sym_size) ),
			      ( toViewY (y[i]) - (i_sym_size) ) );
	  
	  m_painter->setBrush (qcolor);

	  m_painter->drawPolygon (triangleArray);
	  
	  break;


 	case Symbol::CIRCLE:

	  m_painter->drawEllipse ( ( toViewX (x[i]) - (i_sym_size) ), 
				   ( toViewY (y[i]) - (i_sym_size) ),
				   (i_sym_size*2 ),
				   (i_sym_size*2 ) );
 	  
	  break;


 	case Symbol::FILLED_CIRCLE:

	  m_painter->setBrush (qcolor);

	  m_painter->drawPie ( ( toViewX (x[i]) - (i_sym_size) ), 
			       ( toViewY (y[i]) - (i_sym_size) ),
			       ( i_sym_size*2 ),
			       ( i_sym_size*2 ),
			       ( 16 * 0 ),
			       ( 16 * 360 ) );

	  break;

 	case Symbol::PLUS:
	  m_painter -> drawLine ( o_x - i_sym_size, o_y,
				  o_x + i_sym_size, o_y );
	  m_painter -> drawLine ( o_x, o_y - i_sym_size,
				  o_x, o_y + i_sym_size );
 	  break;
	  
 	case Symbol::TIMES:
	  m_painter -> drawLine ( o_x - i_sym_size, o_y - i_sym_size,
				  o_x + i_sym_size, o_y + i_sym_size );
	  m_painter -> drawLine ( o_x + i_sym_size, o_y - i_sym_size,
				  o_x - i_sym_size, o_y + i_sym_size );
 	  break;
	  
 	default:
 	  break;
 	
	}
    }
  
  m_painter->restore();

}

QPen
QtViewImp::
createPen ( const Color & color, double size, Line::Style style )
{
  int rgb[3];
  rgb[0] = color.getRed();
  rgb[1] = color.getGreen();
  rgb[2] = color.getBlue();
  QColor qcolor ( rgb[0], rgb[1], rgb[2] );

  unsigned int isize = static_cast < unsigned int > ( size );
  Qt::PenStyle pen_style = s_line_style [ style ];

  return QPen ( qcolor, isize, pen_style );
}

void
QtViewImp::
drawPolygon ( const std::vector < double > & x,
	      const std::vector < double > & y,
	      const Color & color,
	      const Color & edge
	      )
{
  m_painter->save();

  unsigned int size = x.size();
  assert ( size == y.size() );

  int rgb[3];
  rgb[0] = color.getRed();
  rgb[1] = color.getGreen();
  rgb[2] = color.getBlue();
  QColor qcolor ( rgb[0], rgb[1], rgb[2] );

  int rgb2[3];
  rgb2[0] = edge.getRed();
  rgb2[1] = edge.getGreen();
  rgb2[2] = edge.getBlue();
  QColor qcolor2 ( rgb2[0], rgb2[1], rgb2[2] );

  QPolygonF array ( size );
  transformAndFill ( array, x, y,
		     &QtViewImp::toViewX, &QtViewImp::toViewY );

  m_painter->setBrush(qcolor);
  m_painter->setPen(qcolor2);
  m_painter->drawPolygon(array);

  m_painter->restore();

}

void 
QtViewImp::
drawPolyLine ( const std::vector< double > & x,
	       const std::vector< double > & y,
	       Line::Style style,
	       const Color & color,
	       double size )
{
  m_painter->save();

  QPen pen = createPen ( color, size, style );
  m_painter->setPen ( pen );
  m_painter -> setBrush ( Qt::NoBrush );
  drawMethod ( x, y, style, 0 );

  m_painter->restore();

}

void 
QtViewImp::
drawLines ( const std::vector< double > & x,
	    const std::vector< double > & y,
	    Line::Style style,
	    const Color & color,
	    double size )
{

  m_painter->save();

  QPen pen = createPen ( color, size, style );
  m_painter->setPen ( pen );

  unsigned int xsize = x.size();
  assert ( xsize == y.size() );

  QPolygonF array ( xsize );
  transformAndFill ( array, x, y,
		     &QtViewImp::toViewX, &QtViewImp::toViewY );

  m_painter->drawLines ( array );
  m_painter->restore();

}

void
QtViewImp::
drawColorLines ( const std::vector< double > & x,
		 const std::vector< double > & y,
		 Line::Style style,
		 const std::vector < Color > & colors,
		 double size )
{
  unsigned int ssize = x.size();
  assert ( ssize == y.size() );
  assert ( ssize == colors.size() );

  for ( unsigned int i = 0; i < ssize; i+=2 ) {
    
    m_painter->save();
    const Color & color = colors[i];
    QColor qcolor ( color.getRed (), color.getGreen (), color.getBlue() );
    QPen pen ( qcolor, static_cast < int > (size) );
    Qt::PenStyle pen_style = s_line_style [ style ];
    pen.setStyle ( pen_style );
    m_painter->setPen ( pen );
      
    m_painter->drawLine ( QPointF(toViewX(x[i]), toViewY(y[i])),
			  QPointF(toViewX(x[i+1]), toViewY(y[i+1])));

    m_painter->restore();

  }
  
}

/** @bug bool argument is not used.
 */
void 
QtViewImp::
drawViewLines ( const std::vector< double > & x,
		const std::vector< double > & y,
		Line::Style style,
		bool,
		double size )
{

  m_painter->save();

  QPen pen ( (m_painter->pen()).color(), (int)(size) );
  Qt::PenStyle pen_style = s_line_style [ style ];
  pen.setStyle ( pen_style );
  m_painter->setPen ( pen );

  drawViewMethod ( x, y, style, 0 ); // last argument was default color

  m_painter->restore();

}

void 
QtViewImp::
drawViewLines ( const std::vector< double > & x,
		const std::vector< double > & y,
		Line::Style style,
		const Color & color,
		double size )
{

  m_painter->save();

  QPen pen = createPen ( color, size, style );
  m_painter->setPen ( pen );

  unsigned int xsize = x.size();
  assert ( xsize == y.size() );

  QPolygonF array ( xsize );
  transformAndFill ( array, x, y,
		     &QtViewImp::toCanvasX, &QtViewImp::toCanvasY );

  m_painter->drawLines(array);
  m_painter->restore();

}

void 
QtViewImp::
draw_Text ( const std::string &s, 
	    double xx, double yy, double fontsize, 
	    double angle, char xp, char yp, bool resize,
	    QFont & font, const QColor & color  )
{
  if ( fontsize > 0 ) {
    font.setPointSize( fontsize );
    font.setPixelSize( fontsize );
  }
  
  if ( m_painter == 0 ) return;

  m_painter->setFont ( font );

  m_painter -> setPen ( color );

  QString qstring ( s.c_str() );

  QFontMetrics metrics1 = m_painter->fontMetrics();
  QRect new_rect1 = metrics1.boundingRect ( qstring );
  int h1 = new_rect1.height ();
  if ( fontsize > 0 ) {
    while ( h1 >= fontsize ){
    
      fontsize-- ;

      if ( fontsize < 1 ) break;

      font.setPixelSize ( fontsize );

      QFontMetrics metrics2 ( font );
      QRect new_rect2 = metrics2.boundingRect ( qstring );
      int h2 = new_rect2.height ();

      h1 = h2;
    }
  }

  if ( resize == true ) {

    // This part of the code makes sense only for TextReps. Nobody else
    // should call draw_Text with resize == true. It will not
    // work if the text rep draws the strings that appear on the bottom of
    // the display before the ones that appear near the top.

    QFontMetrics metrics = m_painter->fontMetrics();
    QRect new_rect = metrics.boundingRect ( qstring );
    int lines = qstring.count('\n');

    QStringList sl = qstring.split('\n');
    int maxw = 0;
    for ( QList<QString>::size_type i = 0; i < sl.count (); i++ ) {
      QString s = sl[i];
      QRect sr = metrics.boundingRect ( s );
      maxw = std::max ( maxw, sr.width () );
    }
    int w = new_rect.width ();
    int h = new_rect.height ();
    h *= ( lines + 1 );

    // fix the height if this is not top string
    double delta_y = yy;
    if ( delta_y > 0 )
      h += delta_y;

    // fix the width if this is not left positioned string
    double delta_x = xx;
    if ( delta_x > 0 )
      w += delta_x;

    // need a little extra of the width for some reason.
#ifdef _MSC_VER
    maxw *= 1.6;
#endif
    setDrawRect ( getDrawRect().getX(), getDrawRect().getY(),
	1.2 * maxw, 1.2 * h );
  }

  double width = getDrawRect().getWidth();
  double height = getDrawRect().getHeight();

  QRect text_rect = m_painter->boundingRect ( 0, 0,
					  height, width,
					  Qt::AlignLeft | Qt::AlignTop,
					  qstring );
  double dx = 0;
  double dy = 0;

  if ( angle == 0.0 )
    {
      
      switch ( xp ) {
      case 'l' :
      case 'L' :
	dx = xx;
	break;
      case 'c' :
      case 'C' :
	dx = xx - text_rect.width () / 2;
	break;
      case 'r' :
      case 'R' :
	dx = xx - text_rect.width ();
	break;
      default:
	dx = xx;
      }

      switch ( yp ) {
      case 't' :
      case 'T' :
	dy = yy;
	break;
      case 'c' :
      case 'C' :
	dy = yy - text_rect.height () / 2;
	break;
      case 'b' :
      case 'B' :
	dy = yy - text_rect.height ();
	break;
      default:
	dy = yy;
      }

      text_rect.translate ( dx, dy );
      text_rect.setWidth ( text_rect.width() + 2 );
      text_rect.setHeight ( text_rect.height() + 2 );

      m_painter->drawText ( text_rect, Qt::AlignLeft | Qt::AlignTop,
			    qstring );
    }

  else // angle not 0.0
    {
      m_painter->save();
      
      m_painter->translate ( xx, yy );
      
      m_painter->rotate ( - angle );

      switch ( xp ) {
      case 'l' :
      case 'L' :
	dx = 0;
	break;
      case 'c' :
      case 'C' :
	dx = 0 - text_rect.width () / 2;
	break;
      case 'r' :
      case 'R' :
	dx = 0 - text_rect.width ();
	break;
      default:
	dx = 0;
      }

      switch ( yp ) {
      case 't' :
      case 'T' :
	dy = 0;
	break;
      case 'c' :
      case 'C' :
	dy = 0 - text_rect.height () / 2;
	break;
      case 'b' :
      case 'B' :
	dy = 0 - text_rect.height ();
	break;
      default:
	dy = 0;
      }

      text_rect.translate ( dx, dy );
      text_rect.setWidth ( text_rect.width() + 2 );
      text_rect.setHeight ( text_rect.height() + 2 );
	    
      m_painter->drawText ( text_rect, Qt::AlignLeft | Qt::AlignTop,
			    qstring );
      
       m_painter->restore();

    }

}

/** @note This method is only here to avoid introducing FontBase to
    SIP based Python extension module.
*/
void 
QtViewImp::
drawText ( const std::string &s, 
	   double xx, double yy,
	   double fontsize, double angle,
	   char xp, char yp, bool resize )
{
  Color black; // default constructor
  drawText ( s, xx, yy, fontsize, angle, xp, yp, resize, 0, & black );
}

void 
QtViewImp::
drawText ( const std::string &s, 
	   double xx, double yy,
	   double fontsize, double angle,
	   char xp, char yp, bool resize,
	   const FontBase * font, const Color * color )
{
  QColor qcolor = Qt::black;

  if (color != NULL)
	qcolor.setRgb(color->getRed(), color->getGreen(), color->getBlue());
  m_painter -> setPen ( qcolor );

  if ( font != NULL ) {
    const QtFont * qtfont = dynamic_cast < const QtFont * > ( font );
    const QFont & qfont = qtfont -> font();
    QFont & qf = const_cast < QFont & > ( qfont );

    draw_Text ( s, xx, yy, fontsize, angle, 
		xp, yp, resize, qf, qcolor );
  } else {
    draw_Text ( s, xx, yy, fontsize, angle, 
		xp, yp, resize, m_font_default, qcolor );
  }
}

void 
QtViewImp::
drawSquare ( double x1, double y1, double x2, double y2,
	     int red, int green, int blue )
{
  // If reversed, larger coordinate is to the left
  if (m_plotter -> isReverse()) {
    if (x1<x2) std::swap(x1,x2);
  } else {
    if (x2<x1) std::swap(x1,x2);
  }

  if (y2<y1) std::swap(y1,y2);
  
  QRectF rect ( toViewX(x1), toViewY(y1), toViewX(x2) - toViewX(x1),
	toViewY(y2) - toViewY(y1));

  const QColor color ( red, green, blue );

  m_painter->fillRect ( rect, color );
}

void 
QtViewImp::
drawViewSquare ( double x1, double y1, double x2, double y2,
		 int red, int green, int blue )
{
  QRectF rect (toCanvasX(x1),toCanvasY(y2),toCanvasX(x2) - toCanvasX(x1) + 1.,
	toCanvasY(y1) - toCanvasY(y2) + 1.);
  QColor color ( red, green, blue );

  m_painter->fillRect ( rect, color );
}

void QtViewImp::setCrossX ( double val )
{
  m_plotter->setCrossX ( val );
}

void QtViewImp::setCrossY ( double val )
{
  m_plotter->setCrossY ( val );
}


void
QtViewImp::
setDefaultFont ( const QFont& font )
{
  m_font_default = font;
}

const QFont &
QtViewImp::
defaultFont()
{
  return m_font_default;
}

void 
QtViewImp::
drawImage ( const std::string &filename, int position)
{
  QPixmap pixmap;

  // Manually add the directory. 
  QString fn ( ("temp_latex/"+filename).c_str() );
  switch ( position ){
  case 0: // Scale to fit the plot
    if (pixmap.load(fn)) {
      Rect rect = getDrawRect();
      double h = getDrawRect().getHeight();
      double w = h*pixmap.width()/pixmap.height();
      setDrawRect(rect.getX(),rect.getY(),w,h);
      QRectF qrect = QRectF(0, 0, w, h);
      m_painter->drawPixmap( qrect, pixmap, QRectF() ); 
    }
    break;
    
  case 1:
    if (pixmap.load(fn)) {
      Rect rect = getDrawRect ();
      Rect mr = getMarginRect ();
      double x = mr.getX();
      double h = pixmap.height()*0.8;
      double w = pixmap.width()*0.8;

      // Limit the size
      if (h > 36.0) {
	w *= 36.0 / h;
	h = 36.0;
      }

      if (w > mr.getWidth() * 0.8) {
	double ratio = mr.getWidth() * 0.8 / w ;
	h *= ratio;
	w *= ratio;
      }

      x+=mr.getWidth()/2.0-w/2.0;
      QRectF qrect = QRectF(x, 0 /* y */, w, h);
      m_painter->drawPixmap (qrect, pixmap, QRectF());
    }
    break;

  case 2:
    if (pixmap.load(fn)) {
      Rect rect = getDrawRect ();
      Rect mr = getMarginRect ();
      double x = mr.getX();
      double y = 0;
      double h = pixmap.height();
      double w = pixmap.width();

     // Limit the size
      if (h > 40.0) {
	w *= 40.0 / h;
	h = 40.0;
      }

      if ( w>mr.getWidth() * 0.8 ) {
	double ratio = mr.getWidth() * 0.8 / w ;
	h *= ratio;
	w *= ratio;
      }


      x += mr.getWidth()/2.0-w/2.0;
      y += rect.getHeight()-0.9*h;
      QRectF qrect = QRectF(x, y, w, h);
      m_painter->drawPixmap (qrect, pixmap, QRectF());
    }
    break;

  case 3:
    if (pixmap.load(fn)) {
      QTransform t;
      t.rotate(-90.0);
      pixmap = pixmap.transformed(t);

      Rect rect = getDrawRect ();
      Rect mr  = getMarginRect ();
      double y = mr.getY();
      double h = pixmap.height();
      double w = pixmap.width();

     // Limit the size
      if (w > 40.0) {
	h *= 40. / w;
	w = 40.;
      }

      if (h>mr.getHeight() * 0.8) {
	double ratio = mr.getHeight() * 0.8 / h ;
	h *= ratio;
	w *= ratio;
      }

      y += mr.getHeight()/2.0-h/2.0;
      QRectF qrect = QRectF(0 /* x */, y, w, h);
      m_painter->drawPixmap (qrect, pixmap, QRectF());
    }
    break;

  case 4:
    if ( pixmap.load ( fn ) ) {
      Rect rect = getDrawRect ();
      Rect mr = getMarginRect ();
      double x = mr.getX();
      double y = mr.getY();
      double h = pixmap.height()*0.7;
      double w = pixmap.width()*0.7;

     // Limit the size
      if (h > 34.0) {
	w *= 34.0 / h;
	h = 34.0;
      }

      if (w > mr.getWidth() * 0.8) {
	double ratio = mr.getWidth() * 0.8 / w ;
	h *= ratio;
	w *= ratio;
      }

      x += mr.getWidth()/2.0-w/2.0;
      y += -30.0-h;
      QRectF qrect = QRectF(x, y, w, h);
      m_painter->drawPixmap (qrect, pixmap, QRectF());
    }
    break;
  }

  // TODO: Show error if failed to load the image.
}


void
QtViewImp::
drawLatex ( const std::string &eq, int position )
{
  std::map< const std::string, std::string>::iterator it = m_eq_png.find(eq);
  if ( it!=m_eq_png.end() ) {
    drawImage (m_eq_png[eq], position);
  }
  else {
    // Work in the temp_latex directory
    assert(QDir::setCurrent("temp_latex"));

    // Generate the filenames. It's unique and based on the current time. Need millisecond accuracy.
    QString image = qApp->translate("QtViewImp", "image");
    QString current_time = QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");
    QString tex_filename = image+current_time+".tex";
    QString png_filename = image+current_time+".png";
    QString ps_filename  = image+current_time+".ps";
    QString dvi_filename = image+current_time+".dvi";
    QString tmp_filename = image+current_time+".tmp";
    QString aux_filename = image+current_time+".aux";
    QString log_filename = image+current_time+".log";

    // Command 1 to 3 generate the png file and command 4 remove all temp files
    QString command1 = "latex -interaction=nonstopmode "+tex_filename+" > "+tmp_filename;
    QString command2 = "dvips -q -o "+ps_filename+" "+dvi_filename;
    QString command3 = "gs < /dev/null -dTextAlphaBits=4 -dGraphicsAlphaBits=4 -sDEVICE=ppmraw -sOutputFile=- -r150 -q -dNOPAUSE "+ps_filename+" | pnmcrop -white | pnmmargin -white 10 | pnmtopng -interlace - >"+png_filename;
    QString command4 = "rm "+tex_filename+" "+ps_filename+" "+dvi_filename+" "+tmp_filename+" "+aux_filename+" "+log_filename;
    
    // Write the Latex text to file and call the commands to generate the image
    QFile file(tex_filename);
    if (file.open(QIODevice::WriteOnly) ) {
      QTextStream stream(&file);
      // Generate the Latex file ( wither header and ending )
      stream << "\\documentclass{article}\n";
      stream << "\\pagestyle{empty}\n";
      stream << "\\usepackage{xspace,amssymb,amsfonts,amsmath}\n";
      stream << "\\usepackage{mathptmx}\n";
      stream << "\\usepackage{color}\n";
      stream << "\\begin{document}\n";
      stream << "\\begin{displaymath}\n";
      stream << eq.c_str() <<"\n";
      stream << "\\end{displaymath}\n";
      stream << "\\end{document}\n";
      file.close();
      
      system(command1.toAscii().constData());
      system(command2.toAscii().constData());
      system(command3.toAscii().constData());
      system(command4.toAscii().constData());
      
      std::string png_fn = png_filename.toAscii().constData();
      m_eq_png[eq]=png_fn;
      
      // Make sure return to the original working directory.
      QDir::setCurrent("..");
      drawImage(png_fn, position);
    } else {
      QDir::setCurrent("..");
      // TODO: Show error if failed to open the text file.
    }
  }
}

bool
QtViewImp::
handleMousePress(QPointF where)
{
	QRectF border(0,0,getDrawRect().getWidth(),getDrawRect().getHeight());

      #define qptdist(a,b) hypot((a-b).x(),(a-b).y())
      #define clickMargin 10

	if (qptdist(where,border.topLeft()) < clickMargin)
		m_dragging = upper_left;
	else if (qptdist(where,border.topRight()) < clickMargin)
		m_dragging = upper_right;
	else if (qptdist(where,border.bottomLeft()) < clickMargin)
		m_dragging = lower_left;
	else if (qptdist(where,border.bottomRight()) < clickMargin)
		m_dragging = lower_right;
	else if (qptdist(where,QPointF(border.left() + border.width()/2.,
	    border.top())) < clickMargin)
		m_dragging = upper_middle;
	else if (qptdist(where,QPointF(border.left() + border.width()/2.,
	    border.bottom())) < clickMargin)
		m_dragging = lower_middle;
	else if (qptdist(where,QPointF(border.left(), border.top() +
	    border.height()/2.)) < clickMargin)
		m_dragging = middle_left;
	else if (qptdist(where,QPointF(border.right(), border.top() +
	    border.height()/2.)) < clickMargin)
		m_dragging = middle_right;
	else
		m_dragging = none;

   #undef qptdist
   #undef clickMargin

	return (m_dragging != none);
}

void
QtViewImp::
handleMouseRelease()
{
	m_dragging = none;
}

bool
QtViewImp::
handleMouseDrag(QPointF where)
{
	if (m_dragging == none)
		return false;

	QRectF border(0,0,getDrawRect().getWidth(),getDrawRect().getHeight());

	switch (m_dragging) {
		case upper_left:
			border.setTopLeft(where);
			break;
		case upper_right:
			border.setTopRight(where);
			break;
		case lower_left:
			border.setBottomLeft(where);
			break;
		case lower_right:
			border.setBottomRight(where);
			break;
		case upper_middle:
			border.setTop(where.y());
			break;
		case lower_middle:
			border.setBottom(where.y());
			break;
		case middle_left:
			border.setLeft(where.x());
			break;
		case middle_right:
			border.setRight(where.x());
			break;
	}

    #define minDim 80
	if (border.width() > minDim && border.height() > minDim) 
		setDrawRect(getDrawRect().getX() + border.x(),
		    getDrawRect().getY() + border.y(),
		    border.width(), border.height());
    #undef minDim

	return true;
}

