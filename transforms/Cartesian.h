/* -*- mode: c++ -*- */

/** @file

Cartesian class interface

Copyright (C) 2002-2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Cartesian.h,v 1.4 2005/11/10 23:43:00 xiefang Exp $

*/

#ifndef _Cartesian_H_
#define _Cartesian_H_

#include "WCSTransform.h"


namespace hippodraw {

/** A transform that performs the Cartesian projection.  
*/
class MDL_HIPPOPLOT_API Cartesian : public WCSTransform
{

private:

  
public:

  /** The constructor. The constructor taking a UnaryTransform for its
      Z axis.  */
  Cartesian ( UnaryTransform * z );

  /** The copy constructor. */
  Cartesian ( const Cartesian & );

  /** The virtual destructor. */
  virtual ~Cartesian();

  /** Creates a new Transform object by copying an existing one. */
#ifdef CLONE_DEFECT
  virtual TransformBase * clone () const;
#else
  virtual Cartesian    * clone () const;
#endif


  virtual double aspectRatio () const;


};

} // namespace hippodraw

#endif // _Cartesian_H_
