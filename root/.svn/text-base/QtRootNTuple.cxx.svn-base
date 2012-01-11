/** @file

hippodraw::QtRootNTuple class implementation.

Copyright (C) 2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtRootNTuple.cxx,v 1.43 2007/07/02 18:17:16 pfkeb Exp $

*/

// include first to avoid _POSIX_C_SOURCE warning.
#include <boost/python.hpp>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// For truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "QtRootNTuple.h"

#include "python/PyApp.h"

#ifdef HAVE_NUMARRAY
#include "numarray/num_util.h"
#endif

#include <stdexcept>
#include <utility>

using std::runtime_error;
using std::string;
using std::vector;

using namespace hippodraw;

QtRootNTuple::
QtRootNTuple ( TTree * tree )
  : RootNTuple ( tree )
{
}

QtRootNTuple::
QtRootNTuple ( )
  : RootNTuple ( )
{
}

unsigned int
QtRootNTuple::
rows () const
{
  PyApp::lock();
  unsigned int rows =  RootNTuple::rows ();
  PyApp::unlock ();

 return rows;
}

unsigned int
QtRootNTuple::
columns () const
{
  // doesn't call ROOT, so no need for lock
 return RootNTuple::columns ();
}

const vector < double > &
QtRootNTuple::
getColumn ( const std::string & name ) const
{
  static vector < double > temp;
  PyApp::lock();
  try {
    const vector < double > & column = RootNTuple::getColumn ( name );
    PyApp::unlock ();
    return column;
  }
  catch ( const runtime_error & e ) {
    PyApp::unlock ();
    throw e;
  }
  return temp;
}

const vector < double > &
QtRootNTuple::
getColumn ( const std::string & name,
	    const std::vector < int > & indexes ) const
{
  static vector < double > temp;
  PyApp::lock();
  try {
    const vector < double > & column 
      = RootNTuple::getColumn ( name, indexes );
    PyApp::unlock ();
    return column;
  }
  catch ( const runtime_error & e ) {
  PyApp::unlock ();
    throw e;
  }
  return temp;
}

const vector < double > &
QtRootNTuple::
getColumn ( unsigned int index ) const
{
  static vector < double > temp;
  PyApp::lock();
  try {
    const vector < double > & column = RootNTuple::getColumn ( index );
    PyApp::unlock ();
    return column;
  }
  catch ( const runtime_error & e ) {
  PyApp::unlock ();
    throw e;
  }
 
  return temp;
}

const vector < double > &
QtRootNTuple::
getRow ( unsigned int index ) const
{
  static vector < double > temp;
  PyApp::lock();
  try {
    const vector < double > & row = RootNTuple::getRow ( index );
    PyApp::unlock ();
    return row;
  }
  catch  ( const runtime_error & e ) {
    PyApp::unlock ();
    throw e;
  }

  return temp;
}

int
QtRootNTuple::
addColumn ( const std::string & label,
	    const std::vector < double > & column )
{
  return RootNTuple::addColumn ( label, column );
}

const std::vector < std::string > &
QtRootNTuple::
getLabels () const
{
  // doesn't call ROOT, so no lock needed
  return RootNTuple::getLabels ();
}

bool
QtRootNTuple::
isMultiDimensional ( const std::string & column ) const
{
  PyApp::lock();
  try {
    bool yes = RootNTuple::isMultiDimensional ( column );
    PyApp::unlock ();
    return yes;
  }
  catch ( const runtime_error & e ) {
    PyApp::unlock ();
    throw e;
  }
}

void
QtRootNTuple::
sliceRowDimension ( std::vector < int > & shape )
{
  unsigned int rank = shape.size ();
  for ( unsigned int i = 0; i < rank -1; i++ ) {
    shape[i] = shape[i+1];
  }
  shape.pop_back ();
}

const std::vector < int >
QtRootNTuple::
getColumnShape ( const std::string & label ) {
  PyApp::lock();
  try {
    int column = indexOf ( label );
    if ( column < 0 ) {
      string what ( "RootNTuple: No column with named `" );
      what += label;
      what += "'.";
      throw std::runtime_error ( what );
    }
    vector < int > shape;
    fillShape ( shape, column );
    sliceRowDimension ( shape );

    PyApp::unlock ();

    return shape;
  }
  catch ( const runtime_error & e ) {
    PyApp::unlock ();
    throw e;
  }
}

