/* -*- mode: c++ -*- */

/** @file

hippodraw::Inspector class interface.

Copyright (C) 2002-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Inspector.h,v 1.554 2007/09/14 22:42:36 pfkeb Exp $

*/

#ifndef _Inspector_H_
#define _Inspector_H_

#include "qtui/ui_InspectorBase.h"

#include "axes/AxesType.h"
#include "axes/Range.h"

#include <map>

class QEvent;
class QVBoxLayout;

namespace hippodraw {

class CanvasView;
class CanvasWindow;
class CutPlotter;
class DataSourceException;
class DataSource;
class FunctionRep;
class PlotterBase;
class RootNTuple;
class TupleCut;

/** A derived class of InspectorBase class which is generated by the
    Qt designer.  It provides an Inspector it provides methods
    specific to the Qt implementation of the HippoDraw application.

    @bug @@@@@@ Can not change DataSource of plot if function has been applied.

    @bug @@@@@@ When Applying all cuts to DataRep that comes from
    different Ntuple, should raise Dialog to ask whether to not do it,
    or do it with same set of cut names and values, but from different
    datasource.

    @bug @@@@@@ When adding DataRep to another and first has cuts
    applied, then raise dialog to ask user if he wants same cuts
    applied or not.

    @bug @@@@@@ When color map is changed on plot with multiple
    DataReps objects, it should be changed on all on them.

    @bug @@@@@@ When plot has a function cut, trying to add and
    existing data cut causes crash.

    @bug @@@@@@ For test_FitsWriting.py, the time cut doesn't show up
    in Cuts panel when selecting the color plot.

    @bug @@@@@@ On axis tabbed panel, changing the high range via text
    box doesn't set the right bin width. So the range appears set in
    the box, but it is wrong.  Use ConstantModelFit.py, autoscale,
    then set range.

    @bug @@@@@@ In axis tabbed panel, changng Y bin width on color
    plot increases range excessively.

    @bug @@@@@@ One can not change label of histogram on X or Y axis.

    @bug @@@@ On axis tabbed panel, zoom pan doesn't work for static
    histograms.

    @bug @@@@ When changing DataSource on multiple plots, and some
    could not be changed, one gets no warning.

    @bug @@@@ stats panel documentation image needs updating.

    @bug @@ In Stats tabbed panel, the STD button is disabled.

    @bug @@ GUI On the Axis tabbed panel, the low and high sliders have
    large slider handle rectangle than the width and offset ones.
    
    @bug @@@@ On cuts tabbed panel, switching to zoom pan doesn't work
    if one end of cut range is at full range.

    @bug @@@@@@ On the Axis tabbed panel, when in zoom / pan mode, the
    position slider is hard to control when X axis is in log scale.

    @bug In Data tabbed panel, the title doesn't get set if there are
    multiple data reps on plot.  Probably should take title of first
    one.  What if the data reps comes from two different data sources,
    then what title is used?

    @bug On empty DataSource, the axis tabbed panel doesn't show the
    ranges etc. Use empty.py to see it.

   @bug @@ Access to adjustment of fitting cut is not available from
        the inspector if other cuts have been applied to the same
        display.

   @bug @@ Adjusting other cuts also affects the fitting cut.

   @bug @@ Can not compile with gcc 2.96 because of use of ostringstream.

   @bug @@ On data tabbed panel, the Add and Remove buttons should be
   disabled when not appropriate, or info dialog should pop up when
   used in correctly.

   @bug @@@@@@ Trying to change color on display that uses color does
   nothing.  Should bring up an information dialog.

   @bug When changing the DataSource on a plot which has a cut, the
   DataSoruce of the cut and all its targets is also changed without
   warning.

   @bug On Plot tabbed panel, show box edges should be disabled if no
   boxes.

   @bug After saving and loading a variable 3 color scale, it is different
   from the original one. ( Looks like variable rainbow ).

@request It would be helpful if the Canvas settings pop-up had an
  "Apply" button that retiles the canvas, but which does not close the
  window (or save the configuration).  You could just have the
  "Retile" button serve this purpose by not having it close the
  window.

@request For views that have two histograms overlaid, it would be nice
  to have a button that returned the KS test statistic and probability
  that the two data sets are drawn from the same underlying
  distribution.  Not sure what to do if more than two histograms are
  overlaid or if one of the datareps is not a histogram.  Probably
  best to disable this button.

@request In control-left clicking to select a datarep within a given
  view, it would be useful to have an easier way of returning to the
  state where no single datarep is selected. Often I have many
  datareps plotted in a single view, and I want a quick way of
  restoring the view rather than cycling through all of the reps,
  possibly overshooting in my haste.

    @request @@@@ In zoom/pan mode, still should show high and low of
    range.

    @request @@@@ Set to likelihood fitter when statistics of bins go
    below some threshold.

    @request @@ Display objective value to the functions tabbed panel.

    @request Should be able to change title/label text with TeX.

    @request In status tabbed panel, when no plot is selected should
    still be able to add text box.

   @request On the Stats panel, the radio button entry "Chi-squared
            statistics of function" should read "Chi-square per dof ="
            with the value displayed as it is for "Number of entries",
            "Average x-value", and "Average y-value".  Same goes for
            "Standard deviation of x-value".

   @request On Stats tabbed panel, it would be nice when selecting
   text in box that the keyboard focus would go to box and a return
   with trigger the insertion.

   @request @@@@@@ Sliders controlling cuts that are plotted on
            logarithmic x-axes should respond on a logarithmic scale
            instead of a linear one.

    @request Hoyle would like one x but 16 channels in Y displayed on
    one XY plot, but not do it via 16 DataRep objects.

    @todo In Stats tabbed panel, should add a
    feature of a composite text rep that combines the single TextRep
    objects into a composite.

    @todo When the size of the labels of the function parameters
    changes, the sliders change as well.  GUI bug.  low priority to
    fix.

    @todo In cuts tabbed panel, the upper box gets updated when cut
    range changed even when it hasn't changed.

    @request In the stats summary panel, the description should match
    the type used by the factory.

    @todo Replace the ComboBox objects in the Cut inspector with
    lists.  Then the user can see in one glance all the cuts on a
    plot.  There usually will not be that many of them.

    @todo For check boxes, should use clicked signal instead of
    toggled so that the check box can be set to a value without
    emitting a signal.

    @todo On the Cuts tabbed pane, does the Replace button make sense.
    What if selected plot has multiple cuts, which one to replace.

    @bug @@ Have inspector notice that a DataSource have been added
    via the Python interface without having to click on the
    CanvasView.

    @todo On the Inspector data tabbed panel, should leave the four
    binding options on all the time and disable the ones that are not
    used.  This way there would be less flicker.

    @todo In Inspector stats tabbed panel, add average Z if plot has Z
    axis.

    @todo In transform tabbed panel, use QDial instead of QSlider.

    @bug Only first page is showed on Windows platform. Can't scroll 
    down to see other plots. Have to maximize the canvas window.

    @author Kaustuv <kaustuv@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket B. Malde <sanket@stanford.edu>
    @author Joy Rajiv <jraviv@stanford.edu>
    @author Tao Xu <taox@stanford.edu>
    @author Xie Fang <xiefang@stanford.edu>

*/
class MDL_QTHIPPOPLOT_API Inspector : public QDialog, private Ui::InspectorBase
 {
   Q_OBJECT

private:
 
void static stringTokenize(std::string input, const std::string & delimiters,
			   std::vector<std::string> & tokens, bool clear=true);

