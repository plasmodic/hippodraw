/* -*- mode: c++ -*- */

/** @file

Hist1DProjImp class interface

Copyright (C) 2003-2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: Hist1DProjImp.h,v 1.12 2005/10/30 00:10:13 pfkeb Exp $

*/

#ifndef _Hist1DProjImp_H_
#define _Hist1DProjImp_H_

#include "projectors/BinningProjector.h"

namespace hippodraw {

/** A derived class of BinningProjector that implements the member
    function common to classes implementing a one dimensional
    histogram.

    @request @@@@@@ Want to be able to plot E**2 dN/dE as will as
    dN/dE.  Need to decide if this is part of a histogram class or
    part of adding columns to the DataSource.

    @author Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API Hist1DProjImp : public BinningProjector
{

protected:

  /** The label of the y axis when axis is scaled.  When the axis is
      scaled, the values of the ticks appear to be number of entries
      per bin instead of the true density. */
  std::string m_y_label_entries;

  /** The label of the y axis when axis is not scaled.  When the axis is
      not scaled, the values of the ticks are the density. */
  std::string m_y_label_density;

  /** The default constructor. 
   */
  Hist1DProjImp();

  /** The copy constructor. 
   */
  Hist1DProjImp ( const Hist1DProjImp & );

  /** Function to add the acceptable point reps. */
  virtual void addPointReps ();

  /** Returns the smallest positive value on the Y axis. 
   */
  double getPosOnValue () const;

  /** Returns the data range on the Y axis. 
   */
  Range dataRangeOnValue () const;

public:

  /** The Destructor.
   */
  virtual ~Hist1DProjImp ();

  /** Checks the axis scaling.  If bins are all same width, then sets
      the Y axis scale factor.  Otherwise, turns off the scale
      factor on the Y axis. */
  virtual void checkScaling ();

  /** Returns @c true as the projected value is the result of
      accumulation into bins. */
  virtual bool isValueBinned () const;

  virtual void setRange ( hippodraw::Axes::Type, bool );
 
  virtual void reset ( ); 

  virtual Range valueRange() const;

  virtual Range preferredRange ( hippodraw::Axes::Type ) const;

  /** Returns @c true if @a axis is the Y axis. 
   */
  virtual bool wantsScaleFactor ( const std::string & axis ) const;

};

} // namespace hippodraw

#endif // _Hist1DProjImp_H_
