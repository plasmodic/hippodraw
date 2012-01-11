/* -*- mode:c++ -*- */

/** @file

SIP interface definition for Inspector

Copyright (C)  2003, 2005, 2006  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

$Id: Inspector.sip,v 1.10 2007/02/24 22:24:38 pfkeb Exp $

Author: stuvi@slac.stanford.edu

*/


class Inspector : InspectorBase
{
%TypeHeaderCode
#include "qt/Inspector.h"
#include "plotters/CutPlotter.h"
using hippodraw::Inspector;
using hippodraw::CutPlotter;
%End


public:
  
  Inspector ();
  ~Inspector(); // Yes, it is not virtual

  /** Returns the name of the currently selected DataSource in Data
      tabbed panel.
 */
  std::string getSelectedDataSourceName () const;

  /** Enables the new plot box.  Enables the new plot box in the data
      tabbed panel so the user can add new plot to the canvas.
      Applications that have a CanvasView within a window will
      probably want to enable this feature.  Applications that do not
      have a canvas, should leave this feature disabled. */
  void enableNewPlotBox ( bool = true );

  /** The method to receive update events from the application queue.
      The @a event object's data should contain a pointer to a
      PlotterBase object.  This object's properties will be use to
      update the Inspector's tabbed panels and will be the target for
      its controls.
  */
  virtual void customEvent ( QCustomEvent * );

  /** The Update Method.  Updates only the tabbed panel that is
      currently selected (visible).
  */
  virtual void update ( );

  /* Returns the selected cut plotter.
   */
//  virtual CutPlotter *  getSelectedCut ();
  virtual void selectedCutsRadioButton_toggled ( bool );
  virtual void allCutsRadioButton_toggled ( bool );

  /** Updates the Stats tabbed panel's widgets.
   */
  virtual void updateSummaryTab ();

  virtual void statsButtonGroupClicked ( int );

};
