/* -*- mode: c++ -*- */

/** @file

hippodraw::NTupleFCN class interface

Copyright (C) 2003-2006   The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: NTupleFCN.h,v 1.33 2006/08/16 21:51:06 pfkeb Exp $

*/

#ifndef _NTupleFCN_H_
#define _NTupleFCN_H_

#include "StatedFCN.h"

namespace hippodraw {

class DataSource;
class Range;
class TupleCut;

/** A derived class of StatedFCN This class gets its data points from
    a DataSource.  It derives from StatedFCN to access the stated
    model function.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API NTupleFCN : public StatedFCN
{
private:

  /** The cut that manages the range on X to be used for fitting.
   */
  TupleCut * m_fit_cut;

  /** Examines state of error bars.  Returns @c true if at least one
      data point in has non zero error, otherwise returns @c false.
   */
  bool hasErrors () const;

  /** Resets the matrix @a alpha and the vector @a beta.  Resets the
      matrix and vector to the appropriate size, if needed, and sets
      the elements to zero.
  */
void reset ( std::vector < std::vector < double > > & alpha,
	     std::vector < double > & beta,
	     unsigned int size );

protected:

  /** The indexes into the DataSource.  The first set are the
      coordinate variables.  The next to last is the value and the
      last is the error on the value, or @em -1 if no errors
      available.  Thus the size of the vector should be at least 3 and
      can be more for multi-dimensional coordinates.
  */
  std::vector < int > m_indices;

  /** The data source  containing the data points for the fitter. 
   */
  const DataSource * m_ntuple;

  /** Error use flag.  A flag set to @c true, if errors on measured
      points are to be used.  When set @c true, only data points with
      non-zero errors are used.  When set @c false, all error on data
      points is set to 1.0
  */
  bool m_has_errors;

private:

  /** Set to @c true if the fitting range is turn on.
   */
  bool m_fit_range;

protected:

  /** The default constructor. The default constructor which
      initializes points to objects it uses to the null pointer.
   */
  NTupleFCN ();

  /** The copy constructor.
   */
  NTupleFCN ( const NTupleFCN & );

  virtual void copyFrom ( const StatedFCN * );

  /** Returns @a true if the DataSource row @a row is within the fit
      Range.
  */
  bool acceptRow ( unsigned int row ) const;

  /** Returns the index to the column containing error on the value
      coordinate.
  */
  int getErrorColumn ( ) const;

public:

  /** Sets the data source.  Sets the data source and uses indexes for
      one dimensional function from hippodraw::DataPoint2DTuple.
   */
  void setDataSource ( const DataSource * source );

  virtual void setDataSource ( const DataSource * source,
			       int dimension, 
			       const std::vector < int > & );

  /** Sets the use of errors on the data point.  If the data points
      contained in the DataSource contains errors, then set the use of
      errors on and return @c true.  If the data points do not contain
      errors, then set the use of errors off and return @c true.  If
      @a yes is @c false, then always set use of errors off and return
      @c true.
   */
  virtual bool setUseErrors ( bool yes=true );

  /** Gets the use of errors.  If the there is no errors or ignore errors
      is chosen, return false.  If the data points contain errors and 
      errors is not ignored, return true.
  */
  virtual bool getUseErrors () const;

  /** Sets the cut to limit range of fitting.
   */
  virtual void setFitCut ( TupleCut * cut );

  /** Sets use of a fitting range on or off.
   */
  virtual void setFitRange ( bool yes = true );

  /** Calculates the number of degrees of freedom.  Calculates the
      number of degrees of freedom.  If the data points have error
      measurements, then points with zero error are ignored.  @sa
      operator().
   */
  virtual int degreesOfFreedom ( ) const;

 protected:

  /** Calculates alpha and beta.  Calculates the matrix @a alpha and
      the vector @a beta based on the data and the function values and
      derivatives.  Re-sizes them if required. 
      
      @request Find out what these really are and document them.  They
      might be the gradients.
  */
  virtual void calcAlphaBeta ( std::vector < std::vector < double > > & alpha,
			       std::vector < double > & beta );

};

} // namespace hippodraw

#endif // _NTupleFCN_H_
