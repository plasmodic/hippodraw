/* -*- mode: c++ -*- */

/** @file

hippodraw::AxisWidget class interface

Copyright (C) 2003-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: AxisWidget.h,v 1.29 2006/05/05 23:42:22 pfkeb Exp $

*/

#ifndef _AxisWidget_H_
#define _AxisWidget_H_

#include <qwidget.h>
#include <QtGui/QLabel>

#include <string>

class QLabel;
class QLineEdit;
class QCheckBox;
class QScrollBar;

namespace hippodraw {

class Range;

/** A custom widget that creates high and low sliders and text boxes, and
    implements zoom pan on them.

    @author Sanket Biren Malde <sanket@stanford.edu>
*/

class AxisWidget : public QWidget
{

  Q_OBJECT
  
private:
  
  /** The "Low" text label. */
  QLabel * lowTextLabel;
  
  /** The "High" text label. */
  QLabel * highTextLabel;

  /** The low slider. */
  QScrollBar * lowSlider;

  /** The high slider. */
  QScrollBar * highSlider;

  /** The text box that shows the low value. */
  QLineEdit * lowTextBox;

  /** The text box that shows the high value. */
  QLineEdit * highTextBox;

  /** The zoom pan check box. */
  QCheckBox * zoomPanCheckBox;
  
  /** A boolean that is true if the AxisWidget controls a cut. */
  bool m_isCut;

  /** Converts the slider position to Width. */
  double getWidthFromSlider ( const Range & fullRange );
  
  /** Converts the slider position to Position. */
  double getPositionFromSlider ( const Range & fullRange );

  /** Converts the slider position to Low. */
  double getLowFromSlider ( const Range & fullRange );
  
  /** Converts the slider position to High. */
  double getHighFromSlider ( const Range & fullRange );
  
  /** Sets the slider @a s to the value @a value. */
  void setSlider ( QScrollBar * s, double value, const Range & fullRange );
  
  /** Sets the slider @a s to the value @a value, where the slider zero
      corresponds to data zero. Eg: width */
  void setSliderZero ( QScrollBar * s, double value, const Range & fullRange );
  
public:
  
  /** The standard QWidget constructor.  See Qt documentation for
      details. */
  AxisWidget ( QWidget * parent = 0, 
	       const char * name = 0,
	       Qt::WFlags wflags = 0 );
  
  /** The required virtual destructor. */
  virtual ~AxisWidget ();
  
  /** Called by InspectorBase in response to the lowTextReturnPressed
      and highTextReturnPressed signals. Takes the currentRange and
      modifies it appropriately. If m_isCut is true, then uses fullRange
      to set the sliders. */
  void processTextBoxReturnPressed ( Range & currentRange,
				     const Range & fullRange );

  /** Called by InspectorBase in response to the lowSliderReleased
      signal. */
  void processLowSliderReleased ( const Range & fullRange );

  /** Called by InspectorBase in response to the lowSliderMoved
      signal. */
  void processLowSliderMoved ( int value,
			       Range & currentRange,
			       const Range & fullRange );

  /** Called by InspectorBase in response to the highSliderReleased
      signal. */
  void processHighSliderReleased ( const Range & fullRange );

  /** Called by InspectorBase in response to the highSliderMoved
      signal. */
  void processHighSliderMoved ( int value,
				Range & currentRange,
				const Range & fullRange );

  /** Called by InspectorBase in response to the zoomPanCheckBoxClicked
      signal. */
  void processZoomPanCheckBoxClicked ( const Range & currentRange,
				       const Range & fullRange );

  /** Set the isCut flag. */
  void setCut ( bool flag );

  /** Set the text in the lowTextBox.  If @a readonly is @c true, then
      set the text to be read only.
  */
  void setLowText ( const  QString & s, bool readonly = false );

  /** Set the text in the highTextBox. If @a readonly is @c true, then
      set the text to be read only.
  */
  void setHighText ( const QString & s, bool readonly = false );

  /** Returns the low slider widget. 
   */
  QScrollBar * getLowSlider ();

  /** Get the low slider value. */
  int getLowSliderValue ();
  
  /** Set the low slider value. */
  void setLowSliderValue ( int value );

  /** Returns the high slider. 
   */
  QScrollBar * getHighSlider ();

  /** Get the high slider value. */
  int getHighSliderValue ();
  
  /** Set the high slider value. */
  void setHighSliderValue ( int value );

  /** Throws up a message box when an attempt is made to set an invalid
      range. */
  void invalidRangeError ( const QString & bad );

  /** Enable all controls if @a flag = false, disable if true. */
  void setAllDisabled ( bool flag );

  /** Set the textboxes and sliders to the values given by the
  arguments. Only for cuts. */
  void updateCutControlValues ( const Range & currentRange,
				const Range & fullRange );

  /** Returns whether zoomPanCheckBox is checked. */
  bool isZoomPanChecked ();

  /** Checks zoomPanCheckBox if check is true, else un-checks it.
   The second argument can be used to set it to be disabled (i.e. frozen) */
  void setZoomPan ( bool check, bool disables = false );

signals:

  /** Emitted when lowTextBox gets return pressed. */
  void lowTextReturnPressed ();

  /** Emitted when highTextBox gets return pressed. */
  void highTextReturnPressed ();

  /** Emitted when lowSlider is released. */
  void lowSliderReleased ();

  /** Emitted when lowSlider is pressed. */
  void lowSliderPressed ();

  /** Emitted when lowSlider's value has changed. */
  void lowSliderValueChanged ( int value );

  /** Emitted when highSlider is released. */
  void highSliderReleased ();

  /** Emitted when highSlider is pressed. */
  void highSliderPressed ();

  /** Emitted when highSlider's value has changed. */
  void highSliderValueChanged ( int value );

  /** Emitted when zoomPanCheckBox is clicked. */
  void zoomPanCheckBoxClicked ();
  
};

} // namespace hippodraw

#endif // _AxisWidget_H_
