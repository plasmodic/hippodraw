/* -*- mode: c++ -*- */

/** @file

hippodraw::TextRepFactory class interface

Copyright (C) 2001, 2003, 2005, 2006  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: TextRepFactory.h,v 1.15 2006/02/07 00:34:22 pfkeb Exp $

*/

#ifndef _TextRepFactory_H_
#define _TextRepFactory_H_

#include "pattern/Factory.h"
#include "reps/RepBase.h"

namespace hippodraw {

/** A factory for textual representations. 

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API TextRepFactory : public Factory < RepBase >
{

 private:

  /** A pointer to the singleton object. */
  static TextRepFactory * s_instance;

  /** A private copy constructor to avoid copying. */
  TextRepFactory( const TextRepFactory & );

  /** Initializes the factory with a set of default TextReps. */
  void initialize ();

protected:

  /** A default constructor for avoiding creation except by itself or
      derived classes. */
  TextRepFactory();

 public:

  /** Returns a pointer to the singleton instance. */
  static TextRepFactory * instance ();

};

} // namespace hippodraw

#endif // _TextRepFactory_H_
