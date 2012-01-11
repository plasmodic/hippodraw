/* -*- mode: c++; -*- */

/** @file

Bins2DBase  class interface

Copyright (C) 2000-2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Bins2DBase.h,v 1.95 2006/04/12 20:45:52 xiefang Exp $

*/

#ifndef _Bins2DBase_H_
#define _Bins2DBase_H_

#include "BinsBase.h"

namespace hippodraw {

/** The base class for the 2D binner hierarchy. 

    @author Stephane Bonneaud <gandalf@slac.stanford.edu>
    @author Oded Wurman <oded@slac.stanford.edu> 
    @author Matan Shacham <matan@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/


class MDL_HIPPOPLOT_API Bins2DBase : public BinsBase
{

private:

  /** The binner axis object for the X axis. */
  BinnerAxis * binner_axisX;

  /** The binner axis object for the Y axis. */
  BinnerAxis * binner_axisY;

protected:

  /** The accumulated sum of weights.  If all calls to accumulate have
      weight of 1.0, the default, then will be number of entries.  For
      each vector, index @c 0 is the underflow, while the last element
      is the overflow. 
  */
  std::vector<std::vector<double> > m_data;


  /** A constructor that takes bins container @a name as argument. */
  Bins2DBase ( const char * name );

  /** Total number of bins not including overflow and underflow. 
   @todo Do we need this?.
  */
  int m_num_bins;

  /** Returns the number of the bin in which the x value is
   *  situated. */
  int binNumberX ( double x ) const;

  /** Returns the number of the bin in which the y value is
   *  situated. */
  int binNumberY ( double y ) const;

  virtual double getLow ( hippodraw::Axes::Type  axis ) const;

  virtual void setNumberOfBins ( hippodraw::Axes::Type axis, int number );

  /** Resizes the internal arrays.  Takes the size in each dimension
      from the BinnerAxis objects.  If either size is @c 0, then
      does nothing. */
  void resize ();

  /** Resizes the two dimensional arrays. */
  virtual void resize ( int nx, int ny );

  /** Prepares the NTuple.  Creates and returns the NTuple that will
      be filled from the binner's values.  The NTuple will have @a
      rows rows and 6 columns.
  */
  virtual NTuple * prepareNTuple ( unsigned int rows ) const;

public:

  /** The copy constructor. */
  Bins2DBase ( const Bins2DBase & binner );

  /** The destructor. */
  ~Bins2DBase ();

  virtual int getNumberOfAxes () const;
  virtual bool hasEqualWidths () const;

  /** Gets the bin width of the X bin for the @a i bin. */  
  double binWidthX ( int i ) const;

  /** Gets the bin width of the Y bin for the @a i bin. */
  double binWidthY ( int i ) const;

  virtual int numberOfBins ( hippodraw::Axes::Type axis ) const;

  virtual const BinnerAxis * getBinnerOn ( hippodraw::Axes::Type axis ) const;

  /** Sets a new BinnerAxis on the axis @a axis for the binner to
      use.  The old one, if any, will be destroyed.  Raises an
      assertion if @a axis is not @c X or @c Y.*/
  void setBinnerOn ( BinnerAxis *, hippodraw::Axes::Type axis );

  virtual double scaleFactor () const;

  /** Returns the number of bins. */
  int numberOfBins() const;

  virtual double binWidth ( hippodraw::Axes::Type axis ) const;

  virtual const Range & 
  setBinWidth ( hippodraw::Axes::Type axis, double value );
  
  /** Calculates the bin width parameter from dragging slider.
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

  virtual int getNumberOfEntries () const;

  virtual int getUnderflow () const;
  virtual int getOverflow () const;


};

} // namespace hippodraw

#endif // _Bins2DBase_H_
