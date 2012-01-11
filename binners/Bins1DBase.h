/* -*- mode: c++ -*- */

/** @file

hippodraw::Bins1DBase  class interface

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Bins1DBase.h,v 1.97 2006/09/25 22:23:09 pfkeb Exp $

*/

#ifndef _Bins1DBase_H_
#define _Bins1DBase_H_

#include "BinsBase.h"

#include "BinnerAxis.h"

namespace hippodraw {

/** The base class for the one-dimensional binner hierarchy.

    @author Stephane Bonneaud <gandalf@slac.stanford.edu>
    @author Paul_Kunz@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu> 
    @author Matan Shacham <matan@slac.stanford.edu> */

class MDL_HIPPOPLOT_API Bins1DBase : public BinsBase
{
private: 

  /** Resizes the internal arrays. */
  void resize ();

protected:
  
  /** The binner axis object. */
  BinnerAxis * binner_axis;

  /** A constructor taking a bins container name. */
  Bins1DBase ( const char * name );

  /** The copy constructor. */
  Bins1DBase ( const Bins1DBase & binner );

  virtual double getLow ( hippodraw::Axes::Type axis ) const;

  /** Resizes the internal arrays for @a number bins. */
  virtual void resize ( int number ) = 0;

  /** Prepares the NTuple.  Creates and returns the NTuple that will
      be filled from the binner's values.  The NTuple will have @a
      rows rows and 4 columns.
  */
  virtual NTuple * prepareNTuple ( unsigned int rows ) const;

public:

  /** The destructor. */
  virtual ~Bins1DBase ();

  virtual int getNumberOfAxes () const;

  virtual int numberOfBins ( hippodraw::Axes::Type axis ) const;

  virtual bool hasEqualWidths () const;

  /** Returns the width of each bin.  Returns the width of the bin
      with index @a i. The first bin has index 0 and the last bin has
      index of number of bins minus 1.
   */
  double binWidth ( int i ) const;

  virtual const BinnerAxis * getBinnerOn ( hippodraw::Axes::Type axis ) const;

  /** Sets a new BinnerAxis for the binner to use.  The old one,
      if any, will be destroyed. */
  virtual void setBinnerOn ( BinnerAxis *, hippodraw::Axes::Type axis );

  /** Returns the lower edge of the last + 1 bin through binner_axis. */
  double getHigh() const;

  virtual void setNumberOfBins ( hippodraw::Axes::Type axis, int num_bins );

  /** Returns the number of the bin in which the x value is
      situated. */
  inline int binNumber(double x);

  virtual const Range & setBinWidth ( hippodraw::Axes::Type axis, 
				      double value );

  /** Calculates the bin width from dragging slider.
   */
  double calcBinWidth ( hippodraw::Axes::Type axis, 
			int parm, 
			bool dragging ) const;

  /** Calculates and returns a new range from dragging slider. */
  virtual double calcOffset ( const std::string &,
			     int parm,
			     bool dragging ) const;

  /** Returns the offset parameter on specific @a axis. */
  virtual double getOffset ( hippodraw::Axes::Type axis ) const;

  /** Sets the offset parameter on the specified @a axis.
   */
  virtual void setOffset ( hippodraw::Axes::Type axis, double value );

  virtual const Range & setRange ( hippodraw::Axes::Type axis, 
				   const Range &, 
				   bool hold_width = true );

  /** Returns range from binner axis. */
  virtual const Range & getRange ( hippodraw::Axes::Type axis );

  /** Returns the scale factor.  Used to convert the projected values
      to number of entries in a bin.
  */
  virtual double scaleFactor () const;

  
  virtual double binWidth ( hippodraw::Axes::Type axis ) const;

  /** Returns the number of entries in ith bin. Returns the sum of the
      weight parameter of the accumulate member function. Indexing is
      such that first proper bin (i.e. one which is NOT underflow ) is
      numbered 0.*/
  virtual int getNumberOfEntries ( int i ) const  = 0;
  
};

inline
int
Bins1DBase::binNumber(double x)
{
  return binner_axis->axisBinNumber(x);
}


} // namespace hippodraw

#endif // _Bins1DBase_H_
