/* -*- mode: c++ -*- */

/** @file

DyHist2DProjector class implementation

Copyright (C) 2000-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DyHist2DProjector.h,v 1.141 2005/10/30 00:10:13 pfkeb Exp $

*/

#ifndef _DyHist2DProjector_H_
#define _DyHist2DProjector_H_

#include "Hist2DProjImp.h"
#include "NTupleProjector.h"

namespace hippodraw {

/** A derived class of Hist2DProjImp and NTupleProjector that reads
    two columns of data and projects them to a two dimensional
    histogram.  A NTuple is created with projected values for
    presentation to clients.

    @author Stephane Bonneaud <gandalf@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket B. Malde <sanket@slac.stanford.edu>
    @author Matan Shacham <matan@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/
class MDL_HIPPOPLOT_API DyHist2DProjector : public Hist2DProjImp,
					    public NTupleProjector
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
  DyHist2DProjector ( const DyHist2DProjector & projector );

  /** This function is called when the ntuple has been changed to a
      new one. The column indexes are checked for validity and set to
      the last column if they are invalid.  If the weight index is
      invalid, it is set to negative. */
  virtual void changedNTuple();

public:

  /** The default constructor. */
  DyHist2DProjector();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  ProjectorBase * clone();

  /** Projects the data source into bins.
   */
  void execute();

  /** Returns the range of data on the specified axis.  The range may
      need to be calculated. */
  virtual Range dataRangeOn ( hippodraw::Axes::Type ) const;

  /** Returns the minimum positive value of the data on a specified
      axis. */
  virtual double getPosOn ( hippodraw::Axes::Type axis ) const;

  virtual const Range & setBinWidth ( hippodraw::Axes::Type axis,
				      int parm, 
				      bool dragging );

  virtual void setOffset ( const std::string & axis, 
			   int parm, 
			   bool dragging );

  /** Returns @c true if the destined @a axis is "X" or "Y", otherwise
      returns @c false. */
  virtual bool isAxisBinned ( const std::string & axis ) const;

  /** Sets the Range of the binner. Sets the Range of the BinsBase
      object for axes @a axis to @a range.  If @a const_width is @c
      true, the bin width is not changed, otherwise it changes to keep
      the same number of bins.
   */
  virtual void setBinnerRange ( hippodraw::Axes::Type axis,
				const Range & range,
				bool const_width );

  /** Implements Observer pattern.  Receives the update from the
      Observable object and directs it to either
      BinningProject::update or NTupleProjector::update depending on
      class of @a object.
  */
  virtual void update ( const Observable * object );

  /** Implements Observer pattern.  Receives the willDelete from the
      Observable object and directs it to either
      BinningProjector::update or NTupleProjector::update depending on
      the class of @a object.
  */
  virtual void willDelete ( const Observable * object );

};

} // namespace hippodraw

#endif // _DyHist2DProjector_H_
