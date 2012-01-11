/* -*- mode: c++ -*- */

/** @file

AIR class interface

Copyright (C) 2002-2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: AIR.h,v 1.3 2005/11/10 23:43:00 xiefang Exp $

*/

#ifndef _AIR_H_
#define _AIR_H_

#include "WCSTransform.h"


namespace hippodraw {

/** A transform that performs the AIR projection.  
*/
class MDL_HIPPOPLOT_API AIR : public WCSTransform
{

private:

  
public:

  /** The constructor. The constructor taking a UnaryTransform for its
      Z axis.  */
  AIR ( UnaryTransform * z );

  /** The copy constructor. */
  AIR ( const AIR & );

  /** The virtual destructor. */
  virtual ~AIR();

  /** Creates a new Transform object by copying an existing one. */
#ifdef CLONE_DEFECT
  virtual TransformBase * clone () const;
#else
  virtual AIR    * clone () const;
#endif


  virtual double aspectRatio () const;


};

} // namespace hippodraw

#endif // _AIR_H_
