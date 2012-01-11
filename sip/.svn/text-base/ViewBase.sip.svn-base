/* -*- mode:c++ -*- */

/** @file

SIP interface definition for ViewBase

@note The commented out code requires exposure of more types to SIP.

Copyright (C)  2002, 2003, 2005  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

$Id: ViewBase.sip,v 1.10 2007/02/24 22:24:38 pfkeb Exp $

@author <Paul_Kunz@slac.stanford.edu>

*/

class ViewBase : Observer /Abstract/
{
%TypeHeaderCode
#include "graphics/ViewBase.h"
using hippodraw::ViewBase;
using hippodraw::PlotterBase;
using hippodraw::Observable;
%End


public:

 /** The default constructor. */
  ViewBase();

  /** The following constructor takes a plotter as an argument to use
      with a drawing.  */
  ViewBase ( PlotterBase *  );

  /** The copy constructor. */
  ViewBase ( const ViewBase & );

  /** The virtual destructor. */
  virtual ~ViewBase();

  /** Sets the PlotterBase object to @a plotter. The ViewBase object
      does not own the plotter.  If the plotter is being changed, it
      is up to client code to decide to destroy the previous plotter
      or not. */
  virtual void setPlotter ( PlotterBase *  );

  /** Returns the plotter used by this view. */
  //// PlotterBase * getPlotter () const;

  /** Returns the plotter used by this view. */
  PlotterBase * getPlotter ();

  /** Implements the update method of the Observer pattern.  Derived
      classes should send a message to what ever object is the parent
      of this ViewBase object. 
  */
  virtual void update ( const Observable * ) = 0;

  /** Draws itself. */
  virtual void drawSelf();

  /** Initializes a view before drawing.  The view should check it
      size.  The parameter @a aspect, if greater than 0.0, sets the
      aspect ratio of the view, i.e. the ratio of the width to the
      height.*/
  // virtual void initPlot ( double ) = 0;

  /** Finalizes a view when all drawing has been done. */
  // virtual void endPlot () = 0;

//   /** Draws multiple line segments - that is, a line between x0,y0 and
//       x1,y2, another between x2,y2 and x3,y3 and so on. 
//       @p x pairs of coordinates in user space on the X axis for the beginning 
//          and the end of the line segment.  
//       @p y pairs of coordinates in user space on the Y axis for the beginning 
//          and the end of the line segment.  
//       @p style the line style.  
//       @p color The color 
//       @p size The thickness of the line. */
//   virtual void drawLines ( const std::vector< double > & ,
// 			   const std::vector< double > & ,
// 			   hippodraw::Line::Style ,
// 			   const Color & ,
// 			   float ) = 0;

//   /** Draws multiple line segments, each with a different color - that
//       is, a line between x0,y0 and x1,y1, with color[0] =color[1]
//       another between x2,y2 and x3,y3 with color[2]=color[3] and so
//       on.  

//       @p x pairs of coordinates in user space on the X axis for the
//       beginning and the end of the line segment.

//       @p y pairs of coordinates in user space on the Y axis for the
//       beginning and the end of the line segment.

//       @p style the line style.  

//       @p color The RGB values for the color of the line.

//       @p size The thickness of the line.

//       @bug Could call drawPolyLine for each contour with same color.

//   */
//   virtual void drawColorLines ( const std::vector< double > & ,
// 				const std::vector< double > & ,
// 				hippodraw::Line::Style ,
// 				const std::vector < Color > & ,
// 				float ) = 0;

//   /** Draws multiple line segments.  
//       @p x pairs of coordinates in view space on the X axis for the beginning 
//          and the end of the line segment.  
//       @p y pairs of coordinates in view space on the Y axis for the beginning 
//          and the end of the line segment.  
//       @p style the line style.  
//       @p color if @c true, use the display's point color; otherwise, use 
//          the default color. 
//       @p size The thickness of the line. */
//   virtual void drawViewLines ( const std::vector< double > & ,
// 			       const std::vector< double > & ,
// 			       hippodraw::Line::Style ,
// 			       bool ,
// 			       float  ) = 0;

//    /** Draws multiple line segments.  
//       @p x pairs of coordinates in view space on the X axis for the beginning 
//          and the end of the line segment.  
//       @p y pairs of coordinates in view space on the Y axis for the beginning 
//          and the end of the line segment.  
//       @p style the line style.  
//       @p color The color
//       @p size The thickness of the line. */
//   virtual void drawViewLines ( const std::vector< double > & ,
// 			       const std::vector< double > & ,
// 			       hippodraw::Line::Style ,
// 			       const Color & ,
// 			       float ) = 0;
//  /** Draws a polyline.  
//       @p xpoints points along the X axis.
//       @p ypoints points along the Y axis.
//       @p style the line style. 
//       @p color The color.
//       @p size The thickness of the line. */
//   virtual void drawPolyLine ( const std::vector< double > & ,
// 			      const std::vector< double > & , 
// 			      hippodraw::Line::Style ,
// 			      const Color & ,
// 			      float  ) = 0;

//   /** Draws a text string at a point in the view's coordinate system.
      
//       @param s The string to be drawn.  
      
//       @param x The x coordinate in the view's coordinate system.
      
//       @param y The y coordinate in the view's coordinate system.
      
//       @param fontsize The font size.  If zero, then taken from the
//       font object.
    
//       @param angle The angle to draw.  90 degrees is vertical with the
//       beginning of the text towards the bottom.

//       @param xp The horizontal alignment with respect to the
//       coordinate.  'r' means the coordinate is on the right side of
//       the text, 'c' means centered, and 'l' for the left side.

//       @param yp The vertical alignment with respect to the
//       coordinate. 't' means toward the top, 'c' means centered, and
//       'b' means bottom.

//       @param resize If @c true, the bounding rectangle of the view
//       should be resized to fit the text string.

//       @param font If not a null pointer, then the font object to be
//       used, otherwise the default font will be used.

//   */
//   virtual void drawText ( const std::string & , float , float ,
// 			  float , float angle = 0.0,
// 			  char xp = 'l', char yp = 't',
// 			  bool resize = false,
// 			  const FontBase * font = 0 ) = 0;

