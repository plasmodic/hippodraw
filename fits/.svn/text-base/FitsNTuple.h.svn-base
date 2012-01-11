/* -*- c++ -*- */

/** @file

hippodraw::FitsNTuple class interface.

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FitsNTuple.h,v 1.37 2007/05/09 19:28:48 pfkeb Exp $

*/

#ifndef FitsNTuple_H
#define FitsNTuple_H

#include "datasrcs/DataSource.h"

namespace hippodraw {

  class DataColumn;
  class FitsFile;

/** A DataSource class implemented with a vector < DataColumn * >
    objects.  The source of the data can be copied from FITS binary
    table or image.  Data from the FITS file is read in from memory
    only when needed.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API FitsNTuple : public DataSource
{

private:

  /** The DataColumn objects that contains the data and its attributes.
   */
  std::vector < DataColumn * > m_columns;

  /** The FitsFile used for this DataSource.
   */
  FitsFile * m_file;

  /** The HDU number used for this object.  The first (primary) HDU is
      = 1.
   */
  int m_hdu_num;

  /** The default constructor.  

      @note The default constructor is private, because implementation
      depends on reading a fits file before adding columns.
   */
  FitsNTuple ( );

  /** If @a label already exists for a column, throw
      DataSourceException, otherwise do nothing.
  */
  void checkLabel ( const std::string & label );

public:

  /** A constructor with a FitsFile as argument.
   */
  FitsNTuple ( FitsFile * file );

  /** The destructor. 
   */
  virtual ~FitsNTuple();

  /** Raises exception because with this release, copying the fits
      table is not supported.
  */
  virtual void copy ( const DataSource  & );

  /** Returns the FITS file used to create this object.
   */
  const FitsFile * getFile () const;

  /** Notifies observers. 
      @todo remove this method when Observable is exposed to Python.
   */
  virtual void notifyObservers ( ) const;

  /** Returns the number of rows of FITS ASCII or binary table.
   */
  virtual unsigned int rows () const;

  /** Returns the the column.  Returns the total size of the column @a
      c. If the column contains an array, then the size is the product
      of shape dimensions, otherwise the size is the number of rows.
   */
  std::size_t size ( unsigned int c ) const;

  /** Returns @c true, if FitsNTuple is empty, i.e. has no data.
   */
  virtual bool empty () const;

  /** Clears the data cache; doesn't not modify the contained fits
      file.
  */
  virtual void clear ();

  /** Raises assertion as the contained FITS file should not be
      changed.
  */
  virtual void reserve ( unsigned int count );

  /** Raises assertion as this method is not yet implemented.
   */
  virtual double 
  operator [] (  std::vector < unsigned int > & indices ) const;

  virtual double valueAt ( unsigned int row, unsigned int column ) const;

  /** Adds a column to the end of the FitsNTuple.  If the size of the
      input array is the same as the size of the existing columns adds
      input array to then end of the FitsNTuple . Gives the column the
      label @a label and sets its shape as a rank 1 array.  Returns
      the index of the newly created column.  If the size of the input
      array differs from the size of existing columns or if the label
      already exists, then throws a DataSourceException object.
  */
  int addColumn ( const std::string & label, 
		  const std::vector < double > & column );

  /** Adds a column to the end of the FitsNTuple.  If the number of rows of the
      input array is the same as that of  existing columns adds
      input array to then end of the FitsNTuple . Gives the column the
      label @a label and sets its @a shape attribute.  Returns
      the index of the newly created column.  If the size of the input
      array differs from the size of existing columns or if the label
      already exists, then throws a std::runtime_error object.
  */
  virtual int addColumn ( const std::string & label, 
			  const std::vector < double > & column,
			  const std::vector < intptr_t > & shape );

  /** Replaces the data in column @a index.  If @a index is in a valid
      range and if the size of @a data is the same as existing
      columns, then replaces the specified column with a copy of data
      in @a data and sets it shape to rank 1 array.  If @a index is
      not a valid range or the sizes differ, then throws a
      DataSourceException.  In neither case is the FITS file modified.
   */
  void replaceColumn ( unsigned int index,
		       const std::vector < double > & data );

  /** Replaces the data in column @a index.  If @a index is in a valid
      range and if the size of @a data is the same as existing
      columns, then replaces the specified column with a copy of data
      in @a data and sets its shape.  If @a index is not a valid range
      or the sizes differ, then throws a DataSourceException.  In
      neither case is the FITS file modified.
   */
  virtual void replaceColumn ( unsigned int index,
			       const std::vector < double > & data,
			       const std::vector < intptr_t > & shape );

  /** Replaces the data in column labeled @a label.  If the @a label
      is valid and if the size of @a data is the same as existing
      columns, then replaces the specified column with a copy of data
      in @a data and sets its shape.  If the @a label is not valid or
      the sizes differ, then throws a DataSourceException.
   */
  void replaceColumn ( const std::string & label,
		       const std::vector < double > & data,
		       const std::vector < intptr_t > & shape );

  /** Returns a const reference to slice along the axis known as a row. 
   */
  virtual const std::vector < double > & getRow ( unsigned int ) const;

  /** Sets the shape of the data elements. Sets the shape data
      elements from the vector @a shape.  The rank is given by the
      size of the vector.  A rank of 2 correspond to table.  If the
      total number of elements in the FitsNTuple is not equal to the
      product of the shape, then an assertion is raised.
  */
  virtual void setShape ( std::vector < unsigned int > & shape );

  /** Returns the shape of the data elements. 
   */
  const std::vector < unsigned int > & getShape () const;

  virtual bool fillRange ( unsigned int column, Range & ) const;

  /** Returns the shape of the column @a index.
   */
  virtual void fillShape ( std::vector < intptr_t > & shape,
			   unsigned int index ) const;

  /** Returns the data of the column.  Returns a reference to the
      total data in column @a c.  If the column contains an array, the
      size of the vector will the the product of its dimensions
      including the number of rows of the FITS binary table.
   */
  virtual const std::vector < double > & getColumn ( unsigned int c ) const;

private:

  /** Initializes the columns of the DataSource.
   */
  void initColumns ();

  /** Fills the data cache for column @a column.  Returns the cfitsio
      status variable.
   */
  int fillDataCache ( unsigned int column );

  /** Returns data array as double for @a row and @a column.
   */
  virtual double * doubleArrayAt ( unsigned int row,
				    unsigned int column ) const; 

};

} // namespace hippodraw

#endif // FitsNTuple_H
