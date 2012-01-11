/** @file

AIR class implementation

Copyright (C) 2002-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: AIR.cxx,v 1.4 2005/11/10 23:43:00 xiefang Exp $

*/

#include "AIR.h"
#include "UnaryTransform.h"

using namespace hippodraw;


AIR::AIR ( UnaryTransform * z )
  : WCSTransform ( z, true, true, false, true)
{
  m_name = "AIR";
  const std::string projType("AIR");
  double crpix[] = {0, 90}, crval[] = {0,90}, cdelt[] = {1,1};
  initwcs (projType, crpix, crval, cdelt, 0, false);
}

AIR::AIR ( const AIR & t )
  :  WCSTransform ( t )
{
  const std::string projType("AIR");
  double crpix[] = {0, 90}, crval[] = {0,90}, cdelt[] = {1,1};
  initwcs (projType, crpix, crval, cdelt, 0, false);
}

AIR::~AIR ()
{
}

#ifdef CLONE_DEFECT
TransformBase * AIR::clone () const
#else
AIR    * AIR::clone () const
#endif
{
  return new AIR ( *this );
}



/* virtual */
double AIR::aspectRatio () const
{
  return 1.0;
}



