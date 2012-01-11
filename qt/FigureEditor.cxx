/** @file

hippodraw::FigureEditor class implementation

Original taken from $QTDIR/examples/canvas/canvas.cpp version 3.0

Copyright (C) 2001-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FigureEditor.cxx,v 1.251 2007/07/02 18:17:16 pfkeb Exp $

*/


#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

// for MEMFUN1_DEFECT
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "FigureEditor.h"

#include "QtGroupView.h"
#include "CanvasWindow.h"
#include "PickTable.h"
#include "plotters/PlotterBase.h"

#include <QApplication>
#include <QClipboard>
#include <QCursor>
#include <QImage>
#include <QMouseEvent>
#include <QPixmap>
#include <QResizeEvent>
#include <QPrintDialog>
#include <QMatrix>
#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>
#include <QSettings>
#include <QStatusBar>

#include <algorithm>
#include <functional>

#include <cmath>
#include <cctype>

#include <cassert>

using std::abs;
using std::bind2nd;
using std::for_each;
using std::mem_fun;
using std::string;
using std::transform;
using std::vector;


/** An array to allow conversion of integer to enumeration.

    @note Use static as recommended by KDE developers because no long
    symbol will be exported.
 */
static QPrinter::PageSize indexToPageSize[] = 
  { QPrinter::A4, QPrinter::B5, QPrinter::Letter, 
    QPrinter::Legal, QPrinter::Executive,
    QPrinter::A0, QPrinter::A1, QPrinter::A2, QPrinter::A3, 
    QPrinter::A5, QPrinter::A6, QPrinter::A7, QPrinter::A8, 
    QPrinter::A9, QPrinter::B0, QPrinter::B1,
    QPrinter::B10, QPrinter::B2, QPrinter::B3, QPrinter::B4, 
    QPrinter::B6, QPrinter::B7, QPrinter::B8, QPrinter::B9, 
    QPrinter::C5E, QPrinter::Comm10E,
    QPrinter::DLE, QPrinter::Folio, QPrinter::Ledger, 
    QPrinter::Tabloid, QPrinter::Custom, QPrinter::NPageSize };

/** An array to allow conversion of integer to enumeration.

    @note Use static as recommended by KDE developers because no long
    symbol will be exported.
 */
static  QPrinter::Orientation indexToOrientation[] = { QPrinter::Portrait,
						 QPrinter::Landscape };

/** An array to allow conversion of integer to enumeration.

    @note Use static as recommended by KDE developers because no long
    symbol will be exported.
 */
static  QPrinter::ColorMode indexToColorMode[] = { QPrinter::GrayScale,
						   QPrinter::Color };

using namespace hippodraw;

QPrinter * FigureEditor::s_printer = 0;
QString    FigureEditor::s_app_key ( "/HippoDraw/" );
QString    FigureEditor::s_registry ( "/Trolltech" );

FigureEditor::
FigureEditor ( QGraphicsScene * c, QWidget * parent,
	       const char * name, Qt::WFlags f)
  : QGraphicsView ( c, parent ),
    m_scale_factor ( 1.0 ),
    m_printer_bounds ( false ),
    m_ignore_margin ( false )
{
  setObjectName(name);
  setWindowFlags(f);

  m_zoom_mode = false ;

  m_canvas = scene ();

  if ( s_printer == 0 )
      initPrinter ();
  
  calcPrinterMetrics ( s_printer );
  
  m_canvas->setSceneRect( 0, 0, m_apage_w, m_apage_h );
  setDragMode(QGraphicsView::RubberBandDrag);

  //Printer Page margin is not visible by default
  QSettings settings;
  int pb = settings.value ( s_app_key + "Canvas/Margin", 0 ) . toInt();
  bool yes = ( pb == 1 ) ? true : false;
  showPrinterMargins ( yes );

  connect(scene(), SIGNAL(selectionChanged()), this, SLOT(moveSelectedToTop()));
}


