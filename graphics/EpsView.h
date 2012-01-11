/* -*- mode: c++ -*- */

/** @file

hippodraw::EpsView class interface

Copyright (C) 2001-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: EpsView.h,v 1.67 2007/03/05 22:26:18 xiefang Exp $

*/

#ifndef _EpsView_H_
#define _EpsView_H_

#include "DataView.h"

#include <fstream>

namespace hippodraw {

/** The class of views for Encapsulated Postscript generation. 

    @author Sanket Biren Malde <sanket@stanford.edu>
*/
class MDL_HIPPOPLOT_API EpsView : public DataView
{

private:

  std::ofstream m_outfile;

  Rect m_boundingRect;
  
  Rect m_draw_rect;  

  EpsView ( const EpsView & );  

  /** Initializes data for the EPSF file generation. @a filename is the
      name of the file.  The @a x, @a y, @a w, and @a h are the x, y,
      width and height of the bounding rectangle. 
  */
  void initPlot ( const std::string & filename, 
		  double x, double y, double w, double h );

  /** Does a PostScript lineto.
   */
  void lineTo ( double x, double y );

  /** Does a PostScript moveto.
   */
  void moveTo ( double x, double y );

  /** Uses PostScript setdash to set the line style.
   */
  void setDash ( hippodraw::Line::Style style );

  /** Sets the line width with PostScript setlinewidth.
   */
  void setLineWidth ( double size );

  /** Does a PostScript setrgbcolor.  Converts integer 0 to 255 scale
      of the Color object to PostScript double 0. to 1. scale and sets
      the RGB color.
   */
  void setRgbColor ( const Color & color );

  /** Does a PostScript setrgbcolor.  Converts integer 0 to 255 scale
      to PostScript double 0. to 1. scale and sets
      the RGB color.
   */
  void setRgbColor ( int red, int green, int blue );

  /** Draws a symbol.  Draws the symbol at @a x and @a y
      of type @a type and size @a size.
   */
  void drawSymbol ( hippodraw::Symbol::Type type,
		    double x, double  y,
		    double size );

  virtual void draw_Text ( const std::string &s, double x, double y,
			  double fontsize, double angle,
			  char xp, char yp, const FontBase * );

public:

  /** Creates an %EpsView attached to @a file with a bounding
      rectangle. 
  */
  EpsView ( const std::string file, double x, double y, double w, double h );

  virtual ~EpsView ();

  /** Terminate the PostScript file.
   */
  virtual void closeFile ();

  virtual void drawLines ( const std::vector< double > & x,
			   const std::vector< double > & y,
			   hippodraw::Line::Style style,
			   const Color & color,
			   double size );

  virtual void drawColorLines ( const std::vector< double > & x,
				const std::vector< double > & y,
				hippodraw::Line::Style style,
				const std::vector < Color > & color,
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

  virtual void drawPolygon ( const std::vector< double > & xpoints,
			     const std::vector< double > & ypoints, 
			     const Color & color,
			     const Color & edge );

  virtual void drawSquare ( double x1, double y1, double x2, double y2,
			    int red, int green, int blue );
  
  virtual void drawViewSquare ( double x1, double y1, double x2, double y2,
				int red, int green, int blue );
  
  virtual void drawPoints ( const std::vector<double> & x,
			    const std::vector<double> & y,
			    hippodraw::Symbol::Type type, 
			    double sym_size, 
			    const Color & color );

  virtual void drawPoints ( const std::vector< double > & x,
			    const std::vector< double > & y, 
			    const std::vector< Color > & colors,
			    hippodraw::Symbol::Type type, 
			    double sym_size );

  virtual void drawText ( const std::string &s, double x, double y,
			  double fontsize, double angle,
			  char xp, char yp, bool resize, 
			  const FontBase * font,
			  const Color * color );

  virtual void update ( const Observable * );
  virtual Rect getDrawRect () const;
  virtual void setDrawRect ( double x, double y, double w, double h );

  double userToDrawX ( double x ) const;
  double userToDrawXAutoInv ( double x ) const;
  double userToDrawY ( double x ) const;
  double userToDrawColor ( double c ) const;
  double toViewX ( double datX ) const;
  double toViewY ( double datY ) const;
  double toX ( double x ) const;
  double toY ( double y ) const;

};

} // namespace hippodraw

#endif // _EpsView_H_
