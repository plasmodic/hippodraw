/** @file

hippodraw::AxisWidget implementation

Copyright (C) 2003-2006 The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: AxisWidget.cxx,v 1.40 2006/07/10 23:23:44 pfkeb Exp $

*/

#include "AxisWidget.h"
#include "axes/Range.h"

#include <qpushbutton.h>
#include <qpainter.h>
#include <qevent.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qnamespace.h>
#include <qscrollbar.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qsizepolicy.h>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>

#include <cassert>

using std::string;

using namespace hippodraw;

  /** @todo Replace obsolete QScrollBae constructor with Qt 4 one.
   */
AxisWidget::
AxisWidget ( QWidget * parent, const char * name,  Qt::WFlags wflags )
  : QWidget ( parent, wflags )
{

  // Init.
  setObjectName(name);
  
  lowTextLabel = new QLabel  ( tr("Low: "), this );
  lowTextLabel -> setObjectName ( "lowTextLabel" );
  highTextLabel = new QLabel ( tr("High:"), this );
  highTextLabel -> setObjectName ( "highTextLabel" );
  
  lowSlider = new QScrollBar ( Qt::Horizontal, this ); // Qt 4 way
  lowSlider -> setRange ( 0, 99 );
  lowSlider -> setPageStep ( 20 );
  lowSlider -> setValue ( 50 );
  QSizePolicy policy;
  policy.setHorizontalPolicy ( QSizePolicy::MinimumExpanding );
  lowSlider -> setSizePolicy ( policy );
  lowSlider -> setSingleStep ( 1 );
  lowSlider -> setFocusPolicy ( Qt::WheelFocus );

  highSlider = new QScrollBar ( Qt::Horizontal, this );
  highSlider -> setRange ( 0, 99 );
  highSlider -> setPageStep ( 20 );
  highSlider -> setValue ( 50 );
  highSlider -> setSizePolicy ( policy );
  highSlider -> setSingleStep ( 1);
  highSlider -> setFocusPolicy ( Qt::WheelFocus );
  lowTextBox = new QLineEdit ( this );
  lowTextBox -> setObjectName ( "lowTextBox" );
  //  lowTextBox -> setMaximumHeight ( 26 );
  highTextBox = new QLineEdit ( this );
  highTextBox -> setObjectName ( "highTextBox" );
  //  highTextBox -> setMaximumHeight ( 26 );
  
  zoomPanCheckBox = new QCheckBox ( tr("Zoom/Pan"), this );
  zoomPanCheckBox -> setObjectName ( "zoomPanCheckBox" );
  
  m_isCut = false;

  // Layout.

  QVBoxLayout * grandParentLayout = new QVBoxLayout ( this );
  grandParentLayout -> setContentsMargins ( 0, 0, 0, 0 );

  QGridLayout * sliderLayout = new QGridLayout();
  sliderLayout -> setContentsMargins ( 0, 0, 0, 0 );
  sliderLayout -> addWidget ( lowTextLabel, 0, 0 );
  sliderLayout -> addWidget ( lowTextBox, 0, 1 );
  sliderLayout -> addWidget ( lowSlider, 0, 2 );
  sliderLayout -> addWidget ( highTextLabel, 1, 0 );
  sliderLayout -> addWidget ( highTextBox, 1, 1 );
  sliderLayout -> addWidget ( highSlider, 1, 2 );
  grandParentLayout -> addLayout ( sliderLayout );
  
  QHBoxLayout * checkboxlayout = new QHBoxLayout (  );
  checkboxlayout -> setContentsMargins ( 5, 5, 5, 5 );
  checkboxlayout->addWidget ( zoomPanCheckBox );
  checkboxlayout->setAlignment ( Qt::AlignHCenter );
  grandParentLayout -> addLayout ( checkboxlayout );

  // Connect.
  
  connect ( lowTextBox, SIGNAL ( returnPressed() ),
	    this, SIGNAL ( lowTextReturnPressed () ) );
  
  connect ( highTextBox, SIGNAL ( returnPressed () ),
	    this, SIGNAL ( highTextReturnPressed () ) );
  
  connect ( lowSlider, SIGNAL ( sliderReleased () ),
	    this, SIGNAL ( lowSliderReleased () ) );
  
  connect ( highSlider, SIGNAL ( sliderReleased () ),
	    this, SIGNAL ( highSliderReleased () ) );
  
  connect ( lowSlider, SIGNAL ( valueChanged ( int ) ),
	    this, SIGNAL ( lowSliderValueChanged ( int ) ) );
  
  connect ( highSlider, SIGNAL ( valueChanged ( int ) ),
	    this, SIGNAL ( highSliderValueChanged ( int ) ) );

  connect ( lowSlider, SIGNAL ( sliderPressed () ),
	    this, SIGNAL ( lowSliderPressed () ) );
  
  connect ( highSlider, SIGNAL ( sliderPressed () ),
	    this, SIGNAL ( highSliderPressed () ) );
  
  connect ( zoomPanCheckBox, SIGNAL ( clicked () ),
	    this, SIGNAL ( zoomPanCheckBoxClicked () ) );

}

