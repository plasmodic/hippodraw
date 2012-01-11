/** @file

hippodraw::CanvasSettings class implementation

Copyright (C)  2004-2006  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: CanvasSettings.cxx,v 1.25 2006/06/16 18:17:28 pfkeb Exp $

*/

#include "CanvasSettings.h"

#include <qspinbox.h>
#include <qcheckbox.h>
#include <qradiobutton.h>

using namespace hippodraw;

CanvasSettings::
CanvasSettings ( QWidget * parent )
     : QDialog ( parent )
{
   setupUi ( this );
}

void
CanvasSettings::
cancel ()
{
  done ( CanvasSettings::Rejected );
}

void
CanvasSettings::
saveRetile ()
{
  done ( CanvasSettings::AcceptedAndRetile );
}

void
CanvasSettings::
retile_clicked ()
{
  done ( CanvasSettings::Retile );
}

int
CanvasSettings::
getWidthNumber () const
{
  return m_h_spin -> value();
}

void
CanvasSettings::
setWidthNumber ( int number )
{
  m_h_spin -> setValue ( number );
}

int
CanvasSettings::
getHeightNumber () const
{
  return m_v_spin -> value();
}

void
CanvasSettings::
setHeightNumber ( int number )
{
  m_v_spin -> setValue ( number );
}

void
CanvasSettings::
portraitRadioButton_toggled ( bool )
{
  orientationChanged();
}

void
CanvasSettings::
orientationChanged ()
{
  int vSpinNumber = getHeightNumber ();
  int hSpinNumber = getWidthNumber ();

  setHeightNumber( hSpinNumber );
  setWidthNumber( vSpinNumber );
  
}

QPrinter::Orientation
CanvasSettings::orientation()
{
  if ( portraitRadioButton -> isChecked() ) {
    return QPrinter::Portrait;
  }
  else {
    return QPrinter::Landscape;
  }

  return QPrinter::Portrait;
}

void CanvasSettings::setOrientation(  QPrinter::Orientation orientation )
{
  if ( orientation == QPrinter::Portrait ) {
    portraitRadioButton -> setChecked( true );
  }
  else {
    landscapeRadioButton -> setChecked( true );
  }
}

bool CanvasSettings::printerBounds()
{
  return m_visibleMarginRadioButton -> isChecked();
}

void CanvasSettings::setPrinterBounds( bool flag )
{
  if( flag == true )
    m_visibleMarginRadioButton -> setChecked( true );
  else
    m_invisibleMarginRadioButton -> setChecked( true );
}

bool
CanvasSettings::
getAddSelected () const
{
  return m_add_selected -> isChecked ();
}

void
CanvasSettings::
setAddSelected ( bool yes )
{
  m_add_selected -> setChecked ( yes );
}

bool
CanvasSettings::
ignorePrinterMargin () const
{
  return ignore_margin -> isChecked ();
}

void
CanvasSettings::
setIgnorePrinterMargin ( bool yes )
{
  ignore_margin -> setChecked ( yes );
}
