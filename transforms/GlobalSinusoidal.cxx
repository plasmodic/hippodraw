/** @file

GlobalSinusoidal class implementation

Copyright (C) 2002-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: GlobalSinusoidal.cxx,v 1.3 2005/11/10 23:43:00 xiefang Exp $

*/

#include "GlobalSinusoidal.h"
#include "UnaryTransform.h"

using namespace hippodraw;


GlobalSinusoidal::GlobalSinusoidal ( UnaryTransform * z )
  : WCSTransform ( z, true, true, false, true)
{
  m_name = "GlobalSinusoidal";
  const std::string projType("GLS");
  double crpix[] = {0, 0}, crval[] = {0,0}, cdelt[] = {1,1};
  initwcs (projType, crpix, crval, cdelt, 0, false);
}

GlobalSinusoidal::GlobalSinusoidal ( const GlobalSinusoidal & t )
  :  WCSTransform ( t )
{
  const std::string projType("GLS");
  double crpix[] = {0, 0}, crval[] = {0,0}, cdelt[] = {1,1};
  initwcs (projType, crpix, crval, cdelt, 0, false);
}

GlobalSinusoidal::~GlobalSinusoidal ()
{
}

#ifdef CLONE_DEFECT
TransformBase * GlobalSinusoidal::clone () const
#else
GlobalSinusoidal    * GlobalSinusoidal::clone () const
#endif
{
  return new GlobalSinusoidal ( *this );
}



/* virtual */
double GlobalSinusoidal::aspectRatio () const
{
  return 2.0;
}