AxisWidget::~AxisWidget ()
{
  delete lowTextLabel;
  delete highTextLabel;
  delete lowSlider;
  delete highSlider;
  delete lowTextBox;
  delete highTextBox;
  delete zoomPanCheckBox;
}

void
AxisWidget::
processTextBoxReturnPressed ( Range & currentRange, 
			      const Range & fullRange )
{

  if ( !zoomPanCheckBox->isChecked() ){

    QString text1 = highTextBox->text();
    double hi = text1.toDouble();
    QString text = lowTextBox->text();
    double lo = text.toDouble();
    
    if ( lo >= hi ) {
      invalidRangeError ( tr("Low not less than high") );
      highTextBox->setText ( QString("%1").arg(currentRange.high()) );
      lowTextBox->setText ( QString("%1").arg(currentRange.low()) );
      return;
    }
    
    currentRange.setRange ( lo, hi, currentRange.pos() );
    
    if ( !m_isCut ) return;
    
    // If cut, set the sliders.
    
    setSlider ( lowSlider,  currentRange.low(),  fullRange );
    setSlider ( highSlider, currentRange.high(), fullRange );

  }
  
  else {

    if ( m_isCut ) {
      
      double width = ( lowTextBox->text()).toDouble();    
      double oldWidth = getWidthFromSlider ( fullRange );
      double position = ( highTextBox->text()).toDouble();
      double oldPosition = getPositionFromSlider ( fullRange );
      
      if ( position - width / 2 < fullRange.low() || 
	   position + width / 2 > fullRange.high() ) {
	
	lowTextBox->setText ( QString("%1").arg( oldWidth ) );
	highTextBox->setText ( QString("%1").arg( oldPosition ) );
	return;
	
      }
      
      // Set New Range.
      
      currentRange.setRange ( position - width / 2, 
			       position + width / 2, 
			       currentRange.pos() );
      
      // Update Sliders.
      
      setSliderZero ( lowSlider, width, fullRange );
      setSlider ( highSlider, position, fullRange );

    }

    else {

      double width = ( lowTextBox->text()).toDouble();    
      double position = ( highTextBox->text()).toDouble();
      
      // Set New Range.
      
      currentRange.setRange ( position - width / 2, 
			      position + width / 2, 
			      currentRange.pos() );

    }

  }

}

void AxisWidget::processLowSliderReleased ( const Range & fullRange )
{
  if ( !zoomPanCheckBox->isChecked() ){
    assert ( m_isCut );    
    double low = (lowTextBox->text()).toDouble();    
    setSlider ( lowSlider, low, fullRange );
  }
  else {
    double width =  (lowTextBox->text()).toDouble();    
    setSliderZero ( lowSlider, width, fullRange );
  }
  
}

void AxisWidget::processHighSliderReleased ( const Range & fullRange )
{
  double positionOrHigh = ( highTextBox->text() ).toDouble();
  setSlider ( highSlider, positionOrHigh, fullRange );
}

/** @todo int argument is not used, take out of interface.
 */
void
AxisWidget::
processLowSliderMoved ( int,
			Range & currentRange,
			const Range & fullRange )
{

  if ( ! zoomPanCheckBox->isChecked() ) {
    
    assert ( m_isCut );

    double new_low = getLowFromSlider ( fullRange );
    
    if ( new_low < currentRange.high() ){
      currentRange.setLow ( new_low );
    }
    
    // Update other guys : low text.
    
    double low = currentRange.low();
    lowTextBox->setText ( QString("%1").arg(low));
    
  }
  
  else {

    if ( m_isCut ) {
      
      double width = getWidthFromSlider ( fullRange );
      double position = getPositionFromSlider ( fullRange );
      
      if ( position - width / 2 < fullRange.low() || 
	   position + width / 2 > fullRange.high() ) {
	return;
	
      }
      
      // Set New Range.
      
      currentRange.setRange ( position - width / 2, 
			      position + width / 2, 
			      currentRange.pos() );  
      
      // Update TextBox.
      
      lowTextBox->setText ( QString("%1").arg( width ) );
      
    }

    else {

      double oldWidth = fullRange.length();
      double width =  oldWidth * lowSlider->value() / 50 ;
      double position = ( fullRange.high() + fullRange.low() ) / 2 ;

      // Set New Range.
      
      currentRange.setRange ( position - width / 2, 
			      position + width / 2, 
			      currentRange.pos() );  
      
      // Update TextBox.
      
      lowTextBox->setText ( QString("%1").arg( width ) );
      

    }

  }

}

