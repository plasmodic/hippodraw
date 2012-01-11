/** @file

Lambert class implementation

Copyright (C) 2002-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Lambert.cxx,v 1.22 2006/02/09 22:56:59 xiefang Exp $

*/

#include "Lambert.h"
#include "UnaryTransform.h"

using namespace hippodraw;


Lambert::Lambert ( UnaryTransform * z )
  : WCSTransform ( z, true, true, false, true)
{
  m_name = "Lambert";
  const std::string projType("ZEA");
  double crpix[] = {0, 0}, crval[] = {0,90}, cdelt[] = {1, 1};
  initwcs (projType, crpix, crval, cdelt, 0, false);
}

Lambert::Lambert ( const Lambert & t )
  :  WCSTransform ( t )
{
  const std::string projType("ZEA");
  double crpix[] = {0, 0}, crval[] = {0,90}, cdelt[] = {1, 1};
  initwcs (projType, crpix, crval, cdelt, 0, false);
}

Lambert::~Lambert ()
{
}

#ifdef CLONE_DEFECT
TransformBase * Lambert::clone () const
#else
Lambert    * Lambert::clone () const
#endif
{
  return new Lambert ( *this );
}



/* virtual */
double Lambert::aspectRatio () const
{
  return 1.0;
}



