/** @file

hippodraw::PlotTable class implementation

Copyright (C) 2003-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: PlotTable.cxx,v 1.18 2006/04/30 22:20:46 pfkeb Exp $

*/

#include "PlotTable.h"

#include "CanvasView.h"
#include "PlotTableEvent.h"
#include "QtView.h"

#include "datareps/DataRep.h"
#include "graphics/ViewBase.h"
#include "plotters/PlotterBase.h"

#include <QApplication>
#include <QPushButton>
#include <QString>

#include <cassert>
#include <vector>

using std::string;
using std::vector;

using namespace hippodraw;

PlotTable::
PlotTable ()
  : PlotTableBase (),
    m_observer ( 0 ),
    m_browser_mode ( false )
{
  setupUi(this);
}

void
PlotTable::
setCanvas ( CanvasView * canvas )
{
  m_canvas_view = canvas;
  initialize ();
}

void
PlotTable::
initialize ()
{
  //m_plot_table->clear();

  const vector < const ViewBase * > & views = m_canvas_view->views ();
  vector < const ViewBase * > :: const_iterator first = views.begin ();

  m_plot_table -> setRowCount ( views.size() );
  int row = 0;

  while ( first != views.end () ) {
    const ViewBase * view = *first++;
    const PlotterBase * plotter = view->getPlotter ();
    QString title ( plotter->getTitle ().c_str() );
    QString x_axis ( plotter->getLabel ( Axes::X ).c_str() );
    QString y_axis ( plotter->getLabel ( Axes::Y ).c_str() );
    QString z_axis ( QString::null );
    if ( plotter->hasAxis ( Axes::Z ) ) {
      const string & z = plotter->getLabel ( Axes::Z );
      z_axis = z.c_str();
    }
    const DataRep * rep = plotter->getDataRep ( 0 );
    const string & t = rep->name ();
    QString type ( t.c_str() );

    QTableWidgetItem * firstColumn = new QTableWidgetItem ( title );
    m_plot_table -> setItem ( row, 0, firstColumn );
    m_plot_table -> setItem ( row, 1, new QTableWidgetItem ( type ) );
    m_plot_table -> setItem ( row, 2, new QTableWidgetItem ( x_axis ) );
    m_plot_table -> setItem ( row, 3, new QTableWidgetItem ( y_axis ) );
    m_plot_table -> setItem ( row, 4, new QTableWidgetItem ( z_axis ) );

    m_view_map [ firstColumn ] = view;
  }
}

void
PlotTable::
selectionChanged ( )
{
  if ( m_plot_table -> selectedRanges() . count() == 1) {
    assert ( m_plot_table -> selectedRanges() . at (0) . rowCount() == 1 );
    QTableWidgetItem * item = 
      m_plot_table -> item ( m_plot_table -> selectedRanges() . at(0) . topRow(), 0 );
    
    const ViewBase * view = m_view_map [ item ];
    ViewBase * v = const_cast < ViewBase * > ( view );
    QtView * vv = dynamic_cast < QtView * > ( v );
    
    m_canvas_view -> setSelectedItem ( vv );
    m_canvas_view -> ensureVisible ( vv );
    if ( m_browser_mode ) {
      copyButton -> setEnabled ( true );
    }
  }
}

void
PlotTable::
setBrowserMode ( bool yes, QWidget * observer )
{
  m_browser_mode = yes;
  m_observer = observer;
  if ( yes == false ) copyButton -> setEnabled ( yes );
}

void
PlotTable::
copyButton_clicked ()
{
  m_canvas_view -> copySelectedToPasteboard ();

  PlotTableEvent * event = new PlotTableEvent ( PlotTableEvent::Copy );
  QApplication::postEvent ( m_observer, event );
}

void
PlotTable::
closeButton_clicked ()
{
  if ( m_browser_mode ) {
    PlotTableEvent * event = new PlotTableEvent ( PlotTableEvent::Close );
    QApplication::postEvent ( m_observer, event );
  }
  m_browser_mode = false;
}
