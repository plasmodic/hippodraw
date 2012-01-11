/** @file

hippodraw::DataSource class implementation

Copyright (C) 2004-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DataSource.cxx,v 1.63 2007/07/02 18:17:15 pfkeb Exp $

*/

// for distance defect
#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#define isnan _isnan
#endif
#endif

//To have isnan.
#ifdef __APPLE__
#include <cstdlib>
#define _GLIBCPP_USE_C99 1
#endif

#include "DataSource.h"

#include "axes/Range.h"
#include "pattern/Observer.h"
#include "pattern/string_convert.h"

#include <algorithm>
#include <functional>
#include <stdexcept>

#include <cmath>

#include <cassert>

#ifdef __APPLE__
using std::isnan;
#endif

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::distance;
using std::runtime_error;
using std::string;
using std::vector;
#endif

using namespace hippodraw;

DataSource::
DataSource ( const std::string & name )
  : m_ds_name ( name ),
    m_title(),
    m_is_null ( false )
{
}

DataSource::
DataSource ( const char * name )
  : m_ds_name ( name ),
    m_title(),
    m_is_null ( false )
{
}

DataSource::
DataSource ( const DataSource & ds )
  : Observable (),
    m_ds_name ( ds.m_ds_name ),
    m_title( ds.m_title ),
    m_labels ( ds.m_labels ),
    m_is_null ( false ),
    m_shape ( ds.m_shape )
{
}

DataSource::
DataSource ( const std::vector < std::string > & labels )
  : m_labels ( labels ),
    m_is_null ( false )
{
}

DataSource::
DataSource ()
  : m_ds_name ( ),
    m_title(), 
    m_is_null ( false )
{
}

DataSource::
DataSource ( bool yes )
  : m_ds_name ( "null" ),
    m_title ( "Not valid DataSource" ),
    m_is_null ( yes )
{
}

DataSource::~DataSource()
{
  Observable::notifyObservers ( &hippodraw::Observer::willDelete );
}

void
DataSource::
copyPrivate ( const DataSource & other )
{
  m_ds_name = other.m_ds_name;
  m_is_null = other.m_is_null;
  m_labels = other.m_labels;
  m_shape = other.m_shape;
  m_title = other.m_title;
}

bool
DataSource::
isNull () const
{
  return m_is_null;
}

void DataSource::setName ( const std::string & name )
{
  m_ds_name = name;
  notifyObservers ();
}

const string & DataSource::getName () const
{
  return m_ds_name;
}

const string & DataSource::title () const
{
  return m_title;
}

void DataSource::setTitle ( const std::string & title )
{
  m_title = title;
  notifyObservers ();
}

void
DataSource::
addLabel ( const std::string & label )
{
  m_labels.push_back ( label );
}

bool
DataSource::
setLabelAt( const std::string & s, unsigned int i )
{
  if ( i >= m_labels.size() ) return false;
  m_labels[i] = s;
  notifyObservers ();

  return true;
}

const vector <string> &
DataSource::
getLabels () const
{
   return m_labels;
}

const string &
DataSource::
getLabelAt ( unsigned int i ) const
{
  if ( isNull() ) return title ();

  if ( m_labels.size () < i ) {
    string what ( "DataSource: argument out of range" );
    throw runtime_error ( what );
  }

  return m_labels[i];
}

int
DataSource::
indexOf ( const std::string & label ) const
{
  int index = -1;
  vector< string >::const_iterator first 
    = find ( m_labels.begin(), m_labels.end(), label );

  if ( first != m_labels.end() ) {
#ifdef DISTANCE_DEFECT
    index = first - m_labels.begin();
#else
    index = distance ( m_labels.begin(), first );
#endif
  }

  return index;
}

bool
DataSource::
isValidColumn ( unsigned int index ) const
{
  unsigned int size = columns();
  if ( index >= size ) {
    string what ( "DataSource: Index `" );
    what += String::convert ( index );
    what += "' out of range 0 to ";
    what += String::convert ( size - 1 );
    throw runtime_error ( what );
  }

  return true;
}

