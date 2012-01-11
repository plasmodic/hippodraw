/* -*- c++ -*- */

/** @file

hippodraw::ListTuple class interface.

Copyright (C) 2004, 2006, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: ListTuple.h,v 1.18 2007/08/24 22:19:16 pfkeb Exp $

*/

#ifndef ListTuple_H
#define ListTuple_H

#include "datasrcs/DataSource.h"

namespace boost {
  namespace python {
    class list;
  }
}

namespace hippodraw {

/** A DataSource class implemented with a Python list to
    store the column data.  The data elements are stored in Python and
    are not copied or modified.

    @note Some compilers do not handle the using boost::python
    completely correct, they see the name `list' in a declaration as
    an object name, not the boost type.  Gcc 2.95.3 is one of them.
    Thus the implementation gives the full scoped name.

    @todo Could implement addRow()

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class ListTuple : public DataSource
{

private:

  /** The numarray objects that contains the data. 
   */
  std::vector < boost::python::list > m_data;

  /** A temporary array of data from one row of each column.  The
      object is @em mutable because some const member functions fill
      it.
   */
  mutable std::vector < double > m_row;

public:

  /** The default constructor.
   */
  ListTuple ();

  /** The destructor. 
   */
  virtual ~ListTuple();

  /** @note For multi-processor machines, need the to get the the
      Python global interpreter lock when compiled with Qt 4 on Mac OS
      X and maybe other systems.  However, with Qt 3, obtaining the
      lock seems to lock up the system.

      The implementation of this
      member function is taken from
      http://docs.python.org/api.thread.html
  */
  unsigned int static getSize ( const boost::python::list & sequence );

  /** Raises an exception as with this release copying is not
      supported.
  */
  virtual void copy ( const DataSource & );

  /** Notifies observers.  @todo remove this method when Observable is
      exposed to Python.
    */
  virtual void notifyObservers ( ) const;

  /** Returns the size of the slice for the next to last dimension.
   */
  virtual unsigned int rows () const;

  /** Returns @c true, if ListTuple is empty, i.e. has no data.
   */
  virtual bool empty () const;

  virtual double valueAt ( unsigned int row, unsigned int column ) const;

  /** Adds a column to the end of the ListTuple.  If the size of the
      input array is the same as the size of the existing columns adds
      input array to then end of the ListTuple . Gives the column the
      label @a label.  Returns the index of the newly created column.
      If the size of the input array differs from the size of existing
      columns or if the label already exists, then throws a
      DataSourceException object.
  */
  int addColumn ( const std::string & label, 
		  boost::python::list seq );

  /** Replaces the column indexed by @a index with the @a array. If a
      column with @a index doesn't exits or if the column to be
      replace is not empty and the size of @a array is not the same as
      existing columns, then a DataSourceException is thrown.
   */
  void replaceColumn ( unsigned int index, 
		       boost::python::list array );

  /** Replaces the column.  Replaces the numarray at the column with
      label @a label.  This is a convenience member function for using
      indexOf() and replaceColumn ( unsigned int,
      boost::python::list array ).
  */
  void replaceColumn ( const std::string & label,
		       boost::python::list array );

  /** Returns a const reference to slice along the axis known as a row. 
   */
  virtual const std::vector < double > & getRow ( unsigned int index ) const;

  /** Sets the shape of the data elements. Sets the shape data
      elements from the vector @a shape.  The rank is given by the
      size of the vector.  A rank of 2 correspond to table.  If the
      total number of elements in the ListTuple is not equal to the
      product of the shape, then an assertion is raised.
  */
  virtual void setShape ( std::vector < unsigned int > & shape );

  /** Returns the shape of the data elements. 
   */
  const std::vector < unsigned int > & getShape () const;

  /** Raises assertion because the contained Python list should not be
      changed.
   */
  virtual void clear ();

  /** Raises assertion because the contained Python list should not be
      changed.
   */
  virtual void reserve ( unsigned int count );

  /** Raises assertion because this method is not implemented yet.
   */
  virtual double 
  operator [] (  std::vector < unsigned int > & indices ) const;

private:

  /** Returns @c true if all the elements of the list @a seq can be
      converted to type @c double, otherwise returns @c false.
  */
  bool isAcceptable ( const boost::python::list & seq );

};

} // namespace hippodraw

#endif // ListTuple_H
