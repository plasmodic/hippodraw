/* -*- mode: c++ -*- */

/** @file

LineDataRep class interface

Copyright (C) 2004, 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: LineDataRep.h,v 1.8 2005/10/30 00:10:10 pfkeb Exp $

*/

#ifndef _LineDataRep_H_
#define _LineDataRep_H_

#include "DataRep.h"

namespace hippodraw {

/** A derived class of DataRep which displays a value as a line along
    the X or Y axis.

@author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

*/
class MDL_HIPPOPLOT_API LineDataRep
  : public hippodraw::DataRep
{

public:

  /** A constructor taking the value and axis arguments.
   */
  LineDataRep ( hippodraw::Axes::Type, double value );

  /** The default constructor, should never be called.
   */
  LineDataRep ();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual DataRep * clone ();

  /** Returns @c false as this class does not have NTuple bindings. */
  virtual bool hasNTupleBindings () const;

  /** Sets the value that will be displayed.
   */
  void setValue ( double value );

  virtual bool hasAxis ( hippodraw::Axes::Type ) const;

};

} // namespace hippodraw

#endif // _LineDataRep_H_
