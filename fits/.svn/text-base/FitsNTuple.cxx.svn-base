/** @file

hippodraw::FitsNTuple class implementation

Copyright (C) 2004-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FitsNTuple.cxx,v 1.54 2007/05/09 19:28:48 pfkeb Exp $

*/

#include "FitsNTuple.h"

#include "FitsFile.h"

#include "axes/Range.h"
#include "datasrcs/DataColumn.h"
#include "pattern/string_convert.h"

#include <algorithm>
#include <stdexcept>

#include <cassert>

using std::for_each;
using std::mem_fun;
using std::runtime_error;
using std::string;
using std::vector;

using namespace hippodraw;

FitsNTuple::
FitsNTuple ( FitsFile * file )
  : DataSource ( false ),
    m_file ( file )
{
  m_hdu_num = m_file -> getHDUNumber ();
  initColumns ();
}

FitsNTuple::
FitsNTuple ( )
  : DataSource ( false ),
    m_file ( 0 )
{
}

FitsNTuple::~FitsNTuple()
{
}

void
FitsNTuple::
copy ( const DataSource  & )
{
  assert ( false );
}

const FitsFile *
FitsNTuple::
getFile () const
{
  return m_file;
}

void 
FitsNTuple::
initColumns ( )
{
  m_dups.clear();

  vector < string > labels;
  vector < double > values;
  m_file -> fillColumnNames ( labels );
  unsigned int size = labels.size ();
  for ( unsigned int i = 0; i < size; i++ ) {
    const string & label = labels [i];
    int index = indexOf ( label );
    if ( index < 0 ) {
      addColumn ( label, values );
    }
    else {
      m_dups.push_back ( label );
    }
  }
}

void
FitsNTuple::
notifyObservers ( ) const
{
  Observable::notifyObservers ();
}

unsigned int
FitsNTuple::
rows() const
{
  return m_file -> getNumberOfRows ();
}
std::size_t
FitsNTuple::
size ( unsigned int column ) const
{
  const DataColumn * dc = m_columns [ column ];

  return dc -> size ();
}

int
FitsNTuple::
fillDataCache ( unsigned int column )
{
  DataColumn * data_column = m_columns [ column ];

  vector < intptr_t > shape;
  m_file -> fillShape ( shape, column );
  data_column -> setShape ( shape );

  std::size_t size = data_column -> size ();
  vector < double > & data = data_column -> getData ();
  data.resize ( size );
  int status = m_file -> fillDoubleVectorFromColumn ( data, column );

  return status;
}

void
FitsNTuple::
clear ()
{
  for_each ( m_columns.begin(), m_columns.end (),
	     std::mem_fun ( &DataColumn::clear ) );
}

void
FitsNTuple::
reserve ( unsigned int )
{
  assert ( false );
}

bool
FitsNTuple::
empty () const
{
  return rows () == 0;
}

double
FitsNTuple::
operator [] (  std::vector < unsigned int > & ) const
{
  assert ( false );
  return 0.0;
}

double
FitsNTuple::
valueAt ( unsigned int row, unsigned int column ) const
{
  const vector < double > & data = m_columns[ column ] -> getData();
  if ( data.empty () ) {
    FitsNTuple * tuple = const_cast < FitsNTuple * > ( this );
    tuple -> fillDataCache ( column );
  }

  return data [ row ];
}

const vector < double > &
FitsNTuple::
getColumn ( unsigned int index ) const
{
  isValidColumn ( index ); // might throw

  DataColumn * dc = m_columns [ index ];
  const vector < double > & data = dc -> getData ();
  if ( data.empty () ) {
    FitsNTuple * tuple = const_cast < FitsNTuple * > ( this );
    tuple -> fillDataCache ( index );
  }

  return data;
}

const std::vector < double > &
FitsNTuple::
getRow ( unsigned int row ) const
{
  unsigned int size = m_columns.size();
  m_array.resize ( size );
  for ( unsigned int column = 0; column < size; column++ ) {
    m_array [ column ] = valueAt ( row, column );
  }

  return m_array;
}

