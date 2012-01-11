/* -*- mode: c++ -*- */

/** @file

CutRangeRep class interface

Copyright (C) 2002-2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.
 
$Id: CutRangeRep.h,v 1.21 2005/11/06 20:04:37 pfkeb Exp $

*/

#ifndef _CutRangeRep_H_
#define _CutRangeRep_H_

#include "PointRepBase.h"

namespace hippodraw {

class TupleCut;

/** A derived class of PointRepBase class that draws a representation
    of a cut range.

    @author Kaustuv <kaustuv@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API CutRangeRep : public PointRepBase
{
private:

  /** The type of container for TupleCut objects.
   */
  typedef std::vector < TupleCut > CutList_t;

  /** A list of cut objects used to highlight region of the data
      representation.
  */  
  CutList_t m_cut_list;

  /** Draws the Range for @a cut within @a x_range and @a y_range
      using @a view under transform @a tb. */
  void drawNormalRange ( const Range & x_range,
			 const Range & y_range,
			 const TransformBase & tb,
			 ViewBase & view );

  /** Draws the Range for 1D @a cut with inversion within @a x_range and
      @a y_range using @a view under transform @a tb. */
  void drawInvertedRange1D ( const Range & x_range,
			     const Range & y_range,
			     const TransformBase & tb,
			     ViewBase & view );

  /** Draws the Range for 2D @a cut with inversion within @a x_range and
      @a y_range using @a view under transform @a transform. */
  void drawInvertedRange2D ( const Range & x_range,
			     const Range & y_range,
			     const TransformBase & transform,
			     ViewBase & view );

  /** Throws exception that index was out of range.
   */
  void throwRangeException ();

public:

  /** The default constructor. */
  CutRangeRep( );

  /** The copy constructor. */
  CutRangeRep( const CutRangeRep & point_rep );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment.
  */
  virtual CutRangeRep * clone();

  /** Draw the Range of the cut.  Drawing is done within the
      boundaries of @a x_range and @a y_range.  The transform @a tb is
      used before drawing to @a view.
  */
  void drawProjectedValues ( const Range & x_range,
			     const Range & y_range,
			     const TransformBase & tb,
			     ViewBase & view );

  virtual void drawProjectedValues ( const DataSource * ntuple,
				     TransformBase * transform,
				     ViewBase * view );

  /** Returns the TupleCut objects used by the receiving object.
   */
  const std::vector < TupleCut > & getCuts () const;

  /** Add a TupleCut object to the list of cuts.
   */
  void addCut ( const TupleCut & cut );

  /** Set the Range of the @a i-th cut.
   */
  void setCutRangeAt ( const Range & range, unsigned int i );

  /** Toggles the @a i-th cut to be inverted from its current state.
   */
  void toggleInverted ( unsigned int i );

  /** Sets the enabled state of the @a i-th cut.
   */
  void setEnabled ( unsigned int i, bool yes );

};

} // namespace hippodraw

#endif // _CutRangeRep_H_
