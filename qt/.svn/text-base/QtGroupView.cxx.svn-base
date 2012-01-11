/** @file

hippodraw::QtGroupView class implementation

Copyright (C) 2001-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtGroupView.cxx,v 1.5 2006/05/05 18:24:33 pfkeb Exp $
 
*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "QtGroupView.h"
#include "DrawBorder.h"

#include "qpainter.h"
#include <QPaintEngine>
#include <QtGui/QGraphicsSceneMouseEvent>

#include <cassert>
#include <iostream>

using std::vector;
using namespace hippodraw;

enum Location { none, invalid,
  upper_left,  upper_middle, upper_right,
  middle_left, middle_right,
  lower_left,  lower_middle, lower_right };

QtGroupView::QtGroupView ()
 : GroupViewBase(), QtViewImp(), QGraphicsRectItem(NULL), m_dragging(none)
{
  setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
  setHandlesChildEvents(true);
}

QtGroupView::QtGroupView ( const std::vector < ViewBase * > & inViews )
  : GroupViewBase(inViews), QtViewImp(), QGraphicsRectItem(NULL)
{
  setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
  setHandlesChildEvents(true);

  vector < ViewBase * >::const_iterator first = mViews.begin();
  while (first != inViews.end())
	(dynamic_cast<QtView *>(*first++))->setParentItem(this);
}

QtGroupView::~QtGroupView ()
{
  vector < ViewBase * >::const_iterator first = mViews.begin();
  for ( ; first != mViews.end(); ++first )
    delete (*first);
}


void QtGroupView::setDrawRect ( double x, double y, double w, double h )
{
  QRectF rect(0,0,w,h);

  moveBy(x - this->x(), y - this->y());
  setRect(rect);

  if ( m_position.size() == 0 ) return;   // first time to set draw rect.

  // set draw rectangle for the child views.
  vector < ViewBase * >::const_iterator first = mViews.begin();
  for ( unsigned int i = 0; first != mViews.end(); ++first, i=i+4 ) {
    QtView * qtview = dynamic_cast < QtView * > ( *first );

    qtview->setDrawRect ( x + w*m_position[i],
			  y + h*m_position[i+1],
			  w*m_position[i+2],
			  h*m_position[i+3] );
  } 
}

void QtGroupView::setDrawRect ( const QRect & rect )
{
  setDrawRect ( rect.x (), rect.y (), rect.width (), rect. height () );
}

Rect QtGroupView::getDrawRect () const
{
  QRectF rect = QGraphicsRectItem::sceneBoundingRect();

  return Rect ( rect.x(), rect.y(), rect.width(), rect.height() );
}


void QtGroupView::paint ( QPainter *painter,
    const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  // only draw the border
  const QColor color ( "black" );
  painter->setPen ( color );
  m_painter = painter;

  if ( isSelected () )
    {
      DrawBorder * border = DrawBorder::instance();
      border->setView (this);
      border->draw();
    }
}

// Abstract methods from QtViewImp class.
void
QtGroupView::
fillPickedPoint ( double dx, double dy,
		  std::vector < double > & picked ) const
{
}

double QtGroupView::toCanvasX ( double dx ) const
{
  return dx;
}

double QtGroupView::toCanvasY ( double dy ) const
{
  return dy;
}

double QtGroupView::toViewX ( double datX ) const
{
  return userToMarginX ( datX );
}

double QtGroupView::toViewY ( double datY ) const
{
  return userToInvertedMarginY ( datY );
}

void QtGroupView::setPositions()
{
  QRectF local_rect, rect;
  local_rect = QGraphicsRectItem::rect();

  // Is it OK to convert int to double?
  vector < ViewBase * >::const_iterator first = mViews.begin();
  for ( ; first != mViews.end(); ++first ) {
    QtView * qtview = dynamic_cast < QtView * > ( *first );
    rect = qtview->mapRectToParent(qtview->rect());
    m_position.push_back( (rect.x()-local_rect.x())/local_rect.width() );
    m_position.push_back( (rect.y()-local_rect.y())/local_rect.height() );
    m_position.push_back( rect.width()/local_rect.width() );
    m_position.push_back( rect.height()/local_rect.height() );
  }
}

void
QtGroupView::
mousePressEvent ( QGraphicsSceneMouseEvent * e )
{
  if ( e->button() != Qt::LeftButton) {
	e->ignore();
	return;
  }

  if ( !(flags() & QGraphicsItem::ItemIsMovable) ) { // locked
    QGraphicsItem::mousePressEvent(e);
    return;
  }

  // Everything past here modifies the plot

  if (handleMousePress(e->pos())) {
	e -> accept ();
	return;
  }

  QGraphicsItem::mousePressEvent(e);
}

void
QtGroupView::
mouseReleaseEvent ( QGraphicsSceneMouseEvent * e )
{
  if ( e->button() != Qt::LeftButton) {
	e->ignore();
	return;
  }

  handleMouseRelease();

  if (e->button() == Qt::LeftButton && e->modifiers() == Qt::ShiftModifier) {
	setSelected(true);
	e->accept();
	return;
  }

  QGraphicsItem::mouseReleaseEvent(e);
}

void
QtGroupView::
mouseMoveEvent ( QGraphicsSceneMouseEvent * e )
{
  if (!(e->buttons() & Qt::LeftButton)) {
	e->ignore();
	return;
  }

  if (handleMouseDrag(e->pos())) {
	e -> accept ();
	return;
  }

  QGraphicsItem::mouseMoveEvent(e);
}

