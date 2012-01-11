/** @file

hippodraw::Inspector class implementation

oCopyright (C) 2002-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: Inspector.cxx,v 1.649 2007/09/14 22:42:36 pfkeb Exp $

*/

#warning TODO: This file contains several occurances of std::string \
         which should be removed in order to make the code translatable. \
         However one has to be careful which parts of the code can be \
         translated (things that are not written to a file).

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "Inspector.h"

#include "CanvasSelectionEvent.h"
#include "CanvasWindow.h"
#include "PlotterEvent.h"
#include "WindowController.h"
#include "AxisWidget.h"
#include "QtFont.h"
#include "util.h"

#include <qapplication.h>

#include <QtCore/QEvent>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>

#include <QToolTip>
#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QLabel>
#include <QInputDialog>
#include <QFontDialog>
#include <QTabWidget>
#include <QSettings>
#include <QStringList>
#include <QTextStream>


#include "colorreps/BinToColor.h"
#include "controllers/CutController.h"
#include "controllers/DataRepController.h"
#include "controllers/DisplayController.h"
#include "controllers/FunctionController.h"

#include "datareps/FunctionParameter.h"
#include "datareps/CompositeFunctionRep.h"

#include "datasrcs/DataSourceController.h"
#include "datasrcs/NTuple.h"
#include "datasrcs/TupleCut.h"

#include "plotters/CutPlotter.h"
#include "plotters/TextPlotter.h"

#include "projectors/NTupleProjector.h"

#include "reps/ContourPointRep.h"
#include "transforms/TransformFactory.h"
#include "transforms/PeriodicBinaryTransform.h"

#ifdef HAVE_ROOT
#include "root/RootController.h"
#include "boost/tokenizer.hpp"
#include "boost/lexical_cast.hpp"
#endif

#include "pattern/string_convert.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

using std::cout;
using std::endl;

#include <cmath>
#include <cassert>

using std::map;
using std::runtime_error;
using std::string;
using std::vector;

using namespace hippodraw;


namespace {

  const QString qyes = qApp->translate( "Inspector", "Yes" );
  const QString qno = qApp->translate( "Inspector", "No" );

}


void
Inspector::
stringTokenize ( std::string input, const std::string & delimiters,
		 std::vector<std::string> & tokens, bool clear )
{
      if (clear) {
         tokens.clear();
      }
      std::string::size_type j;
      while ( (j = input.find_first_of(delimiters)) != std::string::npos ) {
         if (j != 0) {
            tokens.push_back(input.substr(0, j));
         }
         input = input.substr(j+1);
      }
      tokens.push_back(input);
      if (tokens.back() == "") {
         tokens.pop_back();
      }
}


QString    Inspector::s_registry ( "/Trolltech" );


Inspector::
Inspector ( QWidget * parent, const char * name, bool modal, Qt::WFlags flags )
  : QDialog ( parent, flags ),
    m_plotter ( 0 ),
    m_is_updating ( false ),
    m_user_models_loaded ( false )
{
  setObjectName ( name );
  setModal ( modal );
  setupUi ( this );
  init ();
  FunctionController * controller = FunctionController::instance();
  const vector < string > & names = controller -> getFitterNames ();
  for ( unsigned int i = 0; i < names.size(); i++ ) {
    QString name ( names[i].c_str() );
    m_fitter_names -> addItem ( name );
  }

  connect ( m_all_ntuples, SIGNAL ( currentIndexChanged ( int ) ),
	    this, SLOT ( dataNTupleSelChanged ( int ) ) );

  connect ( axisWidget1, SIGNAL ( lowTextReturnPressed() ),
	    this, SLOT ( setLowText() ) );

  connect ( axisWidget2, SIGNAL ( lowTextReturnPressed() ),
	    this, SLOT ( cutText_returnPressed() ) );

  connect ( axisWidget1, SIGNAL ( highTextReturnPressed() ),
	    this, SLOT ( setHighText() ) );

  connect ( axisWidget2, SIGNAL ( highTextReturnPressed() ),
	    this, SLOT ( cutText_returnPressed() ) );

  connect ( axisWidget1, SIGNAL ( lowSliderReleased() ),
	    this, SLOT ( lowRangeDrag() ) );

  connect ( axisWidget2, SIGNAL ( lowSliderReleased() ),
	    this, SLOT ( cutLowSlider_sliderReleased() ) );

  connect ( axisWidget1, SIGNAL ( lowSliderPressed() ),
	    this, SLOT ( setDragOn() ) );

  connect ( axisWidget1, SIGNAL ( highSliderPressed() ),
	    this, SLOT ( setDragOn() ) );

  connect ( axisWidget1, SIGNAL ( lowSliderValueChanged ( int ) ),
	    this, SLOT ( setLowRange ( int ) ) );

  connect ( axisWidget2, SIGNAL ( lowSliderValueChanged ( int ) ),
	    this, SLOT ( cutLowSlider_sliderMoved ( int ) ) );

  connect ( axisWidget1, SIGNAL ( highSliderReleased() ),
	    this, SLOT ( highRangeDrag() ) );

  connect ( axisWidget2, SIGNAL ( highSliderReleased() ),
	    this, SLOT ( cutHighSlider_sliderReleased() ) );

  connect ( axisWidget1, SIGNAL ( highSliderValueChanged ( int ) ),
	    this, SLOT ( setHighRange ( int ) ) );

  connect ( axisWidget2, SIGNAL ( highSliderValueChanged ( int ) ),
	    this, SLOT ( cutHighSlider_sliderMoved ( int ) ) );

  connect ( axisWidget1, SIGNAL ( zoomPanCheckBoxClicked () ),
	    this, SLOT ( axisZoomPanCheckBox_clicked () ) );

  connect ( axisWidget2, SIGNAL ( zoomPanCheckBoxClicked () ),
	    this, SLOT ( cutZoomPanCheckBox_clicked () ) );

  axisWidget2 -> setCut ( true );
  // Default position of the sliders is center which corrosponds to
  // a value of 50. So initialization takes place as 50.
  m_lowslider1_last_val  = 50;
  m_highslider1_last_val = 50;

  updatePlotTypes ();

/*
 * An ugly workaround to assign group ids to the radio buttons in a button group.
 * I would guess the designer should set them automatically (Qt bug? My fault?)
 *  - FK
 */
  setButtonGroupIds(axis_button_group);
  setButtonGroupIds(cut_button_group);
  setButtonGroupIds(m_line_group);
  setButtonGroupIds(m_symbol_group);
  setButtonGroupIds(show_cut_radio_group);
}

Inspector::
~Inspector ()
{
  DisplayController * controller = DisplayController::instance ();
  delete controller;

}

void
Inspector::
init()
{

  unsigned int n = 5;
  m_new_labels.reserve ( n );
  m_new_labels.push_back ( new_binding_0 );
  m_new_labels.push_back ( new_binding_1 );
  m_new_labels.push_back ( new_binding_2 );
  m_new_labels.push_back ( new_binding_3 );
  m_new_labels.push_back ( new_binding_4 );

  m_new_combos.reserve ( n );
  m_new_combos.push_back ( new_combo_0 );
  m_new_combos.push_back ( new_combo_1 );
  m_new_combos.push_back ( new_combo_2 );
  m_new_combos.push_back ( new_combo_3 );
  m_new_combos.push_back ( new_combo_4 );

  m_sel_labels.reserve ( n );
  m_sel_labels.push_back ( sel_binding_0 );
  m_sel_labels.push_back ( sel_binding_1 );
  m_sel_labels.push_back ( sel_binding_2 );
  m_sel_labels.push_back ( sel_binding_3 );
  m_sel_labels.push_back ( sel_binding_4 );

  m_sel_combos.reserve ( n );
  m_sel_combos.push_back ( sel_combo_0 );
  m_sel_combos.push_back ( sel_combo_1 );
  m_sel_combos.push_back ( sel_combo_2 );
  m_sel_combos.push_back ( sel_combo_3 );
  m_sel_combos.push_back ( sel_combo_4 );

#if 0   // Variable size inspector    2009-12-28 FK
  QSize cur_size = size();
  setFixedSize ( cur_size );
#endif

  m_min_entries = 0;
  m_rotate_enable = true;
  m_dragging = false;
  m_axis = Axes::X;
  m_layoutWidget = new QWidget ( currentPlot );
  m_layoutWidget -> setObjectName ( "m_Layout" );
  m_layoutWidget->setGeometry( QRect ( 7, 75, 360, 0 ) );
  m_vLayout = new QVBoxLayout( m_layoutWidget );
  m_vLayout -> setObjectName ( "m_Layout" );
  m_vLayout -> setContentsMargins ( 0, 0, 0, 0 );
  m_vLayout -> setSpacing ( 6 );

  newPlotButton->setEnabled( false );

  m_newLayoutWidget = new QWidget ( m_new_plot_box );
  m_newLayoutWidget -> setObjectName ( "m_newLayout" );
  m_newLayoutWidget->setGeometry( QRect ( 7, 75, 360, 0 ) );
  m_newVLayout = new QVBoxLayout( m_newLayoutWidget );
  m_newVLayout -> setObjectName ( "m_newVLayout");
  m_newVLayout -> setContentsMargins ( 0, 0, 0, 0 );
  m_newVLayout -> setSpacing ( 6 );
    

  updateValueCombo ();

  m_interval_le->setDisabled ( true );

}

void
Inspector::
updateValueCombo ()
{
  DisplayController * controller = DisplayController::instance ();
  const vector < string > & names = controller -> getValueTransformTypes ();
  m_value_combo -> clear ();
  unsigned int size = names.size ();
  for ( unsigned int i = 0; i < size; i++ ) {
    m_value_combo -> addItem ( names[i].c_str() );
  }
}

void
Inspector::
enableNewPlotBox ( bool yes )
{
  m_new_plot_box->setEnabled ( yes );
  m_summary->setEnabled ( yes );
}

void Inspector::customEvent ( QEvent * event )
{
  PlotterEvent * pev = dynamic_cast < PlotterEvent * > ( event );
  if ( pev != 0 ) {
    m_plotter = pev -> plotter ();
    update ();
  }

  CanvasSelectionEvent * ev
    = dynamic_cast < CanvasSelectionEvent * > ( event );
  if ( ev != 0  ) {
    m_plotter_list = ev -> getPlotters ();
    if ( m_plotter_list.size () == 1 ) {
      m_plotter = m_plotter_list.front ();
    }
    else {
      m_plotter = 0;
    }
    update ();
  }
}

PlotterBase *
Inspector::
getPlotter ()
{
  return m_plotter;
}

void
Inspector::
setZRadioButton ( bool enabled )
{
  if (!enabled && m_axis == Axes::Z )
    {
      QAbstractButton * b = axis_button_group -> button ( 2 );
      QRadioButton * button = dynamic_cast< QRadioButton * > ( b );
      button -> setChecked ( true );
      m_axis = Axes::X;
      updateAxisTab ();
    }

  QAbstractButton * button = axis_button_group -> button ( 2 );

  button -> setEnabled ( enabled );
}

void
Inspector::
tabChanged ()
{
  update ();
}

void
Inspector::
update ()
{ 
  if ( isHidden() == true ) return;
  m_is_updating = true;
  int index = m_plot_tab -> currentIndex ();

  switch ( index )
    {
    case ( 0 ) :
      updateDataTab();
      break;
    case ( 1 ) :
      updatePlotTab();
      break;
    case ( 2 ) :
      updateAxisTab();
      break;
    case ( 3 ) :
      updateCutsTab();
      break;
    case ( 4 ) :
      updateFunctionsTab();
      break;
    case ( 5 ) :
      updateSummaryTab ();
      break;
    case ( 6 ) :
      updateTransformTab ();
      break;
    default :
      assert ( false );
      break;
    }
  
   if ( m_plotter != 0 ) {
     bool hasZ = m_plotter -> hasAxis ( Axes::Z );
     setZRadioButton ( hasZ );
	
   }

  m_is_updating = false;
  updateCutsActive ();
  
}

void
Inspector::
updateCutsActive ()
{
  PlotterBase * plotter = getPlotter ();
  if ( plotter == 0 ) {
    setAllCutsActive ( true );
  }
  else {
    vector < PlotterBase * > cutlist;
    CutController * controller = CutController::instance ();
    controller -> fillCutList ( plotter, cutlist );

    if ( cutlist.empty () ) {
      setAllCutsActive ( false );
      return;
    }
    else {
      setAllCutsActive ( false );
      vector < PlotterBase * >::iterator first = cutlist.begin();
      while ( first != cutlist.end () ) {
	PlotterBase * pb = *first++;
	CutPlotter * cutter = dynamic_cast < CutPlotter * > ( pb );
	assert ( cutter );
	cutter -> setActive ( true );
      }
    }
  }
}

std::string
Inspector::
getSelectedDataSourceName () const
{
  string s;
  const vector < string > & names
    = DataSourceController::instance () -> getNTupleNames ();
  int index = m_all_ntuples -> count () == 0 ? -1 : m_all_ntuples -> currentIndex ();
  if ( index >= 0 &&
       names.empty () == false ) {
    s = names [ index ];
  }

  return s;
}

void
Inspector::
updateNewPlotControls ()
{
  const vector < string > & nt_vector
    = DataSourceController::instance() -> getNTupleNames ();

  
  if ( nt_vector.empty () ) {
   
    m_all_ntuples -> clear ();
    return;
  }

  unsigned int count = m_all_ntuples -> count ();
  if ( count == nt_vector.size () ) return;

#ifdef ITERATOR_MEMBER_DEFECT
  //std::
#endif

  m_all_ntuples -> clear();
  vector < string > ::const_iterator first = nt_vector.begin();
  while ( first != nt_vector.end() ) {
    const string & name = *first++;
    m_all_ntuples -> addItem ( name.c_str() );
  }

  if ( m_all_ntuples -> count () != 0 ) {
    const string & name = nt_vector.back ();

    setNewPlotNTuple ( name );
    

    availPlotTypesActivated ( name.c_str() );
	
  }
  else {
    availPlotTypesActivated ( QString::null );
	
  }
}

void
Inspector::
setNewPlotNTuple ( const std::string & name )
{
  const vector < string > & nt_vector
    = DataSourceController::instance() -> getNTupleNames ();

  for ( unsigned int i = 0; i < nt_vector.size(); i++ ) {
    if ( nt_vector[i] == name ) {
      unsigned int current = m_all_ntuples -> currentIndex ();
      if ( current != i ) {
	m_all_ntuples -> setCurrentIndex ( i );
      }
      break;
    }
  }

  // Update tip tool.
  m_all_ntuples -> setToolTip ( getSelectedDataSourceName().c_str() );
}

void
Inspector::
dataTupleNameChanged ( const QString & )
{
  m_last_ntuple_edited = m_all_ntuples -> currentIndex ();

}

void
Inspector::
changeNTupleName ( const QString & text )
{
  DataSourceController * controller = DataSourceController::instance ();
  vector < DataSource * > nt_vector;
  controller -> getDataSources ( nt_vector ); // get all
  DataSource * ds = nt_vector [ m_last_ntuple_edited ];
  if ( ds == 0 ) return;

  const string new_name = text.toLatin1().data();

  ds -> setName ( new_name );
}

void
Inspector::
dataNTupleSelChanged ( int item )
{
  DataSourceController * controller = DataSourceController::instance ();
  controller -> setCurrentIndex ( item );

  m_all_ntuples -> setCurrentIndex ( item );
  QString text ( "" );

  availPlotTypesActivated ( text );

  // Update tip tool.
  m_all_ntuples -> setToolTip ( getSelectedDataSourceName().c_str() );
}

void
Inspector::
allNtupleComboActivated ( const QString & text )
{
  // Change the number and type of axes depending on what is selected
  // inside m_availPlotTypes. Then insert the axis labels based on the
  // selection of nTupleNameComboBox.

  changeNTupleName ( text );
  m_all_ntuples -> setCurrentIndex ( m_last_ntuple_edited );
  m_all_ntuples -> setItemText ( m_last_ntuple_edited, text );

  DataSourceController * controller = DataSourceController::instance ();
  int index = m_all_ntuples -> currentIndex ();
  controller -> setCurrentIndex( index );

  availPlotTypesActivated ( text );
}

void
Inspector::
sel_combo_0_activated ( const QString & label )
{
  axisLabelChanged ( 0, label );
}

void
Inspector::
sel_combo_1_activated ( const QString & label )
{
  axisLabelChanged ( 1, label );
}

void
Inspector::
sel_combo_2_activated ( const QString & label )
{
  axisLabelChanged ( 2, label );
}

void
Inspector::
sel_combo_3_activated ( const QString & label )
{
  axisLabelChanged ( 3, label );
}

void
Inspector::
axisLabelChanged ( int index, const QString & label )
{
  if ( m_plotter_list.size () > 1 ) {
    multiplePlotError ();
    return;
  }

  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return ;

  QString axisName = m_sel_labels [index] -> text();
  const std::string strAxisName ( axisName.toLatin1().data() );
  const std::string strLabel( label.toLatin1().data() );

  DisplayController * controller = DisplayController::instance();
  controller -> setAxisBinding ( plotter, strAxisName, strLabel );

  bool valid = controller -> isDataValid ( plotter );
  if ( valid == false ) {
    invalidDataWarning ();
  }
}

void
Inspector::
updatePlotTypes ()
{
  const vector < string > & dataRepNames
    = DisplayController::instance() -> getDisplayTypes ();
  if ( dataRepNames.empty () ) return;
  unsigned int size = m_availPlotTypes -> count ();

  if ( dataRepNames.size() != size ) {
    m_availPlotTypes->clear();

    vector < string > ::const_iterator first = dataRepNames.begin ();
    while ( first != dataRepNames.end() ) {
      const string & name = *first++;
      if ( name.find ( "Static" ) != string::npos ) continue;
      m_availPlotTypes->addItem ( name.c_str() );
	}
     m_availPlotTypes->setCurrentIndex ( 2 ); //Histogram
  }

  newPlotButton->setEnabled( true );
}

void
Inspector::
clear ( std::vector < QLabel * > & labels,
	std::vector < QComboBox * > & combos )
{
  unsigned int size = combos.size ();
  for ( unsigned int i = 0; i < size; i++ ) {
    QComboBox * box = combos [ i ];
    box -> clear ();
    box -> setEnabled ( false );
    QLabel * label = labels [ i ];
    label -> setEnabled ( false );
  }
}

void
Inspector::
availPlotTypesActivated ( const QString & )
{
  int index = m_all_ntuples -> count () == 0 ? -1 : m_all_ntuples -> currentIndex ();
  vector < DataSource * > nt_vector;
  DataSourceController::instance() -> getDataSources ( nt_vector );


  int size = static_cast < int > ( nt_vector.size() );
  if ( size == 0 ) {
    clear ( m_new_labels, m_new_combos );
    return;
  }
 

  if ( ! (index < size ) ) {
    index = 0;
  }
  std::string plotTypeStr( (m_availPlotTypes->currentText()).toLatin1().data() );

  DisplayController * controller = DisplayController::instance ();

  const vector < string > & bindingOptions
    = controller -> bindingOptions ( plotTypeStr );

  if ( bindingOptions.empty () ) return;

  //Layout the stuff.

  m_newLayoutWidget->hide();

  vector < int > indices;
  unsigned int s = m_new_combos.size ();
  for ( unsigned int i = 0; i < s; i++ ) {
    indices.push_back ( m_new_combos[i] -> currentIndex () );
  }
  clear ( m_new_labels, m_new_combos );
  QString qs1;

  if ( index >= 0 ) {
    DataSource * nt = nt_vector[index];
    const vector < string > & cols = nt->getLabels();

    for ( unsigned int i = 0; i < m_new_combos.size (); i++ ) {
      if ( i < bindingOptions.size () ) {
	const string & axisName = bindingOptions[i];
	
	qs1 = ( axisName.c_str() );
	m_new_labels [i] -> setEnabled ( true );
	m_new_labels [i] -> setText ( qs1 );
	m_new_combos [i] -> setEnabled ( true );
	for (std::vector<string>::size_type j = 0; j < cols.size(); j++){
	  m_new_combos [i] -> addItem ( cols [j].c_str() );
	}
	
	if ( axisName.find ( "optional" ) != string::npos ) {
	  m_new_combos [i] -> addItem ( "nil" );
	  m_new_combos [i] -> setItemText ( m_new_combos [i] -> currentIndex(), "nil" );
	  indices[i] = -1;
	}
      }
    }
  }
  

  for ( unsigned int i = 0; i < m_new_combos.size(); i++ ) {
	  

    if ( indices[i] >= 0 &&
	 indices[i] < m_new_combos[i] -> count () ) {
      m_new_combos[i] ->setCurrentIndex ( indices[i] );
    }
  }
  
}

