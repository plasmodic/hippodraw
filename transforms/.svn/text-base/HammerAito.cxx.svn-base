/** @file

hippodraw::HammerAito class implementation

Copyright (C) 2002-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: HammerAito.cxx,v 1.49 2005/12/12 20:19:38 pfkeb Exp $

*/

#include "HammerAito.h"
#include "UnaryTransform.h"

using namespace hippodraw;


HammerAito::HammerAito ( UnaryTransform * z )
  : WCSTransform ( z, true, true, false, true)
{
  m_name = "HammerAito";
  const std::string projType("AIT");
  double crpix[] = {0, 0}, crval[] = {0,0}, cdelt[] = {1,1};
  initwcs (projType, crpix, crval, cdelt, 0, false);
}

HammerAito::HammerAito ( const HammerAito & t )
  :  WCSTransform ( t )
{
  const std::string projType("AIT");
  double crpix[] = {0, 0}, crval[] = {0,0}, cdelt[] = {1,1};
  initwcs (projType, crpix, crval, cdelt, 0, false);
}

HammerAito::~HammerAito ()
{
}

#ifdef CLONE_DEFECT
TransformBase * HammerAito::clone () const
#else
HammerAito    * HammerAito::clone () const
#endif
{
  return new HammerAito ( *this );
}



/* virtual */
double HammerAito::aspectRatio () const
{
  return 2.0;
}



