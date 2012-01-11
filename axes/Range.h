/* -*- mode: c++ -*- */

/** @file

hippodraw::Range class interface

Copyright (C) 2000-2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Range.h,v 1.51 2006/09/27 17:19:22 pfkeb Exp $

*/

#ifndef _RANGE_H_
#define _RANGE_H_

#include "pattern/libhippo.h"

#include <vector>

#include <cfloat> // for DBL_MAX

namespace hippodraw {

/** Expresses a range of values.

    @author Stephane Bonneaud <gandalf@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket Malde <sanket@stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/

class MDL_HIPPOPLOT_API Range {

private:

  /** The minimum in the range. 
   */
  double m_min;

  /** The maximum in the range. 
   */
  double m_max;

  /** The minimum positive value in the range, for log scale.
   */
  double m_pos;

  /** A flag indicating that the range is empty.  An empty range is
      one that has not yet been set to a value.
  */
  bool m_empty;
  
 public:

  /** The following constructors sets the range to {0.0, 0.0} but sets
      m_empty to true.
  */
  Range ( );

  /** A constructor that sets the range to {x, y}, with a pos
      argument of p if it is supplied, otherwise DBL_MAX. 
  */
  Range ( double x,
	  double y,
	  double p = DBL_MAX );

  /** A constructor taking its range from the range of data in the
      vector @a array. 
  */
  Range ( const std::vector < double > & array );

  /** Returns the minimum of the range object.
   */
  double low() const;

  /** Sets the minimum of the range object.
   */
  void setLow(double x);

  /** Returns the maximum of the range object.
   */
  double high() const;

  /** Sets the maximum of the range object.
   */
  void setHigh(double x);

  /** Returns the first positive element in range.
   */
  double pos() const;

  /** Sets the first positive element in range.
   */
  void setPos(double x);

  /** Changes the current Range.
   */
  void setRange ( double low, double high, double pos );

  /** Sets the range from the data sequence. 
   */
  template < class Iterator >
  void setRange ( Iterator first, Iterator end );

  /** Returns the length of the range object.
   */
  inline double length() const;

  /** Sets the length of the range.  The boolean high_hold determines
      which end is held fixed and which one is modified.
  */
  void setLength ( double val, bool high_hold = false );
  
  /** Returns @c true if the argument value is inside the range. Returns
      @c true if the @a value is greater than or equal to the lower bound
      and less than or equal to the upper bound.
  */
  bool includes ( double value ) const;

  /** Returns @c true if the argument value is outside the range. Returns
      @c true if the @a value is strictly less than the lower bound or
      strictly  greater than the upper bound.
  */
  bool excludes ( double value ) const;

  /** Returns the fraction of the range that @a value represents. 
   */
  double fraction ( double value ) const;

  /** Forms the intersect with the range in the argument.
   */
  void setIntersect ( const Range & range );

  /** Forms the union with the range @a range.  The resulting range of
      the object extends from the lower range of the two objects to
      the upper range of the two. If the range was empty (@sa
      m_empty), then the result is the range @a range.
  */
  void setUnion ( const Range & range );

  /** Sets the range to empty. @sa m_empty.
   */
  void setEmpty ( bool yes = true );

  /** Returns the number of bins of width @c width required to fill
      the range.
  */
  int numberOfBins ( double width ) const;

};

inline
double
Range::
length () const
{
  return (m_max - m_min);
}

template < class Iterator >
void
Range::
setRange ( Iterator first, Iterator end )
{
  m_min = DBL_MAX;
  m_max = -DBL_MAX;
  m_pos = DBL_MAX;

  while ( first != end ) {
    double test = *first; // input might be pointer to float.
    m_min = std::min ( m_min, test );
    m_max = std::max ( m_max, test );
    if ( test > 0.0 ) m_pos = std::min ( m_pos, test );
    ++first;
  }
}

} // namespace hippodraw

#endif // _RANGE_H_
