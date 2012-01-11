/* HippoPlot AxisTick  class implementation
 *
 * Copyright (C)  2000  The Board of Trustees of The Leland Stanford
 * Junior University.  All Rights Reserved.
 *
 * $Id: AxisTick.cxx,v 1.9 2005/10/30 00:10:09 pfkeb Exp $
 */

// for dll interface warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "AxisTick.h"

using std::string;

namespace hippodraw {

AxisTick::AxisTick ( ) :
  m_v ( 0 ),
  m_c ( "" )
{
}

AxisTick::AxisTick ( const AxisTick & at ) :
  m_v ( at.m_v ),
  m_c ( at.m_c )
{
}

AxisTick::AxisTick ( double v, const std::string & s ) : 
  m_v ( v ), 
  m_c ( s )
{
}

double
AxisTick::value ( ) const
{
  return m_v;
}

void
AxisTick::setValue ( double v )
{
  m_v = v;
}

const string & 
AxisTick::content ( ) const
{
  return m_c;
}

void
AxisTick::setContent ( const std::string & s )
{
  m_c = s;
}

} // namespace hippodraw
