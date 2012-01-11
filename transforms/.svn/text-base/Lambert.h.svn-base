/* -*- mode: c++ -*- */

/** @file

Lambert class interface

Copyright (C) 2002-2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Lambert.h,v 1.19 2005/11/10 23:43:00 xiefang Exp $

*/

#ifndef _Lambert_H_
#define _Lambert_H_

#include "WCSTransform.h"


namespace hippodraw {

/** A transform that performs the Lambert projection.  
*/
class MDL_HIPPOPLOT_API Lambert : public WCSTransform
{

private:

  
public:

  /** The constructor. The constructor taking a UnaryTransform for its
      Z axis.  */
  Lambert ( UnaryTransform * z );

  /** The copy constructor. */
  Lambert ( const Lambert & );

  /** The virtual destructor. */
  virtual ~Lambert();

  /** Creates a new Transform object by copying an existing one. */
#ifdef CLONE_DEFECT
  virtual TransformBase * clone () const;
#else
  virtual Lambert    * clone () const;
#endif


  virtual double aspectRatio () const;


};

} // namespace hippodraw

#endif // _Lambert_H_