/** @todo int argument is not used, take out of interface.
 */
void
AxisWidget::
processHighSliderMoved ( int,
			 Range & currentRange,
			 const Range & fullRange )
{
  if ( ! zoomPanCheckBox->isChecked() ) {
 
    assert ( m_isCut );

    double new_high = getHighFromSlider ( fullRange );
      
    if ( new_high > currentRange.low() ) {
      currentRange.setHigh ( new_high );
    }

    // Update other guys : high text.
      
    double high = currentRange.high();
    highTextBox->setText ( QString("%1").arg(high));  
  } 
  else { // is in zoom/pan mode

    if ( m_isCut ) { //is in zoom/pan mode and is cut

      double width = currentRange.length ();
      double position = getPositionFromSlider ( fullRange );
      
      // Set New Range.
      
      currentRange.setRange ( position - 0.5 * width,
			      position + 0.5 * width,
			      currentRange.pos() );  
      
      // Update TextBoxes..
      
      highTextBox->setText ( QString("%1").arg( position ) );
      
    }
    else { // is in zoom/pan but not cut

      double width = fullRange.length();
      double oldPosition = ( fullRange.high() + fullRange.low() ) / 2 ;
      double position = ( width * 
			  ( ( double ) ( highSlider->value() - 50 ) ) / 50
			  ) + oldPosition;

      // Set New Range.
      
      currentRange.setRange ( position - width / 2, 
			      position + width / 2, 
			      currentRange.pos() );  
      
      // Update TextBox.
      
      highTextBox->setText ( QString("%1").arg( position ) );
    }
  }
}

void
AxisWidget::
processZoomPanCheckBoxClicked ( const Range & currentRange,
				const Range & fullRange )
{

  bool checked = zoomPanCheckBox->isChecked();
  
  if ( m_isCut )
    if ( checked )
      {
	
	// Turn on the zoom / pan mode. Change high low labels, change the
	// texts, change the sliders.
	
	highTextLabel->setText ( tr("Position") );
	lowTextLabel->setText  ( tr("Width   ") );
	
	double currentWidth = currentRange.high() - currentRange.low();
	double currentPosition 
	  = 0.5 * ( currentRange.high() + currentRange.low() );
	
	highTextBox->setText ( QString("%1").arg(currentPosition) );
	lowTextBox->setText ( QString("%1").arg(currentWidth) );
	
	setSlider ( highSlider, currentPosition, fullRange );
	setSliderZero ( lowSlider, currentWidth, fullRange );
	
      }
    else
      {
	highTextLabel->setText ( tr("High    ") );
	lowTextLabel->setText  ( tr("Low     ") );
	
	highTextBox->setText ( QString("%1").arg(currentRange.high()) );
	lowTextBox->setText ( QString("%1").arg(currentRange.low()) );
	
	setSlider ( highSlider, currentRange.high(), fullRange );
	setSlider ( lowSlider, currentRange.low(), fullRange );
	
      }
  else // i.e. ! m_isCut
    if ( checked )
      {
	highTextLabel->setText ( tr("Position") );
	lowTextLabel->setText  ( tr("Width   ") );
	
	double currentWidth = currentRange.high() - currentRange.low();
	double currentPosition 
	  = 0.5 * ( currentRange.high() + currentRange.low() );
	
	highTextBox->setText ( QString("%1").arg(currentPosition) );
	lowTextBox->setText ( QString("%1").arg(currentWidth) );
      }
    else
      {
	highTextLabel->setText ( tr("High    ") );
	lowTextLabel->setText  ( tr("Low     ") );
      
	highTextBox->setText ( QString("%1").arg ( currentRange.high() ) );
	lowTextBox->setText ( QString("%1").arg ( currentRange.low() ) );
      }
}

void AxisWidget::setCut ( bool flag )
{
  m_isCut = flag;
}

