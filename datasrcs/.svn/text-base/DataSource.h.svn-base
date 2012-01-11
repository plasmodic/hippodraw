/* -*- c++ -*- */

/** @file

hippodraw::DataSource class interface.

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DataSource.h,v 1.76 2007/05/17 22:58:21 pfkeb Exp $

*/

#ifndef DataSource_H
#define DataSource_H

#ifdef _MSC_VER
#include <msdevstudio/MSconfig.h>
#endif

#include "pattern/Observable.h"

#include <string>
#include <vector>

#ifndef _MSC_VER
#include <inttypes.h>
#endif

namespace hippodraw {
  class Range;


/** Base class for DataSource.  A data source is a class which is able
    to supply one or more arrays of data.  Access to the arrays can be
    by name, called a @em label, or by an index.  It provides
    translation of label to index and vice verse.  It also provides a
    name by which it can be uniquely identified in a client
    application and a title for the data set.  These attributes have
    concrete implementations in this base class.

    All member functions that access the data are pure virtual
    functions in this class and thus must be implemented in derived
    classes.  This class does not provide the interface for creating
    or modifying the data.

    A DataSource can also have a shape, meaning that a single array
    can be seen as a multi-dimension array with different sizes of
    each dimension.

    @todo Expand to data types other than double.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API DataSource : public Observable
{

 private:

  /** The name of the data source .  Although the use of this data
      member is optional, the suggested use within an application is
      as follows.
      @li If the data source is associated with a file, then it is the file name.
      @li If the data source was created in memory by an application, then it is some unique string created by the application.
  */
  std::string m_ds_name;

  /** The title. The title is descriptive of the data source.
   */
  std::string m_title;

  /** The labels of the vectors of the data. 
   */
  std::vector<std::string> m_labels;

  /** A flag to indicate the object is null.  A null object is a place
      holder so that classes that use this class can continue to
      function after their valid object has been deleted.  This can
      happen, for example, if the user deletes his data source without
      first deleting objects that use it. 
  */
  bool m_is_null;

protected:

  /** A temporary array that can be returned by const reference.
   */
  mutable std::vector < double > m_array;

  /** The shape of the data.  The elements the %DataSource can have an
      arbitrary number of dimensions, called @em rank.  An ordinary table
      has rank of 2 and the shape is number of columns and number of
      rows.  That is m_shape[0] is number of columns and m_share[1] is
      number of rows.
   */
  std::vector < unsigned int > m_shape;

  /** A list of labels that duplicated others in a DataSource file.
   */
  std::vector < std::string > m_dups;

  /** A constructor taking list of array labels as argument. 
   */
  DataSource ( const std::vector < std::string > & labels );

  /** Constructs an DataSource object with name @a name. */
  DataSource ( const std::string & name );

  /** Constructs an DataSource object with name @a name. 

      @note This constructor differs from one with std::string because
      that wouldn't be use by conversion of char *.
   */
  DataSource ( const char * name );

  /** The copy constructor creates an data source that is a copy of an
      existing one. 
  */
  DataSource ( const DataSource & nt);

  /** A constructor for creating a null object.  A null DataSource is one
      that is a place holder for one that got deleted while other
      objects were referencing it.  This can happen, for example, if a
      user deletes an DataSource before deleting other objects that use
      it.  Classes that reference DataSource objects can be Observer of
      then in order to receive the Observer::willDelete signal.  When
      received they can replace the referenced DataSource with a null one.
      They can then continue to function until a valid data source is given
      to them.    The parameter @a dummy is not used.
  */
  DataSource ( bool dummy );

  /** The default constructor creating an data source with 0 columns.
   */
  DataSource ();

  /** Copies the contents of the other DataSource.  For use by derived
      classes only.
   */
  virtual void copyPrivate ( const DataSource & other );

  /** Adds a new label for a column. 
   */
  virtual void addLabel ( const std::string & label );

  /** Returns @c true if @a index is valid, otherwise throws a
      DataSourceException.
  */
  bool isValidColumn ( unsigned int index ) const;

  /** Checks the number of columns.  Check that the number of columns
      of @a source is the same as the receiving object.  Throws a
      std::runtime_error if they are different, otherwise does
      nothing.
   */
  virtual void checkWidth ( const DataSource * source );

 public:

  /** The destructor. 
   */
  virtual ~DataSource();

  /** Returns @c true if the receiving objects is a null object.  @sa
      DataSource( bool ). 
  */
  bool isNull () const;

  /** Sets the name of the data source.  @sa #m_ds_name.
   */
  void setName ( const std::string & name );

  /** Returns the name of the data source.   @sa m_ds_name.
   */
  const std::string & getName () const;

