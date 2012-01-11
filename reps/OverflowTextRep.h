/* -*- mode: c++ -*- */

/** @file

OverflowTextRep class interface

Copyright (C) 2001, 2003   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: OverflowTextRep.h,v 1.1 2006/04/12 20:45:54 xiefang Exp $

*/

#ifndef _OverflowTextRep_H_
#define _OverflowTextRep_H_

#include "reps/TextRepBase.h"

namespace hippodraw {

/** A TextRepBase class that draws the number of overflow entries.  
    
    @author Xie Fang <xiefang@stanford.edu>
*/
class MDL_HIPPOPLOT_API OverflowTextRep : public TextRepBase
{

public:

  /** The default constructor. */
  OverflowTextRep ( );

  /** The copy constructor. */
  OverflowTextRep ( const OverflowTextRep & point_rep );

  virtual RepBase * clone ();

  virtual void drawProjectedValues ( ProjectorBase & projector,
				     ViewBase & view );
};

} // namespace hippodraw

#endif // _OverflowTextRep_H_
