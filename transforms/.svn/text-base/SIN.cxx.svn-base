/** @file

SIN class implementation

Copyright (C) 2002-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: SIN.cxx,v 1.3 2005/11/10 23:43:01 xiefang Exp $

*/

#include "SIN.h"
#include "UnaryTransform.h"

using namespace hippodraw;


SIN::SIN ( UnaryTransform * z )
  : WCSTransform ( z, true, true, false, true)
{
  m_name = "SIN";
  const std::string projType("SIN");
  double crpix[] = {0, 90}, crval[] = {0,90}, cdelt[] = {1,1};
  initwcs (projType, crpix, crval, cdelt, 0, false);
}

SIN::SIN ( const SIN & t )
  :  WCSTransform ( t )
{
  const std::string projType("SIN");
  double crpix[] = {0, 90}, crval[] = {0,90}, cdelt[] = {1,1};
  initwcs (projType, crpix, crval, cdelt, 0, false);
}

SIN::~SIN ()
{
}

#ifdef CLONE_DEFECT
TransformBase * SIN::clone () const
#else
SIN    * SIN::clone () const
#endif
{
  return new SIN ( *this );
}



/* virtual */
double SIN::aspectRatio () const
{
  return 1.0;
}



