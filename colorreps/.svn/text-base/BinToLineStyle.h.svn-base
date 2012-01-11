/* -*- mode: c++ -*- */

/** @file

BinToLineStyle class interface

Copyright (C) 2005  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToLineStyle.h,v 1.1 2006/10/02 23:00:29 xiefang Exp $
 
*/

#ifndef _BinToLineStyle_H_
#define _BinToLineStyle_H_

#include "BinToColor.h"

namespace hippodraw {

/** The class derived from the BinToColor class. The BinToLineStyle class
    implements the transformation of a value to a color by always
    yielding black.  This might be useful, for example, in drawing a
    ContourPlot either by itself, or overlaying a color plot.

    This is useful only for contour plot.

    @author Xie Fang <xiefang@stanford.edu>

 */
class MDL_HIPPOPLOT_API BinToLineStyle : public BinToColor
{

 public:

  /** The constructor.
   */
  BinToLineStyle ( const char * name );

  /** The copy constructor. */
  BinToLineStyle ( const BinToLineStyle & );

  virtual BinToColor * clone () const;

  virtual void doubleToColor ( double value, Color & color ) const;

  virtual bool acceptChangeColor () const;

};

} // namespace hippodraw

#endif // _BinToLineStyle_H_