void
FigureEditor::
initPrinter ()
{
  s_printer = new QPrinter ( );
  
  QSettings settings;

  const int pagesize 
    = settings.value ( s_app_key + "Printer/PageSize", 
		       QPrinter::Letter ) . toInt();
  assert( pagesize < QPrinter::NPageSize );
  QPrinter::PageSize size = indexToPageSize [ pagesize ];
  
  const int orientation
    = settings.value ( s_app_key + "Printer/Orientation",
		       QPrinter::Portrait ) . toInt();
  assert(  orientation == 0 || orientation == 1 );
  QPrinter::Orientation orient = indexToOrientation [ orientation ];

  const int color
    = settings.value ( s_app_key + "Printer/ColorMode",
		       QPrinter::Color ) . toInt();
  assert( color == 0 || color == 1 );
  QPrinter::ColorMode colorMode = indexToColorMode[color];

  s_printer->setOrientation ( orient );
  s_printer->setPageSize ( size );
  s_printer->setFullPage ( true );
  s_printer->setColorMode ( colorMode );
}

void
FigureEditor::
setAppKey ( const std::string & appkey )
{
  s_app_key = appkey.c_str();
}

void
FigureEditor::
showPrinterMargins ( bool on )
{
   
  if ( m_printer_bounds == false && on == true  ) // No margin exists. Draw 'em
    {
      m_printer_bounds = true;
      
      int height = m_canvas -> height();
      int y = 0;
      while ( y < height )
	{
	  addPageMargin ( 0, y );
	  y += m_apage_h;
	}
      
    }
  else if( m_printer_bounds == true && on == false ) // Margins exist delete it
    {
      m_printer_bounds = false;
      
      QList<QGraphicsItem *> itemList = m_canvas -> items();
      
      QList<QGraphicsItem *>::iterator iter = itemList.begin();
            
      while( iter != itemList.end() )
	{
	  QGraphicsItem * item = *iter++;
	  QtView * view = dynamic_cast < QtView * > ( item );
	  
	  if ( view == 0 )  // it is a printer page border
	    {
	      QGraphicsRectItem * rect
		= dynamic_cast< QGraphicsRectItem * >( item );
	      assert( rect );
	      delete rect;
	    }
	}
    }
}

void
FigureEditor::
addPageMargin ( int x, int y )
{
    
  if ( m_printer_bounds == true )
    {
      QGraphicsRectItem * rect 
	= new QGraphicsRectItem ( x, y, m_upage_w, m_upage_h);
      m_canvas->addItem(rect);
      
      int dx = ( m_apage_w - m_upage_w ) / 2;
      int dy = ( m_apage_h - m_upage_h ) / 2;
      rect->moveBy(dx, dy);
      rect->setZValue(-1000);
      
      QColor color ( "lightGray" );
      QPen pen ( color, 1, Qt::DashLine );
      rect -> setPen ( pen );
      
      rect -> show ();
    }
}

/** @bug @@@@@@ Talking the default printer at start-up causes large
    delays if printer is not available on the network.
*/
void 
FigureEditor::
calcPrinterMetrics ( QPaintDevice * device )
{
  QString name = s_printer->printerName();
  if ( name.isNull() )
    {
      if( s_printer -> orientation() == QPrinter::Portrait )
	{
	  m_upage_w = 720; // dimensions of letter size
	  m_upage_h = 990; // HP printer in portrait mode
	  m_apage_w = 816;
	  m_apage_h = 1054;
	}
      else
	{
	  m_upage_h = 720; // dimensions of letter size
	  m_upage_w = 990; // HP printer in landscape mode
	  m_apage_h = 816;
	  m_apage_w = 1054;
	}
    }
  else
    {
      int dpix = device->logicalDpiX ();
      int dpiy = device->logicalDpiY ();

      m_apage_w = device->widthMM ();
      m_apage_h = device->heightMM ();
      
      m_apage_w = m_apage_w * dpix / 25.4; 
      m_apage_h = static_cast < int > ( m_apage_h * dpiy / 25.4 ); 

      if( s_printer -> orientation() == QPrinter::Landscape )
	std::swap( m_apage_w, m_apage_h );
      
      m_upage_w = m_apage_w - 2 * (s_printer->paperRect().left() -
				   s_printer->pageRect().left());
      m_upage_h = m_apage_h - 2 * (s_printer->paperRect().top() -
				   s_printer->pageRect().top());
  }
}

