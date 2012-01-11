/* -*- c++ -*- */

/** @file

hippodraw::NTuple class interface.

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: NTuple.h,v 1.156 2006/10/01 23:36:19 pfkeb Exp $

*/

#ifndef NTUPLE_H
#define NTUPLE_H

#ifdef _MSC_VER
# pragma warning(disable:4217) 
# pragma warning(disable:4290) // exception spec ignored except to indicate
#endif

#include "DataSource.h"

namespace hippodraw {

/** A DataSource class implemented with std::vector<double> to store
    the column data.

    @request @@ Besides interval count, need interval timer.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API NTuple : public DataSource
{

 private:

  /** The interval count.  When used, the number of NotifyObserver messages
      that are received before an update is passed to this object's
      observers.
  */
  int m_i_count;

  /** The current interval count.  When used, this is the current
      count of NotifyObserver messages received since the last update
      was sent to Observer object of this object.
  */
  mutable int m_i_current;

  /** The interval flag.   When true, interval counting is enabled.
   */
  bool m_i_enabled;

  /** The data columns of the data source table.  Note that the
      pointer to the columns is used so that when columns are added,
      the column data doesn't need to be relocated.
  */
  std::vector< std::vector<double> *> m_data;

  /** A temporary vector corresponding to data elements of one row. @sa
      getRow.
  */
  mutable std::vector < double > m_row;

  /** The assignment operator is private to avoid use of it.
   */
  NTuple & operator= ( const NTuple & rhs );

  /** Resizes the the number of columns of the NTuple.  If @a new_size
      is greater then the current number of rows, then the appropriate
      number of columns containing empty vectors are added.  If @a new_size is
      less then the current number of rows, then the appropriate
      number of columns are erased.
  */
  void resizeColumns ( size_t new_size );

 public:

  /** Constructs an NTuple object with name @a name.
   */
  explicit NTuple ( const std::string & name );

  /** Constructs an NTuple object with name @a name.

      @note because explicit is used, this method is different from
      the one that take std::string & argument"
  */
  explicit NTuple ( const char * name );

  /** Constructor taking a vector of column labels as argument.
   */
  explicit NTuple ( const std::vector < std::string > & labels );

  /** The copy constructor creates an ntuple that is a copy of an
      existing one. 
  */
  explicit NTuple ( const NTuple & nt);

  /** A constructor that creates an ntuple from another DataSource
      object.
  */
  explicit NTuple ( const DataSource * ds );

  /** A constructor that creates an ntuple with @a n columns. Creates
      an NTuple with @a n columns.  The objects will be empty, i.e. 0
      rows.
   */
  explicit NTuple ( unsigned int n );

  /** A constructor for creating a null object.  A null NTuple is one
      that is a place holder for one that got deleted while other
      objects were referencing it.  This can happen, for example, if a
      user deletes an NTuple before deleting other objects that use
      it.  Classes that reference NTuple objects can be Observer of
      then in order to receive the Observer::willDelete signal.  When
      received they can replace the referenced NTuple with a null one.
      They can then continue to function until a valid ntuple is given
      to them.    The parameter @a dummy is not used.
  */
  explicit NTuple ( bool dummy );

  /** The default constructor creating an ntuple with 0 columns.
   */
  NTuple ();

  /** The destructor. 
   */
  virtual ~NTuple();

  /** Copies @a rhs data.
   */
  virtual void copy ( const DataSource & rhs );

  /** Appends the data from the @a source.
   */
  virtual void append ( const DataSource * source );

  /** Clears the NTuple.  Clears all the data elements of the %NTuple.
      On return, the number of rows is zero.  Does not change the
      other attributes or capacity.
  */
  virtual void clear();
  
  /** Returns @c true, if NTuple is empty, i.e. has no rows. 
   */
  virtual bool empty () const;

  /** Returns the number of rows.
   */
  virtual unsigned int rows () const;

  /** Adds a row to the end of the ntuple.  If the size of the input
      vector is the same as the width of the ntuple, adds the row to
      the end of the ntuple and notifies its Observer objects.
      Otherwise, throws a std::runtime_error object.
  */
 virtual void addRow ( const std::vector<double> & v );

  /** Inserts a Row.  Adds a row before the existing row specified by
      @a index from the elements of @a v. 
  */
  virtual void insertRow ( unsigned int index,
			   const std::vector < double > & v );

