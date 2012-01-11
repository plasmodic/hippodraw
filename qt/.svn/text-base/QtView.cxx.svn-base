/** @file

hippodraw::QtView class implementation

Copyright (C) 2001-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtView.cxx,v 1.175 2007/03/05 22:49:48 xiefang Exp $
 
*/

// inconsistent dll linkage
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "CanvasView.h"
#include "CanvasWindow.h"
#include "DrawBorder.h"
#include "QtView.h"
#include "QtGroupView.h"

#include "axes/Range.h"
#include "plotters/PlotterBase.h"
#include "plotters/Cut1DPlotter.h"
#include "plotters/Cut2DPlotter.h"

#include "qpainter.h"
#include <QtGui/QPaintEngine>
#include <QtGui/QGraphicsSceneMouseEvent>

#include <cassert>
#include <cmath>
#include <iostream>

using std::vector;
using namespace hippodraw;

enum Location { none, invalid,
  upper_left,  upper_middle, upper_right,
  middle_left, middle_right,
  lower_left,  lower_middle, lower_right };

QtView::QtView ( PlotterBase * plotter )
  : QtViewImp ( plotter ),
    QGraphicsRectItem ( 0 ),
    m_filled ( false ),
    m_crosshairs ( false ),
    m_cut_mode ( drag_edges )
{
  assert ( plotter != 0 );
  m_plotter -> addObserver ( this );
  setFlags ( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable ); 
}

QtView::
~QtView ()
{
  if ( m_plotter != 0 )
    m_plotter -> removeObserver ( this );
}

/** @bug The plotting region for Color Plots is too small. */

void
QtView::
drawWithPixmap ( QPainter & painter )
{
  const Rect rect = getDrawRect ();
  double width = rect.getWidth ();
  double height = rect.getHeight ();
  
  if ( m_filled == false ) {
    QPainter offscrPainter;
    ensureOffScrSize(width, height);
    m_pixmap.fill(Qt::transparent);
    offscrPainter.begin(& m_pixmap);
    m_painter = &offscrPainter;

    drawSelf();

    offscrPainter.end();
    m_filled = true;
  }

  m_painter = & painter;
  painter.drawPixmap ( 0, 0, m_pixmap,
		       0, 0, width, height );
}

void
QtView::
setShowCrossHairs ( bool yes )
{
  m_crosshairs = yes;
}

/** @bug Updating the Inspector from this method is disabled.
    However, not updating allow changes from Python to not be seen
    from the Inspector.  Enabling it makes too many updates to the
    inspector when changes are made from the GUI.  This problem needs
    to be resolved. */
void QtView::paint ( QPainter *painter, const QStyleOptionGraphicsItem *option,
 QWidget *widget )
{
  if ( m_plotter == 0 ) return;

  m_is_drawing = true;
  const QColor color ( "black" );
  painter->setPen ( color );

  QPaintDevice * dev = painter->device ();
  QPaintEngine * pe = dev -> paintEngine ();
  QPaintEngine::Type type = pe-> type ();
  if ( m_plotter -> wantsPixmap () && type != QPaintEngine::PostScript ) { 
    drawWithPixmap ( *painter );
  } else {
    m_painter = painter;
    drawSelf ();
  }

  if ( m_crosshairs )
    m_plotter -> drawCrossHairs ( this );

  if (isSelected () && dynamic_cast<QtGroupView *>(parentItem()) == NULL) {
      DrawBorder * border = DrawBorder::instance();
      border->setView (this);
      border->draw();
  }
  m_is_drawing = false;
}

Rect QtView::getDrawRect () const
{
  QRectF rect = QGraphicsRectItem::sceneBoundingRect();

  return Rect ( rect.x(), rect.y(), rect.width(), rect.height() );
}

void
QtView::
update ( const Observable * display )
{
  if ( m_is_drawing == true ) return;

  if ( display != 0 ) { // from Observable object
    m_filled = false;
  }

  // Update the size of the draw rectangle first. ( For aspect ratio. )
  updateDrawRect();

  QGraphicsItem::update ();
}

