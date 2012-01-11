/* -*- mode: c++ -*- */

/** @file

hippodraw::Map2Projector class interface

Copyright (C)  2000-2006  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: Map2Projector.h,v 1.105 2006/08/05 22:06:57 pfkeb Exp $

*/

#ifndef _Map2Projector_H_
#define _Map2Projector_H_

#include "NTupleProjector.h"

namespace hippodraw {

/** A derived class of NTupleProjector that maps 2 ntuple columns to a
    two dimensional projection.  Optionally, additional columns can be
    mapped to X and Y error bars.  A NTuple is created with the
    projected values for presentation to clients.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/
class MDL_HIPPOPLOT_API Map2Projector : public NTupleProjector
{

  /** The label for the X error binding option. */
  std::string m_x_option;

  /** The label for the Y error binding option. */
  std::string m_y_option;

 protected:

  /** The copy constructor. */
  Map2Projector ( const Map2Projector & projector );

  /** This function is called when the ntuple has been changed to a
      new one. The X and Y axis indexes are checked for validity and
      reset to the last column if invalid.  The X and Y error indexes
      are checked for validity and reset to negative if they are
      invalid. */
  virtual void changedNTuple();

  /** Function to add the acceptable point reps. */
  virtual void addPointReps ();

 public:

  /** This default constructor binds to the first two columns. */
  Map2Projector();

  /** The destructor. */
  virtual ~Map2Projector();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  ProjectorBase * clone();

  /** Sets whether the X error options are to be enabled or not.
      @todo Remove this and setYerrorOption() in favor of passing as
      argument s string with the name of an error option. */
  virtual void setXErrorOption ( bool enable );

  /** Sets whether the Y error options are to be enabled or not.
   */
  virtual void setYErrorOption ( bool enable );

  /** Returns the range of the data on the specified axis.  The range
      may need to be calculated. */
  virtual Range dataRangeOn ( hippodraw::Axes::Type ) const;
  virtual Range valueRange () const;

  /** Returns the minimum positive value of the data on a specified
      axis. */
  virtual double getPosOn ( hippodraw::Axes::Type axis ) const;

  virtual DataSource * createNTuple () const;
  virtual void fillProjectedValues ( DataSource * ntuple, 
				     bool in_range = false ) const;
  virtual void prepareValues ();


};

} // namespace hippodraw

#endif // _Map2Projector_H_
