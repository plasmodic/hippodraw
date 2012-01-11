/* -*- mode: c++ -*- */

/** @file

hippodraw::FigureEditor class interface and nested class implementation

Copyright (C) 2001-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FigureEditor.h,v 1.121 2007/03/21 18:22:06 pfkeb Exp $

*/

#ifndef FIGUREEDITOR_H
#define FIGUREEDITOR_H

#include "libqthippo.h"

#ifdef _MSC_VER
#include <msdevstudio/MSconfig.h>
#endif

#include <qglobal.h> // for version
#include <QtGui/QGraphicsView>

#include <string>
#include <vector>

class QMouseEvent;
class QResizeEvent;

namespace hippodraw {

class PickTable;

/** This class implements additional QCanvasView functionality.   

    @attention This class knows about QCanvasItem objects.  Any
    methods that need to know about classes derived from QCanvasItem
    that are not part of Qt should be implemented in a derived class.

    The original source code was taken from
    $QTDIR/examples/canvas/canvas.h of version 3.0 of Qt.

    @bug @@@@@@ Mac OS X only, printer margins not showing in correct
    place on the Canvas, however printing is working fine.

    @request Move any attached text displays along with moved plotter
    display.

    @request @@@@@@ For plots using pixmap, do not draw plot when
    resizing until mouse is release.

    @todo Need ability to group and un-group items.

    @todo add page by adding another standard size page (either A4 or US
    Letter depending on preferences set somewhere.
    
    @author Kaustuv <kaustuv@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu> 
    @author Sanket B. Malde <sanket@stanford.edu> 
    @author Engineers at <a
    href="http://www.trolltech.com"> Trolltech </a> who wrote
    examples/canvas
*/
class MDL_QTHIPPOPLOT_API FigureEditor : public QGraphicsView
{

Q_OBJECT

protected: 

  /** The actual Page width in Postscript points. */
  double m_apage_w;

  /** The actual Page height in Postscript points. */
  double m_apage_h;

  /** The scale factor.  The factor by which drawing has been reduced
      so that a canvas page fits in the window.  This factor should be
      applied in inverse before printing.
  */
  double m_scale_factor;

  /** Set to @c true if zoom Mode is enabled
   */
  bool m_zoom_mode;

  /** The single QPrinter instance. */
  static QPrinter * s_printer;

  /** A flag to indicate if rectangle display usable printer boundary
      should be displayed or not.
  */
  bool m_printer_bounds;

  /** a flag set to @c true if the printer margins should be ignored
      when adding a plot to the canvas.
  */
  bool m_ignore_margin;

  /** Re-sizes the canvas if necessary so that the @a item is on the
      canvas.  Changes the height by one page size while keeping the
      width constant. */
  void resizeCanvasToFit ( QGraphicsItem * item );

  /** Saves the printer setting with QSettings. */
  void savePrinterSettings ();

  /** Initializes the printer settings from the saved settings.  If
      setting have not been saved, set the default values. 
  */
  void initPrinter ();

  /** Adds page margin.  Adds an indication of the page's margin.  Only
      the contents inside the margin is printable. 
  */
  void addPageMargin ( int x, int y );

  /** Calculate the size of a page by using the printer metrics.  The
      size of a printable page is calculated on the basis that one
      pixel will become one Postscript point.  A QPaintDeviceMetrics
      object is created to find the actual and usable size of a page
      in millimeters which is then converted to Postscript Points.
  */
  void calcPrinterMetrics ( QPaintDevice * );

  /** Returns the largest Z value of all the items on the canvas.
   */
  double maximumZ () const;

  /** Sets the unselected items to visible or not. 
   */
  void setUnselectedVisible ( bool yes );
  
  /** Creates and returns a new QPixmap from the area defined on the canvas.
   */
  QPixmap * createPixmap ( const QRectF & rectangle ) const;

protected:

  /** The usable Page width in Postscript points. */
  int m_upage_w;

  /** The usable Page height in Postscript points. */
  int m_upage_h;

  /** The QSettings application key for searching for setting.  The
      initial value is "HippoDraw", but may be changed by Starkey.
  */
  static QString  s_app_key;

  /** The QSettings Windows registry key. The key is the same as used
      for by Trolltech for their Qt applications 
  */
  static QString s_registry;

  /** The canvas object.  Although one can always retrieve the canvas
      object by calling the canvas() member function, it is convenient
      to have this as a data member. */
  QGraphicsScene * m_canvas;

  /** Scrolls the canvas, if needed, so the QCanvasItem @a item is
      visible. */
  void makeVisible ( const QGraphicsItem * item );

