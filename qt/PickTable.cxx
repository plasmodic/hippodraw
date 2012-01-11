/** @file

hippodraw::PickTable class implementation

Copyright (C) 2003-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: PickTable.cxx,v 1.63 2006/05/01 23:02:40 pfkeb Exp $

*/

#include "PickTable.h"

#include "controllers/DisplayController.h"
#include "datareps/DataRep.h"
#include "datasrcs/DataSourceController.h"
#include "datasrcs/NTuple.h"
#include "datasrcs/NTupleSorter.h"
#include "graphics/SymbolType.h"
#include "plotters/PlotterBase.h"

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>

#include <cassert>

using std::string;
using std::vector;

using namespace hippodraw;

PickTable::
PickTable ( PlotterBase * plotter )
{
  setupUi(this);

  m_target = plotter;

  NTuple * ntuple = plotter -> createPickTuple ();
  m_sorter = new NTupleSorter ( ntuple );

  const string & name = ntuple -> getName ();
  m_title->setText ( name.c_str() );

  const DataRep * rep = plotter->getDataRep ( 0 );
  const string & type = rep -> name ();

  m_type->setText ( tr("Plot type : %1").arg(type.c_str()) );

  const vector < string > & labels = ntuple -> getLabels ();
  QStringList headers;
  headers << tr("Item number")
	  << labels[1].c_str()
	  << labels[2].c_str();

  unsigned int size = ntuple -> columns ();
  m_pick_table -> setColumnCount(size);
  m_column = size;
  if ( size == 3 ) {
    zLineEdit -> setEnabled ( false );
    wLineEdit -> setEnabled ( false );
  }
  else {
    headers << labels[3].c_str();
    if ( size == 5 ) {
      headers << labels[4].c_str();
    }
    else wLineEdit -> setEnabled ( false );
  }

  m_pick_table -> setHorizontalHeaderLabels(headers);

  DataSourceController::instance()->registerNTuple ( ntuple );

  QHeaderView * header = m_pick_table->horizontalHeader();
  connect ( header, SIGNAL ( sectionClicked (int) ), 
	    this, SLOT( listSorted ( int ) ) );
  header->setClickable ( true );
  header->setSortIndicatorShown ( true );
  header->setStretchLastSection ( true );
  m_pick_table->setSortingEnabled ( false );
  m_delete -> setEnabled ( false );

  // Picked points are show by default.
  addDataRep();
}

void
PickTable::
listSorted ( int i )
{
  assert(m_pick_table -> selectedRanges().count() <= 1);

  m_sorter -> setSorting ( i );
  m_sorter -> sort ( );

  if (m_pick_table -> selectedRanges().count() == 1) {
    assert(m_pick_table -> selectedRanges() . at(0) . rowCount() == 1);

    int row = m_pick_table -> selectedRanges () . at(0) . topRow();
    QTableWidgetItem * item = m_pick_table -> item ( row, 0 );
    unsigned int selected = item -> text() . toUInt ();
    
    refreshItems ( selected );
  }
}

void PickTable::clear()
{
  m_pick_table->clear();
  m_sorter->clear();
}

void
PickTable::
addItem ( std::vector < double > & v )
{
  unsigned int row = m_sorter -> rows ();
  v[0] = row;
  m_sorter -> addRow ( v );

  refreshItems ( row );
}

void
PickTable::
refreshItems ( unsigned int select )
{
  //m_pick_table -> clear();

  unsigned int row = m_sorter->rows ();
  bool yes = row > 0;
  m_delete -> setEnabled ( yes );

  m_pick_table -> setRowCount ( row );

  while ( row-- != 0 ) {
    const vector < double > & vec = m_sorter->getRow ( row );
    unsigned int current = static_cast < unsigned int > ( vec[0] );
    unsigned int size = vec.size();

    for ( unsigned int i = 0; i < size; i++ ) {
      m_pick_table -> setItem ( row, i,
				new QTableWidgetItem ( QString::number(vec[i]) ) );
    }

    if ( select == current ) {
      m_pick_table -> selectRow ( row );
      m_pick_table -> scrollToItem ( m_pick_table
				     -> item ( row, 0 ) );
    }
  }
}

unsigned int
PickTable::
indexOf ( QTableWidgetItem * target )
{
  return target->row();
}

void PickTable::deleteSelectedItem ()
{
  assert ( m_pick_table -> selectedRanges() . count() <= 1);
  if ( m_pick_table -> selectedRanges() . count() == 1 ) {
    unsigned int index = m_pick_table -> selectedItems() . at ( 0 ) -> row();

    m_sorter->eraseRow ( index );

    unsigned int row = index == 0 ? 0 : index - 1;
    refreshItems ( row );
  }
}

void
PickTable::
addDataRep()
{
  const string plotTypeStr ( "Scatter Plot" );  
  NTuple * ntuple = m_sorter -> getNTuple ();
  const vector < string > & labels = ntuple -> getLabels ();
  vector < string > bindings;
  bindings.push_back ( labels[1] );
  bindings.push_back ( labels[2] );
  if ( labels.size() == 4 ) {
    if ( labels[3] == "Density" ) {
      bindings[1] = "Density";
    }
  }

  DisplayController * controller = DisplayController::instance();
  DataRep * rep 
    = controller -> addDataRep ( m_target, plotTypeStr, 
				 ntuple, bindings );
  m_datarep = rep;

  const Color red ( Color::red );
  rep -> setRepColor ( red );
  rep -> setRepStyle ( Symbol::CIRCLE );
  rep -> setRepSize ( 6.0 );
}

void
PickTable::
m_pick_table_selectionChanged ()
{
  m_delete -> setEnabled ( m_pick_table -> selectedItems () . count() > 0 );
}

NTuple *
PickTable::
getPickTable () const
{
  NTuple * ntuple = m_sorter -> getNTuple ();
  return ntuple;
}

void
PickTable::
addEntry()
{
  vector <double> new_entry;
  double x=xLineEdit->text().toDouble();
  double y=yLineEdit->text().toDouble();
  new_entry.push_back(3.0);
  new_entry.push_back(x);
  new_entry.push_back(y);
  
  if ( m_column > 3 ){
    double z=zLineEdit->text().toDouble();
    new_entry.push_back(z);
  }
  if ( m_column == 5 ){
    double w=wLineEdit->text().toDouble();
    new_entry.push_back(w);
  }

  addItem(new_entry);
}

void
PickTable::
pickedCheckBoxClicked()
{
  bool show = m_pickedCheckBox->isChecked();
  if (show) addDataRep();
  else m_target -> removeDataRep ( m_datarep );
}
