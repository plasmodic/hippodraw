/* -*- mode: c++ -*- */

/** @file

FunctionRep1 class interface

Copyright (C) 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FunctionRep1.h,v 1.55 2005/10/30 00:10:10 pfkeb Exp $

*/

#ifndef _FunctionRep1_H_
#define _FunctionRep1_H_

#include "FunctionRep.h"

namespace hippodraw {

/** A derived class of FunctionRep that handles functions with only 1
    coordinate.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API FunctionRep1 : public FunctionRep
{

private:

  /** The copy constructor.
   */
  FunctionRep1 ( const FunctionRep1 & );

public:

  /** The constructor taking FunctionBase object and target data.
   */
   FunctionRep1 ( FunctionBase * function, DataRep * rep );

  /** The virtual destructor.  Removes itself as an Observer of the
      target DataRep object.
  */
  virtual ~FunctionRep1 ();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment.
  */
  virtual DataRep * clone ();

  virtual bool hasAxis ( hippodraw::Axes::Type ) const;

  virtual void drawProjectedValues ( TransformBase * transform,
				     ViewBase * view );

};

} // namespace hippodraw

#endif // _FunctionRep1_H_
