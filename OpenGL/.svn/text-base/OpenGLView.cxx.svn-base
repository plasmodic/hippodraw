/* Hippo OpenGLView implementation
 *
 */

// this :
#include "OpenGLView.h"

#ifdef HAVE_CONFIG_H
#include "hippoconfig.h"
#else
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif
#endif

#include "graphics/Color.h"
#include "plotters/CompositePlotter.h"
#include "plotters/PlotterBase.h"
#include "graphics/FontBase.h"

#include "TextTTF.h"
#include "OpenGL.h"

#include <iostream>

#ifdef SSTREAM_DEFECT
#include <strstream>
#else
#include <sstream>
#endif

//#define DEBUG_GL

using namespace hippodraw;

OpenGLView::OpenGLView ()
:m_TTF(0)
{
  m_draw_rect.setRect(0,0,100,100);
  m_TTF = new hippodraw::TextTTF;
}


OpenGLView::~OpenGLView()
{
  delete m_TTF;
}

void OpenGLView::setRect(double aX, double aY, double aW, double aH)
{
  m_draw_rect.setRect(aX,aY,aW,aH);
}


void OpenGLView::setPlotter ( PlotterBase * plotter )
{
  ViewBase::setPlotter ( plotter );
  plotter -> addObserver ( this );
}

PlotterBase* OpenGLView::getPlotter () const
{
  return ViewBase::getPlotter ();
}


void OpenGLView::drawLines ( const std::vector< double > & x,
			     const std::vector< double > & y,
			     hippodraw::Line::Style style,
			     const Color & color ,float)
{
#ifdef DEBUG_GL  
  std::cout << " debug : drawLines " << std::endl;
#endif

  float r = (float)(color.getRed() / 255.0);
  float g = (float)(color.getGreen() / 255.0);
  float b = (float)(color.getBlue() / 255.0);   

  glColor3f(r,g,b);

#ifdef DEBUG_GL  
  std::cout << "debug : drawLines : color " 
            << " r : " << r
            << " g : " << g
            << " b : " << b
            << std::endl;
#endif

  switch (style)
    {
    case Line::Solid:
      break;
    case Line::Dot:
      break;
    case Line::Dash:
      break;
    case Line::DashDot:
      break;
    default:
      break;
    }
  
  for ( unsigned int i = 0; i < x.size(); i = i+2 ) {
    glBegin (GL_LINES);
    glVertex3f (toViewX(x[i]),toViewY(y[i]),0 );
    glVertex3f (toViewX(x[i+1]),toViewY(y[i+1]),0 );
    glEnd ();
#ifdef DEBUG_GL  
    std::cout << "debug : " << x[i] << " " << y[i] << std::endl;
    std::cout << "debug : " << x[i+1] << " " << y[i+1] << std::endl;
#endif
  }
}

void
OpenGLView::
drawColorLines ( const std::vector< double > & x,
		 const std::vector< double > & y,
		 hippodraw::Line::Style style,
		 const std::vector< Color > & colors,
		 float )
{
#ifdef DEBUG_GL  
  std::cout << "hippodraw::OpenGLView::drawColorLines :" << std::endl;
#endif

  for (unsigned int i = 0; i < x.size(); i+=2 ){
    
    const Color & color = colors[i];
    float r = color.getRed () / 255.0;
    float g = color.getGreen () / 255.0;
    float b = color.getBlue () / 255.0;

    glColor3f(r,g,b);

    //FIXME m_outfile << size << " setlinewidth" << endl;

    switch (style) { //FIXME
    case Line::Solid:
      break;
    case Line::Dot:
      break;
    case Line::Dash:
      break;
    case Line::DashDot:
      break;
    default:
      break;
    }

    glBegin (GL_LINES);
    glVertex3f (toViewX(x[i]),toViewY(y[i]),0 );
    glVertex3f (toViewX(x[i+1]),toViewY(y[i+1]),0 );
    glEnd ();
    
  }
}

