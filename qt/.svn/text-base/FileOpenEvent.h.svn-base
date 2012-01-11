/* -*- mode: c++ -*- */

/** @file

hippodraw::FileOpenEvent  class interface

Copyright (C) 2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: FileOpenEvent.h,v 1.3 2006/06/30 23:02:49 pfkeb Exp $

*/

#ifndef _FileOpenEvent_H_
#define _FileOpenEvent_H_

#include "libqthippo.h"

#include <qevent.h>

namespace hippodraw {

/** The QFileOpenEvent class provides an event that will be sent when
    there is a request to open a file.

    File open events will be sent to the QApplication::instance() when
    the operating system requests that a file be opened. This is a
    high-level event that can be caused by different user actions
    depending on the user's desktop environment; for example, double
    clicking on an file icon in the Finder on Mac OS X.

    @note This class will be replaced with QFileOpenEvent in Qt 4.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
 */

  const int OpenEventType = QEvent::User + 116;

class MDL_QTHIPPOPLOT_API FileOpenEvent : public QEvent
{

private: 

  /** The name of the file to be opened.
   */
  QString m_file_name;

  /** The copy constructor.
   */
  FileOpenEvent ( const FileOpenEvent & );

public:

  /** Public constructor with file name as argument.
   */
  FileOpenEvent ( const QString & filename );

  /** The required virtual destructor.
   */
  virtual ~FileOpenEvent ();

  /** Returns the name of the file to be opened.
   */
  QString file () const;

};

} // namespace hippodraw

#endif // _FileOpenEvent_H_
