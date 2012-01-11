/* -*- mode: c++ -*- */

/** @file

BinToKamaeScale class interface

Copyright (C) 2000, 2003, 2004  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToKamaeScale.h,v 1.2 2005/10/30 00:10:17 pfkeb Exp $
 
*/

#ifndef _BinToKamaeScale_H_
#define _BinToKamaeScale_H_

#include "BinToColor.h"

namespace hippodraw {

/** The class derived from the BinToColor class. The BinToKamaeScale
    class implements the transformation of a value to a color with an
    algorithm invented by Tune Kamae.

    @author Tune Kamae <kamae@slac.stanford.edu>
    @author Paul F. Kunz <paul_kunz@slac.stanford.edu>

 */
class MDL_HIPPOPLOT_API BinToKamaeScale : public BinToColor
{
 public:

  /** The constructor.
   */
  BinToKamaeScale ( const char * name );

  /** The copy constructor. */
  BinToKamaeScale ( const BinToKamaeScale & );

  virtual BinToColor * clone () const;

  virtual void doubleToColor ( double value, Color & color ) const;

};

} // namespace hippodraw

#endif // _BinToKamaeScale_H_
