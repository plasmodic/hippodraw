/* -*- mode: c++ -*- */

/** @file

GlobalSinusoidal class interface

Copyright (C) 2002-2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: GlobalSinusoidal.h,v 1.3 2005/11/10 23:43:00 xiefang Exp $

*/

#ifndef _GlobalSinusoidal_H_
#define _GlobalSinusoidal_H_

#include "WCSTransform.h"


namespace hippodraw {

/** A transform that performs the GlobalSinusoidal projection.  
*/
class MDL_HIPPOPLOT_API GlobalSinusoidal : public WCSTransform
{

private:

  
public:

  /** The constructor. The constructor taking a UnaryTransform for its
      Z axis.  */
  GlobalSinusoidal ( UnaryTransform * z );

  /** The copy constructor. */
  GlobalSinusoidal ( const GlobalSinusoidal & );

  /** The virtual destructor. */
  virtual ~GlobalSinusoidal();

  /** Creates a new Transform object by copying an existing one. */
#ifdef CLONE_DEFECT
  virtual TransformBase * clone () const;
#else
  virtual GlobalSinusoidal    * clone () const;
#endif


  virtual double aspectRatio () const;


};

} // namespace hippodraw

#endif // _GlobalSinusoidal_H_