  /** Erases a row from the NTuple.  Removes the row @a index from
      the %NTuple.  Rows beyond the removal point move forward to fill
      the space.  If @a index is out of range, and exception is
      thrown.
  */
  virtual void eraseRow ( unsigned int index );

  /** Returns a temporary vector of data elements in one row.  Returns
      a temporary vector with data elements copied from the NTuple row
      indexed by @a index.  If @a index is out of range, throws a
      DataSourceException object.
  */
  virtual const std::vector < double > & getRow ( unsigned int index ) const;

  virtual void reserve ( unsigned int count );

  /** Adds a column to the end of the ntuple and returns the index to
      the added column.  
      If the size of the input vector is the same
      as the size of the existing columns adds a copy of the input
      vector to then end of the NTuple. Gives the column the label @a
      label.  Returns the index of the newly created column.  If the
      size of the input vector differs from the size of existing
      columns, then throws a std::runtime_error object.
  */
  virtual int addColumn ( const std::string &, 
			  const std::vector < double > & column );

  /** Replaces the data in column @a col.  If @a index is in a valid
      range and if the size of @a data is the same as the existing
      column, then replaces the elements of the specified column with
      a copy of @ data elements.  If @a index is not a valid range or
      the sizes differ, then throws a std::runtime_error object.
  */
  virtual void replaceColumn ( unsigned int index, 
			       const std::vector< double > & data );

  /** Replaces the data in the row @a i. If @a index is in a valid
      range and if the size of @a data is the same as the existing
      column, then replaces the elements of the specified column with
      a copy of @ data elements.  If @a index is not a valid range or
      the sizes differ, then throws a DataSourceException object
   */
  void replaceRow ( unsigned int i,
		    const std::vector < double > & data );


  /** Assigns the label to each column from the vector of strings. If
      the vector is the same size as the width of the ntuple, then it
      sets the label and returns @c true; otherwise, it throws a
      DataSourceException.  If the NTuple is empty, i.e. has no rows,
      then changes the NTuple's number of columns to the size of the
      vector.
  */
  void setLabels ( const std::vector<std::string> & v );


  virtual double 
  operator [] (  std::vector < unsigned int > & indices ) const;

  virtual double valueAt ( unsigned int row, unsigned int column ) const;

  virtual const std::vector< double > & 
  getColumn ( unsigned int index ) const;

  /** Returns a non-const reference to column at the index @a index. 
      @a index is in a valid range, returns a reference to the the
      column at index @a index.  If @a index is not in a valid range,
      throws a DataSourceException object.
  */
  virtual std::vector< double > & getColumn ( unsigned int index );

  const std::vector< double > & getColumn ( const std::string & label ) const;

  virtual unsigned int indexOfMinElement ( unsigned int index ) const;
  virtual unsigned int indexOfMaxElement ( unsigned int index ) const;

  /** Returns the value of the minimum element of column @a index.
   */
  virtual double minElement ( unsigned int index ) const;

  /** Returns the value of the maximum element of column @a index.
   */
  virtual double maxElement ( unsigned int index ) const;

  /** Returns minimum element in a column whose label is given by name
   */
  double columnMin( const std::string & name ) const;

  /** Returns minimum element in a column whose label is  given by name
   */
  double columnMax( const std::string & name ) const;

  virtual void notifyObservers () const;

  /** Sets interval counting to be enabled.  If @a yes is @c false,
      then a notifyObservers is immediately issued.
  */
  void setIntervalEnabled ( bool yes = true );

  /** Returns @c true if interval counting is enabled, otherwise
      returns false.
  */
  bool isIntervalEnabled () const;

  /** Sets the interval count. One needs to call setIntervalEnabled
      for this to have effect. When interval counting is enabled, then
      only each @a number notifyObsevers messages will trigger an
      Observer::update message to the list of Observer objects.
  */
  void setIntervalCount ( int number );

  /** Returns the current interval count.
   */
  unsigned int getIntervalCount ( ) const;

  /** Returns the Range of data in the @a column via the argument.
      Always returns true, i.e. does not check for valid data.
  */
  virtual bool fillRange ( unsigned int column, Range & ) const;
  virtual double sum ( unsigned int column ) const;

};

} // namespace hippodraw

#endif // NTUPLE_H
