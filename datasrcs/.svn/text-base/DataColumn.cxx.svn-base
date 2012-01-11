/** @file

hippodraw::DataColumn class implementation

Copyright (C) 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DataColumn.cxx,v 1.6 2007/05/09 19:28:48 pfkeb Exp $

*/

#include "DataColumn.h"

using std::vector;

using namespace hippodraw;

DataColumn::
DataColumn ()
{
}

DataColumn::
~DataColumn()
{
}

std::vector < double > &
DataColumn::
getData ()
{
  return m_double_data;
}

const std::vector < double > &
DataColumn::
getData () const
{
  return m_double_data;
}

unsigned int 
DataColumn::
getRank () const
{
  unsigned int size = m_shape.size();

  return size;
}

const std::vector < intptr_t > &
DataColumn::
getShape ()
{
  return m_shape;
}

void
DataColumn::
setShape ( const std::vector < intptr_t > & shape )
{
  m_shape = shape;
}

std::size_t
DataColumn::
size ( ) const
{
  unsigned int rank = m_shape.size ();
  std::size_t product = 1;
  for ( unsigned int i = 0; i < rank; i++ ) {
    product *= m_shape[i];
  }

  return product;
}

void
DataColumn::
clear ()
{
  m_double_data.clear ();
}

double *
DataColumn::
doubleArrayAt ( unsigned int row )
{
  unsigned int rank = m_shape.size ();
  unsigned int size = 1;
  for ( unsigned int i = 1; i < rank; i++ ) {
    size *= m_shape [ i ];
  }
  unsigned int pos = row * size;

  return & m_double_data [ pos ];
}