const vector < double > &
DataSource::
getColumn ( unsigned int index ) const
{
  isValidColumn ( index ); // will throw exception if bad
  unsigned int size = rows ();

  m_array.resize ( size, 0.0 );
  for ( unsigned int i = 0; i < size; i++ ) {
    m_array [ i ] = valueAt ( i, index );
  }

  return m_array;
}

const vector < double > &
DataSource::
getColumn ( const std::string & label ) const
{
  int index = indexOf ( label );
  if ( index < 0 ) {
    const string what ( "DataSource: Attempt to get column whose label"
			" doesn't exist" );
    throw runtime_error ( what );
  }

  return getColumn ( index );
}

bool 
DataSource::
isValidLabel ( const std::string & label ) const
{
  vector< string >::const_iterator first 
    = find ( m_labels.begin(), m_labels.end(), label );
  bool yes = first != m_labels.end();

  return yes;
}

void
DataSource::
throwIfInvalidLabel ( const std::string & label ) const
{
  bool yes = isValidLabel ( label );
  if ( yes == false ) {
    string what ( "DataSource: `" );
    what += label;
    what += "' not found in this data source";
    throw runtime_error ( what );
  }
}

void
DataSource::
throwIfInvalidRowSize ( const std:: vector < double > & row )
{
  unsigned int size = columns ();
  unsigned int cols = row.size (); // for Mac OS
  if ( size != cols ) {
    string what ( "DataSource: Attempt to add row of size `" );
    what += String::convert ( cols );
    what += "' to DataSource with `";
    what += String::convert ( size );
    what += "' columns";
    throw runtime_error ( what );
  }
}

void
DataSource::
setLabels ( const std::vector < std::string > & v )
{
  m_labels = v;

  notifyObservers ();
}

void
DataSource::
setShape ( std::vector < unsigned int > & shape )
{
  m_shape = shape;
}

const vector < unsigned int > &
DataSource::
getShape () const
{
  return m_shape;
}

void
DataSource::
fillShape ( std::vector < int > & shape, unsigned int column ) const
{
  shape.clear ();
  shape.push_back ( rows () );
}

void
DataSource::
fillShape ( std::vector < int > & v,
	    const std::string & label ) const
{
  int index = indexOf ( label );
  if ( index < 0 ) {
    string what ( "DataSource: No column with label `" );
    what += label;
    what += "' exists";
    throw runtime_error ( what );
  }
  fillShape ( v, index );
}


unsigned int
DataSource::getRank () const
{
  return m_shape.size();
}

void
DataSource::
replaceColumn ( unsigned int, 
		const std::vector < double > & array )
{
  string what ( "DataSource: The type of data source does not support "
		"replacing a column." );
  throw runtime_error ( what );
}

void
DataSource::
replaceColumn ( const std::string & label,
		const std::vector < double > & array )
{
  int index = indexOf ( label );

  if ( index < 0 ) { // column doesn't exist
    string what ( "DataSource: Attempt to replace column `" );
    what += label;
    what += "' which does not exist.";
    throw runtime_error ( what );
  }
  else {
    replaceColumn ( index, array );
  }
}

void
DataSource::
replaceColumn ( const std::string &,
		const std::vector < double > &,
		const std::vector < intptr_t > & )
{
  string what ( "DataSource: This type of data source does not support"
		" the notion of shape." );
  throw runtime_error ( what );
}


int
DataSource::
addColumn ( const std::string &,
	    const std::vector < double > & )
{
  string what ( "DataSource: This type of data source does not support "
		"adding a column." );
  throw runtime_error ( what );
}

int
DataSource::
addColumn ( const std::string &,
	    const std::vector < double > &,
	    const std::vector < intptr_t > & )
{
  string what ( "DataSource: This type of data source does not support"
		" the notion of shape." );
  throw runtime_error ( what );
}


