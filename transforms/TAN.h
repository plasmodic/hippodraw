/* -*- mode: c++ -*- */

/** @file

TAN class interface

Copyright (C) 2002-2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: TAN.h,v 1.3 2005/11/10 23:43:01 xiefang Exp $

*/

#ifndef _TAN_H_
#define _TAN_H_

#include "WCSTransform.h"


namespace hippodraw {

/** A transform that performs the TAN projection.  
*/
class MDL_HIPPOPLOT_API TAN : public WCSTransform
{

private:

  
public:

  /** The constructor. The constructor taking a UnaryTransform for its
      Z axis.  */
  TAN ( UnaryTransform * z );

  /** The copy constructor. */
  TAN ( const TAN & );

  /** The virtual destructor. */
  virtual ~TAN();

  /** Creates a new Transform object by copying an existing one. */
#ifdef CLONE_DEFECT
  virtual TransformBase * clone () const;
#else
  virtual TAN    * clone () const;
#endif


  virtual double aspectRatio () const;


};

} // namespace hippodraw

#endif // _TAN_H_
