/* -*- mode: c++ -*- */

/** @file

ARC class interface

Copyright (C) 2002-2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: ARC.h,v 1.3 2005/11/10 23:43:00 xiefang Exp $

*/

#ifndef _ARC_H_
#define _ARC_H_

#include "WCSTransform.h"


namespace hippodraw {

/** A transform that performs the ARC projection.  
*/
class MDL_HIPPOPLOT_API ARC : public WCSTransform
{

private:

  
public:

  /** The constructor. The constructor taking a UnaryTransform for its
      Z axis.  */
  ARC ( UnaryTransform * z );

  /** The copy constructor. */
  ARC ( const ARC & );

  /** The virtual destructor. */
  virtual ~ARC();

  /** Creates a new Transform object by copying an existing one. */
#ifdef CLONE_DEFECT
  virtual TransformBase * clone () const;
#else
  virtual ARC    * clone () const;
#endif


  virtual double aspectRatio () const;


};

} // namespace hippodraw

#endif // _ARC_H_
