/* -*- mode: c++ -*- */

/** @file

hippodraw::Bins1DProfile  class interface

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Bins1DProfile.h,v 1.58 2006/12/18 22:50:53 xiefang Exp $

*/

#ifndef _Bins1DProfile_H_
#define _Bins1DProfile_H_

#include "Bins1DBase.h"

namespace hippodraw {

class DataSource;

/** The class for the 1D profile binner hierarchy.  Calculates the
    mean and the standard deviation of data along the y axis in slices
    along the x axis.

    @author Xie Fang <xiefang@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Matan Shacham <matan@slac.stanford.edu>  
    @author Oded Wurman <owurman@stanford.edu>
*/
class MDL_HIPPOPLOT_API Bins1DProfile : public Bins1DBase
{
 private:

  /** The sum of the data along the y axis.  Range of index is 1 to
      m_num_bins. 0 refers to underflow while m_num_bins+1 refers to
      overflow */
  std::vector<double> m_sum;

  /** The sum of the data squared along the y axis.  This quantity is used
      to calculate the variance with only one pass through the
      data. Range of index is 1 to m_num_bins. */
  std::vector<double> m_sumsq;

  /** The total number of entries for each bin. 
     */
  std::vector < double > m_num;

  /** The sum of weight squared. 
   */
  std::vector < double > m_sumwtsq;

  /** The higher edge of the last bin (the lower edge of the last + 1). */
  double m_high;

  virtual void resize ( int number );

public:

  /** The default constructor. */
  Bins1DProfile ();

  /** The copy constructor. */
  Bins1DProfile ( const Bins1DProfile & binner );

  /** The destructor. */
  virtual ~Bins1DProfile ();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  BinsBase * clone () const;

  /** Returns the value of the bin with the largest value. */
  double maxBin();

  /** Returns the value of the bin with the smallest value. */
  double minBin();

  /** Resets the accumulation to zero. */
  virtual void reset();

  /** Accumulate a data point @a x, @a y into a bin.  The remaining
      arguments are ignored.  

      @todo Add weight argument. */
  virtual void accumulate ( double x, double y, 
			    double wt = 1.0, double dummy = 0.0 );

  /* implement pure virtual member functions in base. */
  virtual int getNumberOfEntries () const;

  /* implement pure virtual member functions in base. */
  virtual int getNumberOfEntries ( int i ) const;

  /* implement pure virtual member functions in base. */
  virtual int getOverflow () const;

  /* implement pure virtual member functions in base. */
  virtual int getUnderflow () const;

  virtual NTuple * createNTuple () const;
  virtual void fillDataSource ( DataSource * ntuple ) const;
  virtual void setBinContents ( const DataSource * ntuple );

};

} // namespace hippodraw

#endif // _Bins1DProfile_H_