bool
FitsNTuple::
fillRange ( unsigned int column, Range & range ) const
{
  bool isValid = true;;
  const vector < double > & data = m_columns [ column ] -> getData ();

  if ( data.empty () ) {
    FitsNTuple * rtuple = const_cast < FitsNTuple * > ( this );
    int status = rtuple -> fillDataCache ( column );
    if ( status != 0 ) isValid = false;
  }

  range.setRange ( data.begin(), data.end() );

  return isValid;
}

void
FitsNTuple::
checkLabel ( const std::string & label )
{
  if ( isValidLabel ( label ) == true ) { // alrady exists
    string what ( "FitsNTuple: The column label `" );
    what += label;
    what += "' already exists in this DataSource.";
    throw runtime_error ( what );
  }
}

int
FitsNTuple::
addColumn ( const std::string & label,
	    const std::vector < double > & column )
{
  unsigned int size = column.size ();
  const vector < intptr_t > shape ( 1, size );

  return addColumn ( label, column, shape );
}

/** @todo Should check size of column and shape vector are consistent.
 */
int
FitsNTuple::
addColumn ( const std::string & label,
	    const std::vector < double > & column,
	    const std::vector < intptr_t > & shape )
{
  checkLabel ( label );

  // Check if column has right size.
  if ( m_columns.empty () == false &&
       column.empty() == false ) {
    unsigned int old_size = rows ();
    unsigned int new_size = shape[0];

    if ( old_size != 0 && old_size != new_size ) {
      string what ( "FitsNTuple Attempt to add a column whose size"
		    " is not equal to other columns." );
      throw runtime_error ( what );
    }
  }

  DataColumn * dc =  new DataColumn();
  m_columns.push_back ( dc );
  if ( column.empty () == false ) {
    vector < double > & data = m_columns.back() -> getData(); // last one
    data = column; // copy
  }
  addLabel ( label );
  dc -> setShape ( shape );

  return m_columns.size () - 1;
}

void
FitsNTuple::
replaceColumn ( unsigned int index, const std::vector < double > & data )
{
  int size = data.size();
  const vector < intptr_t > shape ( 1, size );

  replaceColumn ( index, data, shape );
}

void
FitsNTuple::
replaceColumn ( unsigned int index,
		const std::vector < double > & data,
		const std::vector < intptr_t > & shape )
{
  unsigned int size = columns ();
  if ( index >= size ) {
    string what ( "FitsNTuple: Attempt to replace column " );
    what += String::convert ( index );
    what += " with only ";
    what += String ::convert ( size );
    what += " columns in data source.";
    throw runtime_error ( what );
  }
  size = rows ();
  unsigned int new_size = data.size ();
  if ( size != 0 && size != new_size ) {
    const string what
      ( "FitsNTuple: Attempt to replace column with one whose "
	"size is not equal to other columns." );
      throw runtime_error ( what );
  }
  DataColumn * dc = m_columns [ index ];
  vector < double > & pvec = dc -> getData ();
  pvec.resize ( new_size );
  std::copy ( data.begin(), data.end (), pvec.begin () );
  dc -> setShape ( shape );

  notifyObservers ();
}

void
FitsNTuple::
replaceColumn ( const std::string & label,
		const std::vector < double > & data,
		const std::vector < intptr_t > & shape )
{
  unsigned int index = indexOf ( label );
  replaceColumn ( index, data, shape );
}

void
FitsNTuple::
setShape ( std::vector < unsigned int > & shape )
{
  assert ( false ); // not tested
  m_shape = shape;
}

const vector < unsigned int > &
FitsNTuple::
getShape () const
{
  assert ( false ); // not tested
  return m_shape;
}


/** @todo Need to make sure that the shape has been set.
 */
void
FitsNTuple::
fillShape ( std::vector < intptr_t > & shape, unsigned int column ) const
{
  DataColumn * dc = m_columns [ column ];
  shape = dc -> getShape ();
}

double *
FitsNTuple::
doubleArrayAt ( unsigned int row, unsigned int column ) const
{
  DataColumn * data = m_columns [ column ];

  return data -> doubleArrayAt ( row );
}