void
Inspector::
updateDataTab()
{

  
  updateNewPlotControls ();
  

  dataClearSelectedControls ();
  

  PlotterBase * plotter = getPlotter ();
  updateSelectedPlotType ( plotter );

  

  if ( plotter != 0 ) {
	  
    currentPlot->setEnabled ( true );
    bool yes = plotter -> isTargetable ();
    if ( yes == false ) return;
  }
  else {
	  
    if ( m_plotter_list.empty () == true ) {
		
      currentPlot -> setDisabled ( true );
    }
    else {

	  currentPlot -> setDisabled ( false );
    }
	
    return;
  }
  
  

  updateSelectedPlotData ( plotter );
  

  

}

void
Inspector::
updateSelectedPlotDataSource ( const std::string & name )
{
  const vector < string > & nt_vector
    = DataSourceController::instance () -> getNTupleNames ();

  unsigned int size = nt_vector.size ();
  unsigned int count = m_sel_ntuple_name -> count ();
  bool refresh = count != size;
  if ( refresh ) m_sel_ntuple_name -> clear ();
  int jndex = -1;
  for ( std::size_t i = 0; i < size; i++ ) {
    const string & ntname = nt_vector[i];
    if ( ntname == name ) jndex = i;
    if ( refresh ) m_sel_ntuple_name -> addItem ( ntname.c_str () );
  }

  if ( jndex < 0 ) { // not bound to ntuple
    m_sel_ntuple_name -> setEnabled ( false );
  }
  else {
    m_sel_ntuple_name -> setEnabled ( true );
    m_sel_ntuple_name -> setCurrentIndex ( jndex );
  }
  // Update tip tool.
  m_sel_ntuple_name -> setToolTip ( m_sel_ntuple_name -> currentText() );
}

void
Inspector::
updateSelectedPlotType ( const PlotterBase * plotter )
{
  bool yes = plotter != 0;
  if ( yes ) {
    yes = plotter -> isTargetable ();
    if ( yes ) {
      DataRep * datarep = plotter -> getTarget ();
      yes = datarep != 0;
      if ( yes ) {
	int index = plotter -> indexOf ( datarep );
	DisplayController * controller = DisplayController::instance ();
	const string & dataRepName
	  = controller -> getType ( plotter, index );
	QString qst2 ( dataRepName.c_str() );
	m_dataRepNameText->setText ( qst2 );
      }
    }
  }

  m_dataRepNameText -> setEnabled ( yes );
}

void
Inspector::
dataClearSelectedControls ()
{
  QLayoutItem *item;
  while ( (item = m_vLayout -> itemAt(0)) != 0 ) {
    QHBoxLayout * hbox = dynamic_cast <QHBoxLayout *> (item);
    
    QLayoutItem *hit;
    while ( (hit = hbox -> itemAt(0)) != 0 ) {
      QWidget * hwidget = hit->widget();
      hbox -> takeAt(0);
      if ( hwidget ) delete hwidget;
    }

    m_vLayout -> takeAt (0);
  }
}

void
Inspector::
updateSelectedPlotData ( const PlotterBase * plotter )
{
  DisplayController * controller = DisplayController::instance ();
  DataRep * datarep = plotter -> getTarget ();
  int index = plotter -> indexOf ( datarep );
  assert ( datarep != 0 );

  bool ntuple_bindings = datarep -> hasNTupleBindings ( );
  string name;

  if ( ntuple_bindings ) {
     name  = controller -> getDataSourceName ( plotter, index );
    setNewPlotNTuple ( name );
  }
  else {
    name = "<none>";
  }
  updateSelectedPlotDataSource ( name );

  //Layout the stuff.

  m_layoutWidget->hide();

  const vector < string > & bindings
    = controller -> axisBindings ( plotter, index );
  const vector < string > & bindingOptions
    = controller -> bindingOptions ( plotter, index );
  unsigned int listSize;

  if ( bindings.size() < bindingOptions.size() )
    {
      listSize = bindings.size();
    }
  else
    {
      listSize = bindingOptions.size();
    }

  bool yes = plotter -> isTargetable ();
  if ( ntuple_bindings == false ||
       yes == false ) return;

  // Now add the new hlayouts.

  QString qs1, qs2;

  const vector < string > & cols
    = controller -> getDataSourceLabels ( plotter, index );

  if ( cols.empty () ) return;

  clear ( m_sel_labels, m_sel_combos );

  for ( unsigned int i = 0; i < m_sel_combos.size (); i++ )
    {
      if ( i == listSize ) break;
      const string & axisLabel = bindings[i];
      const string & axisName = bindingOptions[i];

      qs1 = ( axisName.c_str() );
      m_sel_labels [i] -> setEnabled ( true );
      m_sel_labels [i] -> setText ( qs1 );

      qs2 = ( axisLabel.c_str() );

      // Insert all column labels from the vector cols, and make qs2
      // the current text.

      m_sel_combos [i] -> setEnabled ( true );
      for (std::vector<string>::size_type j = 0; j < cols.size(); j++ )
	{
	  m_sel_combos [i] -> addItem ( cols [j].c_str() );
	}
      if ( axisName.find ( "optional" ) != string::npos )
	{
	  m_sel_combos [i] -> addItem ( "nil" );
	}
      m_sel_combos [i] -> setItemText ( m_sel_combos [i] -> currentIndex(), qs2 );
    }
}

void
Inspector::
invalidDataWarning ()
{
  const QString message = tr(
			     "One or more columns of the bound data source\n"
			     "contains invalid data."
			     );
 QMessageBox::warning ( this, // parent
			tr("Invalid data"), // caption
			message,
			QMessageBox::Ok,
			Qt::NoButton,
			Qt::NoButton );
}

void
Inspector::
noNTupleSelectedError ()
{
  const QString message = tr(
			     "No n-tuple selected error\n"
			     "Need to load n-tuple to create a plot"
			     );
  QMessageBox::critical ( this, // parent
			  tr("No n-tuple selected error"), // cpation
			  message,
			  QMessageBox::Ok,
			  Qt::NoButton,
			  Qt::NoButton );
}

void
Inspector::
incompatibleDataRepError ( const std::string & type )
{
  QString message = tr(
		       "Plot of type %1 can not be added\n"
		       "to selected plot\n\n"
		       "It might be incompatible.   For example, \n"
		       "requiring a Z axis display while selected\n"
		       "does not have one."
		       ).arg ( type.c_str() );
  QMessageBox::critical ( this, // parent
			  tr("Add to plot error"), // caption
			  message,
			  QMessageBox::Ok,
			  Qt::NoButton,
			  Qt::NoButton );
}
void
Inspector::
incompatibleFitterError ( const std::string & type )
{
  QString message = tr( "Fitter of type %1 can not be used\n"
			"with selected plot\n\n"
			"It might be incompatible.   For example, \n"
			"Maximum Likelihood fitting requires binned\n"
			"data representation."
			).arg ( type.c_str() );
  QMessageBox::critical ( this, // parent
			  tr("Set fitter error"), // caption
			  message,
			  QMessageBox::Ok,
			  Qt::NoButton,
			  Qt::NoButton );
}

void
Inspector::
badFunctionError ( const std::string & name, const char * what )
{
  QString message = tr( "Function `%1' could not be used because ...\n"
		        "%2\n"
		        "Maybe the copy constructor or clone function is bad."
			) . arg( name.c_str() ) . arg(what);

  QMessageBox::critical ( this, // parent
			  tr("Function error"), // caption
			  message,
			  QMessageBox::Ok,
			  Qt::NoButton,
			  Qt::NoButton );
}

void
Inspector::
incompatibleFunctionError ( const std::string & type )
{
  QString message = tr( "Funtion of type %1 can not be used\n"
			"with selected fitter\n\n"
			"It might be incompatible.   For example, \n"
			"the fitter requires derivatives\n"
			"that the function can not supply."
			) . arg( type.c_str() );
  QMessageBox::critical ( this, // parent
			  tr("Set fitter error"), // caption
			  message,
			  QMessageBox::Ok,
			  Qt::NoButton,
			  Qt::NoButton );
}

void
Inspector::
invalidRangeError ( const std::string & bad )
{
  QString message = tr( "Attempt to apply invalid range:\n\n" 
			"%1\n\n"
			"Low end of range must be less than high end."
			) . arg( bad.c_str() );

  QMessageBox::critical ( this, // parent
			  tr("Range error"), // caption
			  message,
			  QMessageBox::Ok,
			  Qt::NoButton,
			  Qt::NoButton );
}
			
void
Inspector::
multipleDataRepError ( const QString & type )
{
  QString message = tr( "Multiple data representations are active.\n"
			"Can not apply a %1.\n\n"
#ifdef Q_OS_MACX
			"Use Command-click to select only one data representation."
#else
			"Use Control-click to select only one data representation."
#endif
			) . arg ( type );
  QMessageBox::information ( this, // parent
			     tr("Multiple data representation error"), // caption
			     message,
			     QMessageBox::Ok,
			     Qt::NoButton,
			     Qt::NoButton );
}

bool
Inspector::
multipleDataRepInfo ( const std::string & type )
{
  bool ok = false;
  QString message = tr( "Multiple data representations are active.\n"
			"Apply %1 to each?\n\n"
			"One can use "
#ifdef Q_OS_MACX
			"Control-click"
#else
			"Command-click"
#endif
			" to apply a %2 to a selected data representation."
			) . arg(type.c_str()) . arg(type.c_str());

  int result = QMessageBox::
    information ( this, // parent
		  tr("Multiple data representation error"), // caption
		  message,
		  QMessageBox::Yes,
		  QMessageBox::No,
		  QMessageBox::NoButton );

  ok = result == QMessageBox::Ok;

  return ok;
}

void
Inspector::
cutOnCutError ()
{
  const QString message = tr( "Attempt to add cut to itself\n"
			      "The request was ignorned" );
  QMessageBox::information ( this, // parent
			     tr("Applying cut error"), // caption
			     message,
			     QMessageBox::Ok,
			     Qt::NoButton,
			     Qt::NoButton );
}

void
Inspector::
multiplePlotError ( )
{
  const QString  message = tr(
			      "Multiple plots are selected.\n"
			      "Can not apply change until only one is selected\n\n"
			      "Use shift-click to deselect a selected plot"
			      );
  QMessageBox::information ( this, // parent
			     tr("Multiple plot error"), // caption
			     message, // .c_str(),
			     QMessageBox::Ok,
			     Qt::NoButton,
			     Qt::NoButton );
}

void Inspector::functionAddError ()
{
  const QString message =
    tr("Functions are not supported on the selected data reaxisWitation.");

  QMessageBox::critical ( this, tr("Function Add Error"),
			  message,
			  QMessageBox::Ok,
			  Qt::NoButton,
			  Qt::NoButton );
}

/** @todo Should something be done with the exception, like deleting
    it?
*/
void
Inspector::
newPlotError ( const std::exception & e )
{
  QString message = tr( "New plot could not be created because:\n"
			"%1"
			) . arg(e.what());
  QMessageBox::critical ( this, // parent
			  tr("New plot error"), // caption
			  message,
			  QMessageBox::Ok,
			  Qt::NoButton,
			  Qt::NoButton );
}

std::string
Inspector::
getArrayTupleLabel( const DataSource * rtuple, const std::string & column )
{
#ifdef HAVE_ROOT
  RootController * controller = RootController::instance ();
  vector < int > dimSize;
  controller -> fillDimSize ( dimSize, rtuple, column );

  // Set the caption
  QString caption = tr("MultiDimensional data in rows of the column ");
  caption.append( QString( column.c_str() ) );

  // Set the label
  QString label = tr( " Rows of this column are of size " );

  unsigned int i;
  for( i = 0; i < dimSize.size() - 1; i++ )
    label += tr("%1 x ").arg(dimSize[i]);
  label += tr("%1\n").arg(dimSize[i]);

  label += tr (
	        "Enter C-style index of a single element of this\n"
		"multidimentional data which you wish to analyse.\n"
		"Index should be a comma separated list of %1 integers.\n"
	       ) . arg(dimSize.size());

  label += tr("\n For example ");
  for( i = 0; i < dimSize.size() - 1; i++ )
    label += tr("0, ");
  label += tr("0 \n");

  bool ok;
  QString text = QInputDialog::getText( this, caption, label,
					QLineEdit::Normal,
					QString::null, &ok );

  // Also create the array tuple label
  std::ostringstream labelstream;
  labelstream << column;

  if ( ok && !text.isEmpty() )
    {
      vector< unsigned int > index;
      string s( text.toAscii().data() );

      // User entered something and pressed OK
      // Creating the list of dropped delimiters.
      boost::char_separator< char > sep( "," );

      // A tokenizer with above dropped delimiters.
      typedef boost::tokenizer< boost::char_separator< char > >  tokenizer;
      tokenizer tok( s, sep );

      // Start extracting the dimension sizes.
      for( tokenizer::iterator tok_iter = tok.begin();
	   tok_iter != tok.end();
	   ++tok_iter )
	{
	  unsigned int idx = boost::lexical_cast< unsigned int >( *tok_iter );
	  index.push_back( idx );
	  labelstream << "[" << idx << "]";
	}
    }

  return labelstream.str();
#else
  return string(); // will not be used.
#endif
}

void
Inspector::
newPlotButton_clicked()
{
  vector < DataSource * > nt_vector;
  DataSourceController::instance() -> getDataSources ( nt_vector );

  if ( nt_vector.empty() )
    {
      noNTupleSelectedError ();
      return;
    }

  // See all comboboxes and create a new plotter.
  int current = m_all_ntuples->currentIndex ();
  DataSource * ds = nt_vector[current];

  // Find the datarep.
  std::string plotTypeStr( (m_availPlotTypes -> currentText()).toLatin1().data() );

  // Find the axis bindings.
  vector < string > bindings;
  for ( unsigned int i = 0; i < m_new_combos.size(); i++ )
    {
      if ( m_new_combos [i] -> count () == 0 ) break;

      QString qstring = m_new_combos [ i ] -> currentText();
      string column ( qstring.toLatin1().data() );
      string label = column;

#ifdef HAVE_ROOT
      if( column != "nil" && ds -> isMultiDimensional( column ) ) {
	bool yes = ds -> isUseable ( column );
	if ( yes == false ) {
	  const QString
	    message = tr( 
			  "This column is not useable because it contains\n"
			  "a multidimension array that varies is size or is\n"
			  "an unsupported data type."
			  );
	  QMessageBox::critical ( this, // parent
				  tr("DataSource error"),
				  message,
				  QMessageBox::Ok,
				  QMessageBox::NoButton,
				  QMessageBox::NoButton );
	  return;
	}

	label = getArrayTupleLabel ( ds, column );
	RootController * rcontroller = RootController::instance();
	rcontroller -> smartExpandRootNTuple ( ds, column );
      }
#endif
      bindings.push_back ( label );
    }

  // Create the plotter.
  try {
    DisplayController * controller =  DisplayController::instance();
    PlotterBase * newDisplay =
      controller -> createDisplay ( plotTypeStr, * ds, bindings );
    CanvasWindow * canvas = WindowController::instance() -> currentCanvas();
    canvas->addPlotDisplay ( newDisplay, true );

    bool valid = controller -> isDataValid ( newDisplay );
    if ( valid == false ) {
      invalidDataWarning ();
    }
  }
//   catch ( const DataSourceException & e ) {
  catch ( const std::exception & e ) {
    newPlotError ( e );
  }
}


void
Inspector::
addDataRepButton_clicked()
{
  // Find the display.

  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;

  // Find the datarep.
  std::string plotTypeStr( (m_availPlotTypes->currentText()).toLatin1().data() );

  vector < DataSource * > nt_vector;
  DataSourceController::instance() -> getDataSources ( nt_vector );

  int current = m_all_ntuples->currentIndex ();
  DataSource * ds = nt_vector[current];

  // Find the axis bindings.

  vector < string > bindings;
  for ( unsigned int i = 0; i < m_new_combos.size(); i++ )
    {
      if ( m_new_combos [i] -> count () == 0 ) break;

      QString qstring = m_new_combos [ i ] -> currentText();
      string column ( qstring.toLatin1().data() );
      string label = column;
#ifdef HAVE_ROOT
      if ( column != "nil" && ds -> isMultiDimensional ( column ) ) {
	label = getArrayTupleLabel ( ds, column );
	RootController * rcontroller = RootController::instance ();
	rcontroller -> smartExpandRootNTuple ( ds, column );
      }
#endif
      bindings.push_back ( label );
    }

  // Add the data rep.

  DisplayController * controller = DisplayController::instance();
  DataRep * rep = controller -> addDataRep ( plotter, plotTypeStr, ds,
					     bindings );

  // If incompatible, do nothing.
  if ( rep == 0 ) 
    {
      incompatibleDataRepError ( plotTypeStr );
      return;
    }

  rep->set(Color::getColor());

  plotter -> autoScale ();
  bool valid = controller -> isDataValid ( rep );
  if ( valid == false ) {
    invalidDataWarning ();
  }

  update ();
}

void
Inspector::
removeDataRepButton_clicked()
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;

  int num_active = plotter->activePlotIndex();
  int num_rep = plotter -> getNumDataReps();

  // If more than one active datarep or only one datarep 
  // in the plot, show warning message and do nothing.
  if (( num_active < 0) || ( num_rep <= 1 ))
    {
      const QString message = tr(
				  "You must have more than one DataReps \n"
				  "in this view AND only one DataRep \n"
				  "selected to be removed."
				 );

      QMessageBox::warning ( this, // parent
			     tr("Unable to remove DataRep"), // caption
			     message,
			     QMessageBox::Ok,
			     Qt::NoButton,
			     Qt::NoButton );
      return;
    }

  DataRep * rep = plotter -> getDataRep( num_active );
  //Reset index before remove the datarep.
  plotter->setActivePlot(-1,false);
  plotter -> removeDataRep ( rep );
  plotter -> autoScale ();
  delete rep;

  update();
}


void
Inspector::
dataCreateNTuple ()
{
  const PlotterBase * plotter = getPlotter ();
  if ( plotter == 0 ) return;
  FunctionController * fc = FunctionController::instance ();
  NTuple * ntuple = fc -> createNTuple ( plotter, 0 );
  if (ntuple == NULL) {
    const QString message = tr(
			        "Creating ntuples from plotters containing multiple plots "
				"is not supported yet."
			       );
    QMessageBox::warning ( this, // parent
			   tr("Invalid request"), // caption
			   message,
			   QMessageBox::Ok,
			   Qt::NoButton,
			   Qt::NoButton );
    return;
  }
  DataSourceController::instance () -> registerNTuple ( ntuple );

  update ();
}


