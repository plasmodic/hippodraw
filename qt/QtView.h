/* -*- mode: c++ -*- */

/** @file

hippodraw::QtView class interface

Copyright (C) 2001-2006  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtView.h,v 1.101 2007/01/26 22:55:32 xiefang Exp $

*/

#ifndef _QtView_H_
#define _QtView_H_

#include "QtViewImp.h"

#include <qgraphicsscene.h>
#include <qpixmap.h>
#include <QGraphicsRectItem>

class QGraphicsSceneResizeEvent;

namespace hippodraw {

enum cut_mode {
	drag_edges	= 1,
	click		= 2,
	drag_position	= 3
};

/** The class of derived from ViewBase and QCanvasRectangle for
    drawing on a QCanvas.  It is derived from QCanvasRectangle in
    order to get a position and size on the canvas. The view thus
    becomes a QCanvasItem. The inherited @em active state is used to
    lock the size and position on the canvas.  The inherited @em
    selected state is used to highlight the selected item.  It also
    derives from QtViewImp which contains most of the drawing code.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket Biren Malde <sanket@stanford.edu>
*/

class MDL_QTHIPPOPLOT_API QtView : public QtViewImp, public QGraphicsRectItem
{
private:

  /** A QPixmap used to buffer the plot. 
   */
  QPixmap m_pixmap;

  /** Page width */
  int m_upage_w;
  
  /** A flag indicating if the buffer pixmap has been filled.
   */
  bool m_filled;

  /** The current cut mode
   */
  cut_mode m_cut_mode;

  /** A flag indicating if cross-hairs should be drawn. 
   */
  bool m_crosshairs;

  /** A flag set true when drawing has stared.
   */
  bool m_is_drawing;

  /** Possible locations where the mouse cursor was clicked */
  int m_dragging;

  /** Handle status bar updates when right clicked */
  void updateCrossHairs(QGraphicsSceneMouseEvent *e);

  /** Handle cut updates when right clicked */
  void updateCuts(QGraphicsSceneMouseEvent *e);

  /** Converts coordinate from view space to canvas space. */
  double toCanvasX ( double dx ) const;

  /** Converts coordinate from view space to canvas space. */
  double toCanvasY ( double dy ) const;

  /** Fills the @a array with QPointF objects whose coordinates are in
      the QPaintDevice space from the vectors @a x and @a y whose
      coordinates are in the data space. The member functions @a xfunc
      and @a yfunc of the QtView class are used for the
      transformation. */
  void transformAndFill ( QPolygonF & array, 
			  const std::vector < double > & x,
			  const std::vector < double > & y,
			  double (QtView::* xfunc ) ( double ) const,
			  double (QtView::* yfunc ) ( double ) const );

  /** Does the drawing with QPixmap cache instead of directly to paint
      device.   

      @attention This method should not be used when drawing to the
      printer.
  */
  void drawWithPixmap ( QPainter & painter );
  /** Draws in the view using the specified method. The coordinates
      are in the user space. */
  void drawMethod ( const std::vector< double > & x,
		    const std::vector< double > & y,
		    int opt1, 
		    int opt2 );

  /** Draws in the view using the specified method. The coordinates 
      are in the view space. 
      
      @todo We may have function like this that takes pointer to
      member function of QPainter as first argument. 
  */
  void drawViewMethod ( const std::vector< double > & x,
			const std::vector< double > & y,
			int opt1, 
			int opt2 );

  /**
    Used by the double buffering done in method draw
    @todo It still does a fair amount of flickering, need to look
    at this further.
  */
  void ensureOffScrSize ( int osw, int osh );

public:

  /** The constructor.  
      @param plotter The plotter of the class derived from
      PlotterBase to be used in a drawing.  */
  QtView ( PlotterBase * plotter );

  /** The copy constructor. */
  QtView ( const QtView & );

  /** The required virtual destructor. */
  virtual ~QtView ();

  /** Re-Implements the inherited function from QGraphicsItem
      First calls base class to draw the contents.  If selected, then
      also draws the border and sends update message to m_inspector.
  */
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
	QWidget *widget);

  /** Responds to Observable update message. If @a display is not
      null, responds to Observable update message by informing the
      QCanvas that its rectangular has changed.
   */
  virtual void update ( const Observable * );

  /** Sets display status of cross-hairs.  Sets the display of
      cross-hairs at the previous defined position on if @a flag is @c
      true, otherwise does not display the cross-hairs.
      
      @sa setCrossX and setCrossY.
   */
  virtual void setShowCrossHairs ( bool flag );

  /** Overrides virtual function of ViewBase.
   */
  virtual Rect getDrawRect () const;


  /** Updates the drawing rectangle in the paint device space. 
      Handle aspect ratio changes here. 
  */
  void updateDrawRect ();
  
  /** Sets the drawing rectangle in the paint device space. */
  void setDrawRect ( const QRectF & rect );

  /** Set the drawing rectangle in the paint device space. */
  void setDrawRect ( Rect & rect );

  /** Sets the drawing rectangle in the paint device space. */
  virtual void setDrawRect ( double x, double y, double width, double height );

  /** Converts the data space coordinate X to the view space coordinate X. */
  double toViewX ( double datX ) const;

  /** Converts the data space coordinate Y to the view space coordinate Y. */
  double toViewY ( double datY ) const;

  virtual void fillPickedPoint ( double x, double y, 
				 std::vector < double > & picked ) const;
  
  /** Set the page width. It is used for setting the dimensions of the
      draw rectangle */
  virtual void setPageWidth( int upage_w );

  /* Test if the plotter of this view is a text plotter */
  bool isTextView() const;		  

  /** overrides the inherited virtual function.  If the event is a
      mouse event with control key down, then toggles the active
      DataRep.  Otherwise, passes passes the event to QObject.
  */
  virtual void mousePressEvent ( QGraphicsSceneMouseEvent * e );
  virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * e );
  virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * e );
};

} // namespace hippodraw

#endif // _QtView_H_
