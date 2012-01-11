/* -*- mode: c++ -*- */

/** @file

STG class interface

Copyright (C) 2002-2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: STG.h,v 1.3 2005/11/10 23:43:01 xiefang Exp $

*/

#ifndef _STG_H_
#define _STG_H_

#include "WCSTransform.h"


namespace hippodraw {

/** A transform that performs the STG projection.  
*/
class MDL_HIPPOPLOT_API STG : public WCSTransform
{

private:

  
public:

  /** The constructor. The constructor taking a UnaryTransform for its
      Z axis.  */
  STG ( UnaryTransform * z );

  /** The copy constructor. */
  STG ( const STG & );

  /** The virtual destructor. */
  virtual ~STG();

  /** Creates a new Transform object by copying an existing one. */
#ifdef CLONE_DEFECT
  virtual TransformBase * clone () const;
#else
  virtual STG    * clone () const;
#endif


  virtual double aspectRatio () const;


};

} // namespace hippodraw

#endif // _STG_H_
