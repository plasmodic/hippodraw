/* -*- mode: c++ -*-
 *
 * Hippo OpenGLView class interface
 *
 */

#ifndef Hippo_OpenGLView_h
#define Hippo_OpenGLView_h

// Inheritance :
#include "graphics/DataView.h"

namespace hippodraw { 
  class TextTTF;
}

/** The ViewBase class using OpenGL for drawing.  @attention This
    class was contributed by its author as an experiment to evaluate
    the core library classes with OpenGL.  The author reports: "Now
    the plotting is very close to what you have with Qt drawing lib."
    This class and others in this directory require CMT for its build.

    @author Guy Barrand <barrand@lal.in2p3.fr>
*/
// class OpenGLView : public hippodraw::ViewBase {
class OpenGLView : public hippodraw::DataView {
public: // ViewerBase :
  virtual void update ( const hippodraw::Observable * );
  virtual void drawLines ( const std::vector< double > & x,
			   const std::vector< double > & y,
			   hippodraw::Line::Style style,
			   const hippodraw::Color & color,float );
  virtual void drawColorLines ( const std::vector< double > & x,
				const std::vector< double > & y,
				hippodraw::Line::Style style,
				const std::vector <hippodraw::Color> & colors,
				float size );

  virtual void drawViewLines ( const std::vector< double > & x,
			       const std::vector< double > & y,
			       hippodraw::Line::Style style,
			       bool color, float);
  virtual void drawViewLines ( const std::vector< double > & x,
			       const std::vector< double > & y,
			       hippodraw::Line::Style style,
			       const hippodraw::Color & color, float);
  virtual void drawPolyLine ( const std::vector< double > & xpoints,
			      const std::vector< double > & ypoints, 
			      hippodraw::Line::Style style,
			      const hippodraw::Color & color , float);
  virtual void drawSquare ( double x1, double y1, double x2, double y2,
			    int red, int green, int blue );
  virtual void drawViewSquare ( float x1, float y1, float x2, float y2,
				int red, int green, int blue );
  virtual void drawPoints ( const std::vector<double> & x,
			    const std::vector<double> & y,
			    hippodraw::Symbol::Type type, 
			    float sym_size, 
			    const hippodraw::Color & color );
  virtual void drawPoints ( const std::vector< double > & x,
			    const std::vector< double > & y, 
			    const std::vector< hippodraw::Color > & colors,
			    hippodraw::Symbol::Type type, 
			    float sym_size );
  virtual void drawMag ( float x, float y, int mag, float fontsize );
//   virtual void drawUserText ( const std::string &s, float x, float y,
// 			      float fontsize, float angle,
// 			      char xp, char yp );
  virtual void drawText ( const std::string &s, float x, float y,
			  float fontsize, float angle,
			  char xp, char yp , bool resize = false );
  virtual void drawText ( const std::string &s, float x, float y,
			  float fontsize, float angle,
			  char xp, char yp , bool resize = false,
			  const hippodraw::FontBase * font = 0,
			  const hippodraw::Color * color = 0 );

  virtual hippodraw::Rect getDrawRect () const;
  virtual void setDrawRect ( float x, float y, float w, float h );
  float userToDrawX ( double x ) const;
  float userToDrawXAutoInv ( double x ) const;
  float userToDrawY ( double x ) const;
  float userToDrawColor ( double c ) const;
public:
  OpenGLView ();
  virtual ~OpenGLView ();
  void setRect( double x, double y, double w, double h);
  void setPlotter ( hippodraw::PlotterBase * );
  hippodraw::PlotterBase* getPlotter () const;
private:
  float toViewX ( double datX ) const;
  float toViewY ( double datY ) const;
  float toCanvasX ( double datX ) const;
  float toCanvasY ( double datY ) const;
  float toX ( double x ) const;
  float toY ( double y ) const;
  void draw_Text ( const std::string &s, float x, float y,
		  float fontsize, float angle,
		  char xp, char yp,const hippodraw::FontBase* font = 0 );
private:
  hippodraw::Rect m_draw_rect;  
  hippodraw::TextTTF* m_TTF;
};


#endif // Hippo_OpenGLView_h
