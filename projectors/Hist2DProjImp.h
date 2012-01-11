/* -*- mode: c++ -*- */

/** @file

hippodraw::Hist2DProjImp class interface

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Hist2DProjImp.h,v 1.144 2006/02/17 19:58:07 pfkeb Exp $

*/

#ifndef _Hist2DProjImp_H_
#define _Hist2DProjImp_H_

#include "BinningProjector.h"

#include "axes/Range.h"

namespace hippodraw {

/** A derived class of BinningProjector projects to a two dimensional
    histogram.  A DataSource object is created with the projected
    values for presentation to clients.

    @author Stephane Bonneaud <gandalf@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket B. Malde <sanket@slac.stanford.edu>
    @author Matan Shacham <matan@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/
class MDL_HIPPOPLOT_API Hist2DProjImp : public BinningProjector
{

private:

  /** The label of the z axis when displaying number of entries per bin. */
  std::string m_z_label_entries;

  /** The label of the z axis when displaying density. */
  std::string m_z_label_density;

  /** The range of the accumulated values. */
  mutable Range m_value_range;

protected:

  /** The copy constructor. */
  Hist2DProjImp ( const Hist2DProjImp & projector );

  /** Checks the axis scaling.  If bins are all same width, then sets
      the Z axis scale factor.  Otherwise, turns off the scale
      factor on the Z axis. */
  virtual void checkScaling ();

  /** Sets the variable so that it can be modified. */
  void setZLabel();

  /** Function to add the acceptable point reps. */
  virtual void addPointReps ();

  /** Returns the data range on the Z axis.
   */
  Range dataRangeOnValue () const;

  /** Returns the smallest positive value on the Y axis.
   */
  double getPosOnValue () const;

public:

  /** The default constructor. */
  Hist2DProjImp();

  /** The destructor. */
  virtual ~Hist2DProjImp();

  /** Finds the range of the projected values.  The range of the
      accumulated bins is returned. 
  */
  virtual Range valueRange() const;

  /** Sets the range of the selected axis. */
  virtual void setRange( hippodraw::Axes::Type axis, bool );

  virtual Range preferredRange ( hippodraw::Axes::Type ) const;

  /** This function sets the binWidth. It is needed when the HippoDraw
      user wants to manually change the bin size.

      @todo Protect against an excessive number of bins, which will
      undoubtedly lead to an excessive drawing time.
  */
  virtual const Range & setBinWidth ( hippodraw::Axes::Type axis, double v );
  virtual void setOffset ( hippodraw::Axes::Type axis, double v );

  /** Returns @c true as the projected value is the result of
      accumulation into bins. */
  virtual bool isValueBinned () const;

  /** Returns the label (title) of the z axis. */
  const std::string & getZLabel() const;

  /** A test function. */
  virtual bool hasZAxis() const;

  /** Returns @c true if @a axis is the Z axis. 
   */
  virtual bool wantsScaleFactor ( const std::string & axis ) const;

};

} // namespace hippodraw

#endif // _Hist2DProjImp_H_
