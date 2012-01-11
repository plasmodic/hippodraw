/*
 * HippoPlot XYZTransformBase class implementation
 *
 *
 * Copyright (C)  2001  The Board of Trustees of The Leland Stanford
 * Junior University.  All Rights Reserved.
 *
 * $Id: XYZTransformBase.cxx,v 1.4 2005/10/30 00:10:18 pfkeb Exp $
 */

#ifdef _MSC_VER
// for truncation warning
#include "msdevstudio/MSconfig.h"
#endif //_MSC_VER

#include "transforms/XYZTransformBase.h"

namespace hippodraw {

/** Should never be called.. Sets the function name to "nil".
    Should be overriden by derived class.  If function name
    is "nil", then that indicated error condition that default
    constructor was called directly. */
XYZTransformBase::XYZTransformBase ()
{
  m_name = "nil";
}

XYZTransformBase::XYZTransformBase ( const XYZTransformBase & t )
  : TransformBase ( t )
{
}

XYZTransformBase::~XYZTransformBase ()
{
}

} // namespace hippodraw

