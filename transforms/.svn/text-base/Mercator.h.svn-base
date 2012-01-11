/* -*- mode: c++ -*- */

/** @file

hippodraw::Mercator class interface

Copyright (C) 2005, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Mercator.h,v 1.4 2006/01/17 17:56:46 pfkeb Exp $

*/

#ifndef _Mercator_H_
#define _Mercator_H_

#include "WCSTransform.h"


namespace hippodraw {

/** A transform that performs the Mercator projection.  
    
    @bug @@@@ This transform draws outside of drawing rectangle.  Use
    useBinner.py to see it.

*/
class MDL_HIPPOPLOT_API Mercator : public WCSTransform
{

private:

  
public:

  /** The constructor. The constructor taking a UnaryTransform for its
      Z axis.  */
  Mercator ( UnaryTransform * z );

  /** The copy constructor. */
  Mercator ( const Mercator & );

  /** The virtual destructor. */
  virtual ~Mercator();

  /** Creates a new Transform object by copying an existing one. */
#ifdef CLONE_DEFECT
  virtual TransformBase * clone () const;
#else
  virtual Mercator    * clone () const;
#endif


  virtual double aspectRatio () const;


};

} // namespace hippodraw

#endif // _Mercator_H_