  /** Returns the number of columns or data arrays available from this
      DataSource. 
      @note This function was selected to be in line sine it contributes
      heavily in time for addRow member function.
  */
  inline unsigned int columns() const;

  /** Returns the number of rows.
   */
  virtual unsigned int rows () const = 0;

  /** Returns @c true, if DataSource is empty, i.e., has no data.
   */
  virtual bool empty () const = 0;

  /** Returns a const reference to the title of the data source.
   */
  const std::string & title() const;

  /** Sets the title of the data source to @a title.
   */
  virtual void setTitle ( const std::string & title );

  /** Returns @c true if @a label is a valid label for a column in the
      DataSource.  The search is case sensitive.  If the label is not
      found, returns @c false.
  */
  bool isValidLabel ( const std::string & label ) const;

  /** Throws a DataSourceException object if @a label is not a valid
      label for this DataSource.  The exception object contains a
      message that the label is invalid.  Derived classes may
      over-ride this method with more information on why the label is
      invalid.
  */
  virtual void throwIfInvalidLabel ( const std::string & label ) const;

  /** Throws a DataSourceException if the size of the row is not equal
      to the number of columns, otherwise does nothing.
  */
  void throwIfInvalidRowSize ( const std::vector < double > & row );

  /** Changes the label of a data column to @a s.
      @param s the new label.  
      @param index the column number starting at zero.  
      @returns true if @a index is less than the width of 
      the data source; otherwise, it does nothing and returns false. 
  */
  virtual bool setLabelAt ( const std::string & s, unsigned int index );

  /** Assigns the label to each column from the vector of strings @ v.
   */
  virtual void setLabels ( const std::vector<std::string> & v );

  /** Returns the value in the table in position given by the @a row and
      @a column indexes.
  */
  virtual double valueAt ( unsigned int row, unsigned int column ) const = 0;

  /** Returns the value in the table in position given by the @a row and
      @a column indexes without storing it into cache.
  */
  virtual double valueAtNoCache ( unsigned int row, unsigned int column ) const;

  /** Returns the data in the column with label @a name.
   */
  virtual const std::vector < double > &
  getColumn ( const std::string & name ) const;

  /** Returns the data in the column with index @a column.
   */
  virtual const std::vector < double > & 
  getColumn ( unsigned int column ) const;

  /** Returns a const reference to slice along the axis known as a row. 
   */
  virtual const std::vector < double > & getRow ( unsigned int ) const = 0;

  /** Returns the list of available labels.
   */
  virtual const std::vector < std::string > & getLabels () const;

  /** Returns the label for the column at index @a index.  Returns the
      label for the column at index @a index, or throws a
      DataSourceException if @a index is out of range.
   */
  virtual const std::string & getLabelAt ( unsigned int index ) const;

  /** Returns @c true if the specified column labeled @a label has
      been filled.
  */

  /** Returns the index of the column whose label is @a label.
      Returns the index of the data column whose label is @a label, or
      -1 if @a label does not exist.
   */
  virtual int indexOf ( const std::string & label ) const;

  /** Sets the shape of the data elements. Sets the shape data
      elements from the vector @a shape.  The rank is given by the
      size of the vector.  A rank of 2 correspond to table.  If the
      total number of elements in the DataSource is not equal to the
      product of the shape, then an assertion is raised.
  */
  virtual void setShape ( std::vector < unsigned int > & shape );

  /** Returns the shape of the data elements. 
   */
  virtual const std::vector < unsigned int > & getShape () const;

  /** Fills the vector with the shape of a column.  Returns via the
      vector @a v, the shape of the data at column @a column.
      Most derived classes will only support a vector in a
      column. Thus as convenience to implementers of derived classes
      this base class implementation returns a shape of size 1
      containing the number of rows.
  */
  virtual void fillShape ( std::vector < int > & v,
			   unsigned int column ) const;

  /** Fills the vector with the shape of a column.  Returns via the
      vector @a v, the shape of the data at column @a column.
      Most derived classes will only support a vector in a
      column. Thus as convenience to implementers of derived classes
      the implementation of this base class returns a shape of size 1
      containing the number of rows.
  */
  virtual void fillShape ( std::vector < int > & v,
			   const std::string & label ) const;

  /** Returns the rank of the data source.  Returns the number of axes
      of the data source, i.e the rank. 
  */
  unsigned int getRank () const;

  /** Replaces data in column @a label with contents of @a array.  If
      column label @a label exists, replace it contents with @a array,
      otherwise throw std::runtime_error.

      @note Not all derived classes support replacing a column and
      this base class implementation throws a std::runtime_error if
      called.
   */
  virtual void replaceColumn ( const std::string & label,
			       const std::vector < double > & array );