void QtView::updateDrawRect ()
{
  double aspect = getAspectRatio();
  Rect drawrect = getDrawRect();
  if ( aspect > 0 ) {
    setDrawRect ( drawrect.getX(), drawrect.getY(), 
		  drawrect.getHeight()*aspect, drawrect.getHeight() );
  } else {
    setDrawRect ( drawrect );
  }
}

void QtView::setDrawRect ( const QRectF & rect )
{
  setDrawRect ( rect.x (), rect.y (), rect.width (), rect. height () );
}

void QtView::setDrawRect ( Rect & rect )
{
  setDrawRect ( rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight() );
}

void QtView::setDrawRect ( double x, double y, double w, double h )
{
    // XXX: setRect should have done the right thing here, but does not for
    //  moving the top left corner. Do this by hand.

    QRectF newrect(0, 0, w, h);

    if (x != scenePos().x() || y != scenePos().y())
	moveBy(x - scenePos().x(), y - scenePos().y());

    if (newrect == boundingRect())
	return;

    prepareGeometryChange();
    setRect(newrect);

    prepareMarginRect ();
    newrect.setWidth(m_margin_rect.getX () + m_margin_rect.getWidth() + 20.);

    prepareGeometryChange();
    setRect(newrect);

    m_filled = false;
}

void
QtView::
transformAndFill ( QPolygonF & array,
		   const std::vector< double > & x,
		   const std::vector< double > & y,
		   double (QtView::* xfunc) ( double ) const,
		   double (QtView::* yfunc) ( double ) const )
{
  unsigned int size = x.size();
  assert ( size == y.size() );

  for ( unsigned int i = 0; i < size; i++ )
    array[i] = QPointF((this->*xfunc)(x[i]), (this->*yfunc)(y[i]));

}

double QtView::toViewX ( double datX ) const
{
  if (m_plotter -> isReverse())
    return userToInvertedMarginX (datX);
  else
    return userToMarginX (datX);
}

double QtView::toViewY ( double datY ) const
{
  return userToInvertedMarginY ( datY );
}

void
QtView::
fillPickedPoint ( double dx, double dy,
		  std::vector < double > & picked ) const
{
  if ( m_plotter != 0 ) {
    double xx;
    if (m_plotter -> isReverse())
      xx = marginToInvertedUserX ( dx );
    else
      xx = marginToUserX ( dx );
    double yy = marginToInvertedUserY ( dy );

    m_plotter -> fillPickedPointFrom ( xx, yy, picked );
  }
}

double QtView::toCanvasX ( double dx ) const
{
  return dx;
}

double QtView::toCanvasY ( double dy ) const
{
  return dy;
}

/** @bug Style and color arguments are not used.
 */
void QtView::drawViewMethod ( const std::vector< double > & x,
			      const std::vector< double > & y,
			      int, // style,
			      int ) //color )
{
  unsigned int size = x.size();
  assert ( size == y.size() );

  QPolygonF array ( size );
  transformAndFill ( array, x, y,
		     &QtView::toCanvasX, &QtView::toCanvasY );

  m_painter->drawLines ( array );

}

/** @bug Style and color arguments are not used.
 */
void QtView::drawMethod ( const std::vector < double > & x,
			  const std::vector < double > & y,
			  int, // style,
			  int ) //color )
{

  unsigned int size = x.size();
  assert ( size == y.size() );

  QPolygonF array ( size );

  transformAndFill ( array, x, y,
		     &QtView::toViewX, &QtView::toViewY );

  m_painter->drawPolyline ( array );
}

void
QtView::
ensureOffScrSize ( int osw, int osh )
{
  if ( osw > m_pixmap.width() || osh > m_pixmap.height() )
	m_pixmap = QPixmap(std::max(osw,m_pixmap.width()),
		      std::max(osh,m_pixmap.height()));
  else if ( m_pixmap.width() == 0 || m_pixmap.height() == 0 ) 
	m_pixmap = QPixmap(std::max( m_pixmap.width(), 1),
		       std::max( m_pixmap.height(), 1 ) );
}

