/* -*- mode: c++ -*- */

/** @file

hippodraw::CanvasSettings class interface

Copyright (C) 2004-2006   The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: CanvasSettings.h,v 1.36 2006/11/21 01:19:16 xiefang Exp $

*/

#ifndef CanvasSettings_H
#define CanvasSettings_H

#include "qtui/ui_CanvasSettingsBase.h"

#include <qprinter.h>

namespace hippodraw {

/** This class manages the CanvasView settings dialog.
    This class manages the CanvasView settings dialog.  It looks like this ...

    @request Plot matrix should have a preview.

    @image html canvaswindowsettingsdialog.png The canvas settings dialog.
    @image latex canvaswindowsettingsdialog.eps The canvas settings dialog.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class CanvasSettings : public QDialog, private Ui::CanvasSettingsBase
 {
   Q_OBJECT

public:

  /** Return codes from running the CanvasSettings dialog.
   */
  enum Type { Accepted, ///< Accept new setting for new plots.
	      Rejected, ///< Reject changes.
	      AcceptedAndRetile, ///< Accept new setting and retile
	      Retile ///< Retile only
  };

private:

  /** Changes the plot matrix orientation.
   */
  void orientationChanged ();


protected slots:

  /** Responds to Cancel button. 
   */
  virtual void cancel();

  /** Responds to Save & Retile button.
   */
  virtual void saveRetile ();

  /** Responds portrait is clicked.
   */
  virtual void portraitRadioButton_toggled ( bool );

  /** Responds to re-tile button clicked.
   */
  virtual void retile_clicked();
  
public:

  /** The constructor. 
   */ 
    CanvasSettings ( QWidget* parent = 0);

  /** Returns the number of new plots horizontally. 
   */
  int getWidthNumber () const;

  /** Sets the number of new plots horizontally.
   */
  void setWidthNumber ( int number );

  /** Returns the number of new plots vertically.
   */
  int getHeightNumber () const;

  /** Sets the number of new plots vertically.
   */
  void setHeightNumber ( int number );

  /** Gets the orientation of the canvas which can be Portrait or
     Landscape */
  QPrinter::Orientation orientation();
  
  /** Sets the orientation of the canvas which can be Portrait or
      Landscape */
  void setOrientation( QPrinter::Orientation orientation );

  /** Sets the if the printer margin/bounds should be visible on
      the canvas or not */
  bool printerBounds();

  /** Gets the flag if the printer margin/bounds should be visible on
      the canvas or not */
  void setPrinterBounds( bool flag );

  /** Returns @c true if added plots should be selected.
   */
  bool getAddSelected () const;

  /** Sets the check box of the state of add plots being selected.
   */
  void setAddSelected ( bool yes );

  /** Returns true if printer margin should be ignored when inserting
      new plots.
   */
  bool ignorePrinterMargin () const;

  /** Sets the status of the ignore printer margins check box.
   */
  void setIgnorePrinterMargin ( bool yes );

};

} // namespace hippodraw

#endif // CanvasSettings_H
