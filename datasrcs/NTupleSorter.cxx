/** @file

NTupleSorter class implementation

Copyright (C) 2003   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: NTupleSorter.cxx,v 1.13 2005/10/30 00:10:11 pfkeb Exp $

*/

#include "NTupleSorter.h"

#include "NTuple.h"

#include <algorithm>
#include <functional>
#include <iterator>

using std::distance;
using std::greater;
using std::sort;
using std::vector;
using std::lower_bound;

namespace hippodraw {

NTupleSorter::
NTupleSorter ( NTuple * ntuple )
  : m_column ( 0 ),
    m_increasing ( true ),
    m_ntuple ( ntuple )
{
}

NTupleSorter::
NTupleSorter ( const NTupleSorter & sorter )
  : m_column ( sorter.m_column ),
    m_increasing ( sorter.m_increasing ),
    m_ntuple ( 0 )
{
}

NTuple *
NTupleSorter::
getNTuple () const
{
  return m_ntuple;
}

void
NTupleSorter::
setSorting ( int column )
{
  if ( column == m_column ) {
    m_increasing = ! m_increasing;
    return;
  }
  m_column = column;
  m_increasing = true;
}

void
NTupleSorter::
rowwiseCopy ( std::vector < std::vector < double > * > & row_copy )
{
  row_copy.clear();
  unsigned int rows = m_ntuple -> rows ();

  for ( unsigned int row = 0; row < rows; row++ ) {
    const vector < double > & v = m_ntuple -> getRow ( row );
    vector < double > * p = new vector < double > ( v );
    row_copy.push_back ( p );
  }
}

bool 
NTupleSorter::
operator () ( std::vector < double > * x,
	      std::vector < double > * y ) const
{
  double left = x -> operator[] ( m_column );
  double right = y -> operator[] ( m_column );

  return m_increasing ? left < right : right < left;
}

void
NTupleSorter::
sort ( )
{
  vector < vector < double > * > table;
  rowwiseCopy ( table );

  std::sort ( table.begin(), table.end(), *this );

  m_ntuple -> clear ();

  vector < vector < double > * > :: iterator first = table.begin ();
  while ( first != table.end () ) {
    vector < double > * p = *first++;
    m_ntuple -> addRow ( *p );
    delete p;
  }
  m_ntuple ->notifyObservers();
}

void
NTupleSorter::
clear ()
{
  m_ntuple->clear();
}

void
NTupleSorter::
addRow ( const std::vector < double > & row )
{
  double sort_value = row[m_column];

  const vector < double > & column = m_ntuple ->getColumn ( m_column );
  vector < double > ::const_iterator first;

  if ( m_increasing ) {
    first = lower_bound ( column.begin(), column.end(), sort_value );
  }
  else {
    first = lower_bound ( column.begin(), column.end(),
			  sort_value, greater< double >() );
  }
  unsigned int index = distance ( column.begin(), first );

  m_ntuple->insertRow ( index, row );
}

void
NTupleSorter::
eraseRow ( unsigned int index )
{
  m_ntuple -> eraseRow ( index );
}

unsigned int
NTupleSorter::
columns ( ) const
{
  return m_ntuple->columns ();
}

unsigned int
NTupleSorter::
rows ( ) const
{
  return m_ntuple->rows ();
}

const vector < double > &
NTupleSorter::
getRow ( unsigned int index ) const
{
  return m_ntuple->getRow ( index );
}

} // namespace hippodraw