void
QtView::
setPageWidth ( int upage_w )
{
  m_upage_w = upage_w;
}


bool
QtView::
isTextView() const
{
  return m_plotter->isTextPlotter();
}

void
QtView::
mousePressEvent ( QGraphicsSceneMouseEvent * e )
{
  if ( e->button() == Qt::RightButton ) {
	updateCrossHairs(e);
	e->accept();
  }

  if ( !(flags() & QGraphicsItem::ItemIsMovable) ) { // locked
    QGraphicsItem::mousePressEvent(e);
    return;
  }

  // Everything past here modifies the plot

  if (e->button() == Qt::RightButton && 
     (dynamic_cast<Cut1DPlotter *>(m_plotter) != NULL ||
      dynamic_cast<Cut2DPlotter *>(m_plotter) != NULL)) {
	updateCuts(e);
	return;
  }

  if (e->button() == Qt::LeftButton) {
	if ( e -> modifiers () == Qt::ControlModifier ) {
		m_plotter -> toggleActivePlot ();
		e -> accept ();
		return;
	}

	if ( e -> modifiers () == Qt::ShiftModifier ) {
		setSelected ( true );
		e -> accept ();
		return;
	}

	if (handleMousePress(e->pos())) {
		e -> accept ();
		return;
	}
  }

  QGraphicsItem::mousePressEvent(e);
}

void
QtView::
mouseReleaseEvent ( QGraphicsSceneMouseEvent * e )
{
  handleMouseRelease();

  if ( e->button() == Qt::LeftButton &&
   (e -> modifiers () == Qt::ControlModifier ||
    e -> modifiers () == Qt::ShiftModifier)) {
	e -> accept ();
	return;
  }

  if ( e->button() == Qt::RightButton ) {
	CanvasWindow *cw =
	    dynamic_cast<CanvasWindow *>(e->widget()->parentWidget()->
	    parentWidget());

	if (cw != NULL)
		cw->statusBar()->clearMessage();

	setShowCrossHairs(false);
	unsetCursor();
	update(0);
  }

  QGraphicsItem::mouseReleaseEvent(e);
}

void
QtView::
mouseMoveEvent ( QGraphicsSceneMouseEvent * e )
{
  if (e->buttons() & Qt::LeftButton) {
	if (handleMouseDrag(e->pos())) {
		e -> accept ();
		return;
	}
  }

  if ( e->buttons() & Qt::RightButton ) {
	updateCrossHairs(e);

	if (flags() & QGraphicsItem::ItemIsMovable &&
	    (dynamic_cast<Cut1DPlotter *>(m_plotter) != NULL ||
	     dynamic_cast<Cut2DPlotter *>(m_plotter) != NULL))
		updateCuts(e);
	e->accept();
	return;
  }

  QGraphicsItem::mouseMoveEvent(e);
}

void
QtView::updateCrossHairs(QGraphicsSceneMouseEvent *e)
{
	vector < double > picked;

	// First, update the status bar with our coordinates
	fillPickedPoint ( e->pos().x(), e->pos().y(), picked );

	CanvasWindow *cw =
	    dynamic_cast<CanvasWindow *>(e->widget()->parentWidget()->
	    parentWidget());

	// If we have coordinates, and are in a CanvasWindow, say where we are.
	if (cw != NULL && picked.size() > 0) {
		QStatusBar *sb = cw->statusBar();
		QString qstr = QString( "Coordinates are: (%1").arg(picked[1]);

		for (int i = 2; i < picked.size(); i++)
			qstr += QString(", %1").arg(picked[i]);
		qstr += ")";
		sb->showMessage(qstr);
	}

	// Add cross-hairs
	if (picked.size() > 0) {
		setCrossX(picked[1]);
		if (fabs(picked[0] - 1.) < 0.5)	// Information about transforms
			setCrossY(picked[3]);	// See CompositePlotter.cxx
		else				// for information.
			setCrossY(picked[2]);
		setShowCrossHairs(true);
		setCursor(Qt::CrossCursor);
		update(0);
	}
}

