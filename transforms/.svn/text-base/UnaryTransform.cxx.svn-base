/** @file

UnaryTransform class implementation

Copyright (C) 2002, 2004  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: UnaryTransform.cxx,v 1.6 2005/10/30 00:10:18 pfkeb Exp $

*/

#include "UnaryTransform.h"

#include <cassert>

namespace hippodraw {

UnaryTransform::UnaryTransform ( double low, double high )
  : m_limits ( low, high )
{
}

UnaryTransform::~UnaryTransform ()
{
}

/* virtual */ 
const Range & UnaryTransform::limits () const
{
  return m_limits;
}

} // namespace hippodraw

