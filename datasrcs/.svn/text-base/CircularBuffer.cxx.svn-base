/** @file

hippodraw;:CircularBuffer class implementation

Copyright (C) 2003, 2004, 2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: CircularBuffer.cxx,v 1.10 2006/09/28 23:24:27 pfkeb Exp $

*/

#include "CircularBuffer.h"

#include <stdexcept>

using std::runtime_error;
using std::string;

using namespace hippodraw;

CircularBuffer::CircularBuffer ( const std::string & filename )
  : NTuple ( filename ),
    m_capacity ( 0 ),
    m_next_row ( 0 ),
    m_has_filled ( false )
{
}

CircularBuffer::CircularBuffer ( const std::vector< std::string >  & v )
  : NTuple ( v ),
    m_capacity ( 0 ),
    m_next_row ( 0 ),
    m_has_filled ( false )
{
}

CircularBuffer::CircularBuffer ( const CircularBuffer & nt )
  : NTuple ( nt ),
    m_capacity ( nt.m_capacity ),
    m_next_row ( nt.m_next_row ),
    m_has_filled ( nt.m_has_filled )
{
}

CircularBuffer::CircularBuffer ( unsigned int n )
  : NTuple ( n ),
    m_capacity ( 0 ),
    m_next_row ( 0 ),
    m_has_filled ( false )
{
}

CircularBuffer::CircularBuffer ()
  : NTuple (),
    m_capacity ( 0 ),
    m_next_row ( 0 ),
    m_has_filled ( false )
{
}

void CircularBuffer::clear()
{
  m_next_row = 0;
  m_has_filled = false;

  NTuple::clear ();
}

void
CircularBuffer::
incrementRowIndex ()
{
  m_next_row++;
  if ( m_next_row == m_capacity ) {
    m_next_row = 0;
    m_has_filled = true;
  }
}

void
CircularBuffer::
addRow ( const std::vector< double > & v )
{
  if ( m_has_filled ) {
    NTuple::replaceRow ( m_next_row, v );
  }
  else {
    NTuple::addRow ( v );
  }

  incrementRowIndex ( );
}

void CircularBuffer::reserve ( unsigned int count )
{
  if ( empty () == false ) {
    const string what ( "CircularBuffer: Attempt to set the capacity of "
			"non-empty buffer is not allowed" );
    throw runtime_error ( what );
  }

  NTuple::reserve ( count );
  m_capacity = count;
}
