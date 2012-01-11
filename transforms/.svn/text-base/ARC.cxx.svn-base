/** @file

ARC class implementation

Copyright (C) 2002-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: ARC.cxx,v 1.5 2006/02/09 22:56:59 xiefang Exp $

*/

#include "ARC.h"
#include "UnaryTransform.h"

using namespace hippodraw;


ARC::ARC ( UnaryTransform * z )
  : WCSTransform ( z, true, true, false, true)
{
  m_name = "ARC";
  const std::string projType("ARC");
  double crpix[] = {0, 0}, crval[] = {0,90}, cdelt[] = {1,1};
  initwcs (projType, crpix, crval, cdelt, 0, false);
}

ARC::ARC ( const ARC & t )
  :  WCSTransform ( t )
{
  const std::string projType("ARC");
  double crpix[] = {0, 0}, crval[] = {0,90}, cdelt[] = {1,1};
  initwcs (projType, crpix, crval, cdelt, 0, false);
}

ARC::~ARC ()
{
}

#ifdef CLONE_DEFECT
TransformBase * ARC::clone () const
#else
ARC    * ARC::clone () const
#endif
{
  return new ARC ( *this );
}



/* virtual */
double ARC::aspectRatio () const
{
  return 1.0;
}