bool FigureEditor::isSingleItemSelected () const
{
  return ( scene()->selectedItems().size() == 1 );
}

QGraphicsItem * 
FigureEditor::
selectedItem () const
{  
  if ( isSingleItemSelected() )
      return scene()->selectedItems()[0];
  else
      return NULL;
}

std::vector < QGraphicsItem * >
FigureEditor::
getSelectedItems () const
{
  std::vector < QGraphicsItem * > vec;
  QList<QGraphicsItem *> selected = scene()->selectedItems();
  QList<QGraphicsItem *>::iterator iter = selected.begin();

  while (iter != selected.end())
	vec.push_back(*iter++);

  return vec;
}

QRectF FigureEditor::getSelectedBounds () const
{
  QList<QGraphicsItem *> selected;
  QRectF rect;

  selected = scene()->selectedItems();
  QList<QGraphicsItem *>::iterator iter = selected.begin();

  while (iter != selected.end()) 
	rect |= (*iter++)->sceneBoundingRect();

  return rect;
}

double
FigureEditor::
maximumZ () const
{
  QList<QGraphicsItem *> items = scene()->items();
  QList<QGraphicsItem *>::const_iterator first = items.begin();
  double max_z = -100;

  while ( first != items.end () )
    max_z = std::max ( max_z, (*first++)->zValue() );

  return max_z;
}

void
FigureEditor::
setUnselectedVisible ( bool yes )
{
  QList<QGraphicsItem *> items = scene()->items();
  QList<QGraphicsItem *>::const_iterator first = items.begin();

  while ( first != items.end() ) {
    QGraphicsItem * item = *first++;

    if (item->isSelected()) continue;
    item->setVisible ( yes );
  }
}

void
FigureEditor::
setCollidingSelected ( const QGraphicsItem * target )
{
  QList<QGraphicsItem *> items = scene()->items();
  QList<QGraphicsItem *>::const_iterator first = items.begin();

  while ( first != items.end () ) {
    QGraphicsItem * item = *first++;

    if ( item != target && item->collidesWithItem(target))
	  item -> setSelected ( true );
  }
}

void
FigureEditor::
setAllSelected ( bool yes )
{
  QList<QGraphicsItem *> items = scene()->items();

#ifdef MEMFUN1_DEFECT
  for_each ( items.begin(), items.end(),
	     bind2nd ( mem_fun1 ( &QGraphicsItem::setSelected ), yes ) );
#else
  for_each ( items.begin(), items.end(),
	     bind2nd ( mem_fun ( &QGraphicsItem::setSelected ), yes ) );
#endif
}

void
FigureEditor::makeVisible ( const QGraphicsItem * item )
{
  if ( item != 0 ) {
    QRectF rect = item->boundingRect ();
    double x = 0, y = 0, w = 0, h = 0;
    rect.getRect( & x, & y, & w, & h );
    QGraphicsView::ensureVisible ( x, y, w, h );
  }
}

void
FigureEditor::
setSelectedItem ( QGraphicsItem * item )
{

  scene()->clearSelection();
  item->setSelected(true);

  item->setZValue( maximumZ() + 1.0 ); // ensures it is on top
}

void
FigureEditor::
ensureVisible ( QGraphicsItem * item )
{
 item->ensureVisible(item->boundingRect());
}

