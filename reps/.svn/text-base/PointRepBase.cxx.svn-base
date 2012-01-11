/** @file

PointRepBase class implementation

Copyright (C)  2000-2002, 2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: PointRepBase.cxx,v 1.35 2005/10/30 00:10:17 pfkeb Exp $
*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "PointRepBase.h"

#include "ErrorBarRep.h"

#include "graphics/ViewBase.h"

using std::string;

namespace hippodraw {

PointRepBase::PointRepBase ( const char * name, float size )
  : RepBase ( name, size ),
    m_error_rep ( 0 )
{
}

PointRepBase::PointRepBase ( const PointRepBase & point_rep )
  : RepBase ( point_rep ),
    m_error_rep ( 0 ) // derived class can reset it.
{
}

/* virtual */
void PointRepBase::setSelected ( bool yes )
{
  RepBase::setSelected ( yes );

  if ( m_error_rep ) {
    m_error_rep->setSelected ( yes );
  }
}

} // namespace hippodraw
