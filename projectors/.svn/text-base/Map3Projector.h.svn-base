/* -*- mode: c++ -*- */

/** @file

hippodraw::Map3Projector class interface

Copyright (C)  2000-2006  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: Map3Projector.h,v 1.101 2006/08/05 22:06:57 pfkeb Exp $

*/

#ifndef _Map3Projector_H_
#define _Map3Projector_H_

#include "NTupleProjector.h"

#include "axes/Range.h"

namespace hippodraw {

/** A derived class of NTupleProjector that maps 3 DataSource columns
    to a 3-dimensional projection.  A DataSource is created with the
    projected values for presentation to clients.
     
     @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
     @author Oded Wurman <owurman@stanford.edu>
*/
class MDL_HIPPOPLOT_API Map3Projector : public NTupleProjector
{

private:

  /** The Z projection range. */
  Range m_z_range;

 protected:

  /** The copy constructor. */
  Map3Projector ( const Map3Projector & projector );

  /** This function is called when the ntuple has been changed to a
      new one. The column indexes are checked for validity and reset
      to the last column if they were invalid. */
  virtual void changedNTuple();

  bool m_z_log;
  bool m_z_auto_scale;
  bool m_z_auto_tick;

  /** Function to add the acceptable point reps. */
  virtual void addPointReps ();

 public:

  /** This default constructor binds to the first three columns. */
  Map3Projector();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  ProjectorBase * clone();

  /** Finds the range of the projected values. The range of the Z
      projection is returned. */
  virtual Range valueRange() const;

  /** Returns the range of the data on the specified axis. The range
      may need to be calculated. */
  virtual Range dataRangeOn ( hippodraw::Axes::Type ) const;

  /** Returns the minimum positive value of the data on a specified
      axis. */
  virtual double getPosOn ( hippodraw::Axes::Type axis ) const;

  /** Returns the label (title) of the z axis. Gets it from the ntuple
      object. */
  const std::string & getZLabel() const;

  /** Get the z value at the specified point (x,y). */
  virtual double getZValue ( double x, double y ) const;

  virtual DataSource * createNTuple () const;

  virtual void prepareValues ();

protected:

  virtual void fillProjectedValues ( DataSource * ntuple, 
				     bool in_range = false ) const;

};

} // namespace hippodraw

#endif // _Map3Projector_H_
