/* -*- mode: c++ -*- */

/** @file

hippodraw::BinsBase class interface

Copyright (C)  2000-2006  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinsBase.h,v 1.79 2006/08/23 18:14:11 pfkeb Exp $

*/

#ifndef _BinsBase_H_
#define _BinsBase_H_

#include "axes/AxesType.h"

#include <list>
#include <vector>

namespace hippodraw {

class BinnerAxis;
class DataSource;
class NTuple;
class Range;

/** The base class for the binner hierarchy.  
    
    @author <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API BinsBase
{

private: 

  /** The name of the object in the Factory. */
  const std::string m_name;

protected:

  /** The scale factor.  This data member is used to scale the output
      so that the total number of entries has this value.
  */
  double m_scale_factor;

  /** A flag which is set @c true to enable scaling the output.
   */
  bool m_is_scaling;

  /** A flag to indicate that the objects in m_values are not correct. */
  mutable bool m_values_dirty;

  /** A flag set to @c true, when no accumulation has yet occurred,
      otherwise is set @c false.
  */
  bool m_empty;

  /** The constructor taking the bins container @a name as argument. */
  BinsBase ( const char * name );

  /** The copy constructor. */
  BinsBase ( const BinsBase & binner );

  /** Returns the low value of the bins on the specified @a axis. */
  virtual double getLow ( hippodraw::Axes::Type axis ) const = 0;

  /** Sets the number of bins on the the specified @a axis. */
  virtual void setNumberOfBins ( hippodraw::Axes::Type axis, int number ) = 0;

 public:

  /** The virtual destructor. */
  virtual ~BinsBase();

  /** The virtual function to make copy of concrete derived class. */
  virtual BinsBase * clone () const = 0;

  /** Returns the name of the Bins container object. */
  const std::string & name () const;

  /** Returns the number of axes handled by the BinsBase derived class. */
  virtual int getNumberOfAxes () const = 0;

  /** Returns the true number of entries.  Returns the sum of the weight
      parameter of the accumulate member function. 
  */
  virtual int getNumberOfEntries () const = 0;

  /** Returns the underflow. */
  virtual int getUnderflow () const = 0;

  /** Returns the overflow. */
  virtual int getOverflow () const = 0;

  /** Sets a scale factor on the output so that the number of entries
      appears to be @a number.  This feature might be used, for
      example, to compare two histograms that have different number of
      entires by normalizing one to the other.
  */
  virtual void scaleNumberOfEntries ( double number );

  /** Sets the scaling number of entries on if @a on is @c true,
      otherwise turns if off.
  */
  virtual void setEntriesScaling ( bool on );

  /** Returns @c true if no accumulation has yet occurred, otherwise
      returns @c false.
  */
  bool isEmpty () const;

  /** Returns the BinnerAxis object used by this object. */
  virtual const BinnerAxis * 
  getBinnerOn ( hippodraw::Axes::Type axis ) const = 0;

  /** Sets the bin calculator on specified @a axis. */
  virtual void setBinnerOn ( BinnerAxis *, hippodraw::Axes::Type axis ) = 0;

  /** Resets the accumulation to zero. */
  virtual void reset() = 0;

  /** Returns @c true if the re-accumulation is needed. */
  bool isDirty();

  /** Sets a flag to indicate that re-binning needs to be done. */
  void setDirty();

  /** Returns the scale factor.  Used to convert the projected value
      to number of entries in a bin.  The default, implemented here,
      is to return 1.0.  Derived class, such as those that do binning,
      may return a value to reflect their binning algorithm.
  */
  virtual double scaleFactor () const;

  /** Returns @c true if all bins have the same width. @sa
      BinnerAxis::hasEqualWidths. */
  virtual bool hasEqualWidths () const = 0;

  /** Returns the number of bins on specified @a axis.  If a
      BinnerAxis object does not exist, returns 0. */
  virtual int numberOfBins ( hippodraw::Axes::Type axis ) const = 0;

  /** Returns the bin width parameter on the specified @a axis.  If
      the bins have all the same width, then returns the bin width.
      If the bin width are determined by some algorithm, then returns
      a parameter used to calculate the bin widths.
   */
  virtual double binWidth ( hippodraw::Axes::Type axis ) const = 0;

  /** Sets the bin width parameter on the specified @a axis. */
  virtual const Range & 
  setBinWidth ( hippodraw::Axes::Type axis, double value ) = 0;

  /** Calculates a new bin width base on dragging slider value.
   */
  virtual double calcBinWidth ( hippodraw::Axes::Type axis,
				int parm,
				bool dragging ) const = 0;

  /** Calculates and returns a new range from dragging slider. */
  virtual double calcOffset ( const std::string &,
			      int parm,
			      bool dragging ) const = 0;

  /** Returns the offset parameter on specific @a axis. */
  virtual double getOffset ( hippodraw::Axes::Type axis ) const = 0;

  /** Sets the offset parameter on the specified @a axis.
   */
  virtual void setOffset ( hippodraw::Axes::Type axis, double value ) = 0;

  /** Sets the Range on the specified @a axis.  If @a hold_width is @c
      true, the bin width parameter will be held constant, otherwise
      it is adjusted to keep the same number of bins for the new
      range. */
  virtual const Range & setRange ( hippodraw::Axes::Type axis, 
				   const Range &, 
				   bool hold_width = true ) = 0;

  virtual const Range & getRange ( hippodraw::Axes::Type axis ) = 0;

  /** Accumulates the data point with weight w.  Some derived class
      implementations make take second argument as the weight and
      ignore the remaining.  Similarly for the third and fourth
      arguments. */
  virtual void accumulate ( double x, double w_or_y = 1.0, 
			    double z = 1.0, double w = 1.0 ) = 0;

  /** Get the z value at the specified point (x,y). */
  virtual double getZValue ( double x, double y ) const;

  /** Prepare NTuple representation of projected values.  The NTuple
      will have @a rows reserved and appropriate number of columns. 
  */
  virtual NTuple * prepareNTuple ( unsigned int rows ) const = 0;

  /** Creates an NTuple.  Derived classes will return an %NTuple
      appropriate to their binned data.  They should at least contain
      the coordinates, error on the coordinates, the value, and error
      on the value.
  */
  virtual NTuple * createNTuple () const = 0;

  /** Fills the DataSource.  Clears and fills the %DataSource based on
      the contents of the bins.  The resulting %DataSource is follows
      the standard defined in hippodraw::DataPoint2DTuple or
      hippodraw::DataPoint3DTuple.
  */
  virtual void fillDataSource ( DataSource * ntuple ) const = 0;

  /** Sets the contents of the bins from the @a ntuple.  Sets the
      contents of the bins from the @a ntuple.  The purpose of this
      method is to restore a histogram from archived file.  The ntuple
      should be cone filled by the fillDataSource method.
   */
  virtual void setBinContents ( const DataSource * ntuple ) = 0;

  /** Set the minimum entries/bin.
   */
  virtual void setMinEntries( int entries );

  /** Get the minimum entries/bin.
   */
  virtual int getMinEntries ( );
};

} // namespace hippodraw

#endif // _BinsBase_H_