  enum ParameterList { Index, Name, Value, Error, Fixed, Dummy };

  /** The color model name settings key.
   */
  QString m_model_name;

  /** The color model break point settings key.
   */
  QString m_break_point;

  /** The color model flat width settings key.
   */
  QString m_flat_width;

  /** The color model color scale settings key.
   */
  QString m_color_scale;

  /** Binding labels for new plot group box.
   */
  std::vector < QLabel * > m_new_labels;

  /** Binding combo boxes for new plot group box.
   */
  std::vector  < QComboBox * > m_new_combos;

  /** Binding labels for selected plot group box.
   */
  std::vector < QLabel * > m_sel_labels;

  /** Binding combo boxes for selected plot group box.
   */
  std::vector  < QComboBox * > m_sel_combos;

  /** Map between a function's QListViewItem and its FunctionRep pointer.
   */
  std::map < QTreeWidgetItem *, FunctionRep * > m_function_lv_map;

  /** The list of functions parameters that are checked for use by the
      error contour display.
  */
  std::vector < QTreeWidgetItem * > m_func_parm_checked;

  std::vector < int > m_functionIndexMap;
  hippodraw::Axes::Type m_axis;
  std::vector < double > m_oldParameters;
  std::map < const PlotterBase *, bool> m_zoompan;
  Range m_range;