void OpenGLView::drawViewLines ( 
 const std::vector< double > & x
,const std::vector< double > & y
,hippodraw::Line::Style style
,bool //color
,float
)
{
#ifdef DEBUG_GL  
  std::cout << " debug : drawViewLines" << std::endl;
#endif
  glColor3f(0,0,0);

  switch (style) { //FIXME
  case Line::Solid:
    break;
  case Line::Dot:
    break;
  case Line::Dash:
    break;
  case Line::DashDot:
    break;
  default:
    break;
  }

  for ( unsigned int i = 0; i < x.size(); i = i+2 ) {
    glBegin (GL_LINES);
    glVertex3f (toX(x[i]),toY(y[i]),0 );
    glVertex3f (toX(x[i+1]),toY(y[i+1]),0 );
    glEnd ();
    //std::cout << "debug : " << toX(x[i]) << " " << toY(y[i]) << std::endl;
    //std::cout << "debug : " << toX(x[i+1]) << " " << toY(y[i+1]) << std::endl;
  }
}
 
void OpenGLView::drawViewLines ( const std::vector< double > & x,
				 const std::vector< double > & y,
				 hippodraw::Line::Style style,
				 const Color & color, float)
{
#ifdef DEBUG_GL  
  std::cout << " debug : drawViewLines 2" << std::endl;
#endif

  float r = (float)(color.getRed() / 255.0);
  float g = (float)(color.getGreen() / 255.0);
  float b = (float)(color.getBlue() / 255.0);   

  glColor3f(r,g,b);

  switch (style) { //FIXME
  case Line::Solid:
    break;
  case Line::Dot:
    break;
  case Line::Dash:
    break;
  case Line::DashDot:
    break;
  default:
    break;
  }
  
  for ( unsigned int i = 0; i < x.size(); i = i+2 ) {
    glBegin (GL_LINES);
    glVertex3d (toX(x[i]),toY(y[i]),0 );
    glVertex3d (toX(x[i+1]),toY(y[i+1]),0 );
    glEnd ();
  }
}
 
void OpenGLView::drawPolyLine ( const std::vector< double > & xpoints,
				const std::vector< double > & ypoints, 
				hippodraw::Line::Style style,
				const Color & color, float)
{
#ifdef DEBUG_GL  
  std::cout << " debug : drawPolyLine" << std::endl;
#endif

  float r = (float)(color.getRed() / 255.0);
  float g = (float)(color.getGreen() / 255.0);
  float b = (float)(color.getBlue() / 255.0);   

  glColor3f(r,g,b);

  switch (style) {
  case Line::Solid:
    break;
  case Line::Dot:
    break;
  case Line::Dash:
    break;
  case Line::DashDot:
    break;
  default:
    break;
  }

  glBegin (GL_LINE_STRIP);
  for ( unsigned int i = 0; i < xpoints.size(); i++ ) {
    glVertex3f (toViewX(xpoints[i]),toViewY(ypoints[i]),0 );
#ifdef DEBUG_GL  
    //std::cout << "debug : pol " << i << " " << toViewX(xpoints[i]) << " " << toViewY(ypoints[i]) << std::endl;
#endif
  }
  glEnd ();
}

void OpenGLView::drawSquare ( double x1, double y1, double x2, double y2,
			   int red, int green, int blue )
{
#ifdef DEBUG_GL  
  std::cout << " debug : drawSquare " << std::endl;
#endif

  float r = (float)(red / 255.0);
  float g = (float)(green / 255.0);
  float b = (float)(blue / 255.0);   

  glColor3f(r,g,b);

  glBegin(GL_POLYGON);
  glVertex3f(toViewX(x1),toViewY(y1),0);
  glVertex3f(toViewX(x2),toViewY(y1),0);
  glVertex3f(toViewX(x2),toViewY(y2),0);
  glVertex3f(toViewX(x1),toViewY(y2),0);
  glEnd();
}

void OpenGLView::drawViewSquare ( float x1, float y1, float x2, float y2,
				int red, int green, int blue ){
#ifdef DEBUG_GL  
  std::cout << "hippodraw::OpenGLView::drawViewSquare : " << std::endl;
#endif  
  
  float r = (float)(red / 255.0);
  float g = (float)(green / 255.0);
  float b = (float)(blue / 255.0);   

  glColor3f(r,g,b);

  glBegin(GL_POLYGON);
  glVertex3f(toX(x1),toY(y1),0);
  glVertex3f(toX(x2),toY(y1),0);
  glVertex3f(toX(x2),toY(y2),0);
  glVertex3f(toX(x1),toY(y2),0);
  glEnd();

}

