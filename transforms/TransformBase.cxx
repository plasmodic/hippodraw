/*
 * HippoPlot TransformBase class implementation
 *
 *
 * Copyright (C)  2001, 2002  The Board of Trustees of The Leland Stanford
 * Junior University.  All Rights Reserved.
 *
 * $Id: TransformBase.cxx,v 1.21 2005/10/30 00:10:18 pfkeb Exp $
 */

// for dll interface warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "TransformBase.h"

#include "axes/Range.h"

#include <cassert>

using std::string;

namespace hippodraw {

/** Should never be called. Sets the function name to "nil".
    It should be overridden by a derived class.  If the 
    function name is "nil", then this indicates the error 
    condition that the default constructor was called 
    directly. */
TransformBase::TransformBase ()
{
  m_name = "nil";
}

TransformBase::TransformBase ( const TransformBase & t ) 
  : m_name ( t.m_name )
{
}

TransformBase::~TransformBase ()
{
}

const string & TransformBase::name () const
{
  return m_name;
}

double TransformBase::aspectRatio() const
{
  return 0.0;
}

} // namespace hippodraw