  QWidget * m_newLayoutWidget;
  CanvasWindow * m_cur_window;
  QWidget * m_layoutWidget;
  QVBoxLayout * m_vLayout;
  QVBoxLayout * m_newVLayout;

  /** A map of the last cut item selected for each plotter.
   */
  std::map < PlotterBase *, int > m_cut_map;

  /** The type of container for current TupleCut objects.
   */
  typedef std::vector < const TupleCut * > TupleCutList_t;

  /** The list of TupleCut objects currently selected.
   */
  TupleCutList_t m_tuple_cuts;

  /** The type of container for PlotterBase objects.
   */
  typedef std::vector < PlotterBase * > PlotterList_t;

  /** The PlotterBase object associated with each TupleCut object.
   */
  PlotterList_t m_tuple_cut_plotters;

  /** The currently selected PlotterBase object.  The currently
      selected PlotterBase object if only one is selected, otherwise a
      null pointer.
   */
  PlotterBase * m_plotter;

  /** The QSettings application key for searching for setting.
   */
  QString  m_app_key;

  /** The currently selected plotters.
   */
  std::vector < PlotterBase * > m_plotter_list;

  /** The m_all_ntuples item that was lasted edited. */
  int m_last_ntuple_edited;

  /** The index of the last selected cut. */
  int m_last_cut_index;

  /** The value of the low slider in Axis Widget 1 as last recorded */
  int m_lowslider1_last_val;

  /** The value of the high slider in Axis Widget 1 as last recorded */
  int m_highslider1_last_val;
  
  /** Set to @c true if updating is being processed.  Used to ignored
      signals from some controls when their value is being set.
   */  
  bool m_is_updating;

  /** Set to true when hippodraw::Inspector::cutEnablePushButton is
      being updated.
   */
  bool m_cut_enable_updating;

  bool m_dragging;

  /** Set to @c true after the user defined value to color transforms
      have been loaded.
   */
  bool m_user_models_loaded;

  /** When changing current plot, set to false so that Inspector don't
      update the rotation repeatedly.
  */
  bool m_rotate_enable;


  /** The list of transform radio buttons.
   */
  std::vector < QRadioButton * > m_transform_buttons;

  /** The min_entries when draging starts.
   */
  int m_min_entries;


  /** Returns the Id of the cut radio button group.

      @note This function was implemented to work around uic3 bugs in
      Qt 4.1.1 where the cut_button_group was generated incorrectly.

   */
  int cutRadioId () const;

  /** Raises warning if TeX not supported. */
  void warningTex ();

  /** Raises operatonal error messge ox with @a messge 
   */
  void invalidOperationError ( const std::string & message );

private slots:
  /** Create display with differenece between two DataRep objects in Plotter. */
  virtual void diffDataRep ();

private:
  /** Help function to find the top level object in a QListViewItem tree.
   */
  QTreeWidgetItem * getTopParent ( QTreeWidgetItem * item );

