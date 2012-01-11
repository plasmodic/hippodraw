/* -*- mode: c++ -*- */

/** @file

FunctionFactory class interface

Copyright (C) 2000-2003   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FunctionFactory.h,v 1.24 2005/10/30 00:10:11 pfkeb Exp $

*/

#ifndef _FunctionFactory_H_
#define _FunctionFactory_H_

#include "pattern/Factory.h"

namespace hippodraw {

class FunctionBase;

/** A Factory singleton class for creating objects whose class derives
    from FunctionBase. 

    @request Be able to add new functions from a shared library.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
 */

class MDL_HIPPOPLOT_API FunctionFactory : public Factory <FunctionBase>
{

 private:

  /** A pointer to the singleton object. */
  static FunctionFactory * s_instance;

  /** A private copy constructor to prevent copying. */
  FunctionFactory ( const FunctionFactory & );

  /** Initializes the factory with a set of default functions. */
  void initialize ();

protected:

  /** A default constructor for prevent creation except by the
      itself or the derived classes. */
  FunctionFactory();

 public:

  /** Returns a pointer to the singleton instance. */
  static FunctionFactory * instance ();

};

} // namespace hippodraw

#endif // _FunctionFactory_H_