void 
AxisWidget::
invalidRangeError ( const QString & bad )
{

  QString message = tr(
		        "Attempt to apply invalid range:\n\n"
			"%1\n\n"
			"Low end of range must be less than high end."
		       ).arg(bad);

  QMessageBox::critical ( this, // parent
			  tr("Range error"), // caption
			  message,
			  QMessageBox::Ok,
			  Qt::NoButton,
			  Qt::NoButton );

}

void AxisWidget::setLowText ( const  QString & s, bool readonly )
{
  lowTextBox->setText ( s );
  lowTextBox->setReadOnly ( readonly );
}

void AxisWidget::setHighText ( const QString & s, bool readonly )
{
  highTextBox->setText ( s );
  highTextBox->setReadOnly ( readonly );
}

QScrollBar *
AxisWidget::
getLowSlider ()
{
  return lowSlider;
}

int AxisWidget::getLowSliderValue ()
{
  return lowSlider->value();
}
  
void AxisWidget::setLowSliderValue ( int value )
{
  lowSlider->setValue ( value );
}

QScrollBar *
AxisWidget::
getHighSlider ()
{
  return highSlider;
}

int AxisWidget::getHighSliderValue ()
{
  return highSlider->value();
}
  
void AxisWidget::setHighSliderValue ( int value )
{
  highSlider->setValue ( value );
}

void AxisWidget::setAllDisabled ( bool flag )
{
  lowTextBox->setDisabled ( flag );
  highTextBox->setDisabled ( flag );
  lowSlider->setDisabled ( flag );
  highSlider->setDisabled ( flag );
  zoomPanCheckBox->setDisabled ( flag );
}

void
AxisWidget::
updateCutControlValues ( const Range & currentRange,
			 const Range & fullRange )
{

  assert ( m_isCut );

  zoomPanCheckBox->setChecked ( false );
  highTextLabel->setText ( tr("High    ") );
  lowTextLabel->setText  ( tr("Low     ") );

  highTextBox->setText ( QString("%1").arg(currentRange.high()) );
  lowTextBox->setText ( QString("%1").arg(currentRange.low()) );

  setSlider ( highSlider, currentRange.high(), fullRange );
  setSlider ( lowSlider, currentRange.low(), fullRange );

}

bool AxisWidget::isZoomPanChecked ()
{
  return zoomPanCheckBox->isChecked();
}

void AxisWidget::setZoomPan ( bool check, bool disabled )
{
  zoomPanCheckBox->setChecked ( check );
  zoomPanCheckBox->setDisabled ( disabled );
}

double
AxisWidget::
getWidthFromSlider ( const Range & fullRange )
{

  double width = ( ( (double)( lowSlider->value() -
			       lowSlider->minimum() ) ) / 
		   ( (double)( lowSlider->maximum() - 
			       lowSlider->minimum() ) ) *
		   fullRange.length()
		   );
  return width;
}

double
AxisWidget::
getPositionFromSlider ( const Range & fullRange )
{

  double position 
    = ( ( static_cast <double>( highSlider->value() -
				highSlider->minimum() ) ) / 
	( static_cast <double>( highSlider->maximum() - 
				highSlider->minimum() ) ) *
	fullRange.length()
	) + fullRange.low();
  return position;
}

double
AxisWidget::
getLowFromSlider ( const Range & fullRange )
{

  double low 
    = ( ( static_cast<double>( lowSlider->value() -
			       lowSlider->minimum() ) ) / 
	( static_cast<double>( lowSlider->maximum() - 
			       lowSlider->minimum() ) ) *
		 fullRange.length()
		 ) + fullRange.low();

  return low;
}

double
AxisWidget::
getHighFromSlider ( const Range & fullRange )
{

  double high = ( ( static_cast <double>( highSlider->value() -
					  highSlider->minimum() ) ) / 
		  ( static_cast<double>( highSlider->maximum() - 
					 highSlider->minimum() ) ) *
		  fullRange.length()
		  ) + fullRange.low();
  return high;
}

void
AxisWidget::
setSlider ( QScrollBar * s, double value, 
	    const Range & fullRange )
{
  int val 
    = static_cast < int >(
			  ( value - fullRange.low() ) / 
			  fullRange.length () *
			  static_cast<double> ( s->maximum() - s->minimum() )
			  ) + s->minimum();
  
  s->setValue ( val );
}  

void
AxisWidget::
setSliderZero ( QScrollBar * s, double value, 
		const Range & fullRange )
{
  int val 
    = static_cast<int>(
		       ( value ) / fullRange.length () *
		       static_cast<double>( s->maximum() - s->minimum() )
		       ) + s->minimum();
  
  s->setValue ( val );    
}
