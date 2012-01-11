/* -*- mode: c++; -*- */
/** @file

hippodraw::BinnerAxis class interface

Copyright (C) 2000-2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinnerAxis.h,v 1.53 2007/04/02 22:42:11 pfkeb Exp $

*/

#ifndef _BinnerAxis_H_
#define _BinnerAxis_H_

#include "axes/Range.h"

#include <string>

namespace hippodraw {

/** The base class for the BinnerAxis hierarchy.  The base class of a
    hierarchy to to maintain information on the binned axis.  It is a
    helper class to the BinsBase hierarchy.  The purpose of this
    hierarchy is two fold.  One is given a value, to return the index
    of the bin.  Second, give the index of a bin, to return the
    information on that bin such as its mean coordinate value and its
    width.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Matan Shacham <matan@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/

class MDL_HIPPOPLOT_API BinnerAxis
{

private:

  /** The name of the binner. */
  const std::string m_name;

  /** The status of dragging. */
  mutable bool m_dragging;

  /** Returns the number of bins given the width parameter @a width. 
   */
  virtual int getNob ( double width ) const= 0;

  /** Gets the number of bins from range. */
  virtual int getNob (const Range & range ) const = 0;

protected:

  /** The default number of bins. The default number of bins used by
      the default constructor. */
  static int s_num_bins;

  /** The scale factor used to calculate new bin width from slider. 
   */
  static double s_bin_factor;

  /** The number of bins. */
  mutable int m_num_bins;

  /** The range of the bins.  The lower edge of the first bin and the
      upper edge of the last bin are represented by this range. */
  Range m_range;

  /** The offset. */
  double m_offset;

  /** The width parameter for the binning.  A negative number
      indicates that no value has been set yet. */
  double m_width;

  /** The coordinates of the edges of bins.  Since there are m_num_bins
      this is a vector of size 1 + m_num_bins. A size zero
      indicates that no value has been set yet. */
  std::vector< double > m_bin_edges;
  
  /** The Range at the start of dragging. */
  mutable Range m_range_start;

  /** The width parameter at the start of dragging. */
  mutable double m_width_start;

  /** The copy constructor. */
  BinnerAxis ( const BinnerAxis & binner );

  /** A constructor taking binner @a name as argument. */
  BinnerAxis ( const char * name );

  /** Calculates the bin width parameter given the number of bins.
   */
  virtual double calcWidthParm ( int number ) const = 0;

 public:

  /** The destructor. */
  virtual ~BinnerAxis();
  
  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual BinnerAxis * clone() = 0;

  /** Returns the name of this binner. */
  const std::string & name () const;

  /** Returns @c true if all the bins have the same width.  The
      implementation in this base class is to return @c false.  In the
      special case where all bins have equal widths, this function
      should be overridden in a derived class.  The intent is to allow
      classes that use such special case to display an alternate label
      for an axis, such as <em> Entries per bin. </em> */
  virtual bool hasEqualWidths () const;

  /** Returns the lower edge of the first bin. */
  double axisGetLow() const;

  /** Returns the lower edge of the last + 1 bin. */
  double axisGetHigh() const;

  /** Returns the range. */
  const Range & getRange() const;

  /** Returns the number of bins. */
  int axisNumberOfBins () const;

  /** Returns the constant width parameter. */
  virtual double getConstWid ( ) const = 0;

  /** Returns the bin width parameter.  If the bin widths are not
      equal, then return a parameter used to calculate the bin
      widths. */
  virtual double getBinWidth ( ) const = 0;

  /** Sets m_num_bins. */
  virtual void axisSetNumberOfBins( int nb ) = 0;

  /** Returns the number of the bin in which the x value is
      situated. */
  virtual int axisBinNumber ( double x ) const = 0;

  /** Returns the coordinate.  Returns the coordinate of the bin at
      index @a i.  The coordinate is not necessarily midway between
      the bin edges.
  */
  virtual double getCoordinate ( int i ) const = 0;

  /** Returns the width of each bin. */
  virtual double axisBinWidth ( int i ) const = 0;

  /** Returns the vector of coordinates of the bin edges i.e returns
      the member m_edges. If m_edges is not set it sets it too */
  virtual const std::vector< double > & binEdges ();
  
  /** Sets the bin width and adjusts the range and number of bins
      accordingly.  Returns the adjusted range. */
  virtual const Range & setBinWidth ( double width ) = 0;

  /** Calculates and returns a double corresponding to a bin offset
      when dragging a slider control. The current range is saved when
      first called with @a dragging @c true.  The @a parm is expected
      to range from 0 to 50 and offsets are calculated with respect
      the the bin width parameter. */
  virtual double calcOffset ( int parm, bool dragging ) const = 0;

  /** Returns the offset. */
  virtual double getOffset () const = 0;

  /** Sets the offset and adjusts the range accordingly. */
  virtual const void setOffset ( double offset ) = 0;

 /** Sets the range. Adjusts the
  number of bins if @a hold_width is @c true, otherwise adjusts the
  width parameter. Returns the adjusted range. */
  virtual const Range & setRange ( const Range &,
				   bool hold_width = true ) = 0;

  /** Returns the bin width associated with the scale factor. */
  virtual double scaleFactorWid ( ) = 0;

  /** Sets the starting Range. */
  void setStartRange ( bool dragging ) const;

  /** Sets the starting width parameter. */
  void setStartWidth ( bool dragging ) const;

  /** Calculates the bin width parameter when dragging a slider
      control.  This member saves the current width parameter when
      first called with @a dragging is @c true.  The width is then
      calculated with the parameter @a parm whose range is expected to
      be from 0 to 99. Returns the calculated width parameter, but
      does not change with current parameter.
  */
  double calcBinWidth ( int parm, bool dragging ) const;

};

} // namespace hippodraw

#endif // _BinnerAxis_H_