  /** Returns the FunctionRep corresponding to the top function
      (composite function if any) parameters QListView item.
   */
  FunctionRep * getTopFunctionRep ( QTreeWidgetItem * item );

  /** Returns the FunctionRep coresponding to the selected item.
   */
  FunctionRep * getFunctionRep ( QTreeWidgetItem * item );

  /** Returns the FunctionRep corresponding to top of selected
      function chain (the composite function if any) parameters
      QListView item.
  */
  FunctionRep * getTopFunctionRep ( );

  /** Returns the FunctionRep directly coresponding to the selected item.
   */
  FunctionRep * getFunctionRep ();


  /** Updates the current plotter's transform without calling update(). This is used to 
   *  keep the periodic transform option's internal state consistent. */
  virtual void transformPeriodicUnitUpdate (void);

  /** Clears and fills
      m_func_parm_checked vector with
      the function parameters that have their check box on.*/
  void fillCheckedFunctionRepItems ();

  /** Initializes the Inspector.
   */
  void init ();

  protected slots:
  /** Responds to combo box of selected plot be activated by changing
      the binding.
  */
  virtual void sel_combo_0_activated ( const QString & );

  /** Responds to combo box of selected plot be activated by changing
      the binding.
  */
  virtual void sel_combo_1_activated ( const QString & );

  /** Responds to combo box of selected plot be activated by changing
      the binding.
  */
  virtual void sel_combo_2_activated ( const QString & );

  /** Responds to combo box of selected plot be activated by changing
      the binding.
  */
  virtual void sel_combo_3_activated ( const QString & );

  /** Populates the three vectors(usr_models,brk_points,flat_widths) with 
      details about the user defined Color Transform models 
   */

  /** Returns the Axes::Type for the TupleCut at index @a i.
   */
  hippodraw::Axes::Type getAxes ( unsigned int index );

  void loadAllUserModels ();

  /** Converts the value to color control points from int slider
      values to double values.
  */
  void convertCtrlPts ( std::vector < double > & sv );

  /** Clears the labels and combo boxes.
   */
  void clear ( std::vector < QLabel * > & labels,
	       std::vector < QComboBox * > & combos );


  /** Updates the controls for the color map if applicable, othewise
      disables them.
  */
  void updateColorMapCtrls ( const PlotterBase * plotter );


  /** Updates the controls in the selected plot QGroupBox of the data
      inspector.
  */
  void updateSelectedPlotData ( const PlotterBase * plotter );

  /** Clears the previous binding controls.
   */
   void dataClearSelectedControls ();

  /** Updates the selected plot data source combo box.
   */
  void updateSelectedPlotDataSource ( const std::string & name );

  /** Updates the selected plot DataRep type.
   */
  void updateSelectedPlotType ( const PlotterBase * plotter );

  /** Updates the active and inactive cuts.
   */

  /** Update the status of the auto scale check box. 
   */
  void updateAutoScaleBox ();

  /** Update the status of the log scale check box. 
   */
  void updateLogBox ();

  /** Update the status of the reverse check box.
   */
  void updateReverseBox ();

  void updateCutsActive ();

  /** Updates the list of TupleCut objects and their corresponding Plotter.
   */
  void updateTupleCuts ( const std::vector < PlotterBase * > & cutlist );

  /** Ensures that the application's key value has been set.
   */
  void setAppKey ();

  /** Returns the list of data cut plotter objects that target @a plotter.
   */
  const std::vector < PlotterBase * > & 
  getDataCutList ( PlotterBase * plotter );

  /** Disables all the control widgets for a cut. */
  void disableCutControls ( bool yes );

  /** Clears and Fills @a plot_list with the PlotterBase objects on
      the current canvas.
  */
  void fillPlotterList ( std::vector < PlotterBase * > & plot_list );

