/* -*- mode: c++ -*- */

/** @file

SIN class interface

Copyright (C) 2002-2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: SIN.h,v 1.3 2005/11/10 23:43:01 xiefang Exp $

*/

#ifndef _SIN_H_
#define _SIN_H_

#include "WCSTransform.h"


namespace hippodraw {

/** A transform that performs the SIN projection.  
*/
class MDL_HIPPOPLOT_API SIN : public WCSTransform
{

private:

  
public:

  /** The constructor. The constructor taking a UnaryTransform for its
      Z axis.  */
  SIN ( UnaryTransform * z );

  /** The copy constructor. */
  SIN ( const SIN & );

  /** The virtual destructor. */
  virtual ~SIN();

  /** Creates a new Transform object by copying an existing one. */
#ifdef CLONE_DEFECT
  virtual TransformBase * clone () const;
#else
  virtual SIN    * clone () const;
#endif


  virtual double aspectRatio () const;


};

} // namespace hippodraw

#endif // _SIN_H_
