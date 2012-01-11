/* -*- c++ -*- */

/** @file

hippodraw::MapMatrixProjector class interface

Copyright (C) 2003-2006 The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: MapMatrixProjector.h,v 1.37 2006/10/22 00:20:27 pfkeb Exp $

*/

#ifndef _MapMatrixProjector_H_
#define _MapMatrixProjector_H_

#include "NTupleProjector.h"

namespace hippodraw {

/** A derived class of NTupleProjector that maps 1 DataSource column
    to a Y axis of two dimensional projection. The X axis is generated
    by the row number. A DataSource is created with the projected
    values for presentation to clients.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API MapMatrixProjector : public NTupleProjector
{

private:

  /** The label for the generated x axis. */
  std::string m_x_label;

  /** The label for the generated y axis. 
   */
  std::string m_y_label;

  /** The number of columns for each row. The number of data points
      along the horizontal or X axis.  A value of 0 indicates that the
      member has not yet been initialized.
   */
  unsigned int m_cols;

  /** The number of rows for each column. The number of data points
      along the vertical or Y axis.  A value of 0 indicates that the
      member has not yet been initialized.
   */
  unsigned int m_rows;

  /** The step along the X axis. The distance along the X axis between
      two columns.  If 0.0, it has not yet been initialized.
   */
  double m_x_step;

  /** The step along the Y axis. The distance along the Y axis between
      two rows.  If 0.0, it has not yet been initialized.
   */
  double m_y_step;

  /** The origin along the X axis. 
   */
  double m_x_origin;

  /** The origin along the Y axis. 
   */
  double m_y_origin;

  /** The scale factor.  The scale factor to be applied to the values
      of the elements in the NTuple. 
  */
  double m_scale_factor;

  /** The value of the null value.  The null value is the value taken
      when no data is in an element of the matrix.  In order to
      preserve the shape, something must be placed in the vector. 
      @request Allow user to set and get this value.
  */
  double m_null_value;

  /** A flag to indicate if the X-Y matrix should be transposed or
      not.  When set @c true, consecutive elements of the vector move
      along the X axis.
  */
  bool m_transpose;

protected:

  /** The copy constructor. */
  MapMatrixProjector ( const MapMatrixProjector & projector );

  /** This function is called when the NTuple has been changed to a
      new one. The X and Y axis indexes are checked for validity and
      reset to the last column if invalid.  The X and Y error indexes
      are checked for validity and reset to negative if they are
      invalid. 
  */
  virtual void changedNTuple();

  /** Function to add the acceptable point reps. */
  virtual void addPointReps ();

  /** Returns @c true if value at @a row is within range. If @a flag
      is @c true, Returns @c true if value at @a row is within range
      on all axis, otherwise uses just the X and Y axis. */

  bool inRangeWithZ ( int row, bool flag ) const;

public:

  /** This default constructor binds to the first two columns. */
  MapMatrixProjector();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  ProjectorBase * clone();

  /** Transposes the X and Y axis of the contained matrix.  Transposes
      the X and Y axis of the contained matrix.  When set @c true,
      connected elements of the vector are taken to be along the X
      axis, otherwise they are along the Y axis.
   */
  virtual void matrixTranspose ( bool yes );

  /** Sets the number of steps.  Sets the number of steps along axis
      @a axis to @a number. The X axis is taken to be the number of
      columns in the matrix and the Y axis the number of rows.  Raises
      assertion if called with invalid @a axis parameter.
  */
  virtual void setNumberOfBins ( hippodraw::Axes::Type axis, 
				 unsigned int number );

  /** Returns the number of bins.  Returns the number of steps along
      the @a axis axis. 
  */
  virtual int getNumberOfBins ( hippodraw::Axes::Type axis ) const;

  /** Sets the bin width.  Sets the distance between data points along
      the axis @a axis to @a step.  For the special case that @a axis
      is equal to Z, sets the scale factor for data point values.
  */
  virtual const Range &  setBinWidth ( hippodraw::Axes::Type axis,
				       double step );