  /* Returns the selected cut plotter.
   */
  virtual CutPlotter * getSelectedCut ();
//   /** Clears and fills the @a cut_list with PlotterBase objects that
//       are cuts whose target is the active DataRep contained by @a
//       plotter.
//   */
//   void fillCutsOn ( const PlotterBase *, 
// 		    std::vector < PlotterBase * > & cut_list );

  /** Returns the TupleCut list.  Returns the TupleCut objects being
      used by active DataRep contained by the @a plotter.  If active
      DataRep can not be found, throws logic_error exception.
  */
  const std::vector < const TupleCut * > & 
  getCutList ( const PlotterBase * plotter ) const;

  /** Returns the index of the cut selections with label @a label.
   */
  int findCutIndex ( const QString & label );

  /** Sets all CutPlotter objects to active state if @a yes is @c
      true, otherwise sets them inactive.
   */
  void setAllCutsActive ( bool yes );

  /** Raises information dialog to say applying a cut to itself was not done.
   */
  void cutOnCutError ();

  /** Raises a dialog to warn user that bound DataSource has invalid
      data.
   */
  void invalidDataWarning ();

  /** Raises a dialog for new plot error.
   */
  void newPlotError ( const std::exception & e );

  /** Raises no NTuple selected error dialog. */
  void noNTupleSelectedError ();

  /** Raises invalid range error dialog. */
  void invalidRangeError ( const std::string & bad );

  /** Raises incompatible DataRep error dialog.
   */
  void incompatibleDataRepError ( const std::string & type );

  /** Raises incompatible Fitter error dialog.
   */
  void incompatibleFitterError ( const std::string & type );

  /** Raise bad function error dialog.
   */
  void badFunctionError ( const std::string & name, const char * what );

  /** Raises failed fit error dialog.
   */
  void fitFailedError ();

  /** Raises the incompatible function error dialog.
   */
  void incompatibleFunctionError ( const std::string & type );

  /** Raises a multiple DataRep error QMessageBox.  When an
      application action requires only one DataRep to be active, then
      this function is called.  The @a type is inserted in to the
      message string.  This is part of the learn by doing GUI pattern.
      Rather than disabling a control when multiple DataRep objects
      are active, one calls this function to tell user what he should
      do to get the action he wants.  Namely, shift click to make one
      DataRep active. */
  void multipleDataRepError ( const QString & type );

  /** Raises a multiple DataRep info dialog.  Returns @c true if user
      answers to apply request to each, otherwise returns @c false.
   */
  bool multipleDataRepInfo ( const std::string & type );
 
  /** Raises a multiple plot error message box.
   */
  void multiplePlotError ();

  /** Raises a function add error QMessageBox when the selected DataRep
      does not support the selected function. */
  void functionAddError ();

  /** Fills the QListView with items for each function parameter or @a
      function as a child of @a parent. Uses and updates the running index.
   */
  void fillFunctionParameters ( QTreeWidgetItem * parent,
				const FunctionRep * function,
				unsigned int & index );

  /** Strips the value part of the text field. 
   */
  void statsStripValue ( QRadioButton * box );

  /** Updates the controls in the new plot group box. 
   */
  void updateNewPlotControls ();

  /** Sets the current item in the available NTuple selection box to
      one that matches @a name. 
  */
  void setNewPlotNTuple ( const std::string & name );

  /** Changes the name NTuple.  Changes the name of a %NTuple object
      to @a new_name.  The %NTuple whose names get changed is the one
      whose name was last edited in the m_all_ntuples QComboBox.  It
      was save in m_last_ntuple_edited.
  */
  void changeNTupleName ( const QString & );

  /** Responds to combo box that shows the NTuple name of the selected
      plot.
  */
  virtual void ntupleChanged ( int );

  /** Updates the Plot tabbed panel's widgets.
   */
  virtual void updatePlotTab ();

  /** Responds to error bars check box.
   */
  virtual void errorBars_toggled ( bool );

  /** Updates the QGroupBox that displays to controller for creating a
      new cut.
  */
  void updateCutVarGroupBox ( const PlotterBase * plotter, int index );