void
Inspector::
fillPlotterList ( std::vector < PlotterBase * > & plotterlist )
{
  plotterlist.clear();
  CanvasWindow * canvaz = WindowController::instance () ->currentCanvas();

  if ( canvaz != 0 ) {
    canvaz -> fillPlotterList ( plotterlist );
  }
}

void
Inspector::
ntupleChanged ( int index )
{
  // Update tip tool.
  m_sel_ntuple_name -> setToolTip ( m_sel_ntuple_name -> currentText() );

  unsigned int size = m_plotter_list.size ();
  if ( size == 0 ) return;

  CutController * controller = CutController::instance ();
  vector < PlotterBase * > web;
  controller -> fillCutWeb ( m_plotter_list, web );
  size = web.size ();

  for ( unsigned int i = 0; i < size; i++ ) {
    PlotterBase * plotter = web [ i ];
    bool yes = plotter != 0;
    if ( yes ) yes = plotter -> isTargetable ();
    if ( yes == false ) continue;

    DataRep * rep = plotter -> getTarget ();
    yes = rep -> hasNTupleBindings ();
    if ( yes == false ) continue;

    DataSourceController * ds_controller = DataSourceController::instance ();
    const vector < string > & names = ds_controller -> getNTupleNames ();
    const string & ds_name = names [ index ];
    const DataSource * source = ds_controller -> getDataSource ( ds_name );
    DataRepController * dr_controller = DataRepController::instance ();

    try {
      dr_controller -> changeDataSource ( rep, source );
    }
    catch ( const runtime_error & e ) {
      QString message = tr( "Could not change binding because\n" );
      message += e.what ();
      QMessageBox::critical ( this, // parent
			      tr("Data source error"),
			      message,
			      QMessageBox::Ok,
			      Qt::NoButton,
			      Qt::NoButton );
    }
  }
}

void
Inspector::
updateColorMapCtrls (const PlotterBase * plotter )
{
  DisplayController * controller = DisplayController::instance ();

  const vector < double > & sv =
    controller -> getValueCtrlPts ( plotter );
  unsigned int size = sv.size ();
  if ( size > 0 ) {
    brk_pt -> setEnabled ( true );
    brk_label -> setEnabled ( true );
    brk_pt -> setValue ( static_cast <int> ( sv[0] * brk_pt -> maximum() ));
  }
  if ( size > 1 ) {
    flat_width -> setEnabled ( true );
    flatlabel -> setEnabled ( true );
    flat_width->setValue(static_cast <int>( sv[1] * flat_width->maximum()));
  }
  if ( size > 2 ) {
    color_scale -> setEnabled ( true );
    colorlabel -> setEnabled ( true);
    color_scale->setValue(static_cast<int>( sv[2] * color_scale->maximum()));
  }
  if ( size < 3 ) {
    color_scale -> setEnabled ( false );
    colorlabel -> setEnabled ( false);
  }
  if ( size < 2 ) {
    flat_width -> setEnabled ( false );
    flatlabel -> setEnabled ( false );
  }
  if ( size < 1 ) {
    brk_pt -> setEnabled ( false );
    brk_label -> setEnabled ( false );
  }
}

/** @todo Should move implementation of updating the m_point_stack to
    a separate method.
*/
void
Inspector::
updatePlotTab()
{ 
  
  loadAllUserModels ();

  bool yes = m_plotter_list.empty();

  m_plot_title->setDisabled(yes);

  PlotterBase * plotter = getPlotter ();

  yes = (plotter != NULL);
  if (yes) 
    yes = (plotter->getNumDataReps() <= 1 || plotter->activePlotIndex() >= 0);

  m_plot_symbols->setEnabled(yes);
  m_interval_le->setEnabled(yes);
  m_interval_cb->setEnabled(yes);
  m_errorBars->setEnabled(yes);
  m_grid->setEnabled(yes);
  m_boxedge->setEnabled(yes);
  m_pointRepComboBox->setEnabled(yes);
  if (!yes) {
    QPalette palette;
    palette.setColor(m_selectedColor->backgroundRole(), "black");
    m_selectedColor->setPalette(palette);

    return;
  }

  // Point Reps stuff.
  m_pointRepComboBox->clear();

  DisplayController * controller = DisplayController::instance ();
  DataRep * datarep = controller -> activeDataRep ( plotter );
  assert ( datarep != NULL );

  m_errorBars->setEnabled(datarep->hasErrorDisplay());

  ProjectorBase *proj = datarep->getProjector();

  if (proj != NULL) {
    const vector<string> &pointreps = proj->getPointReps();

    if (!pointreps.empty ()) {
      for (std::vector<string>::size_type i = 0; i < pointreps.size(); i++)
	m_pointRepComboBox->addItem(pointreps[i].c_str());
    }
	
    m_pointRepComboBox->setCurrentIndex(m_pointRepComboBox->findText(
	datarep->getRepresentation()->name().c_str()));
  }

  if (!plotter->hasAxis( Axes::Z)) { // no Z axis
    m_value_combo->setDisabled(true);
    m_slider_control->setDisabled(true);
  } else { // has z axis (colormap)
    int jndex = controller->getValueTransformIndex(plotter);

    if (jndex < 0)  // no value available, jndex=-1
	m_value_combo->setDisabled(true);
    else {
	m_value_combo->setEnabled(true);
	m_value_combo->setCurrentIndex(jndex);
     
	bool yes = controller->hasControlPoints(plotter);
	    // true if the value to color transform has control points
	m_slider_control->setEnabled(yes);
	yes = controller->isUserDefinedValueTransform(plotter);
	edit_model->setEnabled(yes);
	updateColorMapCtrls(plotter);
    }
  }

  const BinToColor *repp = plotter->getValueRep ();

  yes = true;
  if (repp != NULL)
	yes = repp->acceptChangeColor();

  m_plot_color->setEnabled(yes);
  m_selectedColor->setEnabled(yes);

  // data source
  const DataSource * nt = controller -> getDataSource ( plotter, 0 );

  if ( nt && nt -> empty () ) {
    m_plot_symbols->setDisabled ( true );
    m_plot_title->setDisabled ( true );
    m_plot_color->setDisabled ( true );
    m_selectedColor -> setDisabled ( true );
    return;
  }

  // title
  const std::string & st = plotter->getTitle();
  QString qst ( st.c_str() );
  m_titleText->setText ( qst );

  // error bar and show grid checkbox
  m_errorBars->setChecked ( plotter->errorDisplay ( Axes::Y ) );
  m_grid->setChecked ( plotter->getShowGrid () );
  m_boxedge->setChecked (plotter->getBoxEdge ());

  // whether use symbols(triangle,plus,circle...) to represent data
  yes = controller -> hasSymbolRep ( plotter );
  buttonGroupSetEnabled ( m_symbol_group, yes );

  if ( yes  ) {
    QWidget * widget = m_point_stack -> widget ( 0 );
    if ( widget )
      widget -> raise ();
    unsigned int index = controller -> getRepStyle ( plotter );
    QAbstractButton * button = m_symbol_group -> button ( index );
    if ( button )
      button -> setChecked ( true );
  }

  // whether use line(solid, dash...)to represent data
  yes = controller -> hasLineRep ( plotter );
  buttonGroupSetEnabled ( m_line_group, yes );
  if ( yes ) {
    QWidget * widget = m_point_stack -> widget ( 1 );
    if ( widget )
      widget -> raise ();
    unsigned int style = controller -> getRepStyle ( plotter );
    QAbstractButton * button = m_line_group -> button ( style );
    QRadioButton * rb = dynamic_cast < QRadioButton * > ( button );
    rb -> setChecked ( true );
  }
  // set the point size
  double ptsize =  controller -> pointSize ( plotter );
  m_symbolPointSize -> setText ( QString ("%1").arg (ptsize) );

  // set color the user choose by RGB
  const Color & color = plotter->repColor ();
  QColor qcolor ( color.getRed(), color.getGreen(), color.getBlue () ); // constructor
  QPalette palette;
  palette.setColor(m_selectedColor->backgroundRole(), qcolor);
  m_selectedColor->setPalette ( palette );

  // display interval
  if ( nt == 0 ) {
    m_interval_cb -> setEnabled ( false );
    m_interval_le -> setEnabled ( false );
    return;
  }
  const NTuple * ntuple = dynamic_cast < const NTuple * > ( nt );
  yes = ntuple != 0 && ntuple -> isIntervalEnabled ();
  m_interval_cb->setChecked ( yes );
  m_interval_le->setEnabled ( yes );

  if ( yes ) {
    unsigned int count = ntuple->getIntervalCount ();
    m_interval_le->setText ( QString ("%1").arg ( count ) );
  }
}

void
Inspector::
valueChanged ( int index )
{
  PlotterBase * plotter = getPlotter ();

  if ( plotter != NULL ) {
    DisplayController * controller = DisplayController::instance ();
    controller -> setValueTransform ( plotter, index );

    const BinToColor * rep = plotter -> getValueRep();
    bool yes = true;
    if ( rep != NULL )
      yes = rep->acceptChangeColor ();
    m_plot_color->setEnabled(yes);
    m_selectedColor->setEnabled(yes);

    yes = controller -> hasControlPoints ( plotter );
    m_slider_control -> setEnabled ( yes );
    bool y = rep -> isUserDefined ();
    edit_model -> setEnabled ( y );
    if ( yes ) {
       m_is_updating = true;
	 updateColorMapCtrls ( plotter );
	   m_is_updating = false;
    }
  }
}

void
Inspector::
setAppKey ()
{
  CanvasWindow * canvas = WindowController::instance () ->currentCanvas();
  if ( canvas != 0 ) {
    m_app_key = canvas -> getAppKey ();
    m_model_name  = m_app_key + "/Model Name/";
    m_break_point = m_app_key + "/break point/";
    m_flat_width  = m_app_key + "/flat width/";
    m_color_scale = m_app_key + "/color scale/";
  }
}

void
Inspector::
loadAllUserModels ()
{
  if ( m_user_models_loaded == false ) {    
    setAppKey ();

    QSettings settings;
    
    QString model_name_key ( m_model_name );
    settings.beginGroup ( model_name_key );
    QStringList model_list 
      = settings.childKeys();

    QStringList::size_type size = model_list.size ();
    for ( QStringList::size_type i = 0; i < size ; i++ )
      {
	QString number = model_list [ i ];
	QString name 
	  = settings.value ( model_name_key + number ) . toString();
	
	double brk_pt 
	  = settings.value ( m_break_point + number ) . toDouble();
	double flat_width 
	  = settings.value ( m_flat_width + number ) . toDouble();
	double color_scale 
	  = settings.value ( m_color_scale + number ) . toDouble();

	string mname ( name.toLatin1().data() );
	vector < double > cpts;
	cpts.push_back ( brk_pt );
	cpts.push_back ( flat_width );
	cpts.push_back ( color_scale );

	DisplayController * controller = DisplayController::instance ();
	controller -> addValueTransform ( mname, cpts );
      }

    updateValueCombo ();
    m_user_models_loaded = true;
  }
}

void
Inspector::
sliderChanged ( int )
{
  if ( m_is_updating == false ) {
    vector < double > sv;

    double m = brk_pt -> maximum ();
    assert(m != 0);
    sv.push_back ( (brk_pt -> value()) / m + 0.001);

    double w = flat_width -> maximum();
    assert(w != 0);
    sv.push_back ( (flat_width -> value()) / w + 0.001);

    double c = color_scale -> maximum ();
    assert( c!= 0 );
    sv.push_back ( ( ( (color_scale -> value()) / c ) ) * 1.5 );

    PlotterBase * plotter = getPlotter ();
    if ( plotter != 0 ) {
      DisplayController * controller = DisplayController::instance ();
      controller -> setValueCtrlPts (plotter,sv );
    }
  }
}

void
Inspector::
resetSlider ( )
{
  if ( m_slider_control -> isEnabled () )
    {
      brk_pt -> setValue ( 50 );
      flat_width -> setValue ( 50 );
      color_scale -> setValue ( 0 );
    }
}


void
Inspector::
convertCtrlPts ( std::vector < double > & sv )
{
  sv.clear();

  int ibp = brk_pt -> value ();
  int ifw = flat_width -> value ();
  int ics = color_scale -> value ();

  double bp = static_cast < double > ( ibp) /
    static_cast <double>( brk_pt -> maximum () );

  double fw = static_cast < double > ( ifw ) /
    static_cast <double>( flat_width -> maximum () );

  double cs = static_cast < double > ( ics ) /
    static_cast <double>(color_scale -> maximum () );

  sv.push_back ( bp );
  sv.push_back ( fw );
  sv.push_back ( cs );
}

void
Inspector::
newColorModel ( )
{
  setAppKey ();

  bool ok = false;
  QString text =  QInputDialog::getText ( this,
					  tr("Save color model"), // caption
					  tr("Enter name"), // label
					  QLineEdit::Normal,
					  QString::null, // default text
					  & ok);

  if ( ok )
    {
      const string name ( text.toLatin1().data() );
      vector < double > sv;
      convertCtrlPts ( sv );
      DisplayController * controller = DisplayController::instance ();
      controller -> addValueTransform ( name, sv );

      updateValueCombo ();

      PlotterBase * plotter = getPlotter ();
      controller -> setValueTransform ( plotter, name );
      int index = controller -> getValueTransformIndex ( plotter );
      m_value_combo -> setCurrentIndex ( index );

      QSettings settings;
      QString model_name_key ( m_model_name );

      settings.beginGroup(model_name_key);
      QStringList model_list 
	= settings.childKeys();
      
      int iat = 0;
      while ( true ) {
	int index = model_list.indexOf ( QString::number ( iat )) ;
	if ( index == -1 ) break;
	iat++;
      }

      QString at ( QString::number ( iat ) );
      settings.setValue ( m_model_name + at, text );
      settings.setValue ( m_break_point + at, sv[0] );
      settings.setValue ( m_flat_width + at, sv[1] );
      settings.setValue ( m_color_scale + at, sv[2] );
    }
}

void
Inspector::
editColorModel ( )
{
  // If it's the first time to save the variable rainbow color scale,
  // we will need special handling. 
  bool isFirstVR = true;
  
  QString item = m_value_combo -> currentText ();
  vector < double > sv;
  convertCtrlPts ( sv );

  PlotterBase * plotter = getPlotter ();
  DisplayController * controller = DisplayController::instance ();
  // Use saveValueCtrlPts instead of setValueCtrlPts to make the 
  // changes effective in this session.
  controller -> saveValueCtrlPts ( plotter, sv );
      
  QSettings settings;

  QString model_name_key ( m_model_name );
  settings.beginGroup(model_name_key);
  QStringList model_list 
    = settings.childKeys();

  for ( int i = 0; i < model_list.size(); i++ ) {
    QString at ( QString::number ( i ) );
    QString name_key ( model_name_key + at );
    QString name = settings.value ( name_key ) . toString();
    if ( name == item ) {
      settings.setValue ( m_break_point + at, sv[0] );
      settings.setValue ( m_flat_width + at, sv[1] );
      settings.setValue ( m_color_scale + at, sv[2] );
      isFirstVR = false;
      break;
    }
  }

  // For the first time, save the variable rainbow color scale.
  if ( isFirstVR ) {
    // Find the end of the settings.
    int iat = 0;
    while ( true ) {
      int index = model_list.indexOf ( QString::number ( iat )) ;
      if ( index == -1 ) break;
      iat++;
    }
    
    QString at ( QString::number ( iat ) );
    settings.setValue ( m_model_name + at, item );
    settings.setValue ( m_break_point + at, sv[0] );
    settings.setValue ( m_flat_width + at, sv[1] );
    settings.setValue ( m_color_scale + at, sv[2] );
  }
}

void
Inspector::
deleteColorModel ( )
{
  QString item = m_value_combo -> currentText ();
  const string name ( item.toLatin1().data() );

  DisplayController * controller = DisplayController::instance ();
  bool yes =  controller -> removeValueTransform ( name );  

  if ( yes ) {
    PlotterBase * plotter = getPlotter ();
    controller -> setValueTransform ( plotter, "Rainbow" );

    QSettings settings;
    QString model_name_key ( m_model_name );
    settings.beginGroup ( model_name_key );
    QStringList model_list 
      = settings.childKeys();
    QStringList::size_type size = model_list.size ();

    for (QStringList::size_type i = 0; i < size; i++ ) {
      QString at ( model_list [ i ] );
      QString name_key ( model_name_key + at );
      QString name = settings.value ( name_key ) . toString();
      if ( name == item ) {
	settings.remove ( name_key );
	settings.remove ( m_break_point + at );
	settings.remove ( m_flat_width + at );
	settings.remove ( m_color_scale + at );
	break;
      }
    }
    updateValueCombo ();
  }
}

void
Inspector::
errorBars_toggled( bool )
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;

  bool checked = m_errorBars->isChecked();
  DisplayController * controller = DisplayController::instance ();

  controller -> setErrorDisplayed ( plotter, Axes::Y, checked );
}
void
Inspector::warningTex ()
{
  const QString message = tr("HippoDraw was not built with TeX support on "
			     "this platfrom.");
  QMessageBox::information ( this, /* parent */
			     tr("Input error"),
			     message,
			     QMessageBox::Ok,
			     Qt::NoButton,
			     Qt::NoButton );
}

void
Inspector::
titleText_returnPressed()
{
  std::string s( (m_titleText->text()).toLatin1().data() );
  unsigned int size = m_plotter_list.size();

  for ( unsigned int i = 0; i < size; i++ ) {
    PlotterBase * plotter = m_plotter_list [ i ];

    /* The size of drawrect, marginrect, need to be updated
       according to title text format. LaTeX format starts with
       "tex:" (case insensitive).
    */
    bool needMargin = String::ci_find(s, "tex:")==0;  
    if ( needMargin ) {
#ifdef HAVE_TEX_UTILS
    plotter -> setTopMargin ( needMargin?10.0:0.0 );
    plotter -> setNeedUpdate(true);
    plotter -> notifyObservers ();
#else
    warningTex ();
    s.erase ( 0, 4 );

#endif
    }
    plotter->setTitle ( s );
  }
  

}

void Inspector::symbolTypeButtonGroup_clicked ( int id )
{
  QAbstractButton * button = m_symbol_group -> button ( id );
  if ( button )
    button -> setEnabled ( true );
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return ;

  DisplayController * controller = DisplayController::instance ();
  DataRep * rep = controller -> activeDataRep ( plotter );
  if ( rep == 0 ) {
    multipleDataRepError ( tr("plot symbol change") );
    return;
  }

  rep -> setRepStyle ( id );
}

void Inspector::lineStyleButtonGroup_clicked ( int id )
{

  QAbstractButton * button = m_line_group -> button ( id );
  if ( button )
    button -> setEnabled ( true );
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return ;

  DisplayController * controller = DisplayController::instance ();
  DataRep * rep = controller -> activeDataRep ( plotter );
  if ( rep == 0 ) {
    multipleDataRepError ( tr("plot symbol change") );
    return;
  }

  rep -> setRepStyle ( id );
}

void Inspector::symbolPointSize_returnPressed()
{

  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;

  DisplayController * controller = DisplayController::instance ();
  DataRep * rep = controller -> activeDataRep ( plotter );
  if ( rep == 0 ) {
    multipleDataRepError ( tr("plot symbol size change") );
    return;
  }

  QString text = m_symbolPointSize->text();
  double size = text.toFloat();

  rep->setRepSize ( size );

}

void Inspector::intervalStateChanged ( bool state )
{
  PlotterBase * plotter = getPlotter ();
  if ( plotter == 0 ) return;

  DisplayController * controller = DisplayController::instance ();
  controller->setIntervalEnabled ( plotter, state );
  m_interval_le->setEnabled ( state );
}

