/* -*- mode: c++ -*- */

/** @file

hippodraw::QtViewWidget  class interface

Copyright (C) 2003-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: QtViewWidget.h,v 1.98 2006/02/12 20:51:35 pfkeb Exp $

*/

#ifndef _QtViewWidget_H_
#define _QtViewWidget_H_

#include "QtViewImp.h"

#include <qpixmap.h>
#include <qwidget.h>

class QPaintEvent;

namespace hippodraw {

/** The class of derived from ViewBase for drawing as a Qt Widget. It
    is derived from QWidget in order to get a position and size in its
    parent.  It also derives from QtViewImp where most of the drawing
    code is implemented.

    @bug @@@@@@ Displaying an empty DataSource leads to QFont error
    messages.  See jira bug report.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket Biren Malde <sanket@stanford.edu>
*/
class MDL_QTHIPPOPLOT_API QtViewWidget : public QtViewImp, public QWidget
{
  /** The copy constructor. */
  QtViewWidget ( const QtViewWidget & );

  /** Converts coordinate from view space to canvas space. */
  double toCanvasX ( double dx ) const;

  /** Converts coordinate from view space to canvas space. */
  double toCanvasY ( double dy ) const;

  /** Fills the @a array with QPoint objects whose coordinates are in
      the QPaintDevice space from the vectors @a x and @a y whose
      coordinates are in the data space. The member functions @a xfunc
      and @a yfunc of the QtViewWidget class are used for the
      transformation.
  */
  void transformAndFill ( QPolygonF & array, 
			  const std::vector < double > & x,
			  const std::vector < double > & y,
			  double (QtViewWidget::* xfunc ) ( double ) const,
			  double (QtViewWidget::* yfunc ) ( double ) const );

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

public:

  /** The standard QWidget constructor.  See Qt documentation for
      details. */
  QtViewWidget ( QWidget * parent = 0, 
		 const char * name = 0, 
		 Qt::WFlags wflags = 0 );

  /** The required virtual destructor. */
  virtual ~QtViewWidget ();

  /** Re-implements inherited member to test something for SIP.
      @attention Shouldn't really need to do this.  But if it isn't
      here, then we have to get SIP to accept the ViewBase class.  The
      problem with that is getting SIP to accept all the argument type
      of all the pure virtual functions.  Until that is done, this
      implementation gives a way out.*/
  void setPlotter ( PlotterBase * );

  /** Re-implements inherited member to return a reasonable size. */
  virtual QSize sizeHint () const;

  /** Re-implements the inherited function.  First calls base class to
      draw the contents and then draws the border if selected. */
  virtual void draw ( QPainter & );

  /** Overrides virtual function of QWidget to do drawing. */
  virtual void paintEvent ( QPaintEvent * );

  virtual void update ( const Observable * );

  /** Overloads the inherit member function in order to prepare the
      margin rectangle.
  */
  virtual void setGeometry ( int x, int y, int w, int h );

  /** Overloads the inherit member function in order to prepare the
      margin rectangle.
  */
  virtual void setGeometry ( const QRect & );

  virtual Rect getDrawRect () const;

  /** Sets the drawing rectangle in the paint device space. */
  void setDrawRect ( const QRect & rect );

  /** Sets the drawing rectangle in the paint device space. */
  virtual void setDrawRect ( double x, double y, double width, double height );

  /** Overloads the inherit member function to set the margin rectangle.
   */
  virtual void resizeEvent ( QResizeEvent * event );

  /** Converts the data space coordinate X to the view space coordinate X. */
  double toViewX ( double datX ) const;

  /** Converts the data space coordinate Y to the view space coordinate Y. */
  double toViewY ( double datY ) const;

  virtual void fillPickedPoint ( double x, double y, 
				 std::vector < double > & picked ) const;

  /** overrides the inherited virtual function.  If the event is a
      mouse event with control key down, then toggles the active
      DataRep.  Otherwise, passes passes the event to QObject.
  */
  virtual void mousePressEvent ( QMouseEvent * e );

};

} // namespace hippodraw

#endif // _QtViewWidget_H_
