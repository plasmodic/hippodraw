// -*- mode: c++; -*-

/** @file

hippodraw::BinnerAxisLinear  class interface

Copyright (C) 2001-2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinnerAxisLinear.h,v 1.41 2006/01/15 01:07:20 pfkeb Exp $
*/

#ifndef _BinnerAxisLinear_H_
#define _BinnerAxisLinear_H_

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "BinnerAxis.h"

namespace hippodraw {

/** The class for the constant width bins.

    @author Matan Shacham <matan@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API BinnerAxisLinear : public BinnerAxis
{

protected:

  /** The copy constructor. */
  BinnerAxisLinear ( const BinnerAxisLinear & binner );

  virtual double calcWidthParm ( int number ) const;

public: // member functions

  /** The default constructor. */
  BinnerAxisLinear ();

  /** The destructor. */
  virtual ~BinnerAxisLinear();
  
  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  BinnerAxis * clone();

  /** Returns true.  This derived class is a special case in that
      it has bins of all the same width. @sa
      BinnerAxis::hasEqualWidths. */
  virtual bool hasEqualWidths () const;

  /** Sets m_num_bins. */
  virtual void axisSetNumberOfBins( int nb );

  /** Returns the number of the bin in which the x value is
      situated. */
  virtual int axisBinNumber(double x) const;

  /** Returns the coordinate.  Returns the coordinate of the bin at
      index @a i.  The coordinate is midway between the bin edges.
  */
  virtual double getCoordinate ( int i ) const;

  /** Returns the width of each bin. */
  virtual double axisBinWidth ( int i ) const;

  /** Sets the bin width and adjusts the range and number of bins
      accordingly.  Returns the adjusted range. */
  virtual const Range & setBinWidth (double width );

  virtual double calcOffset ( int parm, bool dragging ) const;

  /** Returns the offset. */
  virtual double getOffset () const;

  /** Sets the offset and adjusts the range accordingly. */
  virtual const void setOffset ( double offset );

    /** Returns the bin width associated with the scale factor - in the
      case of linear binning, this is the const. bin width */
  virtual double scaleFactorWid ( );

  virtual const Range & setRange ( const Range &,
				   bool hold_width = true );

  /** Returns the constant width parameter. */
  virtual double getConstWid ( ) const;

  virtual double getBinWidth ( ) const;

  /** Sets the constant width parameter. */
  virtual void setConstWid ( );

private: // member functions

  virtual int getNob ( double wid ) const;
  virtual int getNob ( const Range & range ) const;

};

} // namespace hippodraw

#endif // _BinnerAxisLinear_H_
