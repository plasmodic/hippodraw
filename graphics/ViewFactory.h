/* -*- mode: c++ -*- */

/** @file

ViewFactory  class interface

Copyright (C) 2000-2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: ViewFactory.h,v 1.17 2005/10/30 00:10:12 pfkeb Exp $

*/

#ifndef _ViewFactory_H_
#define _ViewFactory_H_

#include "pattern/libhippo.h"

namespace hippodraw {

class FontBase;
class PlotterBase;
class ViewBase;

/** The abstract base class for a factory that creates objects closely
    related to the graphics subsystem in use.  The factory objects
    create objects of concrete classes derived from ViewBase and
    FontBase.  Known examples of concrete view factorys are
    JavaViewFactory, OpenGLViewFactory and QtViewFactory.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

*/

class MDL_HIPPOPLOT_API ViewFactory
{
protected:

  /** An instance of the default view factory. It should be created
      at the application initialization time for the default concrete
      view factory. */
  static ViewFactory * m_instance;

  /** The default constructor is disallowed. */
  ViewFactory();

private:
  /** The disallowed copy constructor. */
  ViewFactory( const ViewFactory & );

public:

  /** The virtual destructor.  The virtual destrcutor which is only
      needed to keep warning messages from some compilers such as Gcc
      4.0.0.
  */
  virtual ~ViewFactory () {};

  /** The public interface for getting to the concrete default view factory. */
  static ViewFactory * instance();

  /** Creates a view for the @a plotter. */
  virtual ViewBase * createView ( PlotterBase * plotter ) const = 0;

  /** Creates a default FontBase object.
   */
  virtual FontBase * createFont ( ) const = 0;

};

} // namespace hippodraw

#endif // _ViewFactory_H_
