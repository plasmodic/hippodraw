/** @file

  Some UI utilities

  Copyright (C) 2002-2006   The Board of Trustees of The Leland Stanford
  Junior University.  All Rights Reserved.

  $Id$

*/

#include <QButtonGroup>
#include <QAbstractButton>


namespace hippodraw {

  /*
   * An ugly workaround to assign group ids to the radio buttons in a button group.
   * I would guess the designer should set them automatically (Qt bug? My fault?)
   *  - FK
   */
  void setButtonGroupIds(QButtonGroup *grp)
  {
    for ( int i = 0; i < grp->buttons().size(); ++i )
      grp -> setId ( grp->buttons().at(i), i );
  }


  // In Qt4 QButtonGroups no longer have a member function to enable all buttons
  void buttonGroupSetEnabled(QButtonGroup *grp, bool enabled)
  {
    QAbstractButton *btn;
    foreach ( btn, grp->buttons() ) {
      btn -> setEnabled ( enabled );
    }
  }

}
