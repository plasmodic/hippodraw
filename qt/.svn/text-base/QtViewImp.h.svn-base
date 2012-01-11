/* -*- mode: c++ -*- */

/** @file

hippodraw::QtViewImp  class interface

Copyright (C) 2001-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: QtViewImp.h,v 1.116 2007/03/23 22:41:33 xiefang Exp $

*/

#ifndef _QtViewImp_H_
#define _QtViewImp_H_

#include "libqthippo.h"

#include "graphics/DataView.h"

#ifdef _MSC_VER
#include <msdevstudio/MSconfig.h>
#endif

#include <qfont.h>
#include <qnamespace.h>
#include <qpoint.h>

#include <map>

class QObject;
class QPainter;
class QPointArray;
class QRect;
class QPolygonF;

namespace hippodraw {

/** The class of derived from ViewBase for drawing to the screen and
    PostScript generation using Qt toolkit.  This class is most of the
    implementation of a concrete view.  A derived class is needed to
    handle size attributes.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket Biren Malde <sanket@stanford.edu>
    @author Xie Fang <xiefang@stanford.edu>
*/

class MDL_QTHIPPOPLOT_API QtViewImp : public DataView
{
private:

  /** The mapping from LineStyle::Type to QPen PenStyle.
   */
  static std::map < hippodraw::Line::Style, Qt::PenStyle > s_line_style;

  /** The mapping from LaTex equation to PNG filename. 
   */
  std::map <const std::string, std::string> m_eq_png;
  
  /** Creates a QPen object from the arguments.
   */
  QPen createPen ( const Color & color, 
		   double size, 
		   hippodraw::Line::Style style );

  /** Possible locations where the mouse cursor was clicked */
  int m_dragging;
  
protected:

  /** The receiver of update messages. 
   */
  QObject * m_inspector;

  /** The current Qt QPainter object. */
  QPainter * m_painter;
  
  /* The font to be used unless overridden by drawing member function.
   */
  QFont m_font_default;

  /** Responds to update message from Observable. 
   */
  virtual void update ( const Observable * display );
  
  /** Converts coordinate from view space to canvas space. */
  virtual double toCanvasX ( double dx ) const = 0;

  /** Converts coordinate from view space to canvas space. */
  virtual double toCanvasY ( double dy ) const = 0;

  /* Handle mouse events to control resizing */
  bool handleMousePress(QPointF where);
  bool handleMouseDrag(QPointF where);
  void handleMouseRelease();
  
private:
  /** Fills the @a array with QPointF objects whose coordinates are in
      the QPaintDevice space from the vectors @a x and @a y whose
      coordinates are in the data space. The member functions @a xfunc
      and @a yfunc of the QtViewImp class are used for the
      transformation. */
  void transformAndFill ( QPolygonF & array, 
			  const std::vector < double > & x,
			  const std::vector < double > & y,
			  double (QtViewImp::* xfunc ) ( double ) const,
			  double (QtViewImp::* yfunc ) ( double ) const );

  /** Draws in the view using the specified method. The coordinates
      are in the user space. */
  void drawMethod ( const std::vector< double > & x,
		    const std::vector< double > & y,
		    int opt1, 
		    int opt2 );
			  
  virtual void drawPoints ( const std::vector<double> & x,
			    const std::vector<double> & y,
			    hippodraw::Symbol::Type type, 
			    double sym_size, 
			    const Color & color );

  /** Draws in the view using the specified method. The coordinates 
      are in the view space. 
      
      @todo We may have function like this that takes pointer to
      member function of QPainter as first argument. 
  */
  void drawViewMethod ( const std::vector< double > & x,
			const std::vector< double > & y,
			int opt1, 
			int opt2 );

public:

  /** The constructor.  
      @param plotter The plotter of the class derived from
      PlotterBase to be used in a drawing.

      @todo Lots more methods could be protected.
  */
  QtViewImp ( PlotterBase * plotter );

  QtViewImp ();

  /** Sets the receiver of update messages.  When this object receives
      update() from one of it Observable objects, it emits a
      custom event message to this object, if one exists. 
  */
  void setInspector ( QObject * );

  /** Converts the data space coordinate X to the view space coordinate X. */
  virtual double toViewX ( double datX ) const = 0;

  /** Converts the data space coordinate Y to the view space coordinate Y. */
  virtual double toViewY ( double datY ) const = 0;

  /** Fills the picked point vector. Converts the view coordinates @a
      x and @a y to data coordinates.  The results are put in the
      vector @a picked.
   */
  virtual void fillPickedPoint ( double x, double y, 
				 std::vector < double > & picked ) const = 0;

  /** Converts the view space coordinate @a x expressed as a @c double
      to one expressed as an @c int. */
  inline int toView ( double x ) const;

  virtual void drawLines ( const std::vector< double > & x,
			   const std::vector< double > & y,
			   hippodraw::Line::Style style,
			   const Color & color,
			   double size );

  virtual void drawColorLines ( const std::vector< double > & x,
				const std::vector< double > & y,
				hippodraw::Line::Style style,
				const std::vector < Color > & colors,
				double size );

  virtual void drawViewLines ( const std::vector< double > & x,
			       const std::vector< double > & y,
			       hippodraw::Line::Style style,
			       bool color,
			       double size );

  virtual void drawViewLines ( const std::vector< double > & x,
			       const std::vector< double > & y,
			       hippodraw::Line::Style style,
			       const Color & color,
			       double size );

  virtual void drawPolyLine ( const std::vector< double > & xpoints,
			      const std::vector< double > & ypoints, 
			      hippodraw::Line::Style style,
			      const Color & color,
			      double size );

  virtual void drawPolygon ( const std::vector < double > & x,
			     const std::vector < double > & y,
			     const Color & color,
			     const Color & edge );

  /** Set the crossX value. */
  void setCrossX ( double val );

  /** Set the crossY value. */
  void setCrossY ( double val );

protected:

  virtual void draw_Text ( const std::string &s, 
			   double x, double y,
			   double fontsize, double angle, 
			   char xp, char yp,
			   bool resize,
			   QFont & font,
			   const QColor & color );

public:

  virtual void drawImage ( const std::string &filename, int position = 0 );

  virtual void drawLatex ( const std::string &eq, int position = 0 );
			  
  virtual void drawText ( const std::string &s, double x, double y,
			  double fontsize, double angle, char xp, char yp,
			  bool resize );

  virtual void drawText ( const std::string &s, double x, double y,
			  double fontsize, double angle, char xp, char yp,
			  bool resize,
			  const FontBase * font,
			  const Color * color );

  virtual void drawPoints ( const std::vector<double> & x,
			    const std::vector<double> & y,
			    hippodraw::Symbol::Type type, 
			    double sym_size );

  virtual void drawPoints ( const std::vector< double > & x,
			    const std::vector< double > & y, 
			    const std::vector< Color > & colors,
			    hippodraw::Symbol::Type type, 
			    double sym_size );

  virtual void drawSquare ( double x1, double y1, double x2, double y2,
			    int red, int green, int blue );

  virtual void drawViewSquare ( double x1, double y1, double x2, double y2,
				int red, int green, int blue );

  double userToDrawX ( double x ) const;
  double userToDrawXAutoInv ( double x ) const;
  double userToDrawY ( double x ) const;
  double userToDrawColor ( double c ) const;
  
  virtual void setDefaultFont( const QFont& font );
  
  virtual const QFont & defaultFont();
  
};

inline int QtViewImp::toView ( double x ) const
{
  return static_cast< int > ( x );
}

} // namespace hippodraw

#endif // _QtViewImp_H_
