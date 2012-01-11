/* -*- mode: c++ -*- */

/** @file

QtViewFactory  class interface

Copyright (C)  2000, 2001 2004, 2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: QtViewFactory.h,v 1.6 2005/10/30 00:10:16 pfkeb Exp $

*/

#ifndef _QtViewFactory_H_
#define _QtViewFactory_H_

#include "libqthippo.h"

#include "graphics/ViewFactory.h"

namespace hippodraw {

class PlotterBase;

/** A @c ViewFactory which creates a view that uses Qt Drawing.
 */

class MDL_QTHIPPOPLOT_API QtViewFactory : public ViewFactory
{
public:

  /** The default constructor. */
  QtViewFactory();

  static ViewFactory * instance ();

private:
  /** The disallowed copy constructor. */
  QtViewFactory ( const QtViewFactory & );

  /** The disallowed assignment operator. */
  QtViewFactory & operator = ( const QtViewFactory & );

public:

  virtual ViewBase * createView ( PlotterBase * plotter ) const;

  virtual FontBase * createFont ( ) const;

};

} // namespace hippodraw

#endif // _QtViewFactory_H_