void
Inspector::
intervalTextChanged ( const QString & text )
{
  PlotterBase * plotter = getPlotter ();
  if ( plotter == 0 ) return;

  DisplayController * controller = DisplayController::instance ();

  unsigned int interval = text.toUInt ();
  controller->setIntervalCount ( plotter, interval );
}

void
Inspector::
colorSelect_clicked()
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;

  int index = plotter->activePlotIndex ();

  if ( index < 0 ) {
    multipleDataRepError ( tr("color change") );
    return;
  }

  const Color & rep_color = plotter->repColor();
  QColor color ( rep_color.getRed(),
		 rep_color.getGreen(),
		 rep_color.getBlue() );
  color = QColorDialog::getColor ( color );
  if ( color.isValid() == false ) return;

  QPalette palette;
  palette.setColor ( m_selectedColor->backgroundRole(), color );
  m_selectedColor->setPalette ( palette );

  Color c ( color.red(), color.green(), color.blue() );
  plotter->setRepColor ( c );
}

void
Inspector::
pointRepComboBox_activated ( const QString & qstr )
{
  PlotterBase * plotter = getPlotter ();

  if ( plotter != 0 ) {
    DisplayController * controller = DisplayController::instance ();
    const string rep ( qstr.toLatin1().data() );

    controller -> setPointRep ( plotter, rep );

    if ( plotter -> hasAxis ( Axes::Z ) == true ) {

      int index = m_value_combo -> currentIndex ();
      controller -> setValueTransform ( plotter, index );
    }

    updatePlotTab (); // to update the m_point_stack and size.
  }
}

void
Inspector::
axis_button_group_clicked ( int id )
{
  m_axis = hippodraw::Axes::convert ( id );
  updateAxisTab ();
}

void
Inspector::
axisZoomPanCheckBox_clicked()
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;

  plotter->setAutoRanging ( m_axis, false );
  const Range & r = plotter->getRange ( m_axis, true );

  m_autoScale->setChecked ( false );

  if ( axisWidget1->isZoomPanChecked() ) {
    m_zoompan[plotter] = true;
  }

  else {

    std::map < const PlotterBase *, bool >::const_iterator it
      = m_zoompan.find ( plotter );
    if ( it != m_zoompan.end () ) {
      m_zoompan[plotter] = false;
    }

  }

  axisWidget1->processZoomPanCheckBoxClicked ( r, r );
}

void
Inspector::
highRangeDrag()
{
  int value = axisWidget1->getHighSliderValue ();
  setHighRange ( value, false );

  axisWidget1->setHighSliderValue ( 50 );
}

void
Inspector::
lowRangeDrag()
{
  int value = axisWidget1->getLowSliderValue ();
  setLowRange ( value, false  );

  axisWidget1->setLowSliderValue ( 50 );
}

void
Inspector::
offsetDrag()
{
  int value = m_offset_range->value ();
  setOffset ( value, false );
  m_offset_range->setValue ( 50 );
}

void
Inspector::
widthDrag ()
{
  int value = m_width_range->value ();
  setBinWidth ( value, false );

  m_width_range->setValue ( 50 );
}

void
Inspector::
entriesDrag ()
{
  int value = min_entries_slider->value ();
  m_dragging = false;
  setMinEntries(value);

  //  min_entries_slider->setValue ( 50 );
}

void
Inspector::
setWidthText()
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;

  DisplayController * controller = DisplayController::instance ();
  int index = controller -> activeDataRepIndex ( plotter );
  bool yes = controller -> hasNTupleBindings ( plotter, index );
  if ( yes ) {
    //Get the string and convert it to double.
    QString text = m_width_text->text();
    double width = text.toDouble();

    if ( width == 0 ) return;    // To prevent it from crashing.
    plotter -> setBinWidth ( m_axis, width );
  }

  updateAxisTab();
}

void
Inspector::
setDragOn ()
{
  m_dragging = true;
  m_min_entries = getMinEntries();

  if ( ! axisWidget1->isZoomPanChecked() )
    {
      m_autoScale->setChecked ( false );
      autoScale_clicked ();
    }
  else
    {
      // Save current width and position.
      m_autoScale->setChecked ( false );
      autoScale_clicked ();

      PlotterBase * plotter = getPlotter ();
      if ( !plotter ) return;
      const Range & r = plotter->getRange ( m_axis, true );
     m_range.setRange ( r.low(), r.high(), r.pos() );
    }

}

void
Inspector::
setOffsetText()
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;

  DisplayController * controller = DisplayController::instance ();
  int index = controller -> activeDataRepIndex ( plotter );
  bool yes = controller -> hasNTupleBindings ( plotter, index );
  if ( yes ) {
    //Get the string and convert it to double.
    QString text = m_offset_text->text();
    double offset = text.toDouble();

    int value = static_cast < int > ( 50.0 * offset ) + 49;
    setDragOn ();
    setOffset( value );
    offsetDrag ();
  }

  updateAxisTab ();
}

void
Inspector::
setBinWidth ( int value )
{
  setBinWidth ( value, m_dragging );

  if ( m_dragging == false ) m_width_range -> setValue ( 50 );
}

void
Inspector::
setBinWidth ( int value, bool drag )
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;
  m_dragging = drag;

  plotter -> setBinWidth ( m_axis, value, m_dragging );
  updateAxisTab ();
}

void
Inspector::
axisLabelText()
{
  PlotterBase * plotter = getPlotter ();
  if ( plotter != 0 ) {
    QString text = m_axis_label -> text ();
    string ltext = text.toLatin1().data();
    const string axis = convertToString ( m_axis );

    /* The size of drawrect, marginrect, need to be updated
       according to title text format. LaTeX format starts with
       "tex:" (case insensitive).
    */
    bool needMargin = String::ci_find(ltext, "tex:")==0;
    if ( needMargin ) {
#ifdef HAVE_TEX_UTILS
    if (m_axis==Axes::X)
      plotter -> setBottomMargin ( needMargin?8.0:0.0 );
    else if (m_axis==Axes::Y)
      plotter -> setLeftMargin ( needMargin?0.0:0.0 );
    else if (m_axis==Axes::Z)
      plotter -> setZMargin ( needMargin?7.0:0.0 );
    plotter -> setNeedUpdate(true);
    plotter -> notifyObservers ();
#else
    warningTex();
    ltext.erase( 0, 4 );
#endif
    }
    plotter -> setLabel ( m_axis, ltext );
  }
  

}

void
Inspector::
setLowText()
{

  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;

  Range r = plotter->getRange ( m_axis, true );

  axisWidget1->processTextBoxReturnPressed ( r, r );

  plotter->setRange ( m_axis, r, true, false ); // scaled, keep bin width
  m_autoScale->setChecked ( false );

  updateAxisTab ();
}

void
Inspector::
setLowRange ( int value )
{
  if ( m_is_updating == false ) {
    setLowRange ( value, m_dragging );
    if ( m_dragging == false ) {
      axisWidget1->setLowSliderValue ( 50 );
    }
  }
}

void
Inspector::
setLowRange ( int value, bool yes )
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;
  m_dragging = yes;

  plotter->setAutoRanging ( m_axis, false );

  if ( ! axisWidget1->isZoomPanChecked() )
    {
      const string axis = convertToString ( m_axis );
      plotter->setLowRange ( m_axis, value, m_dragging );
      const Range & r = plotter->getRange ( m_axis, true );
      double low = r.low();
      axisWidget1 -> setLowText ( QString("%1").arg(low));
    }
  else
    {
      const Range & r = plotter->getRange ( m_axis, true );
      Range range ( r.low(), r.high(), r.pos() );
      axisWidget1->processLowSliderMoved ( value, range, m_range );
      if ( m_dragging ) plotter->setRange ( m_axis, range, true, false );
    }
}

void
Inspector::
setHighRange ( int value )
{
  if ( m_is_updating == false ) {
    setHighRange ( value, m_dragging );
    if ( m_dragging == false ) {
      axisWidget1->setHighSliderValue ( 50 );
    }
  }
}

void
Inspector::
setHighRange ( int value, bool yes )
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;
  m_dragging = yes;

  plotter->setAutoRanging ( m_axis, false );

  if ( !axisWidget1->isZoomPanChecked() )
    {
      const string axis = convertToString ( m_axis );
      plotter->setHighRange ( m_axis, value, m_dragging );
      const Range & r = plotter->getRange ( m_axis, true );
      double high = r.high();
      axisWidget1 -> setHighText ( QString("%1").arg(high));
      return;
    }

  BinaryTransform *t =
    dynamic_cast<  BinaryTransform* > ( plotter->getTransform() );

  if ( axisWidget1->isZoomPanChecked() && ! t->isPeriodic() )
    {
      const Range & r = plotter->getRange ( m_axis, true );
      Range range ( r.low(), r.high(), r.pos() );
      axisWidget1->processHighSliderMoved ( value, range, m_range );
      if ( m_dragging ) plotter->setRange ( m_axis, range, true, false );
      return;
    }

  double offset(0.0), incr(0.0);

  if ( axisWidget1->isZoomPanChecked() && t->isPeriodic() )
    {
          
      PeriodicBinaryTransform *tp =
	dynamic_cast< PeriodicBinaryTransform* > ( t );

      const Range & r = plotter->getRange ( m_axis, true );
      Range range (r.low(), r.high(), r.pos());

      incr = ( value - m_highslider1_last_val ) * r.length() / 100;
      m_highslider1_last_val = value;

      // Exchange axes to make the GUI more understandable.
      if ( m_axis == Axes::Y ) {
          offset = tp->rotation(Axes::X);
	  offset += incr/UNITS_DEGREES*UNITS_RADIANS;
          tp->setRotation(offset, Axes::X);
      } else if ( m_axis == Axes::X ) {
          offset = tp->rotation(Axes::Y);
	  offset += incr/UNITS_DEGREES*UNITS_RADIANS;
          tp->setRotation(offset, Axes::Y);
      }

      axisWidget1 -> setHighText ( QString( "%1" ).arg( offset ) );
      if ( m_dragging ) plotter->setRange ( m_axis, range, true, false );
      return;
    }
}

void
Inspector::
setHighText()
{

  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;

  Range r = plotter->getRange ( m_axis, true );

  axisWidget1->processTextBoxReturnPressed ( r, r );

  plotter->setRange ( m_axis, r, true, false ); // scaled and keep bin width
  m_autoScale->setChecked ( false );

  updateAxisTab ();
}

void
Inspector::
setOffset ( int value  )
{
  setOffset( value, m_dragging );
}

void
Inspector::
setOffset ( int value, bool yes  )
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;
  m_dragging = yes;

  const string axis = convertToString ( m_axis );
  DisplayController * controller = DisplayController::instance();
  controller ->  setOffset ( plotter, axis, value, m_dragging );
  double offset = plotter->getOffset ( m_axis );
  m_offset_text -> setText ( QString ("%1").arg (offset) );

  updateAxisTab ();
}

const std::vector < PlotterBase * > &
Inspector::
getDataCutList ( PlotterBase * plotter )
{
  vector < PlotterBase * > plotterlist;
  fillPlotterList ( plotterlist );


  DisplayController * controller = DisplayController::instance ();
  const DataSource * tuple = controller -> getDataSource ( plotter );
  CutController * cutcontroller = CutController::instance();
  return cutcontroller->getCutList ( plotterlist, tuple );
}

void
Inspector::
cutText_returnPressed ()
{
  m_is_updating = true;

  int id = cutRadioId ();
  bool fit_cut = id == 2;

  int index = m_selCutComboBox -> currentIndex ();
  Range currentRange = m_tuple_cuts [index] -> getRange();
  PlotterBase * plotter = getSelectedCut();

  if ( fit_cut == false ) {
    Axes::Type cut_axis = getAxes ( index );
    const Range & fullRange = plotter -> getRange ( cut_axis, false );
    axisWidget2->processTextBoxReturnPressed ( currentRange, fullRange );
    plotter->setCutRangeAt ( currentRange, cut_axis );
  }
  else {
    const Range & fullRange = plotter -> getRange ( Axes::X, false );
    axisWidget2->processTextBoxReturnPressed ( currentRange, fullRange );
    plotter->setCutRangeAt ( currentRange, index );
  }
}

void Inspector::disableCutControls ( bool yes )
{
  axisWidget2->setAllDisabled ( yes );
  colorSelect_2->setDisabled ( yes );
  cutRemovePushButton->setDisabled ( yes );
  cutInvertPushButton->setDisabled ( yes );
  cutEnablePushButton -> setDisabled ( yes );
  m_cutAddSelected->setDisabled ( yes );
  m_cutAddAll -> setDisabled ( yes );

  if ( yes ) {
    int number = m_selCutComboBox -> count ();
    while ( number-- > 0 ) {
      m_selCutComboBox -> removeItem ( 0 );
    }
  }
  m_selCutComboBox -> setDisabled ( yes );
}

void
Inspector::
updateTupleCuts ( const std::vector < PlotterBase * > & cutlist )
{
  m_tuple_cuts.clear ();
  m_tuple_cut_plotters.clear ();

  unsigned int size = cutlist.size ();

  for ( unsigned int i = 0; i < size; i++ ) {
    PlotterBase * plotter = cutlist[i];
    TupleCutList_t cuts;
    plotter -> fillCutList ( cuts );

    for ( unsigned int j = 0; j < cuts.size (); j++ ) {
      m_tuple_cuts.push_back ( cuts[j] );
      m_tuple_cut_plotters.push_back ( plotter );
    }
  }
}

void
Inspector::
updateCutControls ( const std::vector < PlotterBase * > & cutlist )
{
  QString old_current = m_selCutComboBox -> currentText ();
  int numberItems = m_selCutComboBox->count();

  while ( numberItems-- > 0 ) {
    m_selCutComboBox->removeItem(0);
  }

  m_selCutComboBox -> setEnabled ( true );

  updateTupleCuts ( cutlist );
  bool yes = m_tuple_cuts.empty ();
  if ( yes ) {
    cutRemovePushButton -> setEnabled ( false );
    return;
  }

  int index = -1;
  unsigned int size = m_tuple_cuts.size ();

  for ( unsigned int i = 0; i < size; i++ ) {
    const TupleCut * cut = m_tuple_cuts[i];
    const string & label = cut -> getLabel ();
    QString item = label.c_str ();
    m_selCutComboBox -> addItem ( item );
    if ( item == old_current ) index = i;
  }

  PlotterBase * plotter = getPlotter ();
  assert ( plotter );
  index = -1;
  map < PlotterBase *, int > ::iterator first
    = m_cut_map.find ( plotter );

  if ( first == m_cut_map.end () ) { // not found
    index = 0;
    m_cut_map [ plotter ] = index;
  }
  else {
    index = first -> second;
  }
  int count = m_selCutComboBox -> count ();
  if ( index >= count ) {
    index = count -1;
    m_cut_map [ plotter] = index;
  }

  m_selCutComboBox -> setCurrentIndex ( index );
  updateCutEnableButton ();
}

/** @request This method seems to get called more than once when
    adding a cut.  Should try to fix that.
*/
void
Inspector::
updateCutEnableButton ( )
{
  if ( m_tuple_cuts.empty () ) return;

  int index = m_selCutComboBox -> currentIndex ();
  const TupleCut * cut = m_tuple_cuts [ index ];
  assert ( cut != 0 );
  bool yes = cut -> isEnabled ();
  m_cut_enable_updating = true;
  cutEnablePushButton -> setChecked ( ! yes );
  m_cut_enable_updating = false;
}

Axes::Type
Inspector::
getAxes ( unsigned int index )
{
  Axes::Type axis = Axes::Y;

  PlotterBase * plotter = m_tuple_cut_plotters [ index ];
  unsigned int size = m_tuple_cut_plotters.size ();
  for ( unsigned int i = 0; i < size; i++ ) {
    if ( m_tuple_cut_plotters[i] == plotter ) {
      if ( i == index ) {
	axis = Axes::X;
      }
      break;
    }
  }

  return axis;
}

void
Inspector::
updateCutControlValues ( const PlotterBase * cplotter )
{
  m_is_updating = true;
  int index = m_selCutComboBox -> currentIndex ();
  const Range & currentRange = m_tuple_cuts[index] -> getRange ();

  Axes::Type cut_axis = Axes::X;
  int id = cutRadioId ();
  bool fit_cut = id == 2;
  if ( fit_cut == false ) {
    cut_axis = getAxes ( index );
  }
  const Range & fullRange = cplotter->getRange ( cut_axis, false );

  axisWidget2->updateCutControlValues ( currentRange, fullRange );
  CutController * controller = CutController::instance ();

  bool yes
    = controller -> isZoomPan ( cplotter, cut_axis );
  axisWidget2 -> setZoomPan ( yes );
  axisWidget2->processZoomPanCheckBoxClicked ( currentRange, fullRange );
  m_is_updating = false;
}

// void
// Inspector::
// fillCutsOn ( const PlotterBase * plotter,
// 	     std::vector < PlotterBase * > & cutlist )
// {
//   cutlist.clear();

//   DisplayController * controller = DisplayController::instance ();
//   const DataRep * datarep = controller -> activeDataRep ( plotter );
//   if ( datarep != 0 ) {
//     CutController * cutcontroller = CutController::instance();

//     cutcontroller->fillCutList ( datarep, cutlist );
//   }
// }

const std::vector < const TupleCut * > &
Inspector::
getCutList ( const PlotterBase * plotter ) const
{
  DisplayController * controller = DisplayController::instance ();
  int index = controller -> activeDataRepIndex ( plotter );
  if ( index < 0 ) {
    string what = tr (
		      "Inspector::getCutList: "
		      "no active DataRep in PlotterBase object."
		      ).toAscii().data();
    throw std::logic_error ( what );
  }
  const DataRep * datarep = plotter -> getDataRep ( index );
  CutController * cut_controller = CutController::instance ();

  return cut_controller -> getCutList ( datarep );
}

void
Inspector::
selectedCutsRadioButton_toggled ( bool )
{
  if ( !m_selectedPlotRadioButton->isChecked() ) return;

  // Change the items in the combo box to only cuts over selected datarep.

  PlotterBase * plotter = getPlotter ();
  bool yes = plotter != 0;
  if ( yes ) yes = plotter -> isTargetable ();
  disableCutControls ( yes == false );
  if ( yes == false ) return;

  vector < PlotterBase * > cutlist;

  if ( cutRadioId () != 2 ) {
    CutController * controller = CutController::instance ();
    controller -> fillCutList ( DisplayController::instance()->
	activeDataRep ( plotter ), cutlist );

    // Clear the combobox and insert the new strings.

    if ( cutlist.empty () ) {
      disableCutControls ( true );
      cutRemovePushButton->setEnabled ( false );
      return;
    }
  }
  else { // fitting cut
    cutlist.push_back ( plotter );
  }

  updateCutControls ( cutlist );

  if ( m_tuple_cuts.empty () ) {
    disableCutControls ( true );
    cutRemovePushButton->setEnabled ( false );
    return;
  }

  // Update Controls.

  disableCutControls ( false );
  cutRemovePushButton->setEnabled ( true );
  m_cutAddSelected -> setDisabled ( true );
  m_cutAddAll -> setDisabled ( true );

  // Update texts and sliders.

  int index = m_selCutComboBox -> currentIndex ();
  const PlotterBase * cut_plotter = m_tuple_cut_plotters [ index ];
  updateCutControlValues ( cut_plotter );
}

void
Inspector::
cutAddSelected ()
{
  // Take the selected cut from cutlistcombobox and add it to selected
  // plotter on canvas.

  // Find the selected cutplotter.

  CutPlotter * cp = getSelectedCut();

  // Find the selected Plotter.
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return ;

  // Add the cut to the plotter.
  CutController * controller = CutController::instance();
  controller -> addCut ( cp, plotter );

}

