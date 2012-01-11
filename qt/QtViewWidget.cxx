/** @file

hippodraw::QtViewWidget class implementation

Copyright (C) 2003, 2005, 2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: QtViewWidget.cxx,v 1.161 2006/05/05 23:42:22 pfkeb Exp $

*/

// inconsistent dll linkage
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "QtViewWidget.h"

#include "DrawBorder.h"

#include "plotters/PlotterBase.h"
#include "qpainter.h"
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QPaintEvent>


#include <cassert>

using std::vector;

using namespace hippodraw;

QtViewWidget::
QtViewWidget ( QWidget * parent, const char * name,  Qt::WFlags wflags )
  : QtViewImp ( NULL ), // sets m_plotter to NULL
    QWidget ( parent, wflags )
{
  setObjectName(name);
  QPalette palette;
  palette.setColor(backgroundRole(), Qt::white);
}

QtViewWidget::~QtViewWidget ()
{
}

/* virtual */
QSize
QtViewWidget::
sizeHint () const
{
  return size();
}

void
QtViewWidget::
setPlotter ( PlotterBase * plotter )
{
  ViewBase::setPlotter ( plotter );
  plotter -> addObserver ( this );
}

void QtViewWidget::draw ( QPainter & painter )
{
  m_painter = & painter;

  drawSelf ();
}

void
QtViewWidget::
paintEvent ( QPaintEvent * ev )
{

  if ( m_plotter == 0 ) return;

  prepareMarginRect ();

  QPainter painter ( this );
  m_painter = & painter;
  drawSelf ();
}

Rect
QtViewWidget::
getDrawRect () const
{
  const QRect & rect = geometry ();
  return Rect ( 0.0, 0.0, rect.width(), rect.height() );
}

void QtViewWidget:: update ( const Observable * display )
{
  if ( display == 0 ) return; 

  // Adjust the width if the aspect ratio needs changing.
  if ( m_plotter != 0 ) {
    double ratio = m_plotter -> getAspectRatio ();
    if ( ratio != 0. ) {
      double w = width ();
      double h = height ();
      double diff = ratio - w / h;
      if ( diff > 0.00001 ) {
	int new_h = height();
	int new_w = static_cast < int > ( w * ratio );
	setFixedSize ( new_w, new_h );
	prepareMarginRect ();
      }
    }
  }
  QtViewImp::update ( display );

  QWidget::update ();
}

void QtViewWidget::setDrawRect ( const QRect & rect )
{
  setDrawRect ( rect.x (), rect.y (), rect.width (), rect. height () );
}

void QtViewWidget::setDrawRect ( double x, double y, double w, double h )
{
  int ix = static_cast < int > ( x );
  int iy = static_cast < int > ( y );
  int iw = static_cast < int > ( w );
  int ih = static_cast < int > ( h );

  setGeometry ( ix, iy, iw, ih );
}

void
QtViewWidget::
setGeometry ( int x, int y, int w, int h )
{
  QWidget::setGeometry ( x, y, w, h );
  if ( m_plotter != 0 ) { 
    prepareMarginRect ();
  }
}

void
QtViewWidget::
setGeometry ( const QRect & rect )
{
  setGeometry ( rect.x(), rect.y(), rect.width(), rect.height() );
}


void
QtViewWidget::
resizeEvent ( QResizeEvent * event )
{
  QWidget::resizeEvent ( event );
  if ( m_plotter != 0 ) {
    prepareMarginRect ();
  }
}

double QtViewWidget::toCanvasX ( double datX ) const
{
  return datX;
}

double QtViewWidget::toCanvasY ( double datY ) const
{
  return datY;
}

double QtViewWidget::toViewX ( double datX ) const
{
  return userToMarginX ( datX );
}

double QtViewWidget::toViewY ( double datY ) const
{
  return userToInvertedMarginY ( datY );
}

void
QtViewWidget::
fillPickedPoint ( double x, double y, 
		  std::vector < double > & picked ) const
{
  double xx = marginToUserX ( x );
  double yy = marginToInvertedUserY ( y );

  m_plotter -> fillPickedPointFrom ( xx, yy, picked );
}

void
QtViewWidget::
transformAndFill ( QPolygonF & array,
		   const std::vector< double > & x,
		   const std::vector< double > & y,
		   double (QtViewWidget::* xfunc) ( double ) const,
		   double (QtViewWidget::* yfunc) ( double ) const )
{
  unsigned int size = x.size();
  assert ( size == y.size() );

  for ( unsigned int i = 0; i < size; i++ )
    array[i] = QPointF((this->*xfunc)(x[i]), (this->*yfunc)(y[i]));

}

/** @bug style and color arguments are not used.
 */
void 
QtViewWidget::
drawViewMethod ( const std::vector< double > & x,
		 const std::vector< double > & y,
		 int, int )
{
  unsigned int size = x.size();
  assert ( size == y.size() );

  QPolygonF array ( size );
  transformAndFill ( array, x, y,
		     &QtViewWidget::toCanvasX, &QtViewWidget::toCanvasY );

  m_painter->drawLines ( array );
}

/** @bug Style and color arguments are not used.
 */
void 
QtViewWidget::
drawMethod ( const std::vector < double > & x,
	     const std::vector < double > & y,
	     int, int)
{

  unsigned int size = x.size();
  assert ( size == y.size() );

  QPolygonF array ( size );
  transformAndFill ( array, x, y,
		     &QtViewWidget::toViewX, &QtViewWidget::toViewY );

  m_painter->drawPolyline ( array );
}

void
QtViewWidget::
mousePressEvent ( QMouseEvent * e )
{
  if ( e -> modifiers () == Qt::ControlModifier ) {
    m_plotter -> toggleActivePlot ();
    e -> accept ();
  }
}
