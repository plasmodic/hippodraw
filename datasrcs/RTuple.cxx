/** @file

hippodraw::RTuple class implementation

Copyright (C) 1999-2004, 2006, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: RTuple.cxx,v 1.20 2007/04/23 21:57:20 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "RTuple.h"

#include "axes/Range.h"
#include "pattern/Observer.h"

#include <algorithm>
#include <functional>
#include <numeric>
#include <stdexcept>

#include <cassert>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::distance;
using std::runtime_error;
using std::string;
using std::vector;
#endif

using namespace hippodraw;

RTuple::RTuple ( const std::vector< std::string >  & labels )
  : DataSource ( labels )
{
  std::size_t size = labels.size ();
  for ( std::size_t i = 0; i < size; i++ ) {
    vector< double > * vp = new vector< double > ();
    m_data.push_back ( vp );
  }
}

RTuple::RTuple ( const RTuple & nt )
  : DataSource ( nt )
{
  // do nothing, is private
}

RTuple::RTuple ( unsigned int n )
  : DataSource ( )
{
  vector < string > labels;
  for ( unsigned int i = 0; i < n; i++ ) {
    labels.push_back ( string ( "nil" ) );
  }

  setLabels ( labels );
}

RTuple::~RTuple()
{
  Observable::notifyObservers ( &hippodraw::Observer::willDelete );

  vector< vector<double> *>::iterator it = m_data.begin();
  for ( ; it != m_data.end(); ++it ) {
    delete *it;
  }
}
void
RTuple::
copy ( const DataSource & other )
{
  DataSource::copyPrivate ( other );
  clear ();

  try {
    const RTuple & ntuple = dynamic_cast < const RTuple & > ( other );
    vector < vector < double > * > ::const_iterator first 
      = ntuple.m_data.begin ();
    while ( first != ntuple.m_data.end () ) {
      vector < double > * v = new vector < double > ( **first++ );
      m_data.push_back ( v );
    }
  }
  catch ( ... ) {
    unsigned int size = other.rows ();
    for ( unsigned int i = 0; i < size; i++ ) {
      const vector < double > & src = other.getRow ( i );
      vector < double > * dst = new vector < double > ( src );
      m_data.push_back ( dst );
    }
  }
}

void RTuple::clear()
{
  vector < vector < double > * >::iterator it = m_data.begin();
  while ( it != m_data.end() ) {
    delete *it++;
  }

  m_data.clear ();
}

bool
RTuple::
empty () const
{
  return m_data.empty ();
}

unsigned int
RTuple::
rows () const
{
  return m_data.size ();
}

void
RTuple::
addRow ( const std::vector < double > & v )
{
  throwIfInvalidRowSize ( v );

  vector < double > * row = new vector < double > ( v );
  m_data.push_back ( row );

//   notifyObservers ();
}

const std::vector < double > & RTuple::getRow ( unsigned int row ) const
{
  if ( row >= m_data.size() ) {
    string what ( "RTuple::getRow: argument out of range" );
    throw runtime_error ( what );
  }

  return *m_data[row];
}

double
RTuple::
operator [] ( std::vector < unsigned int > & indices ) const
{
  unsigned int rank = getRank ();
  assert ( indices.size() == rank );

  
if ( rank == 1 ) {
    unsigned int size = columns ();
    unsigned int row = indices[0] / size;
    unsigned int col = indices[0] % size;
    const vector < double > & rowvec = *m_data[row];
    return rowvec[col];
  }

  if ( rank == 2 ) {
    unsigned int col = indices[1];
    unsigned int row = indices[0];
    const vector < double > & rowvec = *m_data[row];
    return rowvec[col];
  }

  if ( rank == 3 ) {
    unsigned int size = columns ();
    unsigned int col = indices[2];
    unsigned int j = indices[1];
    unsigned int i = indices[0];

    assert ( col < size );
    assert ( j < m_shape[1] );
    assert ( i < m_shape[0] );

    unsigned int row = j + i * m_shape[1];
    const vector < double > & rowvec = *m_data[row];
    return rowvec[col];
  }
  return 0.0;
}

double
RTuple::
valueAt ( unsigned int row, unsigned int column ) const
{
  return (*m_data[row])[column];
}

void
RTuple::
reserve ( unsigned int count )
{
  m_data.reserve ( count );
}