void OpenGLView::drawPoints ( const std::vector<double> & x,
			      const std::vector<double> & y,
			      hippodraw::Symbol::Type type, 
			      float sym_size, 
			      const Color & color )
{
#ifdef DEBUG_GL  
  std::cout << " debug : drawPoints " << std::endl;
#endif

  //printf("debug : type %d %g\n",type,sym_size);

  float r = (float)(color.getRed() / 255.0);
  float g = (float)(color.getGreen() / 255.0);
  float b = (float)(color.getBlue() / 255.0);   

  glColor3f(r,g,b);

  sym_size *= 0.2;

  for (unsigned int i = 0; i < x.size(); i++) {
    switch(type) {
      case hippodraw::Symbol::SQUARE:
	glBegin(GL_LINE_STRIP);
        glVertex3f(toViewX(x[i])-(sym_size/2),toViewY(y[i])-(sym_size/2),0);
        glVertex3f(toViewX(x[i])+(sym_size/2),toViewY(y[i])-(sym_size/2),0);
        glVertex3f(toViewX(x[i])+(sym_size/2),toViewY(y[i])+(sym_size/2),0);
        glVertex3f(toViewX(x[i])-(sym_size/2),toViewY(y[i])+(sym_size/2),0);
        glVertex3f(toViewX(x[i])-(sym_size/2),toViewY(y[i])-(sym_size/2),0);
        glEnd ();
        break;
      case hippodraw::Symbol::SOLIDSQUARE:
	glBegin(GL_POLYGON);
        glVertex3f(toViewX(x[i])-(sym_size/2),toViewY(y[i])-(sym_size/2),0);
        glVertex3f(toViewX(x[i])+(sym_size/2),toViewY(y[i])-(sym_size/2),0);
        glVertex3f(toViewX(x[i])+(sym_size/2),toViewY(y[i])+(sym_size/2),0);
        glVertex3f(toViewX(x[i])-(sym_size/2),toViewY(y[i])+(sym_size/2),0);
        glEnd ();
        break;
      case hippodraw::Symbol::TRIANGLE:
	glBegin(GL_LINE_STRIP);
        glVertex3f(toViewX(x[i])-(sym_size/2),toViewY(y[i])-(sym_size/2),0);
        glVertex3f(toViewX(x[i])+(sym_size/2),toViewY(y[i])-(sym_size/2),0);
        glVertex3f(toViewX(x[i]),toViewY(y[i])+(sym_size/2),0);
        glVertex3f(toViewX(x[i])-(sym_size/2),toViewY(y[i])-(sym_size/2),0);
        glEnd ();
        break;
      case hippodraw::Symbol::FILLED_TRIANGLE:
	glBegin(GL_POLYGON);
        glVertex3f(toViewX(x[i])-(sym_size/2),toViewY(y[i])-(sym_size/2),0);
        glVertex3f(toViewX(x[i])+(sym_size/2),toViewY(y[i])-(sym_size/2),0);
        glVertex3f(toViewX(x[i]),toViewY(y[i])+(sym_size/2),0);
        glEnd ();
        break;
      case hippodraw::Symbol::CIRCLE:
      case hippodraw::Symbol::FILLED_CIRCLE:
        std::cout << " debug : drawPoints : circle : dummy." << std::endl;
	glBegin(GL_LINE_STRIP);
/*
	  s_outfile << toViewX (x[i]) + (sym_size/2) << " "
		    << toViewY (y[i]) << " moveto" << endl;
	  s_outfile << toViewX (x[i]) << " "
		    << toViewY (y[i]) << " "
		  << sym_size/2 << " "
		  << "0.0 360 arc" << endl;
*/
        glEnd ();
        break;
      case hippodraw::Symbol::PLUS:
	glBegin(GL_LINES);
        glVertex3f(toViewX(x[i])-(sym_size/2),toViewY (y[i]),0);
        glVertex3f(toViewX(x[i])+(sym_size/2),toViewY (y[i]),0);
        glVertex3f(toViewX(x[i]),toViewY(y[i])-(sym_size/2),0);
        glVertex3f(toViewX(x[i]),toViewY(y[i])+(sym_size/2),0);
        glEnd ();
        break;
      case hippodraw::Symbol::TIMES:
	glBegin(GL_LINES);
        glVertex3f(toViewX(x[i])-(sym_size/2),toViewY(y[i])-(sym_size/2),0);
        glVertex3f(toViewX(x[i])+(sym_size/2),toViewY(y[i])+(sym_size/2),0);
        glVertex3f(toViewX(x[i])+(sym_size/2),toViewY(y[i])-(sym_size/2),0);
        glVertex3f(toViewX(x[i])-(sym_size/2),toViewY(y[i])+(sym_size/2),0);
        glEnd ();
        break;
      default:
	break;
      }
    }
}