void
Inspector::
cutAddAll ()
{
  PlotterBase * plotter = getPlotter ();
  if ( plotter == 0 ) return;

  CutController * controller = CutController::instance ();
  const vector < PlotterBase * > & cut_list = getDataCutList ( plotter );

  controller -> addCuts ( cut_list, plotter );

  unsigned int size = cut_list.size ();
  for ( unsigned int i = 0; i < size; i++ ) {
    PlotterBase * pb = cut_list[i];
    CutPlotter * cut_plotter = dynamic_cast < CutPlotter * > ( pb );
    if ( cut_plotter == plotter ) {
      cutOnCutError ();
    }
  }
}

/* Take the selected cut from cutlistcombobox and remove it from the selected
    plotter on canvas.
*/
void
Inspector::
cutRemovePushButton_clicked()
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return ;

  bool is_fit_radio = cutRadioId () == 2;
  if ( is_fit_radio ) {
    DisplayController * controller = DisplayController::instance ();
    int index = controller->activeDataRepIndex ( plotter );
    DataRep * rep = plotter -> getDataRep ( index );
    FunctionController::instance () -> removeTupleCut ( plotter, rep );
  }
  else {
    // Find the selected cutplotter.
    CutPlotter * cp = getSelectedCut();
    CutController::instance() -> removeCut ( cp, plotter );
  }

  if ( m_selectedPlotRadioButton -> isChecked () ) {
    // post event so we don't delete item that sent us the signal.
    PlotterEvent * event = new PlotterEvent ( plotter );
    QApplication::postEvent ( this, event );
  }
}

void
Inspector::
allCutsRadioButton_toggled ( bool )
{
  if ( !allCutsRadioButton->isChecked() ) return;

  PlotterBase * plotter = getPlotter ();

  bool yes = plotter != 0;
  if ( yes ) yes = plotter -> isTargetable ();

  disableCutControls ( yes == false );
  if ( yes == false )  return;

  const vector < PlotterBase * > & cutlist = getDataCutList ( plotter );

  // Clear the combobox and insert the new strings.

  updateCutControls ( cutlist );

  if ( cutlist.empty () ) {
    disableCutControls ( true );
    return;
  }

  // Update Controls.

  disableCutControls ( false );
  cutRemovePushButton->setDisabled ( true );
  m_cutAddSelected -> setEnabled ( true );
  m_cutAddAll -> setEnabled ( true );

  // Update texts and sliders.

  const PlotterBase * cut_plotter = cutlist.back ();
  updateCutControlValues ( cut_plotter );
}

void Inspector::selCutChanged ( )
{
  CutPlotter * cut_plotter = getSelectedCut ();
  updateCutControlValues ( cut_plotter );

  PlotterBase * plotter = getPlotter (); // the target
  int index = m_selCutComboBox -> currentIndex ();
  m_cut_map [ plotter ] = index;

  updateCutEnableButton ();
}

CutPlotter *
Inspector::
getSelectedCut ()
{
  if ( cutRadioId() != 2 ) {
    PlotterBase * plotter = getPlotter ();
    if ( plotter == 0 ) return 0;

    m_last_cut_index = m_selCutComboBox->currentIndex();

    PlotterBase * pb = m_tuple_cut_plotters [ m_last_cut_index ];
    return dynamic_cast < CutPlotter * > ( pb );
  }
  else { // fitting cut
    PlotterBase * pb = getPlotter ();
    CutPlotter * plotter = dynamic_cast < CutPlotter * > ( pb);
    return plotter;
  }
}

/** @request Be able to add new cut to all DataRep objects contained
    by the plotter.
 */
void
Inspector::
cutNew()
{
  // Get the column label from m_CutVariableComboBox1, and create a cut on
  // the selected plotter with that column label. Also update the list of
  // cuts and the high and low range.

  PlotterBase * plotter = getPlotter ();
  bool yes = plotter != 0;

  if ( yes ) yes = plotter -> isTargetable ();
  if ( yes == false ) {
    int index = plotter -> activePlotIndex ();
    if ( index <  0 ) {
      multipleDataRepError ( tr("Cut") );
    }
    else { // must be a function
      const QString message = tr( "Can not apply cut to a function" );
      QMessageBox::information ( this, /* parent */
				 tr("Cut application error"),
				 message,
				 QMessageBox::Ok,
				 Qt::NoButton,
				 Qt::NoButton );
    }
    return;
  }

  DisplayController * controller = DisplayController::instance ();
  int index = controller->activeDataRepIndex ( plotter );
  assert ( index >= 0 );

  const DataRep * rep = plotter -> getDataRep ( index );
  if ( rep -> hasNTupleBindings () == false ) {
    const QString message = tr( "Can not apply cut to a static histogram" );
    QMessageBox::information ( this, /* parent */
			       tr("Cut application error"),
			       message,
			       QMessageBox::Ok,
			       Qt::NoButton,
			       Qt::NoButton );
      return;
  }

  vector < string > bindings;
  int id = cutRadioId ();
  if ( id != 2 ) {
    QString text1 = m_CutVariableComboBox1 -> currentText();
    string label1 = text1.toLatin1().data();
    QString text2 = m_CutVariableComboBox2 -> currentText();
    string label2 = text2.toLatin1().data();

    switch ( id ) {
    case 0 : // 1d data cut
      bindings.push_back( label1 );
      break;
    case 1 : // 2d data cut
      bindings.push_back( label1 );
      bindings.push_back( label2 );
    break;
    }
    CutController * cutcontroller = CutController::instance();
    PlotterBase * cutplotter = cutcontroller -> addCut ( plotter, bindings );
    CanvasWindow * canvas = WindowController::instance () ->currentCanvas();
    if ( canvas == 0 ) return;

    canvas -> addPlotDisplay ( cutplotter, false );
  }
  else {
    DataRep * datarep = plotter -> getDataRep ( index );
    FunctionController * controller = FunctionController::instance ();
    controller -> setTupleCut ( plotter, datarep );
  }

  // Update other guys.
  selectedCutsRadioButton_toggled ( true );
  allCutsRadioButton_toggled ( true );
}

int
Inspector::
findCutIndex ( const QString & label )
{
  int index = -1;

  int size = m_selCutComboBox -> count ();
  for ( int i = 0; i < size; i++ ) {
    QString text = m_selCutComboBox -> itemText ( i );
    if ( text == label ) {
      index = i;
      break;
    }
  }

  return index;
}

void
Inspector::
updateCutsTab ()
{
  if ( m_new_plot_box->isEnabled () == false ) return;

  PlotterBase * plotter = getPlotter ();
  bool yes = plotter != 0;
  m_new_cut_box->setEnabled ( yes );

  if ( yes ) yes = plotter -> isTargetable ();
  m_cut_selected_box->setEnabled ( yes );

  if ( yes ) {
    int id = cutRadioId ();
    bool is_fit_radio = id == 2;
    selectedCutsRadioButton_toggled (true  );
    allCutsRadioButton_toggled ( true );

    if ( is_fit_radio == false ) {
      updateDataCutsTab ();
    }
    else {
      updateFitCutsTab ();
    }
  }
}

void
Inspector::
updateFitCutsTab ()
{
}

void
Inspector::
updateDataCutsTab ()
{
  int id = cutRadioId ();

  if ( id == 0 ) {
    m_CutVariableComboBox2 -> setEnabled ( false );
  }

  PlotterBase * plotter = getPlotter ();
  DisplayController * controller = DisplayController::instance ();
  int index = controller->activeDataRepIndex ( plotter );
  if ( index < 0 ) {
    m_new_cut_box -> setDisabled ( true );
    m_cut_selected_box -> setDisabled ( true );
  }
  else {
    DataRep * datarep = plotter->getDataRep ( index );
    if ( datarep->hasZeroRows() )
      {
	m_new_cut_box->setDisabled ( true );
	m_cut_selected_box->setDisabled ( true );
	return;
      }

    m_new_cut_box->setDisabled ( false );
    const vector < PlotterBase * > & cuts = getDataCutList ( plotter );
    if ( cuts.empty() ) {
      m_cut_selected_box->setDisabled ( true );
    }
    else {
      m_cut_selected_box -> setDisabled ( false );
    }
  }

  updateCutVarGroupBox ( plotter, index );
}

void
Inspector::
setAllCutsActive ( bool yes )
{
  vector < PlotterBase * > plotters;
  fillPlotterList ( plotters );

  if ( plotters.empty () == false ) {
    CutController * controller = CutController::instance ();
    vector < CutPlotter * > cutters;
    controller -> fillCutList ( plotters, cutters );
    vector < CutPlotter * > ::iterator first = cutters.begin ();

    while ( first != cutters.end () ) {
      CutPlotter * cutter = *first++;
      cutter -> setActive ( yes );
    }
  }
}

void
Inspector::
updateCutVarGroupBox ( const PlotterBase * plotter, int index )
{
  const DataSource * tuple
    = DisplayController::instance() -> getDataSource ( plotter, index );

  if (!tuple) return;
  int index1 = -1;
  int index2 = -1;
  if ( m_CutVariableComboBox1 -> count() > 0 ) {
    index1 = m_CutVariableComboBox1 -> currentIndex ();
  }
  if ( m_CutVariableComboBox2 -> count() > 0 ) {
    index2 = m_CutVariableComboBox2 -> currentIndex ();
  }
  m_CutVariableComboBox1->clear();
  m_CutVariableComboBox2->clear();

  const vector < string > & cols = tuple->getLabels();
#ifdef ITERATOR_MEMBER_DEFECT
  std::
#endif
    vector < string > ::const_iterator first = cols.begin ();
  while ( first != cols.end() ) {
    const string & str = *first++;
    m_CutVariableComboBox1 -> addItem ( str.c_str() );
    m_CutVariableComboBox2 -> addItem ( str.c_str() );
  }
  if ( index1 >= m_CutVariableComboBox1 -> count () ) {
    index1 = 0;
  }
  if ( index2 >= m_CutVariableComboBox2 -> count () ) {
    index2 = 0;
  }

  if ( index1 >= 0 ) {
    m_CutVariableComboBox1 -> setCurrentIndex ( index1 );
  }
  if ( index2 >= 0 ) {
    m_CutVariableComboBox2 -> setCurrentIndex ( index2 );
  }

  buttonGroupSetEnabled(cut_button_group);
  int id = cutRadioId ();
  if ( id == 1 ) {
    m_CutVariableComboBox2 -> setEnabled( true );
  }
}

int
Inspector::
cutRadioId () const
{
  int id = -1;
  if ( m_cut_data1 -> isChecked () ) id = 0;
  if ( m_cut_data2 -> isChecked () ) id = 1;
  if ( m_cut_fit_radio -> isChecked () ) id = 2;

  return id;
}

void
Inspector::
cut_button_group_clicked ( )
{
  int id = cutRadioId ();

  switch  ( id ) {
  case 0 : // 1d data cut
    m_CutVariableComboBox1 -> setEnabled ( true );
    m_CutVariableComboBox2 -> setEnabled ( false );
    break;
  case 1 : // 2d data cut
    m_CutVariableComboBox1 -> setEnabled ( true );
    m_CutVariableComboBox2 -> setEnabled ( true );
    break;
  case 2 : // fit cut
    m_CutVariableComboBox1 -> setEnabled ( false );
    m_CutVariableComboBox2 -> setEnabled ( false );
    break;
  }

  updateCutsTab (); // in case of switching from data to fitting
}
void
Inspector::
  setSelectedFitter ( const std::string & name )
{
  FunctionController * controller = FunctionController::instance ();
  const vector < string > & fitters = controller -> getFitterNames ();
  for ( unsigned int i = 0; i < fitters.size(); i++ ) {
    if ( name == fitters[i] )
      {
	m_fitter_names -> setCurrentIndex ( i );
	break;
      }
  }
}

void
Inspector::
updateFunctionsTab ()
{
  FunctionController * controller = FunctionController::instance ();
  const vector < string > & names = controller -> getFunctionNames ();
  int current = newFunctionsComboBox->currentIndex ();

  if ( newFunctionsComboBox -> count () !=
       static_cast < int > ( names.size () ) ) {
    newFunctionsComboBox->clear();

    for ( unsigned int i = 0; i < names.size(); i++) {
      if ( names[i] != "Linear Sum" ) {
	newFunctionsComboBox->addItem ( names[i].c_str() );
      }
    }
  }

  PlotterBase * plotter = getPlotter();
  bool yes = plotter != 0;
  m_func_new->setEnabled ( yes );

  if ( yes ) yes = plotter -> isTargetable ();
  functionsRemoveButton -> setEnabled ( yes );
  m_IgnoreErrorCheckBox -> setEnabled (yes);
  functionsFitToDataButton -> setEnabled ( yes );
  functionsResetButton -> setEnabled ( yes );
  m_resid->setEnabled ( yes );
  if ( yes == false ) {
    return;
  }

  const DataSource * nt
    = DisplayController::instance()->getDataSource ( plotter, 0 );

  if ( nt && nt -> empty () )
    {
      m_func_new->setDisabled ( true );
      functionsRemoveButton -> setEnabled ( false );
      m_IgnoreErrorCheckBox -> setDisabled (true);
      functionsFitToDataButton -> setEnabled ( false );
      functionsResetButton -> setEnabled ( false );
      return;
    }

  // Update new functions section //
  //------------------------------//
  newFunctionsAddButton->setEnabled ( true );
  m_func_new->setEnabled ( true );

  if ( current >= 0 ) {
    newFunctionsComboBox->setCurrentIndex(current);
  }
  newFunctionsComboBox->setEnabled ( true );

  // Update functionsAppliedComboBox. //
  //----------------------------------//
  bool to_enable = false;
  DisplayController * d_controller = DisplayController::instance ();
  int index = d_controller -> activeDataRepIndex ( plotter );

  FunctionController * f_controller = FunctionController::instance ();

  if ( index >= 0 ) {
    DataRep * datarep = plotter ->getDataRep ( index );
    if ( f_controller -> hasFunction ( plotter, datarep ) ) {

      const vector < string > & fnames
	= f_controller -> functionNames ( plotter, datarep );
	
      if ( fnames.size() != 0 )
	{
	  to_enable = true;
	  m_functionIndexMap.clear();
	
	  for ( unsigned i = 0; i < fnames.size(); i++)
	    {
	      if ( fnames[i].find ( "Linear Sum" ) == string::npos )
		{
		  m_functionIndexMap.push_back ( i );
		}
	    }
	}
    }
  }

  functionsRemoveButton -> setEnabled ( to_enable );
  m_IgnoreErrorCheckBox -> setEnabled (to_enable);
  functionsFitToDataButton -> setEnabled ( to_enable );
  functionsResetButton -> setEnabled ( to_enable );
  if ( to_enable == false ) {
    m_FunctionParamsListView -> clear();
  }

  if ( to_enable )
    {
      Fitter * fitter = f_controller -> getFitter ( plotter );
      string name = "none";
      if ( fitter != 0 ) {
	name = f_controller -> getFitterName ( plotter );
	setSelectedFitter ( name );
      }
    }

  m_resid->setEnabled ( to_enable );

  // Update function parameters tab //
  //--------------------------------//

  // Set Parameters in list view as well as in line editor and
  // the check box. Focus is set to the current selected item
  // or in case none is selected 1st item.
  if ( to_enable ) setParameters ( index, plotter );
    
  // Set the slider to be in the center
  m_FunctionParamsSlider -> setValue(50);

}

void
Inspector::
functionsRemoveButton_clicked()
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return ;

  FunctionRep * frep = getFunctionRep ( );
  FunctionController * controller = FunctionController::instance ();
  controller -> removeFunction ( plotter, frep );
  if ( plotter->activePlotIndex ( ) != 0 )
    plotter->setActivePlot ( -1, true );
  else
    plotter->setActivePlot ( 0, true );

  // Update the rest.
  updateFunctionsTab();
}

void
Inspector::
functionsResetButton_clicked()
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return ;

  DisplayController * dcontroller = DisplayController::instance ();
  int index = dcontroller -> activeDataRepIndex ( plotter );
  if ( index < 0 ) return;
  DataRep * datarep = plotter -> getDataRep ( index );

  FunctionController * fcontroller = FunctionController::instance();
  if ( ! ( fcontroller -> hasFunction ( plotter, datarep ) ) ) {
    return;
  }

  fcontroller -> restoreParameters ( plotter );

  // Set the parameters
  setParameters ( index, plotter );
}

void
Inspector::
setParameters ( int index, PlotterBase * plotter )
{
  m_FunctionParamsListView -> clear();
  m_FunctionParamsCheckBox -> setChecked( false );
  m_FunctionParamsLineEdit -> clear();

  DataRep * datarep = plotter -> getDataRep ( index );
  assert ( datarep != 0 );

  FunctionController * controller = FunctionController::instance ();
  if ( ! ( controller -> hasFunction ( plotter, datarep ) ) )
    {
      return;
    }

  const vector < string > & fnames
    = controller -> functionNames ( plotter, datarep );

  if ( fnames.empty () ) {
    return;
  }

  m_function_lv_map.clear ();
  vector < FunctionRep * > freps;
  controller -> fillTopLevelFunctionReps ( freps, plotter, datarep );
  for ( unsigned int i = 0; i < freps.size (); i++ ) {
    FunctionRep * frep = freps [ i ];
    const string & func_name = frep -> functionName ();

    QTreeWidgetItem * parent
      = new QTreeWidgetItem ( m_FunctionParamsListView );
    parent -> setExpanded ( true );
    parent -> setText ( Index, QString ( func_name.c_str() ) );
    m_function_lv_map [ parent ] = frep;

    //Ignore errors flag, conected with the ignoreError check box.
    bool ignoreFlag = true;

    Fitter * fitter = frep -> getFitter ();
    if ( fitter != 0 ) {
      //Get the ignore errors flag of the current function.
      ignoreFlag = frep -> getIgnoreErrors ();
    }
    const vector < double > & parms = frep -> parameters ();
    unsigned int start_index = parms.size();
    fillFunctionParameters ( parent, frep, start_index );

    m_FunctionParamsListView -> setAllColumnsShowFocus ( true );
    QTreeWidgetItem * firstItem = parent -> child (0);
    m_IgnoreErrorCheckBox -> setChecked(ignoreFlag);

    if ( firstItem != 0 ) {
      m_FunctionParamsLineEdit -> setText ( firstItem -> text( Value ) );
      m_FunctionParamsListView -> selectionModel() -> clearSelection();
      firstItem -> setSelected ( true );
      m_FunctionParamsListView -> setCurrentItem ( firstItem );
    
      QString fixedFlag = firstItem -> text( Fixed );
      m_FunctionParamsCheckBox->setChecked((fixedFlag == qyes ) ? 
					   true : false );
    }
  }
}

