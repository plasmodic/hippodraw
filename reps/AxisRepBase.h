/* -*- mode: c++ -*- */

/** @file

hippodraw::AxisRepBase class interface

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: AxisRepBase.h,v 1.86 2006/11/15 23:57:40 xiefang Exp $ 

*/

#ifndef _AxisRepBase_H_
#define _AxisRepBase_H_

#include "axes/AxesType.h"
#include "graphics/FontBase.h"

#include <vector>

namespace hippodraw {

class AxisModelBase;
class AxisTick;
class BinToColor;
class TransformBase;
class ViewBase;
class Range;

/** The base class for the axis representation hierarchy.  The derived
    classes use the information kept by the AxisModelBase hierarchy
    objects. They communicate with the ViewBase hierarchy objects in
    order to get the information concerning the actual display. They
    are, therefore, carrying out adequate calculations of fonts and
    positions to be drawn in good proportion to the axes, their
    labels, and the main title.

    @request Let user draw border in different color.

    @todo There is lots of duplicated code in the derived classes that
    could be moved to the base.

    @author Stephane Bonneaud  <gandalf@slac.stanford.edu>
    @author Kaustuv <kaustuv@stanford.edu>
    @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API AxisRepBase
{

protected:

  bool m_sci_note_x;
  bool m_sci_note_y;

  /** The x coordinate of the origin of the coordinate system. It is
   *  in fact the x coordinate of the lower left corner of the
   *  rectangle in which the drawing is made. */
  float m_axis_x_origin;

  /** The y coordinate of the origin of the coordinate system. It is
   *  in fact the y coordinate of the lower left corner of the
   *  rectangle in which the drawing is made. */
  float m_axis_y_origin;

  /** The width of the rectangle in which the drawing is made. */
  float m_axis_width;

  /** The height of the rectangle in which the drawing is made. */
  float m_axis_height;
  
  /** Font size for the tick labels.
   */
  double m_font_size;

  /** Font size for the x tick labels.
   */
  double m_x_tick_font_size;

  /** Font size for the y tick labels.
   */
  double m_y_tick_font_size;

  /** Font size for X axis label.
   */
  double m_x_font_size;

  /** Font size for the Y axis label.
   */
  double m_y_font_size;

  /** Font size for the Z axis label.
   */
  double m_z_font_size;
  
  /** The font to be used to for the X label overriding the default font */
  FontBase * m_xLabelFont;
      
  /** The font to be used to for the Y label overriding the default font */
  FontBase * m_yLabelFont;

  /** The font to be used to for the Z label overriding the default font */
  FontBase * m_zLabelFont;

  /** The font to be used to for the Z label overriding the default font */
  FontBase * m_titleFont;
  
  /** The following flag indicates whether the titles have to be drawn
      or not. */
  bool m_draw_titles;

  /** Gets the sizes of the two rectangles (the rectangles of the
   *  drawing on the screen) memorized by the ViewBase object, and
   *  initializes the m_axis_x_origin, m_axis_y_origin, m_axis_width,
   *  and m_axis_height fields. Having the dimensions of the place 
   *  where they will be drawn, the AxisRepBase object will then be
   *  able to calculate the adequate values of the different
   *  components of the drawing. */
  void initAxisRect ( ViewBase & view );
  
  /** Draws the X tick lines. */
  void drawXTickLines ( const AxisModelBase & axisModelX,
			const AxisModelBase & axisModelY,
			const TransformBase & transform,
			ViewBase & view );

  /** Draws the Y tick lines. */
  void drawYTickLines ( const AxisModelBase & axisModelX,
			const AxisModelBase & axisModelY,
			const TransformBase & transform,
			ViewBase & view );

 public:

  /** The default constructor. 
   */
  AxisRepBase();

  /** The copy constructor. */
  AxisRepBase( const AxisRepBase & axis_rep );

  /** The virtual destructor. */
  virtual ~AxisRepBase();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual AxisRepBase * clone() = 0;

  /** Every class which draws has its beginPlot function. It
   *  initializes the data that it requires. (For example, it might
   *  get the size of the actual drawing on the screen in order to set
   *  the good font size of the writing, etc...). */
  void beginPlot ( ViewBase & view );

  
  /** Calculates the font size for the x axis. It is called by
      setFontSize(...). */
  void setXFontSize ( const AxisModelBase & axis_model,
		      ViewBase & view );
  
  /** Set the font to be used to override the default while drawing X label.*/
  void setXLabelFont( FontBase* font );

  /** What font is being used to override the default while drawing X
      label. */
  FontBase* xLabelFont();
  
  /** Calculates the font size for the y axis. It is called by
      setFontSize(...). */
  void setYFontSize ( const AxisModelBase & axis_model, ViewBase & view );
  
  /** Set the font to be used to override the default while drawing Y label.*/
  void setYLabelFont( FontBase* font );

  /** What font is being used to override the default while drawing Y
      label. */
  FontBase* yLabelFont();

