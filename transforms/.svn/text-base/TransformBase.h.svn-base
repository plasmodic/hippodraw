/* -*- mode: c++ -*- */

/** @file

TransformBase  class interface

Copyright (C) 2001-2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: TransformBase.h,v 1.26 2005/11/10 21:17:33 xiefang Exp $

*/

#ifndef _TransformBase_H_
#define _TransformBase_H_

#include "pattern/libhippo.h"

#include <string>

namespace hippodraw {

class Range;

/** A transform that transforms coordinates from one coordinate system
 *  to another.
 *
  
 * @todo When a Coordinate class is implemented, we can replace many
 * functions below with ones that take a Coordinate as an argument.

 * @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu> 
*/

class MDL_HIPPOPLOT_API TransformBase
{

protected:


  /** Name of the Transform. */
  std::string m_name;

  /** The default constructor. */
  TransformBase ();

  /** The copy constructor. */
  TransformBase ( const TransformBase & t );

 public:

  /** The virtual destructor. */
  virtual ~TransformBase();

  /** Returns the name of the Transform. */
  const std::string & name () const;

  /** Creates a new Transform object by copying an existing one. */
  virtual TransformBase * clone () const = 0;

  /** Get the aspect ratio. */
  virtual double aspectRatio () const;

};

} // namespace hippodraw

#endif // _TransformBase_H_
