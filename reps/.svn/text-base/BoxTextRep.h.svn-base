/* -*- mode: c++ -*- */

/** @file

hippodraw::BoxTextRep class interface

Copyright (C) 2003, 2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BoxTextRep.h,v 1.11 2007/01/26 22:55:32 xiefang Exp $

*/

#ifndef _BoxTextRep_H_
#define _BoxTextRep_H_

#include "TextRepBase.h"

namespace hippodraw {

/** A TextRepBase class that draws a user defined string.

    @request Be able to edit the text.

    @author Sanket B. Malde <sanket@slac.stanford.edu>
 */
class MDL_HIPPOPLOT_API BoxTextRep : public TextRepBase
{

public:

  /** The default constructor. */
  BoxTextRep ( );

  /** The copy constructor. */
  BoxTextRep ( const BoxTextRep & rep );

  virtual RepBase * clone ();

  virtual void drawProjectedValues ( ProjectorBase & projector,
				     ViewBase & view );


};

} // namespace hippodraw

#endif // _BoxTextRep_H_
