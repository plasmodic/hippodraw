/* -*- mode: c++; -*- */

/** @file

ProfileProjector class interface

Copyright (C)  2000-2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: ProfileProjector.h,v 1.124 2005/10/30 00:10:14 pfkeb Exp $

*/

#ifndef _ProfileProjector_H_
#define _ProfileProjector_H_

#include "BinningProjector.h"
#include "NTupleProjector.h"

namespace hippodraw {

/** A derived class of BinningProjector which projects to a profile
    display. A DataSource object is created and the objects are
    presented to a client.

    @bug The total number of entries doesn't add up to the correct
    value.  Try displays.py and look at stats panel.

    @author Stephane Bonneaud <gandalf@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Matan Shacham <matan@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/

class MDL_HIPPOPLOT_API ProfileProjector : public BinningProjector,
			 public NTupleProjector
{

protected:

  /** The copy constructor. */
  ProfileProjector ( const ProfileProjector & projector );

  virtual void execute();

  /** This function is called when the DataSource has been changed to a
      new one. The column indexes are checked for validity and reset
      to the last column if they were invalid. */
  virtual void changedNTuple();

  /** Function to add the acceptable point reps. */
  virtual void addPointReps ();

public:

  /** The default constructor. */
  ProfileProjector();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  ProjectorBase * clone();

  virtual Range valueRange () const;

  /** Returns the range of the data on the specified axis.  The range
      may need to be calculated. */
  virtual Range dataRangeOn ( hippodraw::Axes::Type ) const;

  /** Returns the minimum positive value of the data on a specified
      axis. */
  virtual double getPosOn ( hippodraw::Axes::Type axis ) const;

  virtual bool isAxisBinned ( const std::string & axis ) const;

  virtual void setRange ( hippodraw::Axes::Type axis, bool const_width );

  /** Sets the Range of the binner. Sets the Range of the BinsBase
      object for axes @a axis to @a range.  If @a const_width is @c
      true, the bin width is not changed, otherwise it changes to keep
      the same number of bins.
   */
  virtual void setBinnerRange ( hippodraw::Axes::Type axis,
				const Range & range,
				bool const_width );

  /** Implements Observer pattern.  Receives the update from the
      Observable object and directs it to either
      BinningProject::update or NTupleProjector::update depending on
      class of @a object.
  */
  virtual void update ( const Observable * object );

  /** Implements Observer pattern.  Receives the willDelete from the
      Observable object and directs it to either
      BinningProjector::update or NTupleProjector::update depending on
      the class of @a object.
  */
  virtual void willDelete ( const Observable * object );

};

} // namespace hippodraw

#endif // _ProfileProjector_H_
