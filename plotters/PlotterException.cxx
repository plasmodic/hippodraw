/** @file

PlotterException class implementation

Copyright (C)  2003  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: PlotterException.cxx,v 1.3 2005/10/30 00:10:13 pfkeb Exp $

*/

#include "PlotterException.h"

namespace hippodraw {

PlotterException::
PlotterException ( const std::string & type ) 
{
  m_message = type;
}

PlotterException::
PlotterException ( const PlotterException & e )
  : exception ()
{
  m_message = e.m_message;
}

PlotterException::
~PlotterException () throw () 
{
}

const char * 
PlotterException::
what () const throw ()
{
  return m_message.c_str();
}

} // namespace hippodraw

