/* -*- mode: c++ -*-  */

/** @file

hippodraw::HdThread class declaration

Copyright (C) 2001-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: HdThread.h,v 1.16 2006/03/21 00:33:08 pfkeb Exp $

*/

#ifndef HDTHREAD_H
#define HDTHREAD_H

#include "qt/libqthippo.h"

#include <qthread.h>

namespace hippodraw {

class QtApp;

/** A derived class of QThread that runs the QApplication object.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_QTHIPPOPLOT_API HdThread : public QThread
{
protected:

  /** Creates the application object and runs it.
   */
  virtual void run ();

public:

  /** The default constructor. */
  HdThread ( );

  /** The destructor.  Does nothing but keep gcc warning messages
      away. */
  virtual ~HdThread ();

};

} // namespace hippodraw

#endif // HDTHREAD_H
