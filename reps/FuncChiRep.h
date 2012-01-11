/* -*- mode: c++ -*- */

/** @file

FuncChiRep class interface

Copyright (C) 2001-2003   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FuncChiRep.h,v 1.18 2005/10/30 00:10:17 pfkeb Exp $

*/

#ifndef _FuncChiRep_H_
#define _FuncChiRep_H_

#include "reps/TextRepBase.h"

namespace hippodraw {

/** A PointRep that draws the Chi-squared of a function on a display. 

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API FuncChiRep : public TextRepBase
{

public:

  /** The default constructor. */
  FuncChiRep ( );

  /** The copy constructor. */
  FuncChiRep( const FuncChiRep & point_rep );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual RepBase * clone ();

  virtual void drawProjectedValues ( ProjectorBase & projector,
				     ViewBase & view );
};

} // namespace hippodraw

#endif // _FuncChiRep_H_