void
QtView::updateCuts(QGraphicsSceneMouseEvent *e)
{
	vector < double > picked;
	double pickedx, pickedy;

	if (m_plotter == NULL) 
		return;

	fillPickedPoint ( e->pos().x(), e->pos().y(), picked );

	pickedx = picked[1];
	pickedy = (fabs(picked[0] - 1.) < 0.5) ? picked[3] : picked[2];

	if (dynamic_cast<Cut1DPlotter *>(m_plotter) != NULL) {
		Cut1DPlotter *cut = dynamic_cast<Cut1DPlotter *>(m_plotter);	

		double cut_low = cut->getCutRange().low();
		double cut_high = cut->getCutRange().high();
		double plotter_low = cut->getRange( Axes::X, true ).low();
		double plotter_high = cut->getRange( Axes::X, true ).high();

		Range currentRange = Range::Range(cut_low, cut_high);

		// Limit to visible area
		if ( cut_low < plotter_low )
			currentRange = Range::Range(plotter_low, cut_high);
		if ( cut_high > plotter_high )
			currentRange = Range::Range(cut_low, plotter_high);
		   
		// Figure out which side was selected and adjust cut
		if (pickedx < (currentRange.high() + currentRange.low())/2.)
			currentRange.setLow(pickedx);
		else
			currentRange.setHigh(pickedx);

		cut->setCutRangeAt(currentRange, 0);
	}

	if (dynamic_cast<Cut2DPlotter *>(m_plotter) != NULL) {
		Cut2DPlotter *cut = dynamic_cast<Cut2DPlotter *>(m_plotter);	
		Location edge;
		
		Range xrange, yrange;
		Range xcutrange, ycutrange;

		xrange = cut->getRange(Axes::X, true);
		yrange = cut->getRange(Axes::Y, true);

		xcutrange = cut->getCutRange();
		ycutrange = cut->getCutRangeY();

	#define corner(xend, yend) \
		   (fabs(yend - pickedy) < 0.1 * ycutrange.length() && \
		    fabs(xend - pickedx) < 0.1 * xcutrange.length())
	#define	checkclosest(a,b,c) \
		   if (fabs(a - b) < bestdelta) { \
			bestdelta = fabs(a - b); \
			edge = c; \
		   }

		if (corner(xcutrange.low(), ycutrange.high()))
			edge = upper_left;
		else if (corner(xcutrange.high(), ycutrange.high()))
			edge = upper_right;
		else if (corner(xcutrange.high(), ycutrange.low()))
			edge = lower_right;
		else if (corner(xcutrange.low(), ycutrange.low()))
			edge = lower_left;
		else {
			// Find the edge to which we are closest
			edge = middle_left;
			double bestdelta = fabs(pickedx - xcutrange.low());

			checkclosest(pickedx, xcutrange.high(), middle_right);
			checkclosest(pickedy, ycutrange.high(), upper_middle);
			checkclosest(pickedy, ycutrange.low(), lower_middle);
		}
	#undef corner
	#undef checkclosest

		switch (edge) {
			case upper_left:
				xcutrange.setLow(pickedx);
				ycutrange.setHigh(pickedy);
				break;
			case upper_right:
				xcutrange.setHigh(pickedx);
				ycutrange.setHigh(pickedy);
				break;
			case lower_right:
				xcutrange.setHigh(pickedx);
				ycutrange.setLow(pickedy);
				break;
			case lower_left:
				xcutrange.setLow(pickedx);
				ycutrange.setLow(pickedy);
				break;
			case upper_middle:
				ycutrange.setHigh(pickedy);
				break;
			case lower_middle:
				ycutrange.setLow(pickedy);
				break;
			case middle_right:
				xcutrange.setHigh(pickedx);
				break;
			case middle_left:
				xcutrange.setLow(pickedx);
				break;
			case none:
			case invalid:
				break;
		}

		cut->setCutRangeAt(xcutrange, 0);
		cut->setCutRangeAt(ycutrange, 1);
	}

	CanvasView *cv =
	    dynamic_cast<CanvasView *>(e->widget()->parentWidget());
	if (cv != NULL)
		cv->notifyObservers();
}

