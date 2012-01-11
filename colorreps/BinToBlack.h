/* -*- mode: c++ -*- */

/** @file

BinToBlack class interface

Copyright (C) 2005  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToBlack.h,v 1.29 2006/09/25 16:19:25 pfkeb Exp $
 
*/

#ifndef _BinToBlack_H_
#define _BinToBlack_H_

#include "BinToColor.h"

namespace hippodraw {

/** The class derived from the BinToColor class. The BinToBlack class
    implements the transformation of a value to a color by always
    yielding black.  This might be useful, for example, in drawing a
    ContourPlot either by itself, or overlaying a color plot.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
	@author Tao Xu <taox@stanford.edu>

 */
class MDL_HIPPOPLOT_API BinToBlack : public BinToColor
{

 public:

  /** The constructor.
   */
  BinToBlack ( const char * name );

  /** The copy constructor. */
  BinToBlack ( const BinToBlack & );

  virtual BinToColor * clone () const;

  virtual void doubleToColor ( double value, Color & color ) const;

  virtual bool acceptChangeColor () const;

};

} // namespace hippodraw

#endif // _BinToBlack_H_
