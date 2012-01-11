/* -*- mode: c++ -*- */

/** @file

FunctionParameter class implementation

Copyright (C) 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FunctionParameter.cxx,v 1.2 2005/10/30 00:10:10 pfkeb Exp $

*/

#include "FunctionParameter.h"

namespace hippodraw {

FunctionParameter::
FunctionParameter ( const std::string & name,
		    double value,
		    double error,
		    bool fixed,
		    double low_limit,
		    double high_limit,
		    bool has_low,
		    bool has_high )
  : m_name ( name ),
    m_value ( value ),
    m_error ( error ),
    m_lower_limit ( low_limit ),
    m_upper_limit ( high_limit ),
    m_fixed ( fixed ),
    m_has_lower ( has_low ),
    m_has_upper ( has_high )
{
}

const std::string & 
FunctionParameter::
name () const
{
  return m_name;
}

double 
FunctionParameter::
value () const
{
  return m_value;
}

double 
FunctionParameter::
error () const
{
  return m_error;
}

bool 
FunctionParameter::
isFixed () const
{
  return m_fixed;
}

double
FunctionParameter::
lowerLimit () const
{
  return m_lower_limit;
}

double 
FunctionParameter::
upperLimit () const
{
  return m_upper_limit;
}

bool 
FunctionParameter::
hasLowerLimit () const
{
  return m_has_lower;
}


bool 
FunctionParameter::
hasUpperLimit () const
{
  return m_has_upper;
}

} // namespace hippodraw

