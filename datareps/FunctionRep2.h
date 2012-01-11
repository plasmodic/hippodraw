/* -*- mode: c++ -*- */

/** @file

FunctionRep2 class interface

Copyright (C) 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FunctionRep2.h,v 1.2 2005/10/30 00:10:10 pfkeb Exp $

*/

#ifndef _FunctionRep2_H_
#define _FunctionRep2_H_

#include "FunctionRep.h"

namespace hippodraw {

/** A derived class of FunctionRep that handles functions with 2
    coordinates.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API FunctionRep2 : public FunctionRep
{

private:

  /** The copy constructor.
   */
  FunctionRep2 ( const FunctionRep2 & );

public:

  /** The constructor taking FunctionBase object and target data.
   */
   FunctionRep2 ( FunctionBase * function, DataRep * rep );

  /** The virtual destructor.  Removes itself as an Observer of the
      target DataRep object.
  */
  virtual ~FunctionRep2 ();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment.
  */
  virtual DataRep * clone ();

  virtual bool hasAxis ( hippodraw::Axes::Type ) const;

  virtual void drawProjectedValues ( TransformBase * transform,
				     ViewBase * view );

};

} // namespace hippodraw

#endif // _FunctionRep2_H_