  /** Updates the Cut controls in the Cut tabbed panel. Cut list is a vector
   of selected cut plotters.*/
  void updateCutControls ( const std::vector < PlotterBase * > & cutlist );

  /** Updates the Cut control values with those of the designated
      cut Plotter. */
  void updateCutControlValues ( const PlotterBase * cutplotter );

  /** Updates the cut enabled push button control.
   */
  void updateCutEnableButton ();

  /** Sets the Z axis radio button to proper state.
   */
  virtual void setZRadioButton ( bool enabled );

  protected:

  /** Updates the value to color transform combo box.
   */
  void updateValueCombo ();

  /** Return true if the range is valid for HammerAito or Lambert 
      transform. 
  */
  int validPeriodicTransformRange();
  bool validPeriodicTransformRange( int miny );

  /** Displays a warning message that application was not compiled
      with WCSLIB support.
   */
  void invalidPeriodicTransform ();

  /** The QSettings Windows registry key. The key is the same as used
      for by Trolltech for their Qt applications 
  */
  static QString s_registry;
 
  protected slots:

 /* In the member functions below, all are declared in the base class
     where they are commented for documentation, except those that are
     commented here. 
  */
  virtual PlotterBase * getPlotter();

  /** Responds to click on the add Data Rep button.  
   */
  virtual void addDataRepButton_clicked ( );

  /** Responds to click on the remove Data Rep button.
   */
  virtual void removeDataRepButton_clicked();

  virtual void allNtupleComboActivated ( const QString & );
  /** Updates the available plot types combo box.
   */
  virtual void updatePlotTypes ();
  virtual void availPlotTypesActivated ( const QString & );
  virtual void dataCreateNTuple ();

  /** Respond to change of DataSource name change.
      @note the argument is ignored.
  */
  virtual void dataTupleNameChanged (const QString & );
  virtual void dataNTupleSelChanged ( int item );

  /** Responds to title text return.
   */
  virtual void titleText_returnPressed();

  /** Responds to change in axis tabbed panel's axis radio button group.
   */
  virtual void axis_button_group_clicked ( int id );

  /** Responds to change in zoom/pan check box.
   */
  virtual void axisZoomPanCheckBox_clicked ();

  /** Responds to change in axis binding on existing plotter.
   */
  virtual void axisLabelChanged ( int index, const QString & axisName );

  virtual void highRangeDrag();
  virtual void lowRangeDrag();
  virtual void newPlotButton_clicked ( );

  /** Called when any slider is pressed.
   */
  virtual void setDragOn ();

  virtual void offsetDrag();
  virtual void entriesDrag();
  virtual void symbolTypeButtonGroup_clicked ( int id );
  virtual void lineStyleButtonGroup_clicked ( int id );
  virtual void symbolPointSize_returnPressed ( );
  virtual void intervalStateChanged ( bool state );
  virtual void intervalTextChanged ( const QString & );
  virtual void colorSelect_clicked ();

  /** Sets the Color of the cut plotter.
   */
  virtual void colorSelect_2_clicked ();

  /** Responds to return in bin width text field.
   */
  virtual void setWidthText ();

  /** Responds to return in bin offset text field.
   */
  virtual void setOffsetText ();

  virtual void setBinWidth ( int value );

  /** Fits the selected function to the data.
   */
  virtual void functionsFitToDataButton_clicked ();

  /** Removing the selected function and updating appropriate tabbed
      panels.
  */
  virtual void functionsRemoveButton_clicked ();

  /** Reset the function parameters from the parameters saved before
      the last fit.
  */
  virtual void functionsResetButton_clicked();

