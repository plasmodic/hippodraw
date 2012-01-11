/** @file

hippodraw::TupleCut class implementation

Copyright (C) 2000, 2001, 2004, 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: TupleCut.cxx,v 1.56 2005/12/01 19:23:40 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "TupleCut.h"

#include "datasrcs/DataSource.h"

#include <algorithm>

using namespace hippodraw;

TupleCut::
TupleCut ()
  : m_column ( 0 ),
    m_invert ( false ),
    m_enabled ( true )
{
}

void
TupleCut::
setEnabled ( bool yes )
{
  m_enabled = yes;
}

bool
TupleCut::
isEnabled () const
{
  return m_enabled;
}

bool TupleCut::acceptRow ( const DataSource * ntuple, 
			   unsigned int i ) const
{
  bool shouldBeAccepted = true;

  if ( m_enabled ) {
    double value = ntuple -> valueAt ( i, m_column );
    bool included = m_range.includes ( value );
    shouldBeAccepted  = 
      ( included && ! m_invert ) ||
      ( !included && m_invert );
  }

  return shouldBeAccepted;
}

void TupleCut::setColumn( unsigned int column  )
{
  m_column = column;
}

unsigned int
TupleCut::
getColumn () const
{
  return m_column;
}

void
TupleCut::
setRange ( const Range & range )
{
  m_range = range;
}

const Range &
TupleCut::
getRange ( ) const
{
  return m_range;
}

void
TupleCut::
setInversion ( bool state )
{
  m_invert = state;
}

bool
TupleCut::
getInversion ( ) const
{
  return m_invert;
}

void TupleCut::toggleInverted()
{
  m_invert = m_invert ? false : true;
}

const std::string &
TupleCut::
getLabel () const
{
  return m_label;
}

void
TupleCut::
setLabel ( const std::string & label )
{
  m_label = label;
}