void
Inspector::
fillFunctionParameters ( QTreeWidgetItem * parent,
			 const FunctionRep * frep,
			 unsigned int & index )
{
  const CompositeFunctionRep * composite
    = dynamic_cast < const CompositeFunctionRep * > ( frep );

  if ( composite != 0 ) {
    const vector < FunctionRep * > & freps 
      = composite -> getFunctionReps ();
    unsigned int size = freps.size();

    //  because children are inserted at the beginning, we must do things
    // backwards
    for ( int i = size -1; i >= 0; i-- ) {
      FunctionRep * rep = freps[i];
      const string & func_name = rep -> functionName ();
      QTreeWidgetItem * child
	= new QTreeWidgetItem ( parent );
      child -> setExpanded ( true );
      child -> setText ( Index, QString ( func_name.c_str() ) );
      m_function_lv_map [ child ] = rep;

      fillFunctionParameters ( child, rep, index );
    }
  }
  else { // not composite
    vector < FunctionParameter > function_parameters;
    frep -> fillFunctionParameters ( function_parameters );

    // Because items are inserted at the begining, we must do things
    // backwards
    unsigned int size = function_parameters.size ();

    for ( int pindex = size-1; pindex >= 0; pindex-- ) {
      FunctionParameter fp = function_parameters[pindex];
      QTreeWidgetItem * item
	= new QTreeWidgetItem ( parent );
      item -> setFlags ( Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
      item -> setCheckState ( Index, Qt::Unchecked );
      item -> setText ( Index, QString( "%1" ).arg( index-- ) );
      
      QString name = fp.name().c_str();
      QString fixedFlag = fp.isFixed () ? qyes : qno;

      item -> setText( Name, QString( "%1" ).arg( name ) );
      item -> setText( Value, QString( "%1" ).arg( fp.value() ) );
      item -> setText( Error, QString( "%1" ).arg( fp.error() ));
      item -> setText( Fixed, QString( "%1" ).arg( fixedFlag ) );
      item -> setText( Dummy, QString( "%1" ).arg( pindex ) );
    }
  }
}

/** @bug @@@@@@ If the fitter setting is set to one that needs
    integration and target DataRep is one that is not binned, then
    adding a function causes a crash. */
void
Inspector::
functionAdd ()
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return ;

  bool yes = plotter -> isTargetable ();
  if ( yes == false ) {
    multipleDataRepError ( tr( "function" ) );
    return;
  }

  DisplayController * dc = DisplayController::instance();

  DataRep * datarep = dc -> activeDataRep ( plotter );
  assert ( datarep != 0 );

  if ( !datarep->acceptFunction(1) ){
    functionAddError ();
    return;
  }

  // Get the selected function name.

  QString qstr  =  newFunctionsComboBox->currentText();
  std::string fun_name = qstr.toLatin1().data();

  // Add the function.

  QString s = m_fitter_names -> currentText ( );
  const string fit_name = s.toLatin1().data();

  FunctionController * fc = FunctionController::instance();
  yes = fc -> isCompatible ( fun_name, fit_name );

  if ( !yes ) {
    incompatibleFunctionError ( fun_name );
    return;
  }

  FunctionRep * new_rep = 0;
  try {
    QTreeWidgetItem * item = m_FunctionParamsListView -> currentItem ();
    bool is_selected = item ? item->isSelected() : false;
    FunctionRep * frep = 0;
    if ( is_selected ) {
      frep = getTopFunctionRep ( item );
    }
    
    new_rep = fc->addFunction ( plotter, fun_name, frep, datarep );
  }
  catch ( std::exception & e ) {
    badFunctionError ( fun_name, e.what() );
  }

  int index = m_fitter_names -> currentIndex ();
  fitterNamesActivated ( index ); // sets the fitter
  
  functionsRemoveButton -> setEnabled ( true );
  m_IgnoreErrorCheckBox -> setEnabled (true);
  functionsFitToDataButton -> setEnabled ( true );
  functionsResetButton -> setEnabled ( true );
  fc->saveParameters ( plotter );
  
  bool ok = false;
  if ( new_rep != 0 ) {
    ok = fc -> tryFitFunction ( plotter, new_rep ); // try fit
  }
  if ( ! ok ) {
    fitFailedError ();
  }
  // Update other tabs that need it.
  
  updateFunctionsTab();
}

void
Inspector::
fitterNamesActivated ( int index )
{
  FunctionController * controller = FunctionController::instance ();
  const vector < string > & names = controller -> getFitterNames ();
  const string & def_fitter = names [ index ];
  controller -> setDefaultFitter ( def_fitter );

  PlotterBase * plotter = getPlotter ();
  if ( plotter != 0 ) {
    const DataRep * datarep = plotter -> getTarget ();
    bool yes = controller -> hasFunction ( plotter, datarep );
    if ( yes ) {
      bool ok = controller -> changeFitter ( plotter, datarep,
					     def_fitter );
      if ( ok == false ) {
	incompatibleFitterError ( def_fitter );
	functionsFitToDataButton -> setEnabled ( false );
      }
      else {
	functionsFitToDataButton -> setEnabled ( true );
      }
    }
  }
}

void
Inspector::
fitFailedError ()
{
  const QString message = tr( "The Fit failed to converge" );
  QMessageBox::critical ( this, // parent
			  tr("Fit failed"),
			  message,
			  QMessageBox::Ok,
			  Qt::NoButton,
			  Qt::NoButton );
}

FunctionRep *
Inspector::
getTopFunctionRep ( QTreeWidgetItem * item )
{
  FunctionRep * rep = 0;
  if ( item != 0 ) {
    item = getTopParent ( item );
    rep = m_function_lv_map [ item ];
  }
  return rep;
}

FunctionRep *
Inspector::
getFunctionRep ( QTreeWidgetItem * item )
{
  FunctionRep * rep = 0;
  if ( item != 0 ) {
    rep = m_function_lv_map [ item ];
  }
  return rep;
}

FunctionRep *
Inspector::
getTopFunctionRep ()
{
  QTreeWidgetItem * item = m_FunctionParamsListView -> currentItem();

  return getTopFunctionRep ( item );
}

FunctionRep *
Inspector::
getFunctionRep ()
{
  QTreeWidgetItem * item = m_FunctionParamsListView -> currentItem();

  if ( item -> childCount() == 0 ) { // parameter item
    item = item -> parent ();
  }

  return getFunctionRep ( item );
}

void
Inspector::
functionsFitToDataButton_clicked()
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return ;

  FunctionController * fcnt = FunctionController::instance();
  if ( ! ( fcnt -> hasFunction ( plotter, 0 ) ) ) { // any function
    return;
  }

  fcnt -> saveParameters ( plotter );

  FunctionRep * fun_rep = getTopFunctionRep ();

  bool ok = fcnt -> fitFunction ( plotter, fun_rep );
  if ( ! ok ) {
    fitFailedError ();
  }

  // Set the parameters
  DisplayController * dcontroller = DisplayController::instance ();
  int index = dcontroller -> activeDataRepIndex ( plotter );

  setParameters ( index, plotter );
}

void
Inspector::
statsStripValue ( QRadioButton * box )
{
  QString text_str = box -> text ();
  int i = text_str.indexOf ( "=" );
  text_str.remove ( i + 1, 1024 );
  box -> setText ( text_str );
}

void Inspector::updateSummaryTab()
{
  if ( m_new_plot_box->isEnabled() == false ) return;

  PlotterBase * plotter = getPlotter();
  bool yes = plotter == 0;
  if ( yes == false ) {
    TextPlotter * text = dynamic_cast < TextPlotter * > ( plotter );
    yes |= text != 0;
  }
  bool enable = ! yes;
  m_summary->setEnabled ( enable );

  if ( enable == false ) return;

  DisplayController * dcontroller = DisplayController::instance ();
  const DataSource * nt = dcontroller -> getDataSource ( plotter, 0 );

  if ( nt && nt -> empty ()  ) {
    m_summary->setEnabled ( false );
    return;
  }

  enable = false; // might be changed below...
  int index = dcontroller -> activeDataRepIndex ( plotter );
  if ( index >= 0 ) {
    DataRep * datarep = plotter -> getDataRep ( index );
    FunctionController * controller = FunctionController::instance();

    enable = controller->hasFunction ( plotter, datarep );
  }
  if ( enable == false ) {
    if ( m_stats_fparms->isChecked () ||
	 m_stats_chi->isChecked () ) {
      m_stats_number->setChecked ( true );
    }
  }

  m_stats_fparms->setEnabled ( enable );
  m_stats_chi->setEnabled ( enable );

  yes = index < 0;

  m_stats_number->setDisabled ( yes );
  m_stats_underflow->setDisabled ( yes );
  m_stats_overflow->setDisabled ( yes );
  m_stats_avg_x->setDisabled ( yes );
  m_stats_avg_y->setDisabled ( yes );
  m_stats_text->setDisabled ( yes );
  yes = m_stats_text -> isChecked ();
  m_statsTextField ->setEnabled ( yes );

  statsStripValue ( m_stats_number );
  statsStripValue ( m_stats_underflow );
  statsStripValue ( m_stats_overflow );
  statsStripValue ( m_stats_avg_x );
  statsStripValue ( m_stats_avg_y );

  if ( index >= 0 ) {
    int number = dcontroller -> getNumberOfEntries ( plotter, index );
    QString text = m_stats_number -> text ();
    QString str;
    str.setNum ( number );
    text += " ";
    text += str;
    m_stats_number -> setText ( text );

    int underflow = dcontroller -> getUnderflow ( plotter, index );
    text = m_stats_underflow -> text ();
    if ( underflow == -1 ) str = "meaningless";
    else str.setNum ( underflow );
    text += " ";
    text += str;
    m_stats_underflow -> setText ( text );

    int overflow = dcontroller -> getOverflow ( plotter, index );
    text = m_stats_overflow -> text ();
    if ( overflow == -1 ) str = "meaningless";
    else str.setNum ( overflow );
    text += " ";
    text += str;
    m_stats_overflow -> setText ( text );

    double average = dcontroller -> getAverage ( plotter, Axes::X, index );
    text = m_stats_avg_x -> text ();
    str.setNum ( average );
    text += " ";
    text += str;
    m_stats_avg_x -> setText ( text );

    average = dcontroller -> getAverage ( plotter, Axes::Y, index );
    text = m_stats_avg_y -> text ();
    str.setNum ( average );
    text += " ";
    text += str;
    m_stats_avg_y -> setText ( text );
  }

}

void
Inspector::
statsButtonGroupClicked ()
{
  bool yes = m_stats_text -> isChecked ();
  m_statsTextField -> setEnabled ( yes );
}

/** @todo This method should get the strings from the factory. */
void Inspector::
summaryNew ()
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;

  DisplayController * d_controller = DisplayController::instance ();
  int index = d_controller->activeDataRepIndex ( plotter );
  if ( index < 0 ) {
    multipleDataRepError ( tr ( "summary" ) );
    return;
  }

  CanvasWindow * canvas = WindowController::instance () ->currentCanvas();

  string nullstring ("");

  if ( m_stats_number->isChecked() )
    {
      const string s = tr("Total Entries").toAscii().data();
      canvas->addTextDisplay ( plotter, s, nullstring );
    }

  else if ( m_stats_underflow->isChecked() )
    {
      const string s = tr("Underflow").toAscii().data();
      canvas->addTextDisplay ( plotter, s, nullstring );
    }
  
  else if ( m_stats_overflow->isChecked() )
    {
      const string s = tr("Overflow").toAscii().data();
      canvas->addTextDisplay ( plotter, s, nullstring );
    }

  else if ( m_stats_avg_x->isChecked() )
    {
      const string s = tr("averagex").toAscii().data();
      canvas->addTextDisplay ( plotter, s, nullstring );
    }

  else if ( m_stats_avg_y->isChecked() )
    {
      const string s = tr("averagey").toAscii().data();
      canvas->addTextDisplay ( plotter, s, nullstring );
    }

  else if ( m_stats_fparms->isChecked() )
    {
      const string s = tr("Function Parameters").toAscii().data();
      FunctionController * controller = FunctionController::instance ();
      assert ( controller -> hasFunction ( plotter, 0 ) );
      canvas->addFuncDisplay ( plotter, s );

    }

  else if ( m_stats_chi->isChecked() )
    {
      const string s = tr("Chi-squared").toAscii().data();
      FunctionController * controller = FunctionController::instance ();
      assert ( controller -> hasFunction ( plotter, 0 ) );
      canvas->addFuncDisplay ( plotter, s );

    }

  else if ( m_stats_text->isChecked() )
  {

      QString qtext = m_statsTextField->text();
      const string t = qtext.toLatin1().data();
    bool needMargin = String::ci_find(t, "tex:")==0;  
    if ( needMargin ) {
#ifdef HAVE_TEX_UTILS
#else
        qtext.remove (0, 4);
        warningTex ();
#endif
    }
    string text (qtext.toLatin1().data());
    const string s = tr("Text From Box").toAscii().data();
    canvas->addTextDisplay ( plotter, s, text );
  }
}

/** Creates a display showing the residuals of the function.
 */
void Inspector::createResiduals()
{
  PlotterBase * plotter = getPlotter ();
  if ( plotter == 0 ) return;

  FunctionRep * func_rep = getTopFunctionRep ();
  FunctionController * controller = FunctionController::instance ();
  PlotterBase * res_plotter 
    = controller -> createResidualsDisplay ( plotter, func_rep );
  const Range & range = plotter -> getRange ( Axes::X, false );
  res_plotter -> setRange ( Axes::X, range, false );

  CanvasWindow * canvas = WindowController::instance () -> currentCanvas ();

  canvas -> addPlotDisplay ( res_plotter, true );
}

void Inspector::
fillCheckedFunctionRepItems ( )
{
  m_func_parm_checked.clear ();

  QTreeWidgetItemIterator it ( m_FunctionParamsListView );
  while ( *it ) {
    QTreeWidgetItem * item = *it;
    if ( item->flags() & Qt::ItemIsUserCheckable ) {
      if ( item -> checkState(Index) == Qt::Checked ) {
	m_func_parm_checked.push_back ( item );
      }
    }
    ++it;
  }
}

/** The slot that recieves the signal when the PushButtonNewErrorPlot
    is clicked. It plots a new error plot with the two parameters
    selected.
 */
void
Inspector::
pushButtonNewErrorPlotClicked()
{
  PlotterBase * plotter = getPlotter ();
  if ( plotter == 0 ) return;

  FunctionController * fcontroller = FunctionController::instance ();

  fillCheckedFunctionRepItems ();
  if ( m_func_parm_checked.size () != 2 ) {
    const QString message = tr(
			        "Two and only two function parameters should be\n"
				"checked to create error contour display.\n"
			       );
      QMessageBox::critical( this, // parent
			     tr("Invalid parameter pair selection"), // caption
			     message,
			     QMessageBox::Ok,
			     Qt::NoButton,
			     Qt::NoButton );
      return;
    }

  QTreeWidgetItem * first = m_func_parm_checked[0];
  QTreeWidgetItem * second = m_func_parm_checked[1];
  if ( getTopParent ( first ) != getTopParent ( second ) ) {
    const QString message = tr(
			        "Both checked function parameters must\n"
				"have same parent function."
			       );
    QMessageBox::critical( this, // parent
			   tr("Invalid parameter pair selection"), // caption
			   message,
			   QMessageBox::Ok,
			   Qt::NoButton,
			   Qt::NoButton );
    return;
  }
  QString text = first -> text ( Index );
  bool ok = true;
  int index = text.toInt ( & ok ) - 1;
  fcontroller -> setEllpsoidParamIndex ( Axes::X , index );

  text = second -> text ( Index );
  ok = true;
  index = text.toInt ( & ok ) -1 ;
  fcontroller -> setEllpsoidParamIndex ( Axes::Y , index );

  // Create / refresh the error plot
  const QString xlabel = first -> text ( Name );
  const QString ylabel = second -> text ( Name );
  QString stat = m_PushButtonNewErrorPlot -> text();
  QTreeWidgetItem * parent = getTopParent ( first );
  FunctionRep * frep = m_function_lv_map [ parent ];
  if( stat == QString( "Change Error Plot" ) )
    {
      fcontroller -> refreshEllipsoidDisplay ( plotter, frep );
      plotter -> setLabel( Axes::X, xlabel.toLatin1().data() );
      plotter -> setLabel( Axes::Y, ylabel.toLatin1().data() );
    }
  else // "New Error Plot"
    {
      PlotterBase * err_plotter =
	fcontroller -> createNewEllipsoidDisplay ( plotter, frep );
      assert( err_plotter != 0);

      err_plotter -> setLabel( Axes::X, xlabel.toLatin1().data() );
      err_plotter -> setLabel( Axes::Y, ylabel.toLatin1().data() );

      CanvasWindow * canvas
	= WindowController::instance () -> currentCanvas ();
      assert( canvas != 0 );

      // Add the plot to the display BUT donot select it. Let mother
      // plot be the one which is selected.
      canvas -> addPlotDisplay ( err_plotter, false );
    }
}

const std::string
Inspector::
convertToString ( hippodraw::Axes::Type axis )
{
  if ( axis == Axes::X ) return "X";
  else if ( axis == Axes::Y ) return "Y";
  else if ( axis == Axes::Z ) return "Z";

  return "nil";
}

void
Inspector::
updateLogBox ()
{
  bool yes = m_plotter_list.empty () == false;

  logScale -> setEnabled ( yes );
  if ( yes ) {
    PlotterBase * plotter = m_plotter_list.front ();
    bool log = DisplayController::instance () -> getLog ( plotter, m_axis );
    logScale -> setChecked ( log );
  }
}

void
Inspector::
updateAutoScaleBox ()
{
  bool yes = m_plotter_list.empty () == false;
  m_autoScale -> setEnabled ( yes );
  if ( yes ) {
    PlotterBase * plotter = m_plotter_list.front ();
    bool scaled = plotter -> isAutoRanging ( m_axis );
    m_autoScale -> setChecked ( scaled );
  }
}

void
Inspector::
updateReverseBox ()
{
  bool yes = ( m_plotter_list.empty () == false ) 
    && ( m_axis == Axes::X );
  m_reverse -> setEnabled ( yes );

  if ( yes ) {
    PlotterBase * plotter = m_plotter_list.front ();
    bool reversed = plotter -> isReverse ();
    m_reverse -> setChecked ( reversed );
  }
}

/** @bug When bin widths are the same, enables the width text box
    initially, but not after one toggles each data rep and returns to
    all active.  Try it on datareps.py

    @todo Offset slider and text changes behave wildly on when on
    log scale.  For now it is disabled.  Should try to fix it.*/
