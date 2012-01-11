/* -*- c++ -*- */

/** @file

hippodraw::NumArrayTuple class interface.

Copyright (C) 2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: NumArrayTuple.h,v 1.48 2007/07/02 18:17:15 pfkeb Exp $

*/

#ifndef NumArrayTuple_H
#define NumArrayTuple_H

// for have numarray etc
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_NUMPY
#include <numpy/noprefix.h>
#else
#ifdef HAVE_NUMERIC
#include <Numeric/arrayobject.h>
#else
#include <numarray/arrayobject.h>
typedef int intp;
#endif
#endif

#include "datasrcs/DataSource.h"

namespace boost {
  namespace python {
    namespace numeric {
      class array;
    }
  }
}

namespace hippodraw {

/** A DataSource class implemented with a Python numarray array to
    store the column data.  The data elements are stored in Python and
    are not copied or modified.

    @bug @@@@@@ Currently the config/m4/pfkeb_have_numarray passes its
    test when Numeric is found instead of numarray.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class NumArrayTuple : public DataSource
{

 private:

  /** The numarray objects that contains the data. 
   */
  std::vector < boost::python::numeric::array > m_data;

  /** A temporary array of data from one row of each column.  The
      object is @em mutable because some const member functions fill
      it.
   */
  mutable std::vector < double > m_row;

 public:

  /** The default constructor.
   */
  NumArrayTuple ();

  /** The destructor. 
   */
  virtual ~NumArrayTuple();

  /** Raises exception because with this release making a copy is not
      supported.
  */
  virtual void copy ( const DataSource & );

  /** Notifies observers. 
      @todo remove this method when Observable is exposed to Python.
   */
  virtual void notifyObservers ( ) const;

  /** Returns the size of the slice for the next to last dimension.
   */
  virtual unsigned int rows () const;

  /** Returns @c true, if NumArrayTuple is empty, i.e. has no data.
   */
  virtual bool empty () const;

  virtual double valueAt ( unsigned int row, unsigned int column ) const;

  /** Adds a column to the end of the NumArrayTuple.  If the size of the
      input array is the same as the size of the existing columns adds
      input array to then end of the NumArrayTuple . Gives the column the
      label @a label.  Returns the index of the newly created column.
      If the size of the input array differs from the size of existing
      columns or if the label already exists, then throws a
      DataSourceException object.
  */
  int addColumn ( const std::string & label, 
		  boost::python::numeric::array array );

  /** Replaces the column indexed by @a index with the @a array. If a
      column with @a index doesn't exits or if the column to be
      replace is not empty and the size of @a array is not the same as
      existing columns, then a DataSourceException is thrown.
   */
  void replaceColumn ( unsigned int index, 
		       boost::python::numeric::array array );

  /** Replaces the column.  Replaces the numarray at the column with
      label @a label.  This is a convenience member function for using
      indexOf() and replaceColumn ( unsigned int,
      boost::python::numeric::array array ).
  */
  void replaceColumn ( const std::string & label,
		       boost::python::numeric::array array );

   /** Return the reference to the desired numarray by column label. */
   boost::python::numeric::array
   getNumArray( const std::string & label ) const;

   /** Return the reference to the desired numarray by column index. */
   boost::python::numeric::array
   getNumArray( unsigned int index ) const;

  /** Returns a const reference to slice along the axis known as a row. 
   */
  virtual const std::vector < double > & getRow ( unsigned int index ) const;

  /** Sets the shape of the data elements. Sets the shape data
      elements from the vector @a shape.  The rank is given by the
      size of the vector.  A rank of 2 correspond to table.  If the
      total number of elements in the NumArrayTuple is not equal to the
      product of the shape, then an assertion is raised.
  */
  virtual void setShape ( std::vector < unsigned int > & shape );

  /** Returns the shape of the data elements. 
   */
  const std::vector < unsigned int > & getShape () const;

  /** Fills the vector with the shape of a column.  Returns via the
      vector @a v, the shape of the contained numarray at the @a
      column column.  The results is identical the shape returned by
      the Python object, i.e. the first index is row index.  @note @c
      int is chosen to be compatible with num_util.
  */
  virtual void fillShape ( std::vector < intptr_t > & v,
			   unsigned int column ) const;

  /** Raises assertion as this method is not implemented yet. */
  virtual void clear ();

  /** Raises assertion as this method is not implemented yet. */
  virtual void reserve ( unsigned int count );

  /** Raises assertion as this method is not implemented yet. */
  virtual double 
  operator [] (  std::vector < unsigned int > & indices ) const;

};

} // namespace hippodraw

#endif // NumArrayTuple_H
