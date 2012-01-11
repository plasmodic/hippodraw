/* -*- mode: c++ -*- */

/** @file

DyHist1Projector class interface

Copyright (C) 2000-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DyHist1DProjector.h,v 1.159 2006/06/23 22:09:33 pfkeb Exp $

*/

#ifndef _DyHist1DProjector_H_
#define _DyHist1DProjector_H_

#include "Hist1DProjImp.h"
#include "NTupleProjector.h"

namespace hippodraw {

class Range;

/** A derived class of BinningProjector that projects a column of data
    into a one dimensional histogram. A DataSource object is created
    and presented to a client.  Since this class is connected to an
    NTupleProjector, the binning parameters can be changed and this
    projector can redo the binning, hence it is called dynamic 1D
    histogram.

    @author Stephane Bonneaud <gandalf@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket B. Malde <sanket@stanford.edu>
    @author Matan Shacham <matan@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/

class MDL_HIPPOPLOT_API DyHist1DProjector : public Hist1DProjImp,
					    public NTupleProjector
{

private:

  /** Sets true if the bins are disconnected from the data source and
      are, thus, fixed. */
  bool m_fixed;

protected:

  /** The copy constructor.
  */
  DyHist1DProjector ( const DyHist1DProjector & projector );

  /** Projects the data source into bins.
   */
  virtual void execute();

  /** This function is called when the NTuple has been changed to a
      new one. The column indexes are checked for validity, and they 
      are reset to the last column if they are invalid. The weight 
      index is set to negative if it was invalid.*/
  virtual void changedNTuple();

public:

  /** The default constructor. */
  DyHist1DProjector();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual ProjectorBase * clone();

  /** Returns the range of the data on a specified axis. For the X axis,
      it returns the range of the data.  For the Y axis, it returns the
      range of the binned data.
  */
  virtual Range dataRangeOn ( hippodraw::Axes::Type axis ) const;

  /** Returns the minimum positive value of the data on a specified
      axis. */
  virtual double getPosOn ( hippodraw::Axes::Type axis ) const;

  /** Returns the label of the axis. If @a density is @c true, always
      return the label used when the axis is not scaled.  If @a
      density is @c false, then return the label used for scaled axis
      if the axis is scaled, otherwise return the label for the
      unscaled axis.
   */
  virtual const std::string & getYLabel ( bool density = false ) const;

  /** Get the Average of all projected values on the specified axis. */
  virtual double getAverage ( hippodraw::Axes::Type axis ) const;

  virtual bool isAxisBinned ( const std::string & axis ) const;

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

  /** Returns underflow. */
  virtual int getUnderflow () const;

  /** Returns overflow. */
  virtual int getOverflow () const;


};

} // namespace hippodraw

#endif // _DyHist1DProjector_H_