void
Inspector::
updateAxisTab ()
{
  updateLogBox ();
  updateAutoScaleBox ();
  updateReverseBox ();

  PlotterBase * plotter = getPlotter ();
  bool yes = plotter == 0;
  if ( yes == false ) {
    TextPlotter * text = dynamic_cast < TextPlotter * > ( plotter );
    yes |= text != 0;
  }
  m_axis_frame->setEnabled ( ! yes );

  if ( yes ) return;

  DisplayController * controller = DisplayController::instance ();
  int index = -1;
  if ( plotter -> isTargetable () ) {
    index = controller->activeDataRepIndex ( plotter );
  }

  bool has_ntuple = controller->hasNTupleBindings ( plotter, 0 );
  if ( has_ntuple )
    {
      const DataSource * nt
	= DisplayController::instance() -> getDataSource ( plotter, 0 );
      if ( nt && nt -> empty () ) return;
    }

  if ( plotter -> hasAxis ( m_axis ) == false ) setZRadioButton ( false );

  const string & label = plotter -> getLabel ( m_axis );
  const QString ltext = label.c_str();
  m_axis_label -> setText ( ltext );

  const string s_axis = convertToString ( m_axis );
  bool axis_bined = controller -> isAxisBinned ( plotter, s_axis );
  const Range & r = plotter->getRange(m_axis, true);
  double low = r.low();
  double high = r.high();

  axisWidget1 -> setLowText ( QString("%1").arg(low),
			      axis_bined == true &&
			      has_ntuple == false );
  axisWidget1 -> setHighText ( QString("%1").arg(high),
			       axis_bined == true &&
			       has_ntuple == false );

  axisWidget1 -> setLowSliderValue( 50 );
  m_lowslider1_last_val = 50;
  axisWidget1 -> setHighSliderValue( 50 );
  m_highslider1_last_val = 50;

  if (getMinEntries()==-1) {
    m_combine_checkbox->setEnabled( false );
    min_entries_text->setEnabled ( false );
    min_entries_slider->setEnabled ( false );
  }

  else {
    m_combine_checkbox->setEnabled( true );

    if ( m_combine_checkbox->isChecked() )
      { 
	min_entries_text->setEnabled ( true );
	min_entries_slider->setEnabled ( true );
	unsigned int min = getMinEntries();
	min_entries_text->setText( QString ("%1").arg(min) );
	min_entries_slider->setValue (50);
      }
    else
      {
	min_entries_text->setEnabled ( false );
	min_entries_slider->setEnabled ( false );
	unsigned int min = getMinEntries();
	min_entries_text->setText( QString ("%1").arg(min) );
	min_entries_slider->setValue ( 50 );
      }
  }




  if (  axis_bined == false )
    {
      m_width_text->setEnabled ( false );
      m_width_range->setEnabled ( false );
      m_width_text -> setText ( QString ("%1").arg (0) );

      m_offset_text->setEnabled ( false );
      m_offset_range->setEnabled ( false );
      m_offset_text -> setText ( QString ("%1").arg (0) );
    }
  else
    {
      m_width_text->setEnabled ( true );
      m_width_range->setEnabled ( true );
      double width = plotter->getBinWidth ( m_axis );
      m_width_text -> setText ( QString ("%1").arg (width) );

      m_offset_text->setEnabled ( true );
      m_offset_range->setEnabled ( true );
      double offset = plotter->getOffset ( m_axis );
      m_offset_text -> setText ( QString ("%1").arg (offset) );

      // make read only if not bound to ntuple
      m_width_text->setReadOnly ( ! has_ntuple );
      m_width_range->setEnabled ( has_ntuple );
      m_offset_text->setReadOnly ( ! has_ntuple );
      m_offset_range->setEnabled ( has_ntuple );
    }

  //----------------------------//
  // Handling the log check box //
  //----------------------------//
  bool disable_log = has_ntuple == false && axis_bined == true;
  logScale -> setDisabled ( disable_log );
  m_autoScale -> setDisabled ( disable_log );

  const PeriodicBinaryTransform *pbtf =
    dynamic_cast < const PeriodicBinaryTransform * >
    ( plotter->getTransform() );

  if( pbtf == 0   ) {
    bool log = DisplayController::instance() -> getLog ( plotter, m_axis );

    if( log )
      {
	m_offset_text -> setEnabled( false );
	m_offset_range -> setEnabled( false );
      }
  }


  axisWidget1->setAllDisabled ( false );

  //--------------------------------//
  // Handling of zoom pan check box //
  //--------------------------------//
  bool isZoomPan = false;

  std::map < const PlotterBase *, bool >::const_iterator it
    = m_zoompan.find ( plotter );
  if ( it != m_zoompan.end () )
    isZoomPan = it->second;

  // By default for periodic binary transforms (pbtf) zoom pan mode should
  // be set for both X and Y axis. For Z axis setting zoom pan mode does
  // not make sense.
  if( pbtf != 0 && m_axis != Axes::Z) {
    axisWidget1->setZoomPan ( true, true );
  }
  else {
    if ( m_axis == Axes::Z ) {
      axisWidget1 -> setZoomPan ( false, true ); //disables it
    }
    else {
      axisWidget1->setZoomPan ( isZoomPan );
    }
  }

  axisWidget1->processZoomPanCheckBoxClicked ( r, r );

  yes = false;
  if ( index >= 0 ) {
    const DataRep * datarep = plotter -> getDataRep ( index );
    RepBase * rep = datarep -> getRepresentation ();
    ContourPointRep * contourRep = dynamic_cast < ContourPointRep * > ( rep );
    yes = contourRep != 0;
    if ( yes ) {
      bool user = contourRep->getUsingUserValues();
      contourRadioButton1->setChecked ( !user );
      contourRadioButton2->setChecked ( user );
      contourRadioButton1_toggled ( true );
    }
  }

  m_contourBox->setEnabled ( yes );
}


/** Slot which recieves the signal if Current item changes in the List View
    of the function Params.
*/
void Inspector::functionParamsListViewCurrentChanged ()
{
  QTreeWidgetItem * item = m_FunctionParamsListView -> currentItem ();
  bool is_selected = item->isSelected();
  bool is_parm = (item -> childCount () == 0);
  bool enable = is_selected && is_parm;

  m_FunctionParamsCheckBox -> setEnabled ( enable );
  m_FunctionParamsLineEdit -> setEnabled ( enable );
  m_FunctionParamsSlider -> setEnabled ( enable );

  if ( enable ) {
    QString fixedFlag = item -> text( Fixed );
    m_FunctionParamsCheckBox ->
      setChecked(fixedFlag == qyes);
    m_FunctionParamsLineEdit -> setText ( item -> text( Value ) );
  }

  enable = is_selected && (! is_parm);
  functionsRemoveButton -> setEnabled ( enable );

}


QTreeWidgetItem *
Inspector::
getTopParent ( QTreeWidgetItem * item ) 
{
  QTreeWidgetItem * parent = item;
  while ( parent -> parent() )
    parent = parent -> parent();
  return  parent;
}

/** Slot which recieves the signal if state of the checkbox changes in the
    the function Params tab. */
void
Inspector::
functionParamsCheckBoxToggled( bool )
{
  PlotterBase * plotter = getPlotter();
  if ( !plotter ) return;

  FunctionController * fcontroller = FunctionController::instance();
  if ( ! ( fcontroller -> hasFunction ( plotter, 0 ) ) ) {
    return;
  }

  fcontroller -> saveParameters ( plotter );

  QTreeWidgetItem * item = m_FunctionParamsListView -> currentItem();
  if( !item ) return;

  FunctionRep * frep = getTopFunctionRep ( item );

  vector < int > fixed = frep -> getFixedFlags ();

  QString pidx = item -> text( Index ); // As a hack we hide in the 5th place
  int paramindex = pidx.toUInt() - 1;   // the index of the parameter

  // Set the new fixed flag for the function
  bool flag = m_FunctionParamsCheckBox -> isChecked();

  fixed[ paramindex ] = flag == true ? 1 : 0;
  frep -> setFixedFlags( fixed );

  if ( item -> childCount () == 0 ) {
    QString fixedFlag = ( flag == true ) ? qyes : qno;
    item -> setText ( Fixed, fixedFlag );
  }

}


/** Slot which recieves the signal if enter is pressed in the line edit in the
    the function Params tab.
*/
void Inspector::functionParamsLineEditReturnPressed()
{
  // Check if there is plotter.
  PlotterBase * plotter = getPlotter();
  if ( !plotter ) return;

  // Check if there is a function attached to this plotter.
  FunctionController * fcontroller = FunctionController::instance();
  if ( ! ( fcontroller -> hasFunction ( plotter, 0 ) ) )
    return;

  // Save old parameters
  fcontroller -> saveParameters ( plotter );

  // Get the current item and item-number
  QTreeWidgetItem * item = m_FunctionParamsListView -> currentItem();
  if( !item ) return;

  QString pidx = item -> text( Index );
  int paramindex = pidx.toUInt() -1;   // the index of the parameter

  FunctionRep * frep = getTopFunctionRep ( item );
  vector < double >  parameters = frep-> parameters();

  // Set the new fixed flag for the function
  QString text = m_FunctionParamsLineEdit -> text();
  parameters[ paramindex ] = text.toDouble();
  frep  -> setParameters( parameters );
  frep  -> setDirty();

  // Change the new parameter in ListView
  item -> setText ( Value, QString ( "%1" ).arg ( parameters[ paramindex ] ) );
}


/** Slot which recieves signal when params slider is presssed. It basically
   stores the old values of parameters.
*/
void
Inspector::
functionParamsSliderSliderPressed()
{
  PlotterBase * plotter = getPlotter();
  if ( !plotter ) return;

  FunctionController * fcontroller = FunctionController::instance();
  if ( ! ( fcontroller -> hasFunction ( plotter, 0 ) ) )
    return;

  // Save old parameters
  fcontroller -> saveParameters ( plotter );

  FunctionRep * frep = getTopFunctionRep ( );

  if ( frep != 0 ) {
    m_oldParameters = frep -> parameters ();
  }
}


/** Slot which alters the parameter values as the function Params slider is
    moved.
*/
void
Inspector::
functionParamsSliderSliderMoved( int )
{
  PlotterBase * plotter = getPlotter();
  if ( !plotter ) return;

  FunctionController * fcontroller = FunctionController::instance();
  if ( ! ( fcontroller -> hasFunction ( plotter, 0 ) ) )
    return;

  QTreeWidgetItem * item = m_FunctionParamsListView -> currentItem();

  if( !item ) return;

  QString pidx = item -> text( Index );
  int paramindex = pidx.toUInt() - 1;   // the index of the parameter

  vector < double > newParameters = m_oldParameters;

  int sliderValue = m_FunctionParamsSlider -> value();
  int sign = ( m_oldParameters[ paramindex ] < 0 )? -1:1;

  newParameters[ paramindex ]
    = m_oldParameters[ paramindex ] *
    pow ( 2.0,  static_cast<double>( (sliderValue - 50) * sign) / 50.0 );

  FunctionRep * frep   = getTopFunctionRep ( item );
  frep -> setParameters( newParameters ); // will set projector dirty

  item -> setText( Value, QString ( "%1" ).arg ( newParameters[ paramindex ]));
  m_FunctionParamsLineEdit ->
    setText ( QString ( "%1" ).arg ( newParameters[ paramindex ] ) );
}

/** Slot which set the slider to mid point after it has been release.
 */
void
Inspector::
functionParamsSliderSliderReleased()
{
  m_FunctionParamsSlider -> setValue(50);
}

void 
Inspector::
invalidOperationError ( const string & message )
{
  QMessageBox::critical ( this,  // parent )
			  tr("Operation error"), // caption
			  message.c_str(), // message
			  QMessageBox::Ok,
			  Qt::NoButton,
			  Qt::NoButton );
}

/** The Qt slot which responds to the toggle in state of the logScale
    checkbox.
*/
void Inspector::logScale_clicked()
{
  bool log = logScale -> isChecked();
  bool auto_scale = m_autoScale -> isChecked ();

  vector < PlotterBase * > ::iterator first = m_plotter_list.begin ();

  while ( first != m_plotter_list.end () ) {
    PlotterBase * plotter = *first++;
    try {
      DisplayController::instance()-> setLog ( plotter, m_axis, log );
      plotter -> setAutoRanging ( m_axis, auto_scale );
    }
    catch ( const runtime_error & e ) {
      invalidOperationError ( e.what () );
    }
  }

  updateAxisTab ();
}

void Inspector::reverse_clicked()
{
  bool reverse = m_reverse -> isChecked();

  vector < PlotterBase * > ::iterator first = m_plotter_list.begin ();

  while ( first != m_plotter_list.end () ) {
    PlotterBase * plotter = *first++;
    plotter -> setReverse ( reverse );
  }

  updateAxisTab ();
}

void Inspector::autoScale_clicked()
{
  bool scale = m_autoScale -> isChecked ();
  vector < PlotterBase * > ::iterator first = m_plotter_list.begin ();

  while ( first != m_plotter_list.end () ) {
    PlotterBase * plotter = *first++;
    plotter -> setAutoRanging ( m_axis, scale );

    // If the transform be periodic it sets both the offsets to be 0.0
    PeriodicBinaryTransform * tp =
      dynamic_cast< PeriodicBinaryTransform* > ( plotter->getTransform() );
    if ( tp != 0 )
      {
	tp->setRotation( 0.0, Axes::X );
	tp->setRotation( 0.0, Axes::Y );
      }
  }

  updateAxisTab();
}

void Inspector::cutHighSlider_sliderMoved ( int value )
{
  if ( m_is_updating == false ) {
    int index = m_selCutComboBox -> currentIndex ();
    Range currentRange = m_tuple_cuts [ index] -> getRange ();
    int id = cutRadioId ();
    bool fit_cut = id == 2;

    if ( fit_cut == false ) { //data cut
      Axes::Type axis = getAxes ( index );
      PlotterBase * plotter = getSelectedCut();
      const Range & fullRange = plotter -> getRange ( axis, false );
      axisWidget2 -> processHighSliderMoved ( value, currentRange, fullRange );
      plotter -> setCutRangeAt ( currentRange, axis );
    }
    else { // fit cut
      PlotterBase * plotter = getPlotter ();
      const Range & fullRange = plotter -> getRange ( Axes::X, false );
      axisWidget2 -> processHighSliderMoved ( value, currentRange, fullRange );
      plotter -> setCutRangeAt ( currentRange, index );
    }
  }
}

void Inspector::cutLowSlider_sliderMoved ( int value )
{
  if ( m_is_updating == false ) {
    int index = m_selCutComboBox -> currentIndex ();
    Range currentRange = m_tuple_cuts [ index ] -> getRange ();

    bool fit_cut = cutRadioId () == 2;

    if ( fit_cut == false ) { //data cut
      Axes::Type axis = getAxes ( index );
      PlotterBase * plotter = getSelectedCut();
      const Range & fullRange = plotter -> getRange ( axis, false );
      axisWidget2 -> processLowSliderMoved ( value, currentRange, fullRange );
      plotter -> setCutRangeAt ( currentRange, axis );
    }
    else { // fit cut
      PlotterBase * plotter = getPlotter ();
      const Range & fullRange = plotter -> getRange ( Axes::X, false );
      axisWidget2 -> processLowSliderMoved ( value, currentRange, fullRange );
      plotter -> setCutRangeAt ( currentRange, index );
    }
  }
}

void Inspector::cutLowSlider_sliderReleased()
{
  PlotterBase * cd = getSelectedCut();
  if ( cd == 0 ) return;
  int index = m_selCutComboBox -> currentIndex ();
  Axes::Type axis = getAxes ( index );
  const Range & fullRange = cd->getRange ( axis, false );
  axisWidget2->processLowSliderReleased ( fullRange );
}

void Inspector::cutHighSlider_sliderReleased()
{
  PlotterBase * cd = getSelectedCut();
  if ( cd == 0 ) return;
  int index = m_selCutComboBox -> currentIndex ();
  Axes::Type axis = getAxes ( index );
  const Range & fullRange = cd->getRange ( axis, false );
  axisWidget2->processHighSliderReleased ( fullRange );
}

void Inspector::cutZoomPanCheckBox_clicked()
{
  PlotterBase * plotter = getSelectedCut();

  int index = m_selCutComboBox -> currentIndex ();
  int id = cutRadioId ();
  bool fit_cut = id ==2;

  Axes::Type axis = Axes::X;
  if ( fit_cut == false ) {
    axis = getAxes ( index );
  }
  bool yes = axisWidget2 -> isZoomPanChecked ();
  CutController * controller = CutController::instance ();
  controller -> setZoomPan ( plotter, axis, yes );

  Range currentRange = m_tuple_cuts [ index ] -> getRange ();
  const Range & fullRange = plotter -> getRange ( axis, false );

  axisWidget2 -> processZoomPanCheckBoxClicked ( currentRange, fullRange ) ;
}


void Inspector::cutInvertPushButton_clicked()
{
  int id = cutRadioId ();
  bool fit_cut = id == 2;

  if ( fit_cut == false ) { //data cut
    PlotterBase * plotter = getSelectedCut ();
    CutPlotter * cp = dynamic_cast < CutPlotter * > ( plotter );
    cp -> toggleInverted ();
  }
  else {
    int index = m_selCutComboBox -> currentIndex ();
    bool state = ! m_tuple_cuts [ index ] -> getInversion ();
    PlotterBase * plotter = getPlotter ();
    XyPlotter * xyplotter = dynamic_cast < XyPlotter * > ( plotter );
    xyplotter -> setCutInverted ( index, state );
  }
}

void
Inspector::
cutEnablePushButton_toggled ( bool on)
{
  if ( m_cut_enable_updating == true ) return;
  
  bool fit_cut = cutRadioId () == 2;

  if ( fit_cut == false ) { //data cut
    PlotterBase * plotter = getSelectedCut ();
    CutPlotter * cut_plotter = dynamic_cast < CutPlotter * > ( plotter );
    cut_plotter -> setEnabled ( ! on );
  }
  else {
    int index = m_selCutComboBox -> currentIndex ();
    PlotterBase * plotter = getPlotter ();
    XyPlotter * xyplotter = dynamic_cast < XyPlotter * > ( plotter );
    xyplotter -> setCutEnabled ( index, ! on );
  }
}

void
Inspector::
colorSelect_2_clicked()
{
  int id = cutRadioId ();
  bool fit_cut = id == 2;
  if ( fit_cut ) {
  }
  else {
    PlotterBase * cplotter = getSelectedCut();
    CutPlotter * cp = dynamic_cast < CutPlotter * > ( cplotter );

    const Color & rep_color = cp -> getCutColor ();
    QColor color ( rep_color.getRed(),
		   rep_color.getGreen(),
		   rep_color.getBlue () );

    color = QColorDialog::getColor ( color );

    if ( color.isValid() == true ) {
      Color c( color.red(), color.green(), color.blue() );
      cp -> setCutColor ( c );
    }
  }
}

void
Inspector::
contourSlider_valueChanged ( int val )
{

  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;
  DisplayController * controller = DisplayController::instance ();
  int index = controller->activeDataRepIndex ( plotter );
  DataRep * datarep = plotter->getDataRep ( index );

  RepBase * rep = datarep->getRepresentation();
  ContourPointRep * contourRep = dynamic_cast < ContourPointRep * > ( rep );

  if ( !contourRep ) return;

  contourRep->setUsingUserValues ( false );
  contourRep->setNumContours ( val );
  m_numContoursTextBox->setText ( QString("%1").arg ( val ) );

  datarep->notifyObservers();

}

void
Inspector::
contourTextBox_returnPressed()
{

  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;
  DisplayController * controller = DisplayController::instance ();
  int index = controller->activeDataRepIndex ( plotter );
  DataRep * datarep = plotter->getDataRep ( index );

  RepBase * rep = datarep->getRepresentation();
  ContourPointRep * contourRep = dynamic_cast < ContourPointRep * > ( rep );

  if ( !contourRep ) return;

  QString text = m_numContoursTextBox->text();
  int val = text.toInt();

  if ( val < 1 || val > 100 ) {
    int num = contourRep->getNumContours ();
    m_numContourSlider->setValue ( num );
    m_numContoursTextBox->setText ( QString ("%1").arg ( num ) );
    return;
  }

  contourRep->setUsingUserValues ( false );
  contourRep->setNumContours ( val );
  m_numContourSlider->setValue ( val );

  datarep->notifyObservers();

}

void
Inspector::
contourRadioButton1_toggled ( bool )
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;
  if ( plotter -> isTargetable () == false ) return;

  DataRep * datarep = plotter -> getTarget ( );
  RepBase * rep = datarep->getRepresentation();

  ContourPointRep * contourRep = dynamic_cast < ContourPointRep * > ( rep );

  if ( !contourRep ) return;

  if ( contourRadioButton1->isChecked() ) {

    m_numContourSlider->setEnabled ( true );
    m_numContoursTextBox->setEnabled ( true );
    m_numContoursLabel->setEnabled ( true );
    m_contourLevelsTextBox->setEnabled ( false );

    int num = contourRep->getNumContours ();
    m_numContourSlider->setValue ( num );
    m_numContoursTextBox->setText ( QString ("%1").arg ( num ) );

    contourSlider_valueChanged ( num );

  }

  else {

    m_numContourSlider->setEnabled ( false );
    m_numContoursTextBox->setEnabled ( false );
    m_numContoursLabel->setEnabled ( false );
    m_contourLevelsTextBox->setEnabled ( true );

    contourLevelsTextBox_returnPressed();

  }

}

void
Inspector::
contourError ()
{
//   const QString message =
//     "Invalid Input String. Please check that\n"
//     "1. The string contains only numbers separated by white spaces, and,\n"
//     "2. The numbers are in increasing order without any duplicates.\n";
  const QString message = tr(
			      "Invalid Input String.\n"
			      "Please check that the string contains only numbers,\n"
			      "separated by commas or white space.\n"
			     );
  QMessageBox::critical ( this, // parent
			  tr("Invalid Input String"), // caption
			  message,
			  QMessageBox::Ok,
			  Qt::NoButton,
			  Qt::NoButton );
}