  virtual void setParameters ( int, PlotterBase * );
  virtual void functionParamsListViewCurrentChanged ();
  virtual void functionParamsCheckBoxToggled( bool );
  /** Slot which responds to the ignoreError check box.
   */
  virtual void ignoreErrorCheckBoxToggled(bool);
  virtual void functionParamsLineEditReturnPressed();
  virtual void functionParamsSliderSliderReleased();
  virtual void functionParamsSliderSliderMoved( int );
  virtual void functionParamsSliderSliderPressed();

  /** Responds to change of the cut panel's radio group.

      @note This method does not use ability of button group to send
      radio button id because of bugs in Qt 4.1.1 in uic3's generation
      of the button group.
   */
  virtual void cut_button_group_clicked ();

  virtual void logScale_clicked();

  /** Slot which respons to the toggle in state of the reverse check box.
   */
  virtual void reverse_clicked();

  /** Slot which responds to the toggle in state of the autoScale check box.
   */
  virtual void autoScale_clicked();

  /** Responds to the toggle in state of the show grid check box.
   */
  virtual void m_grid_clicked();
  virtual void m_boxedge_clicked();
  virtual void cutLowSlider_sliderMoved ( int value );
  virtual void cutHighSlider_sliderMoved ( int value );
  virtual void cutLowSlider_sliderReleased();
  virtual void cutHighSlider_sliderReleased();
  virtual void cutZoomPanCheckBox_clicked();
  virtual void cutInvertPushButton_clicked();

  /** Adds selected cut to the DataRep.
   */
  virtual void cutAddSelected ();

  /** Adds all the cuts to the DataRep.
   */
  virtual void cutAddAll ();

  /** Responds to the button that removes a cut.
   */ 
  virtual void cutRemovePushButton_clicked ();

  /** Responds to entry in one of the cut text fields.
   */
  virtual void cutText_returnPressed();

  /** Responds to the cut enable push button.
   */
  virtual void cutEnablePushButton_toggled ( bool on );
  
  /** Sets the bin width and the slider dragging flag. 
   */
  virtual void setBinWidth ( int value, bool drag );

  /** Responds to return in axis label text box. 
   */
  virtual void axisLabelText ();

  virtual void setLowRange ( int value );

  /** Sets the low range and slider dragging flag. 
   */
  virtual void setLowRange ( int value, bool yes );

  virtual void setLowText ();
  virtual void setHighRange ( int value );

  /** Sets the high range and the slider dragging flag. 
   */
  virtual void setHighRange ( int value, bool yes );

  virtual void setHighText ();
  virtual void setOffset ( int value  );

  /** Rotation for periodic transform.
   */
  virtual void rotateX ( int offset );
  virtual void rotateY ( int offset );
  
  virtual void setXRotateText ();
  virtual void setYRotateText ();

  
  /** Reset to zero.
   */
  virtual void resetRotate ();

  /** Reset the rotation slider and text, from the plotter.
   */
  virtual void setRotate ( double x, double y );

  /** sets the bin offset and the slider dragging flag. 
   */
  virtual void setOffset ( int value, bool yes  );
  
  virtual void cutNew ();
  virtual void functionAdd ();
  virtual void summaryNew ();

  /** Responds to change in transform tabbed panel's combo box.

      @note Did not use QButtonGroup's ability to send id because with
      Qt 4.1.1 the code generation for InspectorBase.h was faulty.

   */
  virtual void transform_changed (const QString &);
  virtual void transformPeriodicUnitChange (void);

  virtual void selCutChanged ();
  virtual void updateDataTab();
  virtual void pointRepComboBox_activated ( const QString & qstr );

  /** Updates the cut tabbed panel's widgets' state and values.
   */
  virtual void updateCutsTab ();

  /** Updates the cut tabbed panel when in data cut mode.
   */
  virtual void updateDataCutsTab ();

  /** Update the cut tabbed panel when in fit cut mode.
   */
  virtual void updateFitCutsTab ();

  /** Updates the functions tabbed panel's widgets.
   */
  virtual void updateFunctionsTab ();

  virtual void createResiduals();
  virtual void fitterNamesActivated ( int );
  virtual void pushButtonNewErrorPlotClicked ();

