/** @file

hippodraw::NumArrayTuple class implementation

Copyright (C) 2004-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: NumArrayTuple.cxx,v 1.67 2007/07/02 18:17:15 pfkeb Exp $

*/

// include first to avoid _POSIX_C_SOURCE warning.
#include <boost/python.hpp>

// for have numarray etc
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define PY_ARRAY_UNIQUE_SYMBOL HippoPyArrayHandle
#define NO_IMPORT_ARRAY

#include "NumArrayTuple.h"

#include "axes/Range.h"

#include "num_util.h"

#include <algorithm>
#include <stdexcept>

using namespace boost::python;

using std::runtime_error;
using std::string;
using std::vector;

using namespace hippodraw;

NumArrayTuple::NumArrayTuple ()
  : DataSource ()
{
}

NumArrayTuple::~NumArrayTuple()
{
}

void
NumArrayTuple::
copy ( const DataSource & )
{
  assert ( false );
}

void
NumArrayTuple::
notifyObservers ( ) const
{
  Observable::notifyObservers ();
}

unsigned int
NumArrayTuple::
rows() const
{
  unsigned int size = 0;

  if ( m_data.empty () == false ) {
    numeric::array na = getNumArray ( 0 );
    size = static_cast < unsigned int > ( num_util::get_dim ( na, 0 ) );
  }

  return size;
}

bool
NumArrayTuple::
empty () const
{
  return rows () == 0;
}

/** @todo Determine if the Python GIL should also be obtained when
    using numarray instead of Numeric.
*/
double
NumArrayTuple::
valueAt( unsigned int row, unsigned int column ) const
{
#if HAVE_NUMPY || HAVE_NUMERIC
  PyGILState_STATE state = PyGILState_Ensure ();
#endif

  assert ( column < m_data.size () );

  const numeric::array  array = m_data[column];

  int size = num_util::size ( array );
  assert ( row < static_cast < unsigned int > ( size ) );

  object result = array[row];

  double value = extract < double > ( result );
#if  HAVE_NUMPY || HAVE_NUMERIC
  PyGILState_Release ( state );
#endif

  return value;
}

/** @todo Could bypass calling valueAt if performance is shown to be
    an issue.
*/
const std::vector < double > &
NumArrayTuple::
getRow ( unsigned int row ) const
{
  unsigned int size = m_data.size();
  m_row.resize ( size );
  for ( unsigned int column = 0; column < size; column++ ) {
    m_row [ column ] = valueAt ( row, column );
  }

  return m_row;
}

/** @todo Should not have duplicated code from NTuple class. 
 */
int
NumArrayTuple::
addColumn ( const std::string & label,
	    boost::python::numeric::array array )
{
  // Check if label already exists.
  int index = indexOf ( label );
  if ( index >= 0 ) {
    string what ( "NumArrayTuple Attempt to add a column whose label, `");
    what += label;
    what += "', is same as existing column.";
    throw runtime_error ( what );
  }

  unsigned int new_size = num_util::get_dim ( array, 0 );
  // Check if column has right size.
  if ( m_data.empty () == false ) {
    unsigned int old_size = rows ();

    if ( old_size != 0 && old_size != new_size ) {
      string what ( "NumArrayTuple Attempt to add a column whose size"
		    " is not equal to other columns." );
      throw runtime_error ( what );
    }
  }
  m_data.push_back ( array );
  addLabel ( label );

  return m_data.size() - 1;
}

void
NumArrayTuple::
replaceColumn ( unsigned int col, 
		boost::python::numeric::array array )
{
  unsigned int size = columns ();
  if ( col >= size ) {
    const string what ( "NunArrayTuple: column doesn't exist" );
    throw runtime_error ( what );
  }

  const numeric::array old_array = m_data[col];
  int old_size = num_util::size ( old_array );
  int new_size = num_util::size ( array );

  if ( old_size != 0 && old_size != new_size ) {
    const string what ( "NumArrayTuple: Attempt to replace column with one "
			"whose size is not equal to other columns." );
    throw runtime_error ( what );
  }
  m_data[col] = array;

  notifyObservers ();
}

void
NumArrayTuple::
replaceColumn ( const std::string & column,
		boost::python::numeric::array array )
{
  unsigned int index = indexOf ( column );

  replaceColumn ( index, array );
}

numeric::array
NumArrayTuple::
getNumArray( unsigned int index ) const
{
  unsigned int size = columns ();
  if ( index >= size ) {
    const string what ( "NunArrayTuple: column doesn't exist" );
    throw runtime_error ( what );
  }
  return m_data[index];
}

numeric::array
NumArrayTuple::
getNumArray( const std::string & label ) const
{
   unsigned int index = indexOf ( label );
   return getNumArray( index );
}

void
NumArrayTuple::
setShape ( std::vector < unsigned int > & shape )
{
   m_shape = shape;
}

const vector < unsigned int > &
NumArrayTuple::
getShape () const
{
  return m_shape;
}

void
NumArrayTuple::
fillShape ( std::vector < intptr_t > & shape, unsigned int column ) const
{
  shape.clear ();
  numeric::array na = getNumArray ( column );

  shape = num_util::shape ( na );
}

void
NumArrayTuple::
clear ()
{
  assert ( false );
}

void
NumArrayTuple::
reserve ( unsigned int ) //row )
{
  assert ( false );
}

double 
NumArrayTuple::
operator [] (  std::vector < unsigned int > & ) const // indices ) const
{
  assert ( false );
  return 0.;
}
