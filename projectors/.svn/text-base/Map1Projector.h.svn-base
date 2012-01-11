/* -*- c++ -*- */

/** @file

hippodraw::Map1Projector class interface

Copyright (C) 2001-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Map1Projector.h,v 1.50 2006/08/05 22:06:57 pfkeb Exp $

*/

#ifndef _Map1Projector_H_
#define _Map1Projector_H_

#include "NTupleProjector.h"

namespace hippodraw {

/** A derived class of NTupleProjector that maps 1 DataSource column
    to a Y axis of two dimensional projection. The X axis is generated
    by the row number.  A DataSource is created with the projected
    values for presentation to clients.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket B. Malde <sanket@stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>

    @request Optionally, an additional column can be mapped to Y error
    bars.
*/
class MDL_HIPPOPLOT_API Map1Projector : public NTupleProjector
{

private:

  /** The label for the generated x axis. 
   */
  std::string m_x_label;

  /** The label for the Y error binding option.
   */
  std::string m_y_option;


protected:

  /** The copy constructor. */
  Map1Projector ( const Map1Projector & projector );

  /** This function is called when the NTuple has been changed to a
      new one. The X and Y axis indexes are checked for validity and
      reset to the last column if invalid.  The X and Y error indexes
      are checked for validity and reset to negative if they are
      invalid. */
  virtual void changedNTuple();

  /** Function to add the acceptable point reps. */
  virtual void addPointReps ();

public:


  /** This default constructor binds to the first two columns. */
  Map1Projector();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  ProjectorBase * clone();

  virtual bool inRange ( int row ) const;

  /** Sets whether the Y error options are to be enabled or not.  @todo
      Remove this and setX`errorOption() in favor of passing as
      argument string with name of axis option. */
  virtual void setYErrorOption ( bool enable );

  /** Returns the range of the data on the specified axis.  The range
      may need to be calculated. */
  virtual Range dataRangeOn ( hippodraw::Axes::Type ) const;
  virtual Range valueRange ( ) const;

  /** Returns the minimum positive value of the data on a specified
      axis. */
  virtual double getPosOn ( hippodraw::Axes::Type axis ) const;

  /** Returns the label (title) of the x axis. Gets it from the NTuple
      object. */
  const std::string & getXLabel() const;

  /** Returns the label for the Y axis. Returns the label for the Y
      axis which is always the label of the bound NTuple column.  The
      argument @a density is ignored.
  */
  const std::string & getYLabel ( bool density = false ) const;

  /** Get the Average of all projected values on the specified axis. */
  virtual double getAverage ( hippodraw::Axes::Type axis ) const;

  virtual DataSource * createNTuple () const;

  virtual void prepareValues ();

protected:

  virtual void fillProjectedValues ( DataSource * ntuple, 
				     bool in_range = false ) const;

};

} // namespace hippodraw

#endif // _Map1Projector_H_
