/* -*- mode: c++ -*- */

/** @file

hippodraw::CreateNTuple class interface

Copyright (C) 2006   The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: CreateNTuple.h,v 1.8 2006/04/25 18:30:46 pfkeb Exp $

*/

#ifndef CreateNTuple_H
#define CreateNTuple_H

#include "qtui/ui_CreateNTupleBase.h"

#include <map>
#include <vector>
#include <string>

namespace hippodraw {

class PlotterBase;
class NTuple;
class TupleCut;

/** This class manages the Create NTuple dialog.

    @author Xie Fang <xiefang@stanford.edu>
*/
class CreateNTuple : public QDialog,
 private Ui::CreateNTupleBase
 {
   Q_OBJECT
private:

  unsigned int columnNumber;
  unsigned int cutNumber;

  /** The list of TupleCut objects.
   */
  std::vector < const TupleCut * > m_cut_list;
  std::vector < std::string > m_column_list;

  NTuple * newNTuple;

  /** Update the ComboBox */
  void updateNTupleIn();

  /** Update column list view */
  void updateColumnList();

  /** Update cut list view */
  void updateCutList();

  /** Initiate the list views, set column names.*/
  void initListViews();

  /** Get all the data cut used by the DataSource.
   */
  const std::vector < PlotterBase * > &  getDataCutList();

  /** Get all the plotter used by the canvas window. */
  void getPlotterList( std::vector < PlotterBase * > & );

  /** Filter according to the selected column. */
  void setColumnList();

  /** Filter according to the selected cut. */ 
  void setCutList();


protected slots:

  /** Current Item in the ComboBox changed. */
  virtual void currentDataSourceChanged( int item );

  /** Create a new NTuple. */
  virtual void createButtonClicked();

  /** Create a new NTuple and save to a file. */
  virtual void createFileButtonClicked();

  /** Select or deselect all in cut list. */
  virtual void cutListCheckBox_toggled ( bool yes );

  /** Select or deselect all in column list. */
  virtual void columnListCheckBox_toggled ( bool yes );

public:

  /** The constructor. 
   */ 
    CreateNTuple ( QWidget * parent = 0,
		   const char * name = 0 );

};

} // namespace hippodraw

#endif // CreateNTuple_H
