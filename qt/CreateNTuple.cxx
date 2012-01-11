/** @file

hippodraw::CreateNTuple class implementation

Copyright (C)  2006  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: CreateNTuple.cxx,v 1.28 2007/07/02 18:17:16 pfkeb Exp $

*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "CreateNTuple.h"

#include "CanvasWindow.h"
#include "WindowController.h"
#include "Inspector.h"
#include "QtFileDialog.h"

#include "qcombobox.h"
#include "qlineedit.h"
#include "qmessagebox.h"

#include "datasrcs/DataSourceController.h"
#include "datasrcs/NTupleController.h"
#include "datasrcs/NTuple.h"
#include "datasrcs/TupleCut.h"

#include "controllers/CutController.h"
#include "plotters/Cut2DPlotter.h"

#include "fits/FitsController.h"

#include <cassert>

using std::string;
using std::vector;

using namespace hippodraw;

CreateNTuple::
CreateNTuple ( QWidget * parent, const char * name )
  : QDialog(parent)
{
  setObjectName ( name );
  setupUi ( this );
  updateNTupleIn();
  currentDataSourceChanged (0);
}


void
CreateNTuple::
updateNTupleIn ()
{
  const vector < string > & nt_vector
    = DataSourceController::instance() -> getNTupleNames ();
  if ( nt_vector.empty () ) {
    m_ntuple_in -> clear ();
    return;
  }

  unsigned int count = m_ntuple_in -> count ();
  if ( count == nt_vector.size () ) return;

  m_ntuple_in -> clear();
  vector < string > ::const_iterator first = nt_vector.begin();
  while ( first != nt_vector.end() ) {
    const string & name = *first++;
    m_ntuple_in->addItem ( name.c_str() );
  }

}

void
CreateNTuple::
updateColumnList ()
{
  
  //m_ColumnListView->clear();   // FK - Should not be needed (?)

  DataSource * current_ds = DataSourceController::instance() -> getCurrent();
  columnNumber = current_ds -> columns();

  // temporarily disable sorting
  m_ColumnListView -> setSortingEnabled ( false );
  m_ColumnListView -> setRowCount ( columnNumber );

  for ( unsigned int i = 0; i < columnNumber; i++ )
    {
      QString dummy = QString ("%1").arg(i);
      QString label ( current_ds -> getLabelAt ( i ).c_str() );

      QTableWidgetItem * col1 = new QTableWidgetItem ( dummy );
      col1 -> setFlags ( Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
      col1 -> setCheckState ( Qt::Unchecked );
      m_ColumnListView -> setItem ( i, 0, col1 );
      m_ColumnListView -> setItem ( i, 1,
				    new QTableWidgetItem ( label ) );
    }

  m_ColumnListView -> setSortingEnabled ( true );
}

void
CreateNTuple::
updateCutList()
{
#if 0 // FK - Should not be needed  (?)
  m_CutListView->clear();
  QStringList m_CutListView_Headers;
  m_CutListView_Headers
    << tr("Index")
    << tr("Cut Name")
    << tr("Min")
    << tr("Max")
    << tr("2D Cut")
    << tr("Inversion");
  m_CutListView -> setHorizontalHeaderLabels ( m_CutListView_Headers );
#endif // 0

  m_cut_list.clear ();
  
  // temporarily disable sorting
  m_CutListView -> setSortingEnabled ( false );
  
  const std::vector < PlotterBase * > & cutlist = getDataCutList();
  std::vector < PlotterBase * >::const_iterator it = cutlist.begin();
  m_CutListView -> setRowCount ( cutlist.size() );

  unsigned int row = 0;

  for ( ; it != cutlist.end(); ++ it )
    {
      PlotterBase * plotter = *it;
      CutPlotter * cp = dynamic_cast < CutPlotter * > (plotter);
      cp -> fillCutList ( m_cut_list );

      Range cutrange = cp ->  getCutRange();
      QString dummy = QString ("%1").arg(row);
      const std::string & label = cp -> getLabel(Axes::X);
      double min = cutrange.low();
      double max = cutrange.high();
      QString inversion = cp -> getCutInversion() ? tr("Inverted") : QString();

      QTableWidgetItem * col1 = new QTableWidgetItem ( dummy );
      col1 -> setFlags ( Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
      col1 -> setCheckState ( Qt::Unchecked );
      m_CutListView -> setItem ( row, 0, col1 );
      m_CutListView -> setItem ( row, 1,
				 new QTableWidgetItem ( QString::fromAscii ( label.c_str() ) ) );
      m_CutListView -> setItem ( row, 2, 
				 new QTableWidgetItem ( QString::number ( min ) ) );
      m_CutListView -> setItem ( row, 3,
				 new QTableWidgetItem ( QString::number ( max ) ) );
      m_CutListView -> setItem ( row, 5,
				 new QTableWidgetItem ( inversion ) );
      
      Cut2DPlotter * cp2 = dynamic_cast < Cut2DPlotter * > (cp);
      if (cp2) 
	{
	  m_CutListView -> setItem ( row, 4, 
				     new QTableWidgetItem ( tr( "2D-X" ) ) );
	  
	  // For Cut2DPlotter, update the information for the Y axis
	  ++row;
	  Range cutrange = cp2 -> getCutRangeY();
	  QString dummy = QString("%1").arg(row);
	  const std::string & label = cp2-> getLabel(Axes::Y);
	  double min = cutrange.low();
	  double max = cutrange.high();

	  QTableWidgetItem * col1 =
	    new QTableWidgetItem ( dummy );
	  col1 -> setFlags ( Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
	  col1 -> setCheckState ( Qt::Unchecked );
	  m_CutListView -> setItem ( row, 0, col1);
	  m_CutListView -> setItem ( row, 1, 
				     new QTableWidgetItem ( QString::fromAscii ( label.c_str() ) ) );
	  m_CutListView -> setItem ( row, 2,
				     new QTableWidgetItem ( QString::number ( min ) ) );
	  m_CutListView -> setItem ( row, 3,
				     new QTableWidgetItem ( QString::number ( max ) ) );
	  m_CutListView -> setItem ( row, 4,
				     new QTableWidgetItem ( tr("2D-Y") ) );
	  m_CutListView -> setItem ( row, 5,
				     new QTableWidgetItem ( inversion ) );
	  ++row;
	}
      else
	{
	  m_CutListView -> setItem ( row, 4,
				     new QTableWidgetItem ( tr( "1D" ) ) );
	  ++row;
	}
    }  
  cutNumber = row;

  m_CutListView -> setSortingEnabled ( true );

  assert ( m_cut_list.size() == cutNumber );
}

void
CreateNTuple::
currentDataSourceChanged ( int item )
{
  DataSourceController * controller = DataSourceController::instance ();
  controller -> setCurrentIndex ( item );
 
  // Set the ComboBox.
  m_ntuple_in-> setCurrentIndex ( item );
  updateColumnList();
  updateCutList();

}

void
CreateNTuple::
getPlotterList ( std::vector < PlotterBase * > & plotterlist )
{
  plotterlist.clear();
  CanvasWindow * canvaz = WindowController::instance () -> currentCanvas();

  if ( canvaz !=0 ) {
    canvaz -> fillPlotterList ( plotterlist );
  }
}

const std::vector < PlotterBase * > &
CreateNTuple::
getDataCutList ( )
{
  vector < PlotterBase * > plotterlist;
  getPlotterList ( plotterlist );

  CutController * cutcontroller = CutController::instance();
  const DataSource * current_ds 
    = DataSourceController::instance() -> getCurrent();

  return cutcontroller->getCutList ( plotterlist, current_ds );
}


void
CreateNTuple::
createButtonClicked()
{
  DataSourceController * ds_controller = DataSourceController::instance();
  NTupleController * ntc = NTupleController::instance();
  DataSource * ds = ds_controller -> getCurrent();

  std::string name( (m_ntuple_out->text()).toLatin1().data() );

  if ( name.size()==0 ) name = "<no name>";  // default title

  setColumnList();
  setCutList();

  NTuple * nt = ntc->createNTuple (m_column_list, m_cut_list, ds);

  // Do nothing if no column or row selected.
  if ((nt->rows()==0) || (nt->columns()==0)) return;

  // Register the new NTuple.
  ntc-> registerNTuple ( name, nt );

  // Update the dialog and inspector.
  updateNTupleIn();
  WindowController::instance () -> getInspector() -> update();
}

void
CreateNTuple::
createFileButtonClicked()
{

  QString filename = QtFileDialog::getExportTupleFilename ( this );

  if (filename.isEmpty()) return;


  // Write data to the file.
  int pos = filename.lastIndexOf ('.' );
  QString suffix = filename.mid (pos);
  
  DataSourceController * ds_controller = DataSourceController::instance();
  DataSource * ds = ds_controller -> getCurrent();

  QString name = m_ntuple_out->text();

  if ( name.isEmpty() ) name = tr("<no name>");  // default title

  setColumnList();
  setCutList();

  if ( suffix == ".fits" || suffix == ".gz" ) 
    {
#ifdef HAVE_CFITSIO
      FitsController * fc = FitsController::instance();
      // int retVal =
//       fc -> writeNTupleToFile ( m_column_list, m_cut_list, ds, filename, name);
      fc -> writeNTupleToFile ( ds, filename.toLatin1().data(),
				name.toLatin1().data(),
				m_column_list, m_cut_list );
#else
      QString message ( "Sorry, can not create FITS file.\n"
			"The application was not built with\n"
			"optional FITS support." );

      QMessageBox::critical ( this, // parent
			      "Create NTuple", // caption
			      message,
			      QMessageBox::Ok,
			      Qt::NoButton );
#endif
    }
  else
    {
      NTupleController * ntc = NTupleController::instance();
      // Need to deal with retVal;
      // int retVal = 
      ntc->createNTupleToFile (m_column_list, m_cut_list, ds,
			       filename.toLatin1().data(),
			       name.toLatin1().data());
    }

  accept();

}

void
CreateNTuple::
cutListCheckBox_toggled ( bool yes )
{
  Qt::CheckState checked = yes ? Qt::Checked : Qt::Unchecked;

  for ( int row = 0; row < m_CutListView -> rowCount(); ++row )
    m_CutListView -> item ( row, 0 ) -> setCheckState ( checked );
}

void
CreateNTuple::
columnListCheckBox_toggled ( bool yes )
{
  Qt::CheckState checked = yes ? Qt::Checked : Qt::Unchecked;

  for ( int row = 0; row < m_ColumnListView -> rowCount(); ++row )
    m_ColumnListView -> item ( row, 0 ) -> setCheckState ( checked );
}

void
CreateNTuple::
setColumnList()
{
  m_column_list.clear();

  for (unsigned int i = 0; i < columnNumber; i++ ) {
    if ( m_ColumnListView -> item ( i, 0 ) -> checkState() == Qt::Checked ) {
      std::string name ( m_ColumnListView -> item ( i, 1 ) -> text().toLatin1().data() );
      m_column_list.push_back ( name );
    }
  }
}

void CreateNTuple::
setCutList()
{
  // Keep newNTuple unchanged if there is no cuts.
  if ( cutNumber == 0 ) return;

  // First remove the cuts not used.
  int row = 0;
  vector < const TupleCut * >::iterator first = m_cut_list.begin ();
  while ( first != m_cut_list.end() ) {
    if ( m_CutListView -> item ( row, 0 ) -> checkState() == Qt::Unchecked ) {
      first = m_cut_list.erase ( first );
    } else {
      ++first;
    }
    ++row;
  }
}
