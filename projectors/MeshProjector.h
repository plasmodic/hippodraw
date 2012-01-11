/* -*- mode: c++ -*- */

/** @file

hippodraw::MeshProjector class interface

Copyright (C)  2006  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: MeshProjector.h,v 1.114 2006/11/22 01:35:51 xiefang Exp $

*/

#ifndef _MeshProjector_H_
#define _MeshProjector_H_

#include "NTupleProjector.h"

namespace hippodraw {

/** A derived class of NTupleProjector that maps 3 DataSource columns to a
    two dimensional mesh projection.  Optionally, additional columns can be
    mapped to X and Y widths of the mesh.  A DataSource  is created with the
    projected values for presentation to clients.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API MeshProjector : public NTupleProjector
{

  /** The label for the X width binding option. */
  std::string m_x_option;

  /** The label for the Y width binding option. */
  std::string m_y_option;

 protected:

  /** The copy constructor. */
  MeshProjector ( const MeshProjector & projector );

  /** This function is called when the ntuple has been changed to a
      new one. The X and Y axis indexes are checked for validity and
      reset to the last column if invalid.  The X and Y error indexes
      are checked for validity and reset to negative if they are
      invalid. */
  virtual void changedNTuple();

  /** Function to add the acceptable point reps. */
  virtual void addPointReps ();

  /** Returns the Range of the projected values.*/
  Range dataRangeOnValue () const;

  /** Returns @c true if value at @a row is within range on all axis.
   */
  virtual bool inRange ( int row ) const;

  /** Returns @c true if value at @a row is within range. If @a flag
      is @c true, Returns @c true if value at @a row is within range
      on all axis, otherwise uses just the X and Y axis. */

  bool inRangeWithZ ( int row, bool flag ) const;

 public:

  /** This default constructor binds to the first two columns. */
  MeshProjector();

  /** The destructor. */
  virtual ~MeshProjector();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  ProjectorBase * clone();

  /** Sets whether the X width options are to be enabled or not.
      @todo Remove this and setYerrorOption() in favor of passing as
      argument s string with the name of an error option. */
  virtual void setXErrorOption ( bool enable );

  /** Sets whether the Y width options are to be enabled or not.
   */
  virtual void setYErrorOption ( bool enable );

  /** Returns the range of the data on the specified axis.  The range
      may need to be calculated. */
  virtual Range dataRangeOn ( hippodraw::Axes::Type ) const;
  virtual Range valueRange () const;

  /** Returns the minimum positive value of the data on a specified
      axis. */
  virtual double getPosOn ( hippodraw::Axes::Type axis ) const;

  virtual DataSource * createNTuple () const;
  virtual void fillProjectedValues ( DataSource * ntuple, 
				     bool in_range = false ) const;
  virtual void prepareValues ();

  /** Returns the label (title) of the z axis. Gets it from the ntuple
      object. */
  const std::string & getZLabel() const;

  /** Get the z value at the specified point (x,y). */
  virtual double getZValue ( double x, double y ) const;

  /** Returns the preferred Range on the given @a axis.  Returns the
      preferred Range on the given @a axis that would display all the
      data.  If @a axis is Axes::Z, the returned Range is only for the
      rows where both X and Y values are within their respective
      current ranges.
  */
  virtual Range preferredRange ( hippodraw::Axes::Type axis ) const;


};

} // namespace hippodraw

#endif // _MeshProjector_H_
