/** -*- mode:c++ -*- */

/** @file

DataRepException class implementation

Copyright (C)  2002, 2003, 2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: DataRepException.cxx,v 1.5 2005/10/30 00:10:10 pfkeb Exp $

*/

#include "DataRepException.h"

namespace hippodraw {

DataRepException::
DataRepException ( const std::string & type ) 
{
  m_type = type;
}

DataRepException::
DataRepException ( const DataRepException & e )
  : exception ( )
{
  m_type = e.m_type;
}

DataRepException::
~DataRepException () throw () 
{
}

const char * 
DataRepException::
what () const throw ()
{
  std::string retval ( "No such label: `" );
  retval += m_type;
  retval += "' for ntuple column";

  return retval.c_str();
}

} // namespace hippodraw

