/* -*- mode: c++ -*- */

/** @file

StHist1DProjector class interface

Copyright (C) 2003-2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: StHist1DProjector.h,v 1.151 2006/04/12 20:45:53 xiefang Exp $

*/

#ifndef _StHist1DProjector_H_
#define _StHist1DProjector_H_

#include "projectors/Hist1DProjImp.h"

namespace hippodraw {

/** A derived class of ProjectorBase that projects data accumulated by
    calls to its fill function into a one dimensional histogram. A
    DataSource object is created and presented to a client.  The
    binning parameters are fixed hence it is called static 1D
    histogram-mer.

    @author Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API StHist1DProjector : public Hist1DProjImp
{

private:

  /** The title of the histogram. */
  std::string m_title;

  /** The label of the x axis. */
  std::string m_x_label;

  /** The label of the y axis. It depends on the bin Width. */
  std::string m_y_label;

public:

  /** The default constructor. */
  StHist1DProjector();

  /** The copy constructor. */
  StHist1DProjector( const StHist1DProjector & projector );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  ProjectorBase * clone();

  /** Returns @a false as this class' data source is never empty even
      if no filling has occurred yet.
  */
  bool isEmpty () const;

  /** Returns the range of the data on a specified axis. For the X axis,
      it returns the range of the data.  For the Y axis, it returns the
      range of the binned data.

      @todo Implement special handling if histogram is empty.
  */
  virtual Range dataRangeOn ( hippodraw::Axes::Type axis ) const;

  /** Returns the minimum positive value of the data on a specified
      axis. */
  virtual double getPosOn ( hippodraw::Axes::Type axis ) const;

  /** Returns @c true if the intended axis is "X", otherwise returns
      false. */
  virtual bool isAxisBinned ( const std::string & axis ) const;

  /** Returns the label (title) of the x axis.
   */
  const std::string & getXLabel () const;

  /** Returns the label for the Y axis.  Returns the label for the Y
      axis.  The argument @ flag is ignored.
  */
  const std::string & getYLabel ( bool flag = false ) const;

  /** Sets the Y axis label. */
  void setYLabel ();

  /** Get the Average of all projected values on the specified axis. */
  virtual double getAverage ( hippodraw::Axes::Type axis ) const;

  /* The following need to be implemented. */
  virtual const std::string & getTitle() const;
  virtual int getNumberOfEntries () const;
  
  /* Get the underflow. */
  virtual int getUnderflow () const;

  /* Get the overflow. */
  virtual int getOverflow () const;

  /** Add the values to the histogram.  The first element of the
      vector is the value to be accumulated into the histogram. If the
      size of the vector is 2, then the second element is the weight,
      otherwise the weight is 1.0.
   */
  virtual void addValues ( const std::vector < double > & v );

  /** Sets the Range of the binner. If the BinsBase object is empty,
      the sets the Range of the BinsBase object for axes @a axis to @a
      range.  If @a const_width is @c true, the bin width is not
      changed, otherwise it changes to keep the same number of bins.
      If the BinsBase object is not empty, does nothing.
   */
  virtual void setBinnerRange ( hippodraw::Axes::Type axis,
				const Range & range,
				bool const_width );

  /** Resets the bins. This interface is for the StHist1DProjector
   */

  virtual void reset ();

};

} // namespace hippodraw

#endif // _StHist1DProjector_H_
