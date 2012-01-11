/* -*- mode: c++ -*- */

/** @file

UnderflowTextRep class interface

Copyright (C) 2001, 2003   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: UnderflowTextRep.h,v 1.1 2006/04/12 20:45:54 xiefang Exp $

*/

#ifndef _UnderflowTextRep_H_
#define _UnderflowTextRep_H_

#include "reps/TextRepBase.h"

namespace hippodraw {

/** A TextRepBase class that draws the number of underflow entries.  
    
    @author Xie Fang <xiefang@stanford.edu>
*/
class MDL_HIPPOPLOT_API UnderflowTextRep : public TextRepBase
{

public:

  /** The default constructor. */
  UnderflowTextRep ( );

  /** The copy constructor. */
  UnderflowTextRep ( const UnderflowTextRep & point_rep );

  virtual RepBase * clone ();

  virtual void drawProjectedValues ( ProjectorBase & projector,
				     ViewBase & view );
};

} // namespace hippodraw

#endif // _UnderflowTextRep_H_
