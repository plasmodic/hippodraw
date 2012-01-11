/** @file

BinsBase  class implementation

Copyright (C) 2000-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinsBase.cxx,v 1.37 2006/06/20 21:23:46 xiefang Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "BinsBase.h"

#include <cassert>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::list;
using std::string;
using std::vector;
#endif

namespace hippodraw {

BinsBase::BinsBase ( const char * name )
  : m_name ( name ),
    m_scale_factor ( 1.0 ),
    m_is_scaling ( false ),
    m_values_dirty ( true ),
    m_empty ( true )
{
}

BinsBase::BinsBase ( const BinsBase & binner )
  : m_name ( binner.m_name ),
    m_scale_factor ( binner.m_scale_factor ),
    m_is_scaling ( binner.m_is_scaling ),
    m_values_dirty ( binner.m_values_dirty ),
    m_empty ( binner.m_empty )
{
}

BinsBase::~BinsBase ()
{
}

const string &
BinsBase::
name () const
{
  return m_name;
}

bool BinsBase::isDirty ()
{
  return m_values_dirty;
}

void BinsBase::setDirty()
{
  m_values_dirty = true;
}

double BinsBase::scaleFactor () const
{
  return 1.0;
}

double BinsBase::getZValue ( double, double ) const
{
  return 0;
}

bool
BinsBase::
isEmpty ( ) const
{
  return m_empty;
}

void
BinsBase::
scaleNumberOfEntries ( double number )
{
  m_scale_factor = number;
}

void
BinsBase::
setEntriesScaling ( bool on )
{
  m_is_scaling = on;
}
 

void
BinsBase::
setMinEntries ( int entries )
{
}

int
BinsBase::
getMinEntries ()
{
  return -1;
}

}// namespace hippodraw