void OpenGLView::drawPoints ( 
 const std::vector< double > & //x
,const std::vector< double > & //y
,const std::vector< Color > & //colors
,hippodraw::Symbol::Type //type
,float //sym_size
)
{
#ifdef DEBUG_GL  
  std::cout << " debug : drawPoints 2" << std::endl;
#endif
  std::cout << " debug : drawPoints 2 : dummy" << std::endl;
  /*
  for (unsigned int i = 0; i < x.size(); i++)
    {
    
      s_outfile << "gsave" << endl;
      s_outfile << hue[i] << " 1.0 1.0 sethsbcolor" << endl;
      s_outfile << "newpath systemdict begin" << endl;

      switch ( type )
	{
	case hippodraw::Symbol::SQUARE:
	case hippodraw::Symbol::SOLIDSQUARE:
	  
	  s_outfile << toViewX (x[i])-(sym_size/2) << " " 
		  << toViewY (y[i])-(sym_size/2) << " moveto" << endl;
	  s_outfile << toViewX (x[i])+(sym_size/2) << " " 
		    << toViewY (y[i])-(sym_size/2) << " lineto" << endl;
	  s_outfile << toViewX (x[i])+(sym_size/2) << " " 
		    << toViewY (y[i])+(sym_size/2) << " lineto" << endl;
	  s_outfile << toViewX (x[i])-(sym_size/2) << " " 
		    << toViewY (y[i])+(sym_size/2) << " lineto" << endl;
	  s_outfile << "closepath" << endl;
	  
	  break;
	  
	case hippodraw::Symbol::TRIANGLE:
	case hippodraw::Symbol::FILLED_TRIANGLE:
	  s_outfile << toViewX (x[i]) << " " 
		    << toViewY (y[i]) + (sym_size/2) << " moveto" << endl;
	  s_outfile << toViewX (x[i]) + (sym_size/2) << " " 
		    << toViewY (y[i])-(sym_size/2) << " lineto" << endl;
	  s_outfile << toViewX (x[i]) - (sym_size/2) << " " 
		    << toViewY (y[i])-(sym_size/2) << " lineto" << endl;
	  s_outfile << "closepath" << endl;
	  break;
	  
	case hippodraw::Symbol::CIRCLE:
	case hippodraw::Symbol::FILLED_CIRCLE:
	  s_outfile << toViewX (x[i]) + (sym_size/2) << " "
		  << toViewY (y[i]) << " moveto" << endl;
	  s_outfile << toViewX (x[i]) << " "
		  << toViewY (y[i]) << " "
		  << sym_size/2 << " "
		  << "0.0 360 arc" << endl;
	  break;

	case hippodraw::Symbol::PLUS:
	  s_outfile << toViewX (x[i])-(sym_size/2) << " " 
		    << toViewY (y[i]) << " moveto" << endl;
	  s_outfile << toViewX (x[i])+(sym_size/2) << " " 
		    << toViewY (y[i]) << " lineto" << endl;
	  s_outfile << toViewX (x[i]) << " " 
		    << toViewY (y[i])-(sym_size/2) << " moveto" << endl;
	  s_outfile << toViewX (x[i]) << " " 
		    << toViewY (y[i])+(sym_size/2) << " lineto" << endl;
	  break;
	  
	case hippodraw::Symbol::TIMES:
	  s_outfile << toViewX (x[i])-(sym_size/2) << " " 
		    << toViewY (y[i])-(sym_size/2) << " moveto" << endl;
	  s_outfile << toViewX (x[i])+(sym_size/2) << " " 
		    << toViewY (y[i])+(sym_size/2) << " lineto" << endl;
	  s_outfile << toViewX (x[i])+(sym_size/2) << " " 
		    << toViewY (y[i])-(sym_size/2) << " moveto" << endl;
	  s_outfile << toViewX (x[i])-(sym_size/2) << " " 
		    << toViewY (y[i])+(sym_size/2) << " lineto" << endl;
	  break;

	default:
	  break;

	}
   
      s_outfile << "end" << endl;
      
      if(filled)
	{
	  s_outfile << "fill grestore" << endl;
	}
      else
	{
	  s_outfile << "stroke grestore" << endl;
	}
    }
  */  
}

