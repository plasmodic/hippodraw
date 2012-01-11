/* -*- mode: c++; -*- */

/** @file

Profile2DProjector class interface

Copyright (C) 2000-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Profile2DProjector.h,v 1.65 2005/10/30 00:10:14 pfkeb Exp $

*/

#ifndef _Profile2DProjector_H_
#define _Profile2DProjector_H_

#include "BinningProjector.h"
#include "NTupleProjector.h"

#include "axes/Range.h"

namespace hippodraw {

/** A derived class of BinningProjector that reads three columns of
    data and projects them to a two dimensional profile display.  A
    DataSource is created with projected values for presentation to clients.

    @todo Must of the implementation is the same ash the
    Hist2DProjector.  One should be able to make a BinningProjector
    class which is the overlap of the current two and differ in only
    the kind of binner they have.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Matan Shacham <matan@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/
class MDL_HIPPOPLOT_API Profile2DProjector : public BinningProjector,
			   public NTupleProjector
{

private:

  /** The range of the accumulated values. */
  mutable Range m_value_range;

protected:

  /** The copy constructor. */
  Profile2DProjector ( const Profile2DProjector & projector );

  /** This function is called when the ntuple has been changed to a
      new one. The column indexes are checked for validity and set to
      the last column if they are invalid.  If the weight index is
      invalid, it is set to negative. */
  virtual void changedNTuple();

 public:

  /** The default constructor. It passes an argument of 3 to
      BinningProjector. */
  Profile2DProjector();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  ProjectorBase * clone();

  /** Projects the data source into bins. @todo Implement
      accumulations with weight. */
  void execute();

  /** Finds the range of the projected values.  The range of the
      accumulated bins is returned. */
  virtual Range valueRange() const;

  /** Returns the range of data on the specified axis.  The range may
      need to be calculated. */
  virtual Range dataRangeOn ( hippodraw::Axes::Type ) const;

  /** Returns the minimum positive value of the data on a specified
      axis. */
  virtual double getPosOn ( hippodraw::Axes::Type axis ) const;

  /** Function to add the acceptable point reps. */
  virtual void addPointReps ();

 public:

  virtual void setRange ( hippodraw::Axes::Type axis, bool );

  /** This function sets the binWidth. It is needed when the HippoDraw
      user wants to manually change the bin size. The parameter width
      is of type float because that is how the java interface works,
      but the cast is done here. 

      @todo Protect against an excessive number of bins, which will
      undoubtedly lead to an excessive drawing time.
  */
  virtual const Range &  setBinWidth ( hippodraw::Axes::Type axis, double v );

  virtual const Range &  setBinWidth ( hippodraw::Axes::Type axis,
				       int parm, 
				       bool dragging );

  virtual void setOffset ( const std::string & axis, 
			   int parm, 
			   bool dragging );

  virtual void setOffset ( hippodraw::Axes::Type axis, double v );

  /** Returns true if the destined axis is "X" or "Y", otherwise
      returns false. */
  virtual bool isAxisBinned ( const std::string & axis ) const;

  /** Returns the label (title) of the z axis.  Gets it from the ntuple
      object. */
  const std::string & getZLabel() const;

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

#endif // _Profile2DProjector_H_
