/* -*- mode: c++ -*- */

/** @file

FactoryException class implementation

Copyright (C)  2002-2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: FactoryException.cxx,v 1.6 2005/10/30 00:10:12 pfkeb Exp $

*/

#include "FactoryException.h"

namespace hippodraw {

FactoryException::
FactoryException ( const FactoryException & e )
  : exception ()
{
  m_type = e.m_type;
}

FactoryException::
FactoryException ( const std::string & type ) 
{
  m_type = type;
}

FactoryException::
~FactoryException ( ) throw ()
{
}

char const * 
FactoryException::
what () const throw ()
{
  std::string retval ( "No such type: `" );
  retval += m_type;
  retval += "' could be found in factory";

  return retval.c_str();
}

} // namespace hippodraw

