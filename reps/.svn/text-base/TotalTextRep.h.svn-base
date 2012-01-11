/* -*- mode: c++ -*- */

/** @file

TotalTextRep class interface

Copyright (C) 2001, 2003   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: TotalTextRep.h,v 1.18 2005/10/30 00:10:18 pfkeb Exp $

*/

#ifndef _TotalTextRep_H_
#define _TotalTextRep_H_

#include "reps/TextRepBase.h"

namespace hippodraw {

/** A TextRepBase class that draws projected values as a total.  
    
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API TotalTextRep : public TextRepBase
{

public:

  /** The default constructor. */
  TotalTextRep ( );

  /** The copy constructor. */
  TotalTextRep ( const TotalTextRep & point_rep );

  virtual RepBase * clone ();

  virtual void drawProjectedValues ( ProjectorBase & projector,
				     ViewBase & view );
};

} // namespace hippodraw

#endif // _TotalTextRep_H_
