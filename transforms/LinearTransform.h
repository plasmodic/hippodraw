/* -*- mode: c++ -*- */

/** @file

LinearTransform  class interface

Copyright (C) 2001-2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: LinearTransform.h,v 1.25 2005/10/30 00:10:18 pfkeb Exp $
*/

#ifndef _LinearTransform_H_
#define _LinearTransform_H_

#include "UnaryTransform.h"

namespace hippodraw {

class AxisModelBase;
class AxisTick;

/** A transform on one axis whose output is equal to the input.  That
     is, it does not change the input values.
 
     @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API LinearTransform : public UnaryTransform
{
private:

    /** The ticks generated by this transform.
   */
  std::vector < AxisTick > m_ticks;

void setFirstTick( AxisModelBase & );
  void setTickStep( AxisModelBase & );
  const std::vector < AxisTick > &  
  genTicks ( AxisModelBase & );

public:

  /** The default constructor. */
  LinearTransform ();

  /** The copy constructor. */
  LinearTransform ( const LinearTransform & );

  /** The virtual destructor. */
  virtual ~LinearTransform();

  /** Creates a new Transform object by copying an existing one. */
#ifdef CLONE_DEFECT
  virtual TransformBase *   clone () const;
#else
  virtual LinearTransform * clone () const;
#endif

  bool isLinear () const;

  /** Transforms the coordinate @ x. */
  virtual void transform ( double & x ) const;

  /** Inverse transforms the coordinate @a x. */
  virtual void inverseTransform ( double & x ) const;

  /** Transforms each element of the vector.  */
  virtual void transform ( std::vector< double > & x ) const;

  /** The following function validates the range. It returns a copy of
      the argument Range since all values are valid for a linear
      transform. */
  virtual void  validate ( Range & ) const;
  
  /** Sets the vector of ticks along the axis to which this 
      Linear transform is applied */
  virtual const std::vector < AxisTick > &
  setTicks ( AxisModelBase & axis );

  /** Sets the range of given axis to be a new "nice" within the
      limits given.
  */
  virtual const Range & adjustValues ( AxisModelBase & axis,
				       const Range & limit );
  
};

} // namespace hippodraw

#endif // _LinearTransform_H_
