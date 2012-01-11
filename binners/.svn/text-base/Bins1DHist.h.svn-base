/* -*- mode: c++ -*- */

/** @file

hippodraw::Bins1DHist  class interface

Originally from C code written at SLAC by Jonas Karlsson in August
1990.  Modified substantially since August 1990 by Gravina, Kunz,
Pavel, Rensing.

Copyright (C) 2000-2006  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Bins1DHist.h,v 1.80 2006/08/23 18:14:11 pfkeb Exp $

*/

#ifndef _Bins1DHist_H_
#define _Bins1DHist_H_

#include "Bins1DBase.h"

namespace hippodraw {

class NTuple;

/** The class for the one-dimensional histogram binning.  

    @request Should use BinnerAxis::getCoordinate() and have asymmetric
    errors.

    @author Stephane Bonneaud <gandalf@slac.stanford.edu>
    @author Xie Fang <xiefang@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Matan Shacham <matan@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu> 
*/
class MDL_HIPPOPLOT_API Bins1DHist : public Bins1DBase
{
private:

  /** The array of binned data.  The range of the index is 
      1 to m_num_bins.( 0 refers to underflow while m_num_bins + 1
      refers to overflow ) */
  std::vector< double > m_data;

  /** The accumulated square of the weights. */
  std::vector< double > m_variance;

  /** The moments. */
  double m_moments[3];

  /** The minimum number of entries/bin */
  int m_min_entries;

  /** Resizes the internal arrays. */
  virtual void resize ( int size );

public:

  /** The default constructor. */
  Bins1DHist ();

  /** The copy constructor. */
  Bins1DHist ( const Bins1DHist & binner );

  /** The destructor. */
  virtual ~Bins1DHist ();

  /** The clone function returns an object of its own kind which is a
      copy of this object at this moment. */
  BinsBase * clone () const;

  /** Returns the value of the bin with the smallest value. */
  double minBin();

  /** Returns the value of the bin with the largest value. */
  double maxBin();

  /** Resets the accumulation to zero. */
  virtual void reset();

  /** Accumulates a data point @a x into a bin with weight @a w.  The
      remaining arguments are ignored.
  */
  virtual void accumulate ( double x, double w = 1.0, 
			    double i1 = 0.0, double i2 = 0.0 );

  /** implementation from pure base */
  virtual int getNumberOfEntries () const;

  /** implementation from pure base */
  virtual int getNumberOfEntries ( int i ) const;

  /** implementation from pure base */
  virtual int getUnderflow () const;

  /** implementation from pure base */
  virtual int getOverflow () const;

  
  /** Creates a NTuple.  Creates and returns an %NTuple based on the
      contents of the bins.  The receiver takes possession of the
      %NTuple object.  The columns in the %NTuple are in order: X
      position, value, X width, and error on value.
   */
  virtual NTuple * createNTuple () const;

  virtual void fillDataSource ( DataSource * ntuple ) const;

  virtual void setBinContents ( const DataSource * ntuple );

  /** Set the minimum entries/bin.
   */
  virtual void setMinEntries( int entries );

  /** Get the minimum entries/bin.
   */
  virtual int getMinEntries ( );
};

} // namespace hippodraw

#endif // _Bins1DHist_H_
