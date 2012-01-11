/** @file

hippodraw::NTuple class implementation

Copyright (C) 1999-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: NTuple.cxx,v 1.170 2007/08/24 22:19:16 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "NTuple.h"

#include "axes/Range.h"
#include "pattern/Observer.h"
#include "pattern/string_convert.h"

#include <algorithm>
#include <functional>
#include <numeric>
#include <stdexcept>

#include <cassert>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::back_insert_iterator;
using std::distance;
using std::runtime_error;
using std::string;
using std::vector;
using std::find;
using std::min_element;
using std::max_element;
#endif

using namespace hippodraw;

NTuple::NTuple ( const std::string & name )
  : DataSource ( name ),
    m_i_count ( 1 ),
    m_i_current ( 0 ),
    m_i_enabled ( false )
{
}
NTuple::NTuple ( const char * name )
  : DataSource ( name ),
    m_i_count ( 1 ),
    m_i_current ( 0 ),
    m_i_enabled ( false )
{
}

NTuple::NTuple ( const std::vector< std::string >  & labels )
  : DataSource ( labels ),
    m_i_count ( 1 ),
    m_i_current ( 0 ),
    m_i_enabled ( false )
{
  std::size_t size = labels.size ();
  for ( std::size_t i = 0; i < size; i++ ) {
    vector< double > * vp = new vector< double > ();
    m_data.push_back ( vp );
  }
}

NTuple::NTuple ( const NTuple & nt )
  : DataSource ( nt ),
    m_i_count ( nt.m_i_count ),
    m_i_current ( nt.m_i_current ),
    m_i_enabled ( nt.m_i_enabled )
{
  copy( nt );
}

NTuple::
NTuple ( const DataSource * ds )
  : DataSource ( *ds ),
    m_i_count ( 1 ),
    m_i_current ( 0 ),
    m_i_enabled ( false )
{
  copy ( *ds );
}

NTuple::NTuple ( unsigned int n )
  : DataSource ( ),
    m_i_count ( 1 ),
    m_i_current ( 0 ),
    m_i_enabled ( false )
{
  vector < string > labels;
  for ( unsigned int i = 0; i < n; i++ ) {
    vector<double> * v = new vector<double> ();
    m_data.push_back( v );
    labels.push_back ( string ( "nil" ) );
  }

  setLabels ( labels );
}

NTuple::NTuple ()
  : DataSource ( ),
    m_i_count ( 1 ),
    m_i_current ( 0 ),
    m_i_enabled ( false ){
}

NTuple::
NTuple ( bool yes )
  : DataSource ( yes ),
    m_i_count ( 1 ),
    m_i_current ( 0 ),
    m_i_enabled ( false )
{
}

NTuple::~NTuple()
{
  Observable::notifyObservers ( &hippodraw::Observer::willDelete );

  vector< vector<double> *>::iterator it = m_data.begin();
  for ( ; it != m_data.end(); ++it ) {
    delete *it;
  }
}

void NTuple::resizeColumns ( size_t new_size )
{
  size_t old_size = m_data.size ();

  if ( new_size > old_size ) {
    for ( ; old_size < new_size; old_size++ ) {
      vector< double > * v = new vector<double> ();
      m_data.push_back ( v );
    }
  }
  else {
    m_data.erase ( m_data.begin() + new_size, m_data.end() );
  }

}

void
NTuple::
copy ( const DataSource  & rhs )
{
  DataSource::copyPrivate ( rhs );
  m_data.clear ();
  try {
    const NTuple & ntuple = dynamic_cast < const NTuple & > ( rhs );

    vector< vector<double> *>::const_iterator it = ntuple.m_data.begin();
    for ( ; it != ntuple.m_data.end(); ++it ) {
      vector<double> * v = new vector<double> ( **it );
      m_data.push_back( v );
    }
    m_i_count = ntuple.m_i_count;
    m_i_current = ntuple.m_i_current;
    m_i_enabled = ntuple.m_i_enabled;
  }
  catch ( ... ) {
    unsigned int columns = rhs.columns ();

    for ( unsigned int i = 0; i < columns; i++ ) {
      vector < double > * vec = new vector < double >;
      m_data.push_back ( vec );
    }

    unsigned int size = rhs.rows ();
    for ( unsigned int i = 0; i < size; i++ ) {
      const vector < double > & src = rhs.getRow ( i );
      for ( unsigned int j = 0; j < columns; j++ ) {
	vector < double > & d = *m_data[j];
	d.push_back ( src[j] );
      }
    }
  }

}

void
NTuple::
append ( const DataSource * source ) 
{
  checkWidth ( source ); // might throw 

  const NTuple * ntuple = dynamic_cast < const NTuple * > ( source );
  if ( ntuple != 0 ) {
    vector < vector < double > * >::const_iterator src 
      = ntuple -> m_data.begin ();
    vector < vector < double > * >::iterator dst = m_data.begin();
    while ( dst != m_data.end() ) {
      vector < double > & dst_vec = *(*dst++);
      vector < double > & src_vec = *(*src++);
      std::copy ( src_vec.begin(),
		  src_vec.end(),
		  back_insert_iterator < vector <double > > ( dst_vec) );
    }
  }
  else {
    DataSource::append ( source );
  }
}

void NTuple::clear()
{
  vector< vector<double> *>::iterator it = m_data.begin();
  for ( ; it < m_data.end(); ++it ) {
    (*it)->clear();
  }

  notifyObservers ();
}

bool
NTuple::
empty () const
{
  return m_data.empty () ? true : m_data[0] -> empty ();
}

unsigned int
NTuple::
rows () const
{
  unsigned int count = 0;

  if ( m_data.empty() == false &&
       m_data[0] -> empty () == false ) {
    count = m_data[0] -> size ();
  }

  return count;
}

void
NTuple::
replaceRow ( unsigned int i, const std::vector < double > & v )
{
  if ( ! ( i < rows () ) ) {
    const string what ( "NTuple::replaceRow: index invalid" );
    throw runtime_error ( what );
  }

  throwIfInvalidRowSize ( v );

  vector < vector < double > * >:: iterator first = m_data.begin ();
  vector < double > :: const_iterator d = v.begin();
  while ( first != m_data.end () ) {
    vector < double > * column = *first++;
    column->operator[] ( i ) = *d++;
  }

  notifyObservers ();
}

// note: Using index is twice as fast as using iterators
void
NTuple::
addRow ( const std::vector < double > & v )
{
  throwIfInvalidRowSize ( v );

  unsigned int size = m_data.size ();
  for ( unsigned int i = 0; i < size; i++ ) {
    vector < double > & column = *m_data[i];
    column.push_back ( v[i] );
  }
  notifyObservers ();
}

void
NTuple::
insertRow ( unsigned int index, const std::vector < double > & v )
{
  if ( m_data.size() != v.size() ) {
    const string what ( "NTuple: Attempt to insert a row whose size"
			" is not equal to other rows." );
//     throw DataSourceException ( what );
    throw runtime_error ( what );
  }

  if ( index > rows () ) {
    const string what ( "NTuple::insertRow: index out of range" );
//     throw DataSourceException ( what );
    throw runtime_error ( what );
  }

  vector<double>::const_iterator vit = v.begin();
  vector< vector<double> *>::iterator it = m_data.begin();
  for ( ; it != m_data.end(); ++it ) {
    vector < double > * p = *it;
    vector < double > :: iterator where = p->begin() + index;
    p->insert ( where, *vit++ );
  }

  notifyObservers ();
}

void
NTuple::
eraseRow ( unsigned int index )
{
  if ( index >= rows () ) {
    const string what ( "NTuple::insertRow: index out of range" );
    throw runtime_error ( what );
  }

  vector< vector<double> *>::iterator it = m_data.begin();
  for ( ; it != m_data.end(); ++it ) {
    vector < double > * p = *it;
    vector < double > :: iterator where = p->begin() + index;
    p->erase ( where);
  }

  notifyObservers ();
}

// note: clearing and using push_back was twice as fast as
// resizing and using operator[]
const std::vector < double > & NTuple::getRow ( unsigned int row ) const
{
  if ( row >= rows () ) {
    string what ( "NTuple::getRow: argument= ");
    what += String::convert ( row );
    what += " out of range";
    throw runtime_error ( what );
  }
  unsigned int cols = columns ();
  m_row.clear ();

  for ( unsigned int i = 0; i < cols; i++ ) {
    const vector < double > & column = *m_data[i];
    m_row.push_back ( column[row] );
  }

  return m_row;
}

double
NTuple::
operator [] ( std::vector < unsigned int > & indices ) const
{
  unsigned int rank = getRank ();
  assert ( indices.size() == rank );


if ( rank == 1 ) {
    unsigned int size = m_data.size();
    unsigned int row = indices[0] / size;
    unsigned int col = indices[0] % size;
    const vector < double > & column = *m_data[col];
    return column[row];
  }

  if ( rank == 2 ) {
    unsigned int col = indices[1];
    unsigned int row = indices[0];
    const vector < double > & column = *m_data[col];
    return column[row];
  }

  if ( rank == 3 ) {
    unsigned int size = m_data.size();
    unsigned int col = indices[2];
    unsigned int j = indices[1];
    unsigned int i = indices[0];

    assert ( col < size );
    assert ( j < m_shape[1] );
    assert ( i < m_shape[0] );

    unsigned int row = j + i * m_shape[1];
    const vector < double > & column = *m_data[col];
    return column[row];
  }
  return 0.0;
}

double
NTuple::
valueAt ( unsigned int row, unsigned int column ) const
{
  //return (*m_data[column])[row];
  vector < double > * v = m_data [ column ];
  return v -> operator [] ( row );
}

void NTuple::reserve ( unsigned int count )
{
  vector < vector < double > * >::iterator it = m_data.begin();
  for ( ; it != m_data.end(); ++it ) {
    (*it)->reserve ( count );
  }
}

int
NTuple::
addColumn ( const std::string & label,
	    const std::vector< double > & col )
{
  // Check if label already exists.
  int index = indexOf ( label );
  if ( index >= 0 ) {
    string what ( "NTuple: Attempt to add a column with label `");
    what += label;
    what += "' which duplicates existing label.";
    throw runtime_error ( what );
  }

  // Check if column has right size.
  if ( m_data.empty () == false ) {
    unsigned int size = rows ();
    unsigned int rows = col.size();
    if ( size != 0 && size != rows ) {
      string what ( "NTuple: Attempt to add a column of `" );
      what += String::convert ( rows );
      what += "' rows to DataSource with `";
      what += String ::convert ( size );
      what += "' rows.";
      throw runtime_error ( what );
    }
  }

  vector < double > * vec = new vector < double > ( col );
  m_data.push_back ( vec );
  addLabel ( label );

  return m_data.size() - 1;
}

void
NTuple::
replaceColumn ( unsigned int col, const std::vector< double > & data )
{
  unsigned int size = columns ();
  if ( col >= size ) {
    string what ( "NTuple: Attempt to replace column `");
    what += String::convert ( col );
    what += "' of data source with only `";
    what += String::convert ( size );
    what += "' columns.";
    throw runtime_error ( what );
  }

  size = rows();
  unsigned int new_size = data.size ();
  if ( size != 0 && size != new_size ) {
    string what ( "NTuple:: Attempt to replace column with size `" );
    what += String::convert ( size );
    what += "' with one of size `";
    what += String::convert ( new_size );
    what += "'.";
    throw runtime_error ( what );
  }
  m_data[col]->resize ( data.size() );
  // need std:: below, else conflicts with NTuple::copy()
  std:: copy ( data.begin (), data.end(), m_data[col]->begin() );

  notifyObservers ();
}

void
NTuple::
setLabels ( const std::vector< std::string > & v )
{
  if ( rows () == 0 ) {
    resizeColumns ( v.size () );
  }
  else {
    unsigned int vsize = v.size ();
    if ( vsize  != columns () ) {
      string what ( "NTuple: Attempt to set " );
      what += String::convert ( vsize );
      what += " labels with data source of ";
      what += String::convert ( columns() );
      what += " columns.";
      throw runtime_error ( what );
    }
  }

  DataSource::setLabels ( v );
}

const vector<double> &
NTuple::
getColumn ( unsigned int i ) const
{
  isValidColumn ( i ); // will throw exception if bad

  return *m_data[i];
}

vector<double> & NTuple::getColumn ( unsigned int i )
{
  unsigned int size = columns();
  if ( i >= size ) {
    const string what ( "NTuple::getColumn argument out of range" );
    throw runtime_error ( what );
  }

  return *m_data[i];
}

const vector< double > & NTuple::getColumn ( const std::string & name ) const
{
  throwIfInvalidLabel ( name );
  int index = indexOf ( name );

  return *m_data[index];
}

unsigned int
NTuple::
indexOfMinElement( unsigned int index ) const
{
  const vector< double > & c = getColumn( index );

  vector < double > :: const_iterator first
    = min_element ( c.begin(), c.end() );

  return distance ( c.begin(), first );
}

double
NTuple::
minElement ( unsigned int column ) const
{
  vector < double > & v = *m_data[column];

  return *min_element ( v.begin(), v.end() );
}

unsigned int
NTuple::
indexOfMaxElement ( unsigned int index ) const
{
  const vector< double > & c = getColumn( index );

  vector < double > :: const_iterator first
    = max_element ( c.begin(), c.end() );

  return distance ( c.begin(), first );
}

double
NTuple::
maxElement ( unsigned int column ) const
{
  vector < double > & v = *m_data[column];

  return *max_element ( v.begin(), v.end() );
}

double NTuple::columnMin( const std::string & name ) const
{
  const vector< double > & c = getColumn( name );
  return *min_element( c.begin(), c.end() );
}

double NTuple::columnMax( const std::string & name ) const
{
  const vector< double > & c = getColumn( name );
  return *max_element( c.begin(), c.end() );
}

/* virtual */
void NTuple::notifyObservers ( ) const
{
  if ( m_i_enabled == false ) {
    Observable::notifyObservers ( );
    return;
  }

  m_i_current++;

  if ( m_i_current == m_i_count ) {
    Observable::notifyObservers ( );
    m_i_current = 0;
  }
}

void NTuple::setIntervalEnabled ( bool yes )
{
  m_i_enabled = yes;
  m_i_current = 0;

  if ( yes == false ) notifyObservers ();
}

bool NTuple::isIntervalEnabled () const
{
  return m_i_enabled;
}

void NTuple::setIntervalCount ( int number )
{
  m_i_count = number;
  m_i_current = 0;
}

unsigned int NTuple::getIntervalCount ( ) const
{
  return m_i_count;
}

bool
NTuple::
fillRange ( unsigned int column, Range & range ) const
{
  vector < double > & v = *m_data[column];

  range.setRange ( v.begin(), v.end() );

  return true;
}

double
NTuple::
sum ( unsigned int column ) const
{
  double sum = 0.0;
  const vector < double > & data = *m_data[ column ];

  return accumulate ( data.begin(), data.end(), sum );
}