void OpenGLView::drawMag ( float x, float y, int mag, float fontsize )
{
#ifdef SSTREAM_DEFECT
  std::ostrstream ost;
#else
  std::ostringstream ost;
#endif

  ost << mag << std::ends;
  const std::string s(ost.str());
#ifdef DEBUG_GL  
  std::cout << " debug : drawMag " << s 
            << " x " << x << " y " << y 
            << " fontsize " << fontsize
            << std::endl;
#endif
  // AxisRep2D::drawXTickLabels do not give good y position
  // when having an y upstream. We try to reverse what is done
  // here (yp=t) and correct.

  // Reverse :
/*
  float font_size = fontsize / 0.75;
  y -= m_draw_rect.getY();
  y -= (font_size * 1.2);
  // Correct :
  y -= (font_size * 1.2);
  y += m_draw_rect.getY();
*/

  draw_Text ( std::string("x10"), toX(x), toY(y), fontsize, 0, 'l', 't' );

  draw_Text ( s, toX(x+2*fontsize), toY(y+fontsize*0.2), fontsize, 0, 'l', 't' );
}

// void OpenGLView::drawUserText ( const std::string &s, float x, float y,
// 			     float fontsize, float angle,
// 			     char xp, char yp )
// {
// #ifdef DEBUG_GL  
//   std::cout << " debug : drawUserText " << s << std::endl;
// #endif
//   draw_Text ( s, toViewX (x), toViewY (y), fontsize, angle, xp , yp);
// }

void OpenGLView::drawText ( const std::string &s, float x, float y,
			 float fontsize, float angle,
			 char xp, char yp , bool)
{
#ifdef DEBUG_GL  
  std::cout << " debug : drawText " << s << std::endl;
#endif
  draw_Text ( s, toX(x), toY(y), fontsize, angle, xp, yp );
}

void OpenGLView::drawText ( const std::string &s, float x, float y,
			    float fontsize, float angle,
			    char xp, char yp, bool,
			    const FontBase * font,
			    const Color * color )
{
#ifdef DEBUG_GL  
  std::cout << " debug : drawText2 " << s << std::endl;
#endif
  draw_Text ( s, toX(x), toY(y), fontsize, angle, xp, yp, font );
}

void OpenGLView:: update (  
 const Observable * //display 
)
{
  //std::cout << " debug : update : dummy." << std::endl;
}

float OpenGLView::userToDrawX ( double x ) const
{
//   return m_plotter->userToMarginX( x );
  return userToMarginX( x );
}

float OpenGLView::userToDrawY ( double y ) const
{
//   return m_plotter->userToInvertedMarginY( y );
  return userToInvertedMarginY( y );
}

float OpenGLView::userToDrawColor ( double c ) const
{
 //  return m_plotter->userToMarginColor( c );
// }
return userToMarginColor( c );
}