/** @bug HUGE_VAL doesn't pass gcc on the Mac if -pedantic is used.
    @request use if ( x != x) to test for nan
 */
bool
DataSource::
fillRange ( unsigned int column, Range & range ) const
{
  assert ( column < columns () );
  bool isValid = true;

  unsigned int size = rows ();
  if ( size > 0 ) {
    bool valid = false;
    double min = DBL_MAX;
    double max = DBL_MIN;
    double pos = DBL_MAX;
    for ( unsigned int i = 0; i < size; i++ ) {
      double x = valueAt ( i, column );

      if (  x != HUGE_VAL && 
	    x != -HUGE_VAL &&
	    isnan ( x ) == false ) {
	min = std::min ( min, x );
	max = std::max ( max, x );
	if ( x > 0.0 ) pos = std::min ( pos, x );
	valid = true;
      }
      else {
	isValid = false;
      }
    }
    if ( valid == true ) {
      range.setRange ( min, max, pos );
    }
  }

  return isValid;
}

bool
DataSource::
isMultiDimensional( const std::string & ) const
{
  return false;
}

bool
DataSource::
isUseable( const std::string & ) const
{
  return true;
}

bool
DataSource::
setReleventIndex( const std::string &,
		  const std::vector< unsigned int > & )
{
  // Do not call this fucntion unless implemented in base class
  assert( 0 );
  
  return false;
}

unsigned int
DataSource::
indexOfMaxElement ( unsigned int column ) const
{
  assert ( column < columns () );

  unsigned int size = rows ();
  unsigned int index = 0;
  double m = valueAt ( 0, column );

  for ( unsigned int i = 1; i < size; i++ ) {
    double v = valueAt ( i, column );
    if ( v > m ) {
      index = i;
      m = v;
    }
  }

  return index;
}

unsigned int
DataSource::
indexOfMinElement ( unsigned int column ) const
{
  assert ( column < columns () );

  unsigned int size = rows ();
  unsigned int index = 0;
  double m = valueAt ( 0, column );

  for ( unsigned int i = 1; i < size; i++ ) {
    double v = valueAt ( i, column );
    if ( v < m ) {
      index = i;
      m = v;
    }
  }

  return index;
}

double
DataSource::
sum ( unsigned int column ) const
{
  assert ( column < columns () );

  unsigned int size = rows ();
  double sum = 0.0;

  for ( unsigned int i = 0; i < size; i++ ) {
    sum += valueAt ( i, column );
  }

  return sum;
}

const vector < string > &
DataSource::
getDuplicatedLabels () const
{
  return m_dups;
}

double
DataSource::
valueAtNoCache ( unsigned int row, unsigned int column ) const
{
  return valueAt ( row, column );
}

void
DataSource::
addRow ( const std::vector < double > & row )
{
  string what ( "This data source is not capable of adding rows" );
  throw runtime_error ( what );
}

void
DataSource::
eraseRow ( unsigned int index )
{
  string what ("This data source is not capable of erasing rows" );
  throw runtime_error ( what );
}


void
DataSource::
checkWidth ( const DataSource * source )
{
  unsigned int ncolumns = source -> columns ();
  if ( ncolumns != columns () ) {
    string what ( "DataSource: Number of columns of source (" );
    what += String::convert ( ncolumns );
    what += ") not equal to current (";
    what += String::convert ( columns() );
    what += ").";
    throw runtime_error ( what );
  }
}

void
DataSource::
append ( const DataSource * source )
{
  checkWidth ( source ); // throws runtime_error

  unsigned int size = source -> rows ();
  for ( unsigned int i = 0; i < size; i++ ) {
    const vector < double > & src_row = source -> getRow ( i );
    addRow ( src_row );
  }
}

double *
DataSource::
doubleArrayAt ( unsigned int row, unsigned int column ) const
{
  string what ( "DataSource: This data source is not capable of containing\n"
		"an array in a column." );
  throw runtime_error ( what );
}

void
DataSource::
expandIfNeeded ( const std::vector < std::string > & labels ) const
{

}