  /** Replaces column at @a index with the contents of @a array.
      @note Not all derived classes support replacing a column and
      a std::runtime_error may be thrown if called.
   */
  virtual void replaceColumn ( unsigned int index,
			       const std::vector < double > & array );

  /** Replaces or adds column vector.  If column @a label exists,
      replaces that column with @a data, otherwise adds a new column
      with @a label.  In both cases, the number of rows, as defined by
      first element of the @a shape, must be the same as existing
      columns, if any.  Not all derived classes support the notion of
      shape, therefore as convenience to developers, the implementation
      in this base class throws a std::runtime_error object.
  */
  virtual void replaceColumn ( const std::string & label,
			       const std::vector < double > & data,
			       const std::vector < intptr_t > & shape );

  /** Adds column vector.  Adds a new column with @a label and
      returns its index.  The number of rows, as defined by first
      element of the @a shape, must be the same as existing columns,
      if any.  Not all derived classes support the notion of shape,
      therefore as convenience to developers, the implementation in this
      base class throws a std::runtime_error object.
  */
  virtual int addColumn ( const std::string & label,
			  const std::vector < double > & data,
			  const std::vector < intptr_t > & shape );

  /** Adds column vector. If implemented in a derived class, adds a
      new column with @a label and returns its index. 
      @note Not all derived
      classes support the notion of adding columns. The implementation
      in this base class throws a std::runtime_error object if called.
  */
  virtual int addColumn ( const std::string & label,
			  const std::vector < double > & data );

  /** Fills the Range object from data indexed by @a
      column.  Returns @c true if all the data in the column was
      valid, otherwise returns @c false.  Invalid data is +/- HUGE_VAL
      or NAN. Invalid data is not used in setting the range.
  */
  virtual bool fillRange ( unsigned int column, Range & ) const;

  /** Returns row index of the maximum element in a column for the
      given @a column.
   */
  virtual unsigned int indexOfMaxElement ( unsigned int index ) const;

  /** Returns row index of the minimum element in a column for the
      given @a column.
   */
  virtual unsigned int indexOfMinElement ( unsigned int index ) const;


  /** Returns the sum of all the elements in the sequence of column @a
      column.
  */
  virtual double sum ( unsigned int column ) const;

  /** Returns @c true if the column contains multidimensional data.
      Returns @c false.  Derived classes should return @c true if the
      column contains multidimensional data.
   */ 
  virtual bool isMultiDimensional ( const std::string & column ) const;
  
  /** Returns @c true if the column is usable.  Returns @c true.
      Derived classes should return @c false if the column is not
      usable in the context of a DataSource.  
  */
  virtual bool isUseable ( const std::string & column ) const;

  /** In case we are dealing with multidimensional data in rows of this column
      we would like to deal with on one entry of this multidimensional data.
      So here we set index of this relevant entry. */
  virtual bool setReleventIndex( const std::string & column,
				 const std::vector< unsigned int >& index );

  /** Clears the data source.
   */
  virtual void clear () = 0;

  /** For each column, reserves enough space for the data source to
      grow to @a count rows.
  */
  virtual void reserve ( unsigned int count ) = 0;

  /** Adds a row to the data source.  Since most derived classes are
      not capable of adding a row, the implementation of this member
      functions throws std::runtime_error.
  */
  virtual void addRow ( const std::vector < double > & row );

  /** Erase a row from the data source. The implementation throws
      std::runtime_error.
  */
  virtual void eraseRow ( unsigned int index );

  virtual double 
  operator [] (  std::vector < unsigned int > & indices ) const = 0;


  /** Returns the column labels that were duplicated, or an empty vector.
   */
  const std::vector < std::string > & getDuplicatedLabels () const;

  /** Appends the contents of the DataSource @a source.  Appends the
      contents of the DataSource @a source to end by adding rows.

      @note If a derived class has a more efficient way of appending, it
      should override this method.
  */
  virtual void append ( const DataSource * source );

  /** Returns an array at @a row and @a column. If the DataSource is
      capable of containing an array in a column, then returns pointer
      to the array for a single row, otherwise throws
      std::runtime_error exception.
  */
  virtual double * doubleArrayAt ( unsigned int row,
				   unsigned int column ) const;


  /** Expand the multidimension column if needed. Do nothing in base class.
      Reimplemented in RootNTuple. 
   */
  virtual void expandIfNeeded( const std::vector<std::string> & labels ) const;

};

inline
unsigned int
DataSource::
columns () const
{
  return static_cast < unsigned int > ( m_labels.size() );
}

} // namespace hippodraw

#endif // DataSource_H