Rect OpenGLView::getDrawRect() const
{
  return m_draw_rect;
}
void OpenGLView::setDrawRect ( 
 float //x
,float //y
,float //w
,float //h 
) {
  std::cout << "hippodraw::OpenGLView::setDrawRect : dummy " << std::endl;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void OpenGLView::draw_Text ( 
 const std::string &s
,float x
,float y
,float fontsize
,float angle
,char xp
,char yp
,const FontBase* font
)
{
#ifdef DEBUG_GL  
#endif
  std::cout << " debug : draw_Text \"" << s 
	    << "\" : x " << x
	    << " y " << y
	    << " fontsize " << fontsize
	    << " angle " << angle
	    << " xp " << xp
	    << " yp " << yp
	    << " font " << font
	    << std::endl;
  if(font) 
    std::cout << " debug : draw_Text \"" << font->family() 
	      << "\" : pointSize " << font->pointSize()
	      << std::endl;
  if(angle==90)
    glRasterPos2f(x+12,y);
  else 
    glRasterPos2f(x,y);

  hippodraw::TextTTF::Justification hJ;
  switch (xp) {
  case 'c':
  case 'C':
    hJ = hippodraw::TextTTF::CENTER;
    break;
  case 'r':
  case 'R':
    hJ = hippodraw::TextTTF::RIGHT;
    break;
  default:
    hJ = hippodraw::TextTTF::LEFT;
    break;
  }

  hippodraw::TextTTF::Justification vJ;
  switch (yp){
  case 'c':
  case 'C':
    vJ = hippodraw::TextTTF::MIDDLE;
    break;
  case 't':
  case 'T':
    vJ = hippodraw::TextTTF::TOP;
    break;
  default:
    vJ = hippodraw::TextTTF::BOTTOM;
    break;
  }
  
  // fontsize 4 -> TTF pointSize 10
  // fontsize 5 -> TTF pointSize 25  (CERN Personnel)
  /// p = a * f + b
  //  25 = a * 5 + b;
  //  10 = a * 4 + b;
  //float a = (25-10)/(5-4);
  //float b = 25 - a * 5;
  //float pointSize = a * fontsize + b;
  float pointSize = fontsize * 0.8;

  m_TTF->setString(s);
  m_TTF->setSize(pointSize);
  m_TTF->setJustification(hJ,vJ);
  m_TTF->setRotated(angle==90?true:false);
  m_TTF->render();
}

float OpenGLView::toViewX ( double datX ) const
{
  //return toX(m_plotter->userToMarginX ( datX ));
//   return ( m_draw_rect.getX() + m_plotter->userToMarginX ( datX ) );
  return ( m_draw_rect.getX() + userToMarginX ( datX ) );
}

float OpenGLView::toViewY ( double datY ) const
{
  Rect m_boundingRect(m_draw_rect.getX(),
		      m_draw_rect.getY(),
		      m_draw_rect.getWidth(),
		      m_draw_rect.getHeight());
  //From EpsView::toViewY.
  float s1 = 2 * m_boundingRect.getY() + m_boundingRect.getHeight();

  float s2 = 2 * m_draw_rect.getY() + m_draw_rect.getHeight();

//   return ( s1 - ( s2 -  ( ( m_draw_rect.getY() 
// 			    + m_plotter->userToMarginY ( datY ) ) - 
// 			  ( 2 * m_plotter->getMarginRect().getY() ) +
// 			  ( m_draw_rect.getHeight() ) - 
// 			  ( m_plotter->getMarginRect().getHeight() )
  return ( s1 - ( s2 -  ( ( m_draw_rect.getY() 
			    + userToMarginY ( datY ) ) - 
			  ( 2 * getMarginRect().getY() ) +
			  ( m_draw_rect.getHeight() ) - 
			  ( getMarginRect().getHeight() )
			  )
		  )
	   );
}

float OpenGLView::toCanvasX ( double dx ) const
{
  return static_cast < float > ( dx );
}

float OpenGLView::toCanvasY ( double dy ) const
{
  return static_cast < float > ( dy );
  //return toY(dy);
}

float OpenGLView::toX ( double x ) const
{
  return static_cast<float> ( m_draw_rect.getX() + x );
}
float OpenGLView::toY ( double y ) const
{
  return static_cast<float>(m_draw_rect.getY()+m_draw_rect.getHeight()-y);
}
