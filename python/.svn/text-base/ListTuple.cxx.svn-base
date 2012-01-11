/** @file

hippodraw::ListTuple class implementation

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: ListTuple.cxx,v 1.28 2007/08/24 22:19:16 pfkeb Exp $

*/

// include first to avoid _POSIX_C_SOURCE warning.
#include <boost/python.hpp>

#ifdef HAVE_CONFIG_H
// for have smp and qt4
#include "config.h"
#endif

#include "ListTuple.h"

#include "axes/Range.h"

#include <stdexcept>

using namespace boost::python;

using std::runtime_error;
using std::string;
using std::vector;

// namespace
// {
//   /** @note For multi-processor machines, need the to get the the
//       Python global interpreter lock when compiled with Qt 4 on Mac OS
//       X and maybe other systems.  However, with Qt 3, obtaining the
//       lock seems to lock up the system.

//       The implementation of this
//       member function is taken from
//       http://docs.python.org/api.thread.html
//   */
//   unsigned int getSize ( const boost::python::list & sequence )
//   {
// #ifdef HAVE_QT4
// #ifdef HAVE_SMP
//     PyGILState_STATE gstate;
//     gstate = PyGILState_Ensure ();
// #endif
// #endif

//     object obj = sequence.attr ( "__len__" ) ();
//     unsigned int size = extract < unsigned int > ( obj );

// #ifdef HAVE_QT4
// #ifdef HAVE_SMP
//     PyGILState_Release ( gstate );
// #endif
// #endif

//     return size;
//   }
// }

using namespace hippodraw;

ListTuple::ListTuple ()
  : DataSource ()
{
}

ListTuple::~ListTuple()
{
}

unsigned int 
ListTuple::
getSize ( const boost::python::list & sequence )
{
#ifdef HAVE_QT4
#ifdef HAVE_SMP
  PyGILState_STATE gstate;
  gstate = PyGILState_Ensure ();
#endif
#endif

  object obj = sequence.attr ( "__len__" ) ();
  unsigned int size = extract < unsigned int > ( obj );

#ifdef HAVE_QT4
#ifdef HAVE_SMP
  PyGILState_Release ( gstate );
#endif
#endif

  return size;
}

void
ListTuple::
copy ( const DataSource & )
{
  assert ( false );
}

void
ListTuple::
notifyObservers ( ) const
{
  Observable::notifyObservers ();
}

unsigned int
ListTuple::
rows() const
{
  unsigned int size = 0;
  if ( m_data.empty () == false ) {
    const boost::python::list & seq = m_data[0];

    size = getSize ( seq );
  }

  return size;
}

bool
ListTuple::
empty () const
{
  return rows () == 0;
}

double
ListTuple::
valueAt( unsigned int row, unsigned int column ) const
{
  assert ( column < m_data.size () );
  double value = 0; 

  const boost::python::list & seq = m_data[column];

  unsigned int size = getSize ( seq );

  assert ( row < size );

  object result = seq[row];
  value = extract < double > ( result );

  return value;
}

/** @todo Could bypass calling valueAt if performance is shown to be
    an issue.
*/
const std::vector < double > &
ListTuple::
getRow ( unsigned int row ) const
{
  unsigned int size = m_data.size();
  m_row.resize ( size );
  for ( unsigned int column = 0; column < size; column++ ) {
    m_row [ column ] = valueAt ( row, column );
  }

  return m_row;
}

bool
ListTuple::
isAcceptable ( const  boost::python::list & array )
{
  bool yes = true;
  unsigned int size = getSize ( array );
  for ( unsigned int i = 0; i < size; i++ ) {
    object obj = array[i];
    extract < double > check ( obj );
    if ( check.check() == false ) {
      yes = false;
      break;
    }
  }
  return yes;
}

/** @todo Should not have duplicated code from NTuple class. 
 */
int
ListTuple::
addColumn ( const std::string & label,
	    boost::python::list array )
{
  // Check if label already exists.
  int index = indexOf ( label );
  if ( index >= 0 ) {
    string what ( "ListTuple Attempt to add a column whose label"
		  " is same as other column." );
    throw runtime_error ( what );
  }

  unsigned int new_size = getSize ( array );
  // Check if column has right size.
  if ( m_data.empty () == false ) {
    unsigned int old_size = rows ();

    if ( old_size != 0 && old_size != new_size ) {
      string what ( "ListTuple Attempt to add a column whose size"
		    " is not equal to other columns." );
      throw runtime_error ( what );
    }
  }
  if ( isAcceptable ( array ) == false ) {
    string what ( "ListTuple: Attempt to add a column with one or more"
		  " elements not convertable to float" );
    throw runtime_error ( what );
  }

  m_data.push_back ( array );
  addLabel ( label );

  return m_data.size() - 1;
}

void
ListTuple::
replaceColumn ( unsigned int col, 
		boost::python::list array )
{
  unsigned int size = columns ();
  if ( col >= size ) {
    const string what ( "NunArrayTuple: column doesn't exist" );
    throw runtime_error ( what );
  }

  const boost::python::list & old_array = m_data[col];
  int old_size = getSize ( old_array );
  int new_size = getSize ( array );

  if ( old_size != 0 && old_size != new_size ) {
    const string what ( "ListTuple: Attempt to replace column with one "
			"whose size is not equal to other columns." );
    throw runtime_error ( what );
  }
  m_data[col] = array;

  notifyObservers ();
}

void
ListTuple::
replaceColumn ( const std::string & column,
		boost::python::list array )
{
  unsigned int index = indexOf ( column );

  replaceColumn ( index, array );
}

void
ListTuple::
setShape ( std::vector < unsigned int > & shape )
{
  m_shape = shape;
}

const vector < unsigned int > &
ListTuple::
getShape () const
{
  return m_shape;
}
void
ListTuple::
clear ()
{
  assert ( false );
}

void
ListTuple::
reserve ( unsigned int )
{
  assert ( false );
}

double 
ListTuple::
operator [] (  std::vector < unsigned int > & /* indices */ ) const
{
  assert ( false );
  return 0.0;
}