void
QtRootNTuple::
expandIfNeeded ( const std::vector < std::string > & labels ) const
{
  PyApp::lock ();
  RootNTuple::expandIfNeeded ( labels );
  PyApp::unlock ();
}

std::string 
QtRootNTuple::
createBinding ( const std::string & name, 
		const std::vector < int > & indices ) const
{
  return RootNTuple::createBinding ( name, indices );
}

using namespace boost::python;

numeric::array
QtRootNTuple::
valueAt ( unsigned int row, const std::string & variable )
{
#ifdef HAVE_NUMARRAY
  RootNTuple::throwIfInvalidLabel ( variable );
  int column = RootNTuple::indexOf ( variable );
  vector < int > shape;
  fillShape ( shape, column );
  sliceRowDimension ( shape );
  RootData::Type type = RootNTuple::getType ( column );
  switch ( type ) {
  case RootData::Double:
    {
      double * array = RootNTuple::doubleArrayAt ( row, column );
      numeric::array na = num_util::makeNum ( array, shape );
      return na;
    }
    break;
  case RootData::Float:
    {
      float * array = RootNTuple::floatArrayAt ( row, column );
      numeric::array na = num_util::makeNum ( array, shape );
      return na;
    }
    break;
  case RootData::Int:
    {
      int * array = RootNTuple::intArrayAt ( row, column );
      numeric::array na = num_util::makeNum ( array, shape );
      return na;
    }
    break;
  default:
    assert ( false );
    break;
  }
  return num_util::makeNum ( 1, PyArray_DOUBLE );
#else
  throw std::runtime_error ( "HippoDraw was not built with "
			     "numeric Python support" );
#endif
}

numeric::array
QtRootNTuple::
getColumnAsArray ( const std::string & name )
{
#ifdef HAVE_NUMARRAY
  throwIfInvalidLabel ( name );
  unsigned int length = rows ();
  unsigned int column = indexOf ( name );

  if ( isMultiDimensional ( name ) == false ) {
    vector < int > shape;
    shape.push_back ( length );
    const vector < double > & col_vec = getColumn ( column );
    double * array = const_cast < double * > ( &col_vec[0] );
    numeric::array na = num_util::makeNum ( array, shape );

    return na;
  }

  int size = 1;
  vector < int > shape;
  fillShape ( shape, column );
  unsigned int rank = shape.size();
  for ( unsigned int i = 1; i < rank; i++ ) { // skip first dimension
    size *= shape[i];
  }
  int total_size = size * shape [ 0 ];

  RootData::Type type = getType ( column );

  switch ( type ) {
  case RootData::Double:
    {
      vector < double > array_vec;
      array_vec.reserve ( total_size );
      for ( unsigned int row = 0; row < length; row++ ) {
	double * array =RootNTuple::doubleArrayAt ( row, column );
	array_vec.insert ( array_vec.end(), array, array + size );
      }
      numeric::array na = num_util::makeNum ( &array_vec[0], shape );

      return na;
    }
    break;

  case RootData::Float:
    {
      vector < float > array_vec;
      array_vec.reserve ( total_size );
      for ( unsigned int row = 0; row < length; row++ ) {
	float * array = RootNTuple::floatArrayAt ( row, column );
	array_vec.insert ( array_vec.end(), array, array + size );
      }
      numeric::array na = num_util::makeNum ( &array_vec[0], shape );

      return na;
    }
    break;

  case RootData::Int:
    {
      vector < int > array_vec;
      array_vec.reserve ( total_size );
      for ( unsigned int row = 0; row < length; row++ ) {
	int * array = RootNTuple::intArrayAt ( row, column );
	array_vec.insert ( array_vec.end(), array, array + size );
      }
      numeric::array na = num_util::makeNum ( &array_vec[0], shape );

      return na;
    }
    break;

  case RootData::UInt:
    {
      vector < unsigned int > array_vec;
      array_vec.reserve ( total_size );
      for ( unsigned int row = 0; row < length; row++ ) {
	unsigned int * array = RootNTuple::uintArrayAt ( row, column );
	array_vec.insert ( array_vec.end(), array, array + size );
      }
      numeric::array na = num_util::makeNum ( &array_vec[0], shape );

      return na;
    }
    break;

    default:
    assert ( false );
    break;
  }
  vector < double > array_vec;
  numeric::array na = num_util::makeNum ( & array_vec[0], shape );

  return na; // have to return something for VC++
#else
  throw std::runtime_error ( "HippoDraw was not built with "
			     "numeric Python support" );
#endif
}

