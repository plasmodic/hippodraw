/* -*- mode: c++ -*- */

/** @file

Some UI utility functions

Copyright (C) 2002-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id$

*/

class QButtonGroup;

namespace hippodraw {

  /*
   * An ugly workaround to assign group ids to the radio buttons in a button group.
   * I would guess the designer should set them automatically (Qt bug? My fault?)
   *  - FK
   */
  void setButtonGroupIds(QButtonGroup *grp);

  // In Qt4 QButtonGroups no longer have a member function to enable all buttons
  void buttonGroupSetEnabled(QButtonGroup *grp, bool enabled = true);

}

