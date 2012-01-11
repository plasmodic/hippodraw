/** @file

Mercator class implementation

Copyright (C) 2002-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Mercator.cxx,v 1.3 2005/11/10 23:43:01 xiefang Exp $

*/

#include "Mercator.h"
#include "UnaryTransform.h"

using namespace hippodraw;


Mercator::Mercator ( UnaryTransform * z )
  : WCSTransform ( z, true, true, false, true)
{
  m_name = "Mercator";
  const std::string projType("MER");
  double crpix[] = {0, 0}, crval[] = {0,0}, cdelt[] = {1,1};
  initwcs (projType, crpix, crval, cdelt, 0, false);
}

Mercator::Mercator ( const Mercator & t )
  :  WCSTransform ( t )
{
  const std::string projType("MER");
  double crpix[] = {0, 0}, crval[] = {0,0}, cdelt[] = {1,1};
  initwcs (projType, crpix, crval, cdelt, 0, false);
}

Mercator::~Mercator ()
{
}

#ifdef CLONE_DEFECT
TransformBase * Mercator::clone () const
#else
Mercator    * Mercator::clone () const
#endif
{
  return new Mercator ( *this );
}



/* virtual */
double Mercator::aspectRatio () const
{
  return 2.0;
}