void 
FigureEditor::
setLocked ( bool yes )
{
  const vector < QGraphicsItem * > items = getSelectedItems ();
  if ( items.empty () ) return; // shouldn't be called

  vector < QGraphicsItem * >:: const_iterator first = items.begin ();
  while ( first != items.end () ) {
    QGraphicsItem * item = *first++;
    item->setFlag (QGraphicsItem::ItemIsMovable, !yes );
  }
}

void
FigureEditor::
remove ( QGraphicsItem * item )
{
  delete item;
}

void
FigureEditor::
keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Delete) {
	  while (scene()->selectedItems().size() > 0)
		remove(scene()->selectedItems()[0]);
	  event->accept();
	  return;
  }

  QGraphicsView::keyPressEvent(event);
}

void FigureEditor::clear()
{
  QList<QGraphicsItem *> items = scene()->items();
  QList<QGraphicsItem *>::iterator it = items.begin();

  for ( ; it != items.end(); ++it )
	if ( *it ) delete *it;
}

void
FigureEditor::
placeGraph ( QGraphicsRectItem * item )
{
  QRectF rect = item->sceneBoundingRect();
  rect.setX( 0 );
  rect.setY( 0 );

  double margin_x = 0;
  double margin_y = 0;
  double max_width = 0;

  if ( m_ignore_margin == false ) {
    margin_x = ( m_apage_w - m_upage_w ) / 2.;
    margin_y = ( m_apage_h - m_upage_h ) / 2.;
    max_width = m_apage_w - margin_x;
  } else {
    max_width = m_canvas -> width ();
  }

  if ( rect.x() < margin_x ) rect.setX ( margin_x + 1. );
  if ( rect.y() < margin_y ) rect.setY ( margin_y + 1. );
  item->moveBy(rect.x() - item->x(), rect.y() - item->y());

  if ( scene()->items().empty () ) return;

  double deltax = 1.05 * rect.width ();
  double deltay = 1.05 * rect.height ();

  while ( true ) {
    rect = item->sceneBoundingRect();

    if ( m_ignore_margin == false ) {
      double top = rect.top() / m_apage_h;
      double bot = rect.bottom() / m_apage_h;
      if ( fabs(top - bot) > 2.  ) { // across page boundary
	double new_y = bot * m_apage_h;
	new_y += ( m_apage_h - m_upage_h ) / 2;
	rect.setY ( new_y ); // beginning next actual page
      }
    }

    item->moveBy(rect.x() - item->x(), rect.y() - item->y());

    QList<QGraphicsItem *> items = scene()->items();
    QList<QGraphicsItem *>::iterator it = items.begin();
    bool collides = false;

    for ( ; it != items.end(); ++it ) {
      if (dynamic_cast<QtView *>(*it) == NULL)
	continue;

      if ( (*it)->collidesWithItem ( item ) ) {
	collides = true;

	QRectF crect = (*it)->sceneBoundingRect();
  	deltax = fabs(rect.left() - crect.right()) + 0.05 * rect.width();
  	deltay = fabs(rect.top() - crect.bottom()) + 0.05 * rect.height();

	break;
      }
    }

    if ( !collides ) break;

    if ( rect.right() + deltax < max_width )
      item -> moveBy ( deltax, 0 ); // move across
    else
      item -> moveBy ((margin_x + 1.) - item->x(), deltay); // move down

  } // while true

}

void
FigureEditor::
add ( QGraphicsItem * item ) 
{ 
  resizeCanvasToFit ( item );
  
  if (item->scene() == NULL)
	m_canvas->addItem( item );
  item->setSelected ( false );

  item->show ();
  m_canvas->update(item->sceneBoundingRect());
}

/** @bug If item is pasted second time, it should display even more.
 */
void
FigureEditor::
paste ( QGraphicsItem * item )
{
  add ( item );
  QList<QGraphicsItem *> items = item->collidingItems();
  unsigned int count = items.count ();
  if ( count > 1 ) {

    item->moveBy ( 10., 10. );
  }
}