  /** Returns a QRect that enclosed the selected QCanvasItem objects. */
  QRectF getSelectedBounds () const;

  /** Sets all QCanvasItems that collide with @a target to selected
      state.
  */
  void setCollidingSelected ( const QGraphicsItem * target );

  /** Save the selected objects as Pixmap image file @a filename. 
   */
  void saveSelectedAsPixmap ( const std::string & filename );

  /** Saves the area @a area as pixmap to file @a filename.
   */
  void saveAreaAsPixmap ( const QRectF & area, const std::string & filename );

  /** Adds selected items to system clipboard.
   */
  void copySelectedToClipboard ();

  /** Prints the view to the @a printer. 
   */
  void print ( QPrinter * printer );

public:

  /** The constructor. */
  FigureEditor ( QGraphicsScene *, QWidget * parent = 0, 
		 const char * name = 0, Qt::WFlags f = 0 );

  /** Sets the QSettings application key. */
  void static setAppKey ( const std::string & );

  /** Sets showing the printer margins on or off.
   */
  void showPrinterMargins ( bool on );

  /** Set the default printer settings. */
  void setPrinterSettings ();

  /** Prints the view to the printer.  Presents the Print dialog to
      user and then prints the view to the selected printer. */
  virtual void print ();

    /** Prints the view to Postscript file @a filename.
   */
  virtual void print ( const std::string & filename );

  /** Re-sizes the QCanvas in response to parent window receiving
      QResizeEvent.  The QCanvas may grow in size, but is never made
      smaller by this method.*/
  void resizeEvent ( QResizeEvent * e );

  /** Capture delete events so that the item is deleted */
  void keyPressEvent(QKeyEvent *event);

  /** Adds a page to the canvas. */
  void addPage ();

  /** Returns the selected QCanvasItem. */
  QGraphicsItem * selectedItem () const;

  /** Set the selected QCanvasItem. This is called only when just one item
      is selected. */
  void setSelectedItem ( QGraphicsItem * );

  /** Sets selection flag on all QCanvasItem objects. */
  void setAllSelected ( bool flag = true );

  /** Sets the selected QCanvasItem to be locked in size and position. */
  void setLocked ( bool flag );


 /** Sets the zoom mode to @a flag */
  void setZoomMode ( bool flag );

  /** Returns the bool indicating whether zoom mode is enabled or not  */
  bool getZoomMode ( ) const;
  
  /** Removes all items from canvas and destroy them.  
   */
  void clear();

  /** Moves the QCanvasRectangle object to place on the canvas where
      it will not collide with existing QCanvasItem object and not be
      off the usable page area.
  */
  void placeGraph ( QGraphicsRectItem * );

  /** Adds the item to the canvas.   Expands the canvas' size if necessary. */
  void add ( QGraphicsItem * item );

  /** Adds the item to the canvas.  This method differs from add in
      that it displaces the item by a fixed amount before adding. */
  void paste ( QGraphicsItem * item );

  /** Removes the item from the canvas and destroys it.  @todo This
      function has a side effect of clearing the selection list,
      probably should not do that.
  */
  void remove ( QGraphicsItem * item );

  /** Return the vector of selected items. */
  std::vector < QGraphicsItem * > getSelectedItems () const;

  /** Returns true if just one item is selected, else returns false. */
  bool isSingleItemSelected () const;

  /** Scales the size of the images by @a factor.
      The factor might be modified to ensure we see
      at least a page in single glance of the canvas.
   */
  void scaleBy ( double factor );

  /** Enlarges the view of the canvas. */
  void viewZoomIn();

  /** Reduces the view of the canvas. */
  void viewZoomOut();

  /** Returns the view to normal size. 
   */
  void viewZoomReset ();

  /** Show the pick table. */
  virtual void viewShowPickTable() = 0;

  /** Scrolls the canvas, if needed, so the QGraphicsItem @a is
      visible. */
  void ensureVisible ( QGraphicsItem * item );

  /** Returns a reference the application's QSettings key.
   */
  const QString & getAppKey () const;

  /** Returns a reference to the application's registry
   */
  const QString & getRegistry () const;

  /** Grabs the event when mouse cursor enters the widget.
   */
  void enterEvent ( QEvent * );

  /** Grabs the event when mouse cursor leaves the widget.
   */
  void leaveEvent ( QEvent * );

public slots:
  /** Move selected plot to foreground.
    */
  void moveSelectedToTop();

};

} // namespace hippodraw

#endif // FIGUREEDITOR_H
