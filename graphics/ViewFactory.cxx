/** @file

ViewFactory Implementation

Copyright (C)  2000, 2004  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.
*
$Id: ViewFactory.cxx,v 1.8 2005/10/30 00:10:12 pfkeb Exp $

 */

#include "ViewFactory.h"

namespace hippodraw {

ViewFactory * ViewFactory::m_instance = 0;

ViewFactory::ViewFactory ( )
{
  m_instance = this;
}

ViewFactory * ViewFactory::instance()
{
  return m_instance;
}

} // namespace hippodraw