void
FigureEditor::
resizeCanvasToFit ( QGraphicsItem * item )
{
  QRectF brect = item->sceneBoundingRect ();
  QPointF p = brect.bottomRight ();
  if ( ! sceneRect().contains( p ) )
    {
      int new_w = m_canvas->width ();
      int old_h = m_canvas->height();
      int new_h = old_h + m_apage_h;
      setSceneRect(0, 0, new_w, new_h );
      addPageMargin ( 0, old_h );
  }
}

void
FigureEditor::
scaleBy ( double factor )
{
  m_apage_w = factor * m_apage_w;
  m_apage_h = factor * m_apage_h;
  m_upage_w = factor * m_upage_w;
  m_upage_h = factor * m_upage_h;
  
  setSceneRect(0, 0, m_apage_w, m_apage_h );
  
  m_scale_factor = factor;
  
  if( m_printer_bounds == true )
    {
      showPrinterMargins ( false ); // Delete old ones ( if any    )
      showPrinterMargins ( true );  // Redraw new ones ( if needed )
    }
}

void
FigureEditor::
viewZoomIn ()
{
    QMatrix matrix = QGraphicsView::matrix ();
    matrix.scale( 4./3., 4./3. );

    setMatrix ( matrix );
}

/** @bug Zooming out from the initial canvas, leave area where plots
    can be dragged, but they disappear.
 */
void
FigureEditor::
viewZoomOut ()
{
    QMatrix matrix = QGraphicsView::matrix ();
    matrix.scale( 3./4., 3./4. );

    setMatrix ( matrix );
}

void
FigureEditor::
viewZoomReset ()
{
  QMatrix matrix = QGraphicsView::matrix ();
  matrix.reset();

  setMatrix ( matrix );
}

void
FigureEditor::
savePrinterSettings ()
{
  QSettings settings;

  QPrinter::PageSize page_size = s_printer -> pageSize ();
  settings.setValue ( s_app_key + "Printer/PageSize", page_size );

}

void
FigureEditor::
setPrinterSettings ()
{
 QPrintDialog dialog ( s_printer, this );
 bool yes = ( dialog.exec () == QDialog::Accepted );

 if ( yes ) {
   savePrinterSettings ();
 }

}

/** @bug If user selects page range, it is ignored.
 */
void
FigureEditor::
print ()
{
  QPrintDialog dialog ( s_printer, this );
  if ( dialog.exec() ) {
    print ( s_printer );
  }
}

void
FigureEditor::
print ( const std::string & filename )
{
  const QString name = filename.c_str();
  s_printer -> setOutputFileName ( name );

  print ( s_printer );
}

void
FigureEditor::
print ( QPrinter * printer )
{
  QPainter painter;
  bool ok = painter.begin ( printer );
  assert ( ok );

  calcPrinterMetrics ( painter.device () );
  double scale = 1.0 / m_scale_factor;
  painter.scale ( scale, scale );
  int page_y = 0;
  int trans_y = static_cast < int > ( m_apage_h * m_scale_factor );

  QList<QGraphicsItem *> selected = scene()->selectedItems();
  QList<QGraphicsItem *>::iterator iter = selected.begin();

  // Deselect views so that they don't have a border.
  while (iter != selected.end())
	(*iter++)->setSelected(false);

  while ( page_y < m_canvas->height() ) {
    const QRectF clip ( 0, page_y, m_apage_w, m_apage_h );
    QList<QGraphicsItem *> items = m_canvas->items ( clip );

    if ( items.empty () ) {
      page_y += m_apage_h;
      painter.translate ( 0, - trans_y );
      continue;
    }

    if ( page_y != 0 ) printer->newPage();
    page_y += m_apage_h;

    m_canvas->render ( & painter, QRectF(), clip);
    painter.translate ( 0, - trans_y );
  }

  painter.end();

  // Now reselect any views that were selected before
  iter = selected.begin();
  while (iter != selected.end())
	(*iter++)->setSelected(true);
}