void
Inspector::
contourLevelsTextBox_returnPressed ()
{
  if ( contourRadioButton2->isChecked () == false ) return;

  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;
  DisplayController * controller = DisplayController::instance ();
  int index = controller->activeDataRepIndex ( plotter );
  DataRep * datarep = plotter->getDataRep ( index );

  RepBase * rep = datarep->getRepresentation();
  ContourPointRep * contourRep = dynamic_cast < ContourPointRep * > ( rep );

  if ( !contourRep ) return;

  const QString qstr1 = m_contourLevelsTextBox->text();
  if ( qstr1.isEmpty () ) return;
  
  const QString qstr2 = qstr1.simplified();

  vector < double > values;

// Get a std::string from the QString.
  std::string contourLevels(qstr2.toAscii());
   
// Break string into components; convert and validate each value.
  std::vector<std::string> tokens;
  stringTokenize(contourLevels, " \t,", tokens);
  for (size_t i = 0; i < tokens.size(); i++) {
     QString strval(tokens.at(i).c_str() );
     bool ok(true);
     double value(strval.toDouble(&ok));
     if (!ok) {
        contourError();
        return;
     }
     values.push_back(value);
  }
  
// Sort and remove duplicates.
  std::stable_sort(values.begin(), values.end());
  vector<double>::iterator leftover = 
     std::unique(values.begin(), values.end());
  values.erase(leftover, values.end());

  contourRep->setContourValues ( values, datarep->getProjector() );
  datarep->notifyObservers();
}

void Inspector::editLabelFontClicked()
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return ;

  QFont labelFont;
  bool ok;

  XyPlotter * xyplotter = dynamic_cast < XyPlotter * > ( plotter );
  assert ( xyplotter != 0 );

  const FontBase * fb = xyplotter -> labelFont ( m_axis );
  if ( fb == 0 ) {
    labelFont = QFontDialog::getFont ( &ok, this);
  }
  else {
    const QtFont * qtfont = dynamic_cast < const QtFont * > ( fb );
    const QFont & qfont = qtfont -> font ();
    labelFont = QFontDialog::getFont ( &ok, qfont, this);
  }

  if ( ok )
    {
      QtFont * font = new QtFont;
      font -> setFont( labelFont );
      xyplotter -> setLabelFont( font, m_axis );

      /* The size of drawrect, marginrect, need to be updated
	 according to new font.
      */
      xyplotter -> setNeedUpdate(true);
      xyplotter -> notifyObservers ();
    }
}

void Inspector::editTitleFontClicked()
{
  PlotterBase * plotter = getPlotter ();
  if ( plotter == 0 ) return ;

  XyPlotter * xyplotter = dynamic_cast< XyPlotter* > ( plotter );
  assert( xyplotter != 0 );

  QFont titleFont;
  bool ok;

  const FontBase * fb = xyplotter -> titleFont ();
  if ( fb == 0 ) {
    // From the Qt documentation - "The usual way to use QFontDialog class is
    // to call one of the static convenience functions"
    titleFont = QFontDialog::getFont ( &ok, this );
  }
  else {
    const QtFont * qtfont = dynamic_cast < const QtFont * > ( fb );
    const QFont & qfont = qtfont -> font ();
    titleFont = QFontDialog::getFont ( &ok, qfont, this );
  }

  if ( ok )
    {
      QtFont * font = new QtFont;
      font -> setFont( titleFont );
      xyplotter -> setTitleFont( font );

      /* The size of drawrect, marginrect, need to be updated
	 according to new font.
      */
      xyplotter -> setNeedUpdate(true);
      xyplotter -> notifyObservers ();
    }
}


void Inspector::ignoreErrorCheckBoxToggled( bool )
{
  // Check if there is plotter.
  PlotterBase * plotter = getPlotter();
  if ( !plotter ) return;

  // Check if there is a function attached to this plotter.
  FunctionController * fcontroller = FunctionController::instance();
  if ( ! ( fcontroller -> hasFunction ( plotter, 0 ) ) ) {
    return;
  }

  FunctionRep * frep = getTopFunctionRep ();
  Fitter * fitter = frep -> getFitter ();

  // Set the new ignoreError flag for the function
  bool flag = m_IgnoreErrorCheckBox -> isChecked();
  if ( fitter != 0 ) {
    frep -> setIgnoreError ( flag );
  }
}

void
Inspector::
updateTransformTab()
{

  // Disable the tab if no plotter selected.
  PlotterBase * plotter = getPlotter ();
  bool yes = plotter == 0;
  if ( yes == false ) {
    TextPlotter * tp = dynamic_cast < TextPlotter * > ( plotter );
    yes |= tp != 0;
  }
  if ( yes ) {
    transformsComboBox -> setEnabled ( false );
    periodicGroupBox -> setEnabled ( false );
    return;
  }
  else {
    transformsComboBox -> setEnabled ( true );
  }
 
  bool hasZ = plotter -> hasAxis ( Axes::Z );

  TransformFactory * transforms = TransformFactory::instance();
  transformsComboBox -> clear();
  transformsComboBox -> addItem ( tr("X - Y") );
  transformsComboBox -> addItem ( tr("X - Log Y") );
  transformsComboBox -> addItem ( tr("Log X - Y") );
  transformsComboBox -> addItem ( tr("Log X - Log Y") );

  const vector < string > & transformNames = transforms -> names();
  for ( unsigned int i = 0; i < transformNames.size(); i++ ) {
    if (transformNames[i] == "Linear" || 
      transformNames[i] == "Linear Linear Linear" ||
      transformNames[i] == "Log") continue;

    QString name ( transformNames[i].c_str() );
    transformsComboBox -> addItem ( name );
  }

  /* Add a separator between simple log scaling and more complicated things */

  if (transformsComboBox->count() > 4)
	  transformsComboBox -> insertSeparator(4);
  
  DisplayController *d_controller = DisplayController::instance();

  bool xlog = d_controller -> getLog ( plotter, Axes::X );
  bool ylog = d_controller -> getLog ( plotter, Axes::Y );

  std::string cur_transform = plotter->getTransform()->name();

  if (cur_transform == "Linear Linear Linear" ||
      cur_transform == "Linear Linear Log") cur_transform = tr("X - Y").toAscii().data();
  else if (cur_transform == "Linear Log Linear" ||
	   cur_transform == "Linear Log Log") cur_transform = tr("X - Log Y").toAscii().data();
  else if (cur_transform == "Log Linear Linear" || 
	   cur_transform == "Log Linear Log") cur_transform = tr("Log X - Y").toAscii().data();
  else if (cur_transform == "Log Log Linear" ||
	   cur_transform == "Log Log Log") cur_transform = tr("Log X - Log Y").toAscii().data();

  transformsComboBox->setCurrentIndex(
      transformsComboBox->findText(QString::fromStdString(cur_transform))
  );

  BinaryTransform *t =
      dynamic_cast<  BinaryTransform* > ( plotter->getTransform() );
   
  if (t != NULL) {
      // Enable rotation if periodic transform.
      // Then set the rotation offset.
      if  (t->isPeriodic()) {

	periodicGroupBox -> setEnabled ( true );
	
	PeriodicBinaryTransform *tp =
	  dynamic_cast< PeriodicBinaryTransform* > ( t );

	int yoffset = (int) (tp->rotation(Axes::X)/UNITS_RADIANS
		* tp->units(Axes::Y));
	int xoffset = (int) (tp->rotation(Axes::Y)/UNITS_RADIANS
		* tp->units(Axes::X));

	setRotate ( xoffset, yoffset);

	if (tp->units(Axes::X) == UNITS_DEGREES)
		xAxisPeriodicUnits->setCurrentIndex(0);
	else if (tp->units(Axes::X) == UNITS_RADIANS)
		xAxisPeriodicUnits->setCurrentIndex(1);
	else if (tp->units(Axes::X) == UNITS_HOURS)
		xAxisPeriodicUnits->setCurrentIndex(2);

	if (tp->units(Axes::Y) == UNITS_DEGREES)
		yAxisPeriodicUnits->setCurrentIndex(0);
	else if (tp->units(Axes::Y) == UNITS_RADIANS)
		yAxisPeriodicUnits->setCurrentIndex(1);

	if (tp->limitX().high() + tp->limitX().low() == 0)
		xPeriodicZeroCenter->setChecked(true);
	else
		xPeriodicZeroLeft->setChecked(true);

	if (tp->limitY().high() + tp->limitY().low() == 0)
		yPeriodicZeroCenter->setChecked(true);
	else
		yPeriodicZeroLeft->setChecked(true);

	transformPeriodicUnitUpdate();
      }
      else {
	setRotate ( 0, 0 );
	periodicGroupBox -> setEnabled ( false );
      }
  }
}
    
void
Inspector::
transform_changed (const QString &transform)
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;

  // Reset rotation offset when do another transform
  setRotate ( 0, 0 );

  DisplayController *d_controller = DisplayController::instance();
  try {
    if (transform == "X - Y") {
      d_controller -> setTransformAxis ( plotter, "Linear", "Linear" );
      d_controller -> setTransform ( plotter, "Linear Linear" );
      periodicGroupBox -> setEnabled ( false );
    } else if (transform == "X - Log Y") {
      d_controller -> setTransformAxis ( plotter, "Linear", "Log" );
      d_controller -> setTransform ( plotter, "Linear Log");
      periodicGroupBox -> setEnabled ( false );
    } else if (transform == "Log X - Y") {
      d_controller -> setTransformAxis ( plotter, "Log", "Linear" );
      d_controller -> setTransform ( plotter, "Log Linear" );
      periodicGroupBox -> setEnabled ( false );
    } else if (transform == "Log X - Log Y") {
      d_controller -> setTransformAxis ( plotter, "Log", "Log" );
      d_controller -> setTransform ( plotter, "Log Log" );
      periodicGroupBox -> setEnabled ( false );
    } else {
	d_controller -> setTransformAxis ( plotter, "Linear", "Linear" );
	d_controller -> setTransform ( plotter, transform.toStdString() );
	BinaryTransform *t =
	    dynamic_cast<  BinaryTransform* > ( plotter->getTransform() );
	if (t == NULL)
		periodicGroupBox -> setEnabled(false);
	else
		periodicGroupBox -> setEnabled(t->isPeriodic());
	transformPeriodicUnitUpdate();
	plotter->update();
    }
  }
  catch ( const std::runtime_error & e ) {
    invalidOperationError ( e.what() );
  }
}

void
Inspector::
transformPeriodicUnitChange()
{
  PlotterBase * plotter = getPlotter ();
  transformPeriodicUnitUpdate();

  plotter->update();
}

void
Inspector::
transformPeriodicUnitUpdate()
{
  PlotterBase * plotter = getPlotter ();
  PeriodicBinaryTransform *t =
	    dynamic_cast<PeriodicBinaryTransform*> ( plotter->getTransform() );

  int index;

  index = xAxisPeriodicUnits->currentIndex(); 
  switch(index) {
	case 0:
	  xPeriodicZeroCenter->setText("(-180,180)");
	  xPeriodicZeroLeft->setText("(0,360)");

	  t->setUnits(UNITS_DEGREES, Axes::X);
	  if (xPeriodicZeroCenter->isChecked())
		t->setLimitX(Range(-180,180));
	  else
		t->setLimitX(Range(0,360));
	  break;
	case 1:
	  xPeriodicZeroCenter->setText("(-Pi,Pi)");
	  xPeriodicZeroLeft->setText("(0,2Pi)");

	  t->setUnits(UNITS_RADIANS, Axes::X);
	  if (xPeriodicZeroCenter->isChecked())
		t->setLimitX(Range(-M_PI,M_PI));
	  else
		t->setLimitX(Range(0,2*M_PI));
	  break;
	case 2:
	  xPeriodicZeroCenter->setText("(-12,12)");
	  xPeriodicZeroLeft->setText("(0,24)");

	  t->setUnits(UNITS_HOURS, Axes::X);
	  if (xPeriodicZeroCenter->isChecked())
		t->setLimitX(Range(-12,12));
	  else
		t->setLimitX(Range(0,24));
	  break;
  }
	
  index = yAxisPeriodicUnits->currentIndex(); 
  switch(index) {
	case 0:
	  yPeriodicZeroCenter->setText("(-90,90)");
	  yPeriodicZeroLeft->setText("(0,180)");

	  t->setUnits(UNITS_DEGREES, Axes::Y);
	  if (yPeriodicZeroCenter->isChecked())
		t->setLimitY(Range(-90,90));
	  else
		t->setLimitY(Range(0,180));
	  break;
	case 1:
	  yPeriodicZeroCenter->setText("(-Pi/2,Pi/2)");
	  yPeriodicZeroLeft->setText("(0,Pi)");

	  t->setUnits(UNITS_RADIANS, Axes::Y);
	  if (yPeriodicZeroCenter->isChecked())
		t->setLimitY(Range(-M_PI/2,M_PI/2));
	  else
		t->setLimitY(Range(0,M_PI));
	  break;
  }
}

void
Inspector::
rotateX( int offset )
{
  if (!m_rotate_enable) return;

  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;

  plotter->setAutoRanging ( Axes::X, false );

  PeriodicBinaryTransform *tp =
	dynamic_cast< PeriodicBinaryTransform* > ( plotter->getTransform() );

  assert(tp != NULL);

  const Range & r = plotter->getRange ( Axes::X, true );
  Range range (r.low(), r.high(), r.pos());

  // Actually rotating about Y axis
  tp->setRotation( double(offset)/UNITS_DEGREES * UNITS_RADIANS, Axes::Y );

  plotter->setRange ( Axes::X, range, true, false );
  m_x_offset_text->setText ( 
      QString("%1").arg(double(offset)/UNITS_DEGREES *tp->units(Axes::X))
  );
}


void
Inspector::
rotateY( int offset )
{
  if (!m_rotate_enable) return;

  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;

  plotter->setAutoRanging ( Axes::Y, false );

  PeriodicBinaryTransform *tp =
	dynamic_cast< PeriodicBinaryTransform* > ( plotter->getTransform() );

  assert(tp != NULL);

  const Range & r = plotter->getRange ( Axes::Y, true );
  Range range (r.low(), r.high(), r.pos());

  // Actually rotating Y axes
  tp->setRotation( double(offset)/UNITS_DEGREES * UNITS_RADIANS, Axes::X );

  plotter->setRange ( Axes::Y, range, true, false );

  m_y_offset -> setValue ( offset );
  m_y_offset_text->setText ( 
      QString("%1").arg(double(offset)/UNITS_DEGREES * tp->units(Axes::Y))
  );
}

void 
Inspector::
setRotate( double x, double y )
{
  m_rotate_enable = false;
 
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;
  PeriodicBinaryTransform *tp =
	dynamic_cast< PeriodicBinaryTransform* > ( plotter->getTransform() );

  if (tp != NULL) {
	m_x_offset -> setValue ( x/tp->units(Axes::X) * UNITS_DEGREES );
	m_y_offset -> setValue ( y/tp->units(Axes::Y) * UNITS_DEGREES );
  } else {
	m_x_offset -> setValue ( 0 );
	m_y_offset -> setValue ( 0 );
  }
  m_x_offset_text->setText ( QString("%1").arg(double(x)) );
  m_y_offset_text->setText ( QString("%1").arg(double(y)) );
  m_rotate_enable = true;
}

void
Inspector::
resetRotate()
{
  rotateX(0);
  rotateY(0);
  setRotate(0, 0);
}

void
Inspector::
m_grid_clicked()
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;

  plotter->setShowGrid ( m_grid -> isChecked() );
}

void
Inspector::
m_boxedge_clicked()
{
  PlotterBase * plotter = getPlotter();
  if ( !plotter ) return;
  plotter->setBoxEdge( m_boxedge->isChecked() );
}


void
Inspector::
combineCheckBox_clicked()
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;

  if (m_combine_checkbox->isChecked()) {
    plotter->setMinEntries(m_min_entries);
  }
  else plotter->setMinEntries(0);

  updateAxisTab();
}

void
Inspector::
setMinEntries( int increment )
{ 
  int minEntries = m_min_entries + increment - 50;
  if ( minEntries < 0 ) minEntries = 0;

  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;

  if (!m_combine_checkbox->isChecked()) return;

  plotter->setMinEntries(minEntries);
  min_entries_text->setText(QString("%1").arg(minEntries));

  if (m_dragging == false ) {
    m_min_entries = minEntries;
    min_entries_slider->setValue(50);
  }
}

int
Inspector::
getMinEntries()
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return -1;
  
  return plotter->getMinEntries ();
}

void
Inspector::
setMinEntriesText()
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;
  
  int min = min_entries_text->text().toInt();
  plotter->setMinEntries(min);
  updateAxisTab();
}


void
Inspector::
setXRotateText()
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;
  
  if (!m_rotate_enable) return;

  plotter->setAutoRanging ( Axes::X, false );
  
  PeriodicBinaryTransform *tp =
    dynamic_cast< PeriodicBinaryTransform* > ( plotter->getTransform() );

  double offset = m_x_offset_text->text().toDouble()
		  /tp->units(Axes::X)*UNITS_DEGREES;
  
  const Range & r = plotter->getRange ( Axes::X, true );
  Range range (r.low(), r.high(), r.pos());

  // Rotate about Y axis
  tp->setRotation( offset/UNITS_DEGREES * UNITS_RADIANS, Axes::Y );
  
  plotter->setRange ( Axes::X, range, true, false );

  m_x_offset->setValue(offset);

  updateAxisTab();
}

void
Inspector::
setYRotateText()
{
  PlotterBase * plotter = getPlotter ();
  if ( !plotter ) return;
  
  if (!m_rotate_enable) return;

  plotter->setAutoRanging ( Axes::Y, false );
  
  PeriodicBinaryTransform *tp =
    dynamic_cast< PeriodicBinaryTransform* > ( plotter->getTransform() );
  
  double offset = m_y_offset_text->text().toDouble()
		  /tp->units(Axes::Y)*UNITS_DEGREES;

  const Range & r = plotter->getRange ( Axes::Y, true );
  Range range (r.low(), r.high(), r.pos());
  
  // Rotating about X axis
  tp->setRotation( offset/UNITS_DEGREES * UNITS_RADIANS, Axes::X );
  
  plotter->setRange ( Axes::Y, range, true, false );
  m_y_offset->setValue(offset);

  updateAxisTab();
}

/** Creates a display showing the difference of the two histogram
 */
void
Inspector::
diffDataRep()
{
  PlotterBase * plotter = getPlotter ();

  if ( !plotter ) return;

  int num_rep = plotter -> getNumDataReps();

  // If number of datarep is not equal to 2
  // in the plot, show warning message and do nothing.
  if ( num_rep != 2 )
    {
      const QString message = tr("You must have two DataReps in this view.");

      QMessageBox::warning ( this, // parent
                             tr("Unable to compare DataRep"), // caption
                             message,
                             QMessageBox::Ok,
                             Qt::NoButton,
                             Qt::NoButton );
      return;
    }

  
  FunctionRep * func_rep = getTopFunctionRep ();
  DisplayController * controller = DisplayController::instance ();
  PlotterBase * res_plotter 
    //= controller -> createResidualsDisplay ( plotter, func_rep );
    = controller -> createDifferenceDisplay ( plotter );
  const Range & range = plotter -> getRange ( Axes::X, false );
  res_plotter -> setRange ( Axes::X, range, false );

  CanvasWindow * canvas = WindowController::instance () -> currentCanvas ();

  canvas -> addPlotDisplay ( res_plotter, true );
}