  /** Calculates the font size for the z axis, if any. It is called by
      setFontSize(...). */
  void setZFontSize ( const AxisModelBase & axis_model, ViewBase & view );
    
  /** Set the font to be used to override the default while drawing Z label.*/
  void setZLabelFont( FontBase* font );
  
  /** What font is being used to override the default while drawing Z
      label. */
  FontBase* zLabelFont();

  /** Set the font to be used to override the default while drawing title.*/
  void setTitleFont( FontBase* font );
  
  /** What font is being used to override the default while drawing title
      of plot. */
  FontBase* titleFont();
  
  /** Sets the m_draw_titles flag to the set value. */
  void setDrawTitles( bool set );
  
  /** Draws the labels of the x axis. */
  void drawXLabels( const AxisModelBase & axis_model,
		    ViewBase & view, const std::string & x_label );
  /** Draws the labels of the y axis. */
  void drawYLabels( const AxisModelBase & axis_model,
		    ViewBase & view, const std::string & y_label );

  /** Adjusts the font sizes.  The font sizes are adjusted according
      to the size of the view on a canvas.
  */
  virtual void setFontSize( const AxisModelBase * x_axis,
			    const AxisModelBase * y_axis,
			    const AxisModelBase * z_axis,
			    ViewBase & view );
  
  /** Draws the axes lines after transformation.  This forms the
     rectangle in which the plot is drawn. */
  virtual void drawAxesLines ( TransformBase & transform, 
			       ViewBase & view, 
			       const Range & x_range,
			       const Range & y_range);

  /** Draws the cross hairs after transformation. */
  virtual void drawCrossHairs ( double x, double y,
				TransformBase & transform,
				ViewBase & view );

  /** Draws the title.
   */
  virtual void drawTitle( ViewBase & view, const std::string & title );

  virtual void drawZLabels ( const AxisModelBase & axis_model,
			     ViewBase & view, 
			     const std::string & z_label ) = 0;

  /** This function is the principal function of the ticks drawing family 
   *  because it draws the ticks on the X axis by calling the required 
   *  functions. */
  virtual void drawAllXTicks ( const AxisModelBase & axisModelX,
			       const AxisModelBase & axisModelY,
			       const TransformBase & transform,
			       ViewBase & view );
  
  /** This function is the principal function of the ticks drawing family 
   *  because it draws the ticks on the Y axis by calling the required 
   *  functions. */
  virtual void drawAllYTicks ( const AxisModelBase & axisModelX,
			       const AxisModelBase & axisModelY,
			       const TransformBase & transform,
			       ViewBase & view );
  

  /** This function is the principal function of the ticks drawing family 
   *  because it draws the ticks on the Z axis by calling the required 
   *  functions. */

  virtual void drawAllZTicks ( const AxisModelBase & axis_model,
			       const TransformBase & transform,
			       ViewBase & view ) = 0;

  /** Draw the reference point for X or Y axis. This make sense when 
      the range is small compared to the value. In this mode, we display
      the refenced point and the tick shows the difference from this point.
      TODO: Old interface. Already handled in drawXYLabels(). To be removed.
  */
  void drawReferencePoint( const AxisModelBase & axisModel, 
			   ViewBase & base,
			   const std::string & ref);


  /** Draws the X magnitude label. */
  void drawXMag ( const AxisModelBase & axisModel,
		  ViewBase & view, const std::string & mag="");

  /** Draws the Y magnitude label. */
  void drawYMag ( const AxisModelBase & axisModel,
		  ViewBase & view, const std::string & mag="");

  /** Draws the grid lines after transformation.  This grids the
      enclosed region ( rectangle or ellipse )  in which the plot is drawn. */
  virtual void drawGridLines( const AxisModelBase & axisModelX,
			      const AxisModelBase & axisModelY,
			      TransformBase & transform,
			      ViewBase & view );

  /** Draws the color scale bar.  The base class implementation raises
      an assertion as it should never be called.
   */
  virtual void drawColorScale ( const BinToColor &, ViewBase & );

protected:

  /** Draws the labels for the X ticks. */
  void drawXTickLabels ( const AxisModelBase & axisModelX,
			 const AxisModelBase & axisModelY,
			 const TransformBase & transform,
			 ViewBase & view );

  /** Draws the labels for the Y ticks. */
  void drawYTickLabels ( const AxisModelBase & axisModelX,
			 const AxisModelBase & axisModelY,
			 const TransformBase & transform,
			 ViewBase & view );

  /** Draws a tick label for X axis.
   */
  void drawXTickLabel ( const std::string & label, 
			float x, float y,
			ViewBase & view );

  /** Draws the tick labels for Y axis.
      TODO: Old interface, remove it.
   */
  void drawYTickLabels ( const std::vector < AxisTick > & ticks,
			 const std::vector < float > & xv,
			 const std::vector < float > & yv,
			 ViewBase & view );

  /** Draws the tick label for Y axis.
   */
  void drawYTickLabel ( const std::string & label,
			float x, float y,
			ViewBase & view );

};

} // namespace hippodraw

#endif // _AxisRepBase_H_
