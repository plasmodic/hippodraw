/* -*- mode: c++ -*- */

/** @file

hippodraw::PlotTable class interface

Copyright (C) 2003-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: PlotTable.h,v 1.18 2006/05/01 23:05:36 pfkeb Exp $

*/

#ifndef _PlotTable_H_
#define _PlotTable_H_

#ifdef _MSC_VER
#include <msdevstudio/MSconfig.h>
#endif

#include "libqthippo.h"

#include "qtui/ui_PlotTableBase.h"

#include <map>

namespace hippodraw {

class CanvasView;
class ViewBase;

/** A derived class of PlotTableBase class which is generated by the
    Qt designer.  It provides an PlotTable which lists all plots on a
    canvas.

    @request Display TextRep in more friendly way.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_QTHIPPOPLOT_API PlotTable : public QDialog,
				      private Ui::PlotTableBase
{
  Q_OBJECT

private:

  /** The canvas that is currently being used.
   */
  CanvasView * m_canvas_view;

  /** A map to associate a QListViewItem with its ViewBase. */
  std::map < const QTableWidgetItem *, const ViewBase * > m_view_map;

  /** The Window that initiated the browsing, if any.
   */
  QWidget * m_observer;

  /** The mode of the plot table.
      @sa setMode(bool)
  */
  bool m_browser_mode;

  /** Initialized the table. */
  void initialize ();

protected slots:

  virtual void selectionChanged ();

  /** Responds to click on copy button.
   */
  virtual void copyButton_clicked ();

  /** Response to click on close button.
   */
  virtual void closeButton_clicked ();

public:

  /** The default constructor.
   */
  PlotTable ();

  /** Sets the canvas to be used by the table. */
  void setCanvas ( CanvasView * canvas );

  /** Sets the mode of the plot table.  When @c true, the mode is
      browser mode, i.e. the table is connected to a CanvasView that
      does not have a Window. The widget initiating the browsing, @a
      observer, will receive events from the table. When @c false (the
      default), the table is connected to a CanvasView with an open
      Window.
  */
  void setBrowserMode ( bool browser, QWidget * observer = 0 );

};

} // namespace hippodraw

#endif // _PlotTable_H_
