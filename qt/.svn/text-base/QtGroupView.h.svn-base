/* -*- mode: c++ -*- */

/** @file

hippodraw::QtGroupView class interface

Copyright (C) 2006  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtGroupView.h,v 1.4 2006/05/28 21:24:13 pfkeb Exp $

*/

#ifndef _QtGroupView_H_
#define _QtGroupView_H_

#ifdef _MSC_VER
# pragma warning(disable:4250)  // inherits via dominance
#endif

#include "graphics/GroupViewBase.h"
#include "QtView.h"
#include "QtViewImp.h"

namespace hippodraw {

/** The class of derived from GroupViewBase, QtViewImp and QCanvasRectangle 
    for drawing a grouped view on a QCanvas.  It is derived from 
    QCanvasRectangle in order to get a position and size on the canvas. 
    The view thus becomes a QCanvasItem. The inherited @em active state 
    is used to lock the size and position on the canvas.  The inherited 
    @em selected state is used to highlight the selected item.  It is
    derived from the GroupViewBase class to have a vector of views. It also
    derives from QtViewImp which contains most of the drawing code.  

    @author Xie Fang <xiefang@stanford.edu>
*/
class MDL_QTHIPPOPLOT_API QtGroupView : public GroupViewBase, public QtViewImp, public QGraphicsRectItem
{

 private:

  /** A vector containing the percentage position information for every views.
   */
  std::vector < double > m_position;

  /** Possible locations where the mouse cursor was clicked */
  int m_dragging;

  /** Converts coordinate from view space to canvas space. */
  double toCanvasX ( double dx ) const;

  /** Converts coordinate from view space to canvas space. */
  double toCanvasY ( double dy ) const;

 public:

  /** The constructor.*/
  QtGroupView ( );

  /** The constructor that takes a vector of views as arguments. */
  QtGroupView (  const std::vector < ViewBase * > & inViews );

  /** Copy constructor. */
  //QtGroupView ( const QtGroupView & );

  /** Destructor. */
  virtual ~QtGroupView ();

  /** Sets the drawing rectangle in the paint device space. */
  void setDrawRect ( double x, double y, double w, double h );

  /** Sets the drawing rectangle in the paint device space. */
  void setDrawRect ( const QRect & rect );

  /** Gets the drawing rectangle. */
  virtual Rect getDrawRect () const;

  /** Re-implement the paint() method in QGraphicsItem. */
  virtual void paint ( QPainter *, const QStyleOptionGraphicsItem *option,
        QWidget *widget);

  /** Converts the data space coordinate X to the view space coordinate X. */
  double toViewX ( double datX ) const;

  /** Converts the data space coordinate Y to the view space coordinate Y. */
  double toViewY ( double datY ) const;

  virtual void fillPickedPoint ( double x, double y, 
				 std::vector < double > & picked ) const;

  /** Set the positions in percentage of every views. */
  void setPositions();

  /** overrides the inherited virtual functions for resizing.
  */
  virtual void mousePressEvent ( QGraphicsSceneMouseEvent * e );
  virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * e );
  virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * e );
};

} // namespace hippodraw

#endif // _QtGroupView_H_
