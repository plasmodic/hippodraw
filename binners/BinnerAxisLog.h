/* -*- mode: c++; -*- */

/** @file

hippodraw::BinnerAxisLog  class interface

Copyright (C) 2000-2003, 2006 The Board of Trustees of The Leland
Stanford University

$Id: BinnerAxisLog.h,v 1.42 2006/08/24 18:32:06 pfkeb Exp $

*/

#ifndef _BinnerAxisLog_H_
#define _BinnerAxisLog_H_

#include "BinnerAxis.h"

#include <vector>

namespace hippodraw {

/** The class for bins of logarithmically increasing widths. 

    @bug Resolve this issue.  Is plotting the log(x) on linear scale
    the same as binning x on log scale?

    @author Matan Shacham <matan@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/
class MDL_HIPPOPLOT_API BinnerAxisLog : public BinnerAxis
{
private: // data members

  /** The copy constructor. */
  BinnerAxisLog ( const BinnerAxisLog & binner );
  
  /** Has the lower edge of each bin, starting with [0] = the first
      one. */
  std::vector<double> bins;

  double calcWidthParm ( int number ) const;

public: // member functions

  /** The default constructor. */
  BinnerAxisLog ();
  
  /** The destructor. */
  virtual ~BinnerAxisLog();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  BinnerAxis * clone();

  /** Returns the number of the bin in which the x value and y value
   *  are situated. */
  virtual int axisBinNumber ( double x ) const;

  /** Returns the coordinate.  Returns the coordinate of the bin at
      index @a i.  The coordinate of the bin is not the center of the
      bin.  The calculation is such that doing a chi-square fit to
      power-law on log-log scale is correct.  Also plotting individual
      points is correct.
  */
  virtual double getCoordinate ( int i ) const;

  /** Returns the width of the bin designated by its place number in
   *  the bins structure. */
  virtual double axisBinWidth ( int bins_number) const;

  /** Sets m_num_bins. */
  virtual void axisSetNumberOfBins( int nb );

  /** Returns the constant width parameter. */
  virtual double getConstWid ( ) const;

  virtual double getBinWidth ( ) const;


  /** Returns the bin width associated with the scale factor - in the
      case of log binning, this is always 1.0 */
  virtual double scaleFactorWid ( );

  /** Sets the bin width and adjusts the range and number of bins
      accordingly.  Returns the adjusted range.*/
  virtual const Range & setBinWidth ( double width );

  virtual double calcOffset ( int parm, bool dragging ) const;

  /** Returns the offset. */
  virtual double getOffset () const;

  /** Sets the offset and adjusts the range accordingly. */
  virtual const void setOffset ( double offset );


  virtual const Range & setRange ( const Range &,
				   bool hold_width = true );

private: // member functions
  
  virtual int getNob ( double wid ) const;
  virtual int getNob ( const Range & range ) const;

  /** Sets the bins structure by calculating the lower edge of each
   *  bin. */
  void setBins();

  /** Sets the constant width parameter. 
   @todo Remove this function in favor of calcWidthParm. 
  */
  virtual void setConstWid ( );


};

} // namespace hippodraw

#endif // _BinnerAxisLog_H_