  /** Updates the axis tabbed panel's widgets.
   */
  virtual void updateAxisTab ();

  /** Update the transform tabbed panel's widgets.
   */
  virtual void updateTransformTab ();

  virtual void widthDrag ();

  /** Edit label font,  ( if user wants to overwrite he default ) */
  void editLabelFontClicked();

  /** Edit the title font, ( if user wants to overwrite he default ) */
  void editTitleFontClicked();
  
  /** In case we are dealing with multidimensional data in rows of this column
      in the given DataSource we would like to deal with on one entry of
      this multidimensional data. So here we get the index of this relevant
      entry for this column in the given DataSource.
      @note This method is only available if configured with ROOT support
  */
  std::string getArrayTupleLabel( const DataSource * rtuple,
				  const std::string & column );

  /** Responds to change of the value to color combo box.
   */
  virtual void valueChanged ( int );

  /** Responds to change in the sliders of one of the value to color
      sliders.
   */
  virtual void sliderChanged ( int );
  
  /** Resets sliders to default position
   */
  virtual void resetSlider ( );

  /** Brings up dialog to create a new Value to Color model
   */
  virtual  void newColorModel ( );
 
  /** Brings up dialog to edit an existing Value to Color model
   */
  virtual  void editColorModel ( );

  /** Deletes an existing Value to Color Model
   */
  virtual void deleteColorModel ( );

  /** Responds to the combined bins checkbox clicked.
   */
  virtual void combineCheckBox_clicked();

  /** Responds to minimum entries slider.
   */
  virtual void setMinEntries ( int increment );

  /** Get the minimum entries number of the plot.
   */
  virtual int getMinEntries ();

  /** Responds to the user input of minimum entries number.
   */
  virtual void setMinEntriesText();
public:

  /** The constructor. */
  Inspector ( QWidget * parent = 0,
	      const char * name = 0,
	      bool modal = false,
	      Qt::WFlags flags = 0 );

  /** Returns the name of the currently selected DataSource in Data
      tabbed panel.
 */
  std::string getSelectedDataSourceName () const;

  /** Enables the new plot box.  Enables the new plot box in the data
      tabbed panel so the user can add new plot to the canvas.
      Applications that have a CanvasView within a window will
      probably want to enable this feature.  Applications that do not
      have a canvas, should leave this feature disabled. */
  void enableNewPlotBox ( bool yes = true );

public slots:
  /** The method to receive update events from the application queue.
      The @a event object's data should contain a pointer to a
      PlotterBase object.  This object's properties will be use to
      update the Inspector's tabbed panels and will be the target for
      its controls.
  */
  virtual void customEvent ( QEvent * event );

  /** The Update Method.  Updates only the tabbed panel that is
      currently selected (visible).
  */
  virtual void update ( );

  virtual void selectedCutsRadioButton_toggled ( bool selected );
  virtual void allCutsRadioButton_toggled ( bool selected );

  /** Updates the Stats tabbed panel's widgets.
   */
  virtual void updateSummaryTab ();

  virtual void statsButtonGroupClicked ();

  protected slots:
  virtual void contourSlider_valueChanged ( int val );

  virtual void contourTextBox_returnPressed ();

  /** Responds to change in 1st contour radio button.
   */
  virtual void contourRadioButton1_toggled ( bool );

  /** Responds to change in contour levels text box.
   */
  virtual void contourLevelsTextBox_returnPressed ();

  /** Raises message box on contour input error.
   */
  void contourError ();

  /** Receives the signal that the tabbed panel changed and invokes
      the update method.
   */
  virtual void tabChanged ();

  protected:

  virtual const std::string convertToString ( hippodraw::Axes::Type );

  public:

  /** Sets the selected Fitter.
   */
  void setSelectedFitter ( const std::string & name );

   /** The virtual destructor.
   */
  virtual ~Inspector ();


};

} // namespace hippodraw

#endif // _Inspector_H_