  /** Returns the step size along the axis @a axis. 
   */
  virtual double getBinWidth ( hippodraw::Axes::Type axis ) const;

  /** Sets the bin offset.  Sets the distance origin of the axis @a
      axis to @a origin.  If @a axis is an invalid value, then raises
      an assertion.
  */
  virtual void setOffset ( hippodraw::Axes::Type axis, double origin );

  /** Returns the origin along the axis @a axis.
  */
  virtual double getOffset ( hippodraw::Axes::Type axis ) const;

  virtual void setNTuple ( const DataSource * ntuple );

  virtual bool inRange ( int row ) const;

  /** Returns the range of the data on the specified axis.  The range
      may need to be calculated. */
  virtual Range dataRangeOn ( hippodraw::Axes::Type ) const;
  virtual Range dataRangeOnValue () const;
  virtual Range valueRange ( ) const;
  virtual Range preferredRange ( hippodraw::Axes::Type axis ) const;

  /** Returns the minimum positive value of the data on a specified
      axis. */
  virtual double getPosOn ( hippodraw::Axes::Type axis ) const;

  /** Returns the label (title) of the x axis. Gets it from the NTuple
      object. 
  */
  const std::string & getXLabel () const;

  /** Returns the label for the Y axis. The argument @a flag is
      ignored.
  */
  const std::string & getYLabel ( bool flag ) const;

  /** Returns the label of the z axis.  Returns the column label from
      the NTuple. 
  */
  const std::string & getZLabel () const;

  /** Returns the value at the coordinate. 
   */
  virtual double getZValue ( double x, double y ) const;

  /** Get the Average of all projected values on the specified axis. */
  virtual double getAverage ( hippodraw::Axes::Type axis ) const;

  /** Calculates the index of the column of the matrix, given the @a
      row of the NTuple.
  */
  /** Calculates the index of the column of the matrix, given the @a row
      of the NTuple. 
  */
  inline unsigned int calcColumnIndex ( unsigned int row ) const;

  /** Calculates the value of the column coordinate given the @a row
      of the NTuple. 
  */
  inline double calcColumnValue ( unsigned int row ) const;

  /** Calculates the index of the row of the matrix, given the @a row
      of the NTuple. 
  */
  inline unsigned int calcRowIndex ( unsigned int row ) const;

  /** Calculates the value of the row coordinate given the @a row
      of the NTuple. 
  */
  inline double calcRowValue ( unsigned int row ) const;

  /** Returns the size of the step along the X axis. 
   */
  inline double getXStep () const;

  /** Returns the size of the step along the Y axis. 
   */
  inline double getYStep () const;

  virtual DataSource * createNTuple () const;

  virtual void prepareValues ();

  /** Returns @c true since the projected values can be converted to
      an image.
   */
  virtual bool isImageConvertable () const;

protected:

  virtual void fillProjectedValues ( DataSource * ntuple, 
				     bool in_range = false ) const;
};

inline
unsigned int
MapMatrixProjector::
calcColumnIndex ( unsigned int row ) const
{
  if ( m_rows == 0 ) return 0;

  return row / m_rows;
}

inline
double
MapMatrixProjector::
calcColumnValue ( unsigned int row ) const
{
  unsigned int index = calcColumnIndex ( row );

  return m_x_origin + index * m_x_step + 0.5 * m_x_step;
}

inline
unsigned int
MapMatrixProjector::
calcRowIndex ( unsigned int row ) const
{
  unsigned int column = calcColumnIndex ( row );

  return row - column * m_rows;
}

inline
double
MapMatrixProjector::
calcRowValue ( unsigned int row ) const
{
  unsigned int index = calcRowIndex ( row );

  return m_y_origin + index * m_y_step + 0.5 * m_y_step;
}

inline
double
MapMatrixProjector::
getXStep ( ) const
{
  return m_x_step;
}

inline
double
MapMatrixProjector::
getYStep ( ) const
{
  return m_y_step;
}

} // namespace hippodraw

#endif // _MapMatrixProjector_H_
