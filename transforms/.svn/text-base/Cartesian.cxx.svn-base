/** @file

Cartesian class implementation

Copyright (C) 2002-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Cartesian.cxx,v 1.4 2005/11/10 23:43:00 xiefang Exp $

*/

#include "Cartesian.h"
#include "UnaryTransform.h"

using namespace hippodraw;


Cartesian::Cartesian ( UnaryTransform * z )
  : WCSTransform ( z, true, true, false, true)
{
  m_name = "Cartesian";
  const std::string projType("CAR");
  double crpix[] = {0, 0}, crval[] = {0,0}, cdelt[] = {1,1};
  initwcs (projType, crpix, crval, cdelt, 0, false);
}

Cartesian::Cartesian ( const Cartesian & t )
  :  WCSTransform ( t )
{
  const std::string projType("CAR");
  double crpix[] = {0, 0}, crval[] = {0,0}, cdelt[] = {1,1};
  initwcs (projType, crpix, crval, cdelt, 0, false);
}

Cartesian::~Cartesian ()
{
}

#ifdef CLONE_DEFECT
TransformBase * Cartesian::clone () const
#else
Cartesian    * Cartesian::clone () const
#endif
{
  return new Cartesian ( *this );
}



/* virtual */
double Cartesian::aspectRatio () const
{
  return 2.0;
}