void
FigureEditor::
resizeEvent ( QResizeEvent * e )
{
  const QSize & w_size = e->size();

  // need to specify type for vs.net compiler
  double new_w = std::max<double> ( w_size.width (),  m_canvas->width () );
  double new_h = std::max<double> ( w_size.height (), m_canvas->height () );

  setSceneRect( 0, 0, new_w, new_h );
}

void
FigureEditor::
addPage ()
{
  double old_y = m_canvas->height ();
  setSceneRect( 0, 0, m_canvas->width(), old_y + m_apage_h );
  addPageMargin ( 0, old_y );
}

void 
FigureEditor::
saveSelectedAsPixmap ( const std::string & filename )
{
  saveAreaAsPixmap ( getSelectedBounds(), filename );
}

QPixmap *
FigureEditor::
createPixmap ( const QRectF & rect ) const
{
  QPixmap * pixmap = new QPixmap ( rect.width(), rect.height() );
  pixmap->fill(Qt::white);
  QPainter painter;

  QList<QGraphicsItem *> selected = scene()->selectedItems();
  QList<QGraphicsItem *>::iterator iter = selected.begin();

  // Deselect views so that they don't have a border.
  while (iter != selected.end())
	(*iter++)->setSelected(false);

  painter.begin ( pixmap );
  m_canvas->render ( &painter, QRectF(), rect );
  painter.end ();

  // Now reselect
  iter = selected.begin();
  while (iter != selected.end())
	(*iter++)->setSelected(true);

  return pixmap;
}

void
FigureEditor::
saveAreaAsPixmap ( const QRectF & rect, const std::string & filename )
{
  QPixmap * pixmap = createPixmap ( rect );

  QString fn ( filename.c_str() );
  string::size_type i = filename.find_last_of ( '.' );
  string suffix ( filename.substr ( i + 1 ) );
  std::transform ( suffix.begin(), suffix.end(),
	      suffix.begin(), toupper );
  if ( suffix == "JPG" ) suffix = "JPEG";

  pixmap -> save ( fn, suffix.c_str() );

  delete pixmap;
}

void
FigureEditor::
copySelectedToClipboard ()
{
  QPixmap *pixmap = createPixmap(getSelectedBounds());
  QApplication::clipboard()->setPixmap(*pixmap);
  delete pixmap;
}

/** @bug Mouse tracking is not working.
 */
void 
FigureEditor::
setZoomMode ( bool flag )
{
  m_zoom_mode = flag;
  if ( flag ) {
    QApplication::setOverrideCursor( QCursor ( Qt::CrossCursor ) );
    QWidget * view_port = viewport ();
    view_port -> setMouseTracking ( TRUE );
  }
  else {
    QApplication::restoreOverrideCursor();
    QWidget * view_port = viewport ();
    view_port -> setMouseTracking ( FALSE );
  }
}

void
FigureEditor::
leaveEvent ( QEvent * )
{
  QApplication::restoreOverrideCursor ();
}

void
FigureEditor::
enterEvent ( QEvent * )
{
  if ( m_zoom_mode == true ) {
    QApplication::setOverrideCursor ( QCursor ( Qt::CrossCursor ) );
  }
}

bool 
FigureEditor::
getZoomMode ( ) const
{
  return m_zoom_mode;
}

const QString &
FigureEditor::
getAppKey () const
{
  return s_app_key;
}

const QString &
FigureEditor::
getRegistry () const
{
  return s_registry;
}

void
FigureEditor::moveSelectedToTop()
{
	QList<QGraphicsItem *> selected = scene()->selectedItems();
	QList<QGraphicsItem *>::iterator iter = selected.begin();

	while (iter != selected.end())
		(*iter++)->setZValue(maximumZ() + 1.);
}

