/* -*- mode: c++ -*- */

/** @file

hippodraw;:EqualEntriesHist1Projector class interface

Copyright (C) 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: EqualEntriesHist1DProjector.h,v 1.3 2006/08/05 22:06:57 pfkeb Exp $

*/

#ifndef _EqualEntriesHist1DProjector_H_
#define _EqualEntriesHist1DProjector_H_

#include "NTupleProjector.h"
#include "axes/Range.h"

namespace hippodraw {

class Range;

/** A derived class of NTupleProjector that projects a column of data
    into a one dimensional histogram. This class is different from 
    DyHist1DProjector in that the bin width is a variable and the
    entries in any bins are equal. However, since some bins may have
    zero width, we combine bins together to avoid such bins.

    @author Xie Fang <xiefang@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API EqualEntriesHist1DProjector : public NTupleProjector
{

private:

  /** The label for the y axis.
   */
  std::string m_y_label;

  /** The number of bins.
   */
  unsigned int m_bin_num;

  /** The number of bins when dragging starts.
   */
  unsigned int m_start_num;

  /** The range of the data source.
   */
  Range m_range;

  /** Make sure the number of bins between 1 and number of rows. */
  void adjustNumberOfBins();

protected:

  /** The copy constructor. */
  EqualEntriesHist1DProjector ( const EqualEntriesHist1DProjector & projector );

  /** This function is called when the NTuple has been changed to a
      new one. The column indexes are checked for validity, and they 
      are reset to the last column if they are invalid. The weight 
      index is set to negative if it was invalid.*/
  virtual void changedNTuple();

  /** Function to add the acceptable point reps. */
  virtual void addPointReps ();

public:

  /** The default constructor. */
  EqualEntriesHist1DProjector( );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  ProjectorBase * clone();

  /** Returns the range of the data on a specified axis. For the X axis,
      it returns the range of the data.  For the Y axis, it returns the
      range of the binned data.
  */
  virtual Range dataRangeOn ( hippodraw::Axes::Type axis ) const;
  virtual Range valueRange ( ) const;
  Range dataRangeOnValue ( ) const;

  /** Returns the minimum positive value of the data on a specified
      axis. */
  virtual double getPosOn ( hippodraw::Axes::Type axis ) const;

  /** Returns the label of the axis. 
   */
  virtual const std::string & getYLabel ( bool density = false ) const;

  /** Get the Average of all projected values on the specified axis. */
  virtual double getAverage ( hippodraw::Axes::Type axis ) const;

  virtual bool isAxisBinned ( const std::string & axis ) const;

  /** Returns underflow. */
  virtual int getUnderflow () const;

  /** Returns overflow. */
  virtual int getOverflow () const;

  virtual DataSource * createNTuple () const;

  virtual void prepareValues ();

  /** Sets the number of bins in this class. By dragging the slider.*/
  virtual const Range& setBinWidth ( Axes::Type axis, int parm, bool dragging );
  /** Sets the number of bins in this class. By entering the number.*/
  virtual const Range& setBinWidth ( Axes::Type axis, double number );

  /** Get the number of bins in this class. To update the inspector. */
  virtual double getBinWidth ( Axes::Type axis ) const;

protected:

  virtual void fillProjectedValues ( DataSource * ntuple, 
				     bool in_range = false ) const;
};

} // namespace hippodraw

#endif // _EqualEntriesHist1DProjector_H_