  /** Draws a text string at a point in the data coordinate
      system.

      @bug Is this function used somewhere?
  */
//  virtual void drawUserText ( const std::string & , float , float ,
//			      float , float ,
//			      char , char  ) = 0;

  /** Draws the exponent on the axis label.  The arguments stand for
      the starting position of the writing ( x, y ), for the value of
      the magnitude ( mag ), and for the font ( fontsize ).

      @bug @@@@ This method should not exist.  The x, y coordinates of
      the position of the magnitude should be calculated in the
      AxisRepBase class and that class should use drawText.
  */
//  virtual void drawMag ( float , float , int , float ) = 0;

//   /** Draws symbol points.  @a x and @a y are the coordinates, @a type
//       is the type of symbol, and @a sym_size is the size. All symbols
//       are draw in the same color. */
//   virtual void drawPoints ( const std::vector<double> & ,
// 			    const std::vector<double> & ,
// 			    hippodraw::Symbol::Type , 
// 			    float , const Color & ) = 0;

//   /** Draws symbol points.  @a x and @a y are the coordinates, @a type
//       is the type of symbol, and @a sym_size is the size. @a hue is
//       the hue parameters for each point.  The saturation and brightness
//       are set to 1.0. 
//   */
//   virtual void drawPoints ( const std::vector< double > & ,
// 			    const std::vector< double > & , 
// 			    const std::vector< Color > & ,
// 			    hippodraw::Symbol::Type , float  ) = 0;

  /** Draws a colored square.  Used for shading on the cuts and the
      gray plot. */
  virtual void drawSquare ( double , double , double , double ,
			    int , int, int ) = 0;

  /** Draws a colored square in view space. Used for shading on the cuts
      and the gray plot. */
  virtual void drawViewSquare ( float , float , float , float ,
				int , int , int ) = 0;

//   /** Returns the range of the data space on specified axis. 
//    */
//   const Range & getRange ( hippodraw::Axes::Type ) const;

//   /** Is called by the AxisRepBase object (or one of its derived
//    *  objects), and returns the rectangle where the drawing is
//    *  made. */
//   const HippoRectangle & getUserRect () const;

//   /** Returns the drawing Rectangle in the devices coordinate system.
//       Must be implemented in derived class. */
//   virtual HippoRectangle getDrawRect() const = 0;

  /** Sets the drawing Rectangle in the devices coordinate system. */
  virtual void setDrawRect ( float , float , float , float  ) = 0;

//   /** Returns the Rectangle within which points representations are
//       draw.  

//       @todo It might be better to maintain insets instead of separate
//       Rectangle.  In that way, only one rectangle would move with
//       respect to canvas. 
//   */
//   virtual HippoRectangle getMarginRect() const;

  /** Converts a coordinate in user space to drawing space along the X
      axis. */
  virtual float userToDrawX ( double ) const = 0;

  /** Converts a coordinate in user space to drawing space along the
      Y axis. */
  virtual float userToDrawY ( double  ) const = 0;

  /** Converts a coordinate in user space to drawing space along the
      color (X) axis. */
  virtual float userToDrawColor ( double ) const = 0;

  /** Get the aspect ratio. */
  virtual double getAspectRatio () const;

};
