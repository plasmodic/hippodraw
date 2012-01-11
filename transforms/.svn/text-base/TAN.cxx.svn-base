/** @file

TAN class implementation

Copyright (C) 2002-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: TAN.cxx,v 1.3 2005/11/10 23:43:01 xiefang Exp $

*/

#include "TAN.h"
#include "UnaryTransform.h"

using namespace hippodraw;


TAN::TAN ( UnaryTransform * z )
  : WCSTransform ( z, true, true, false, true)
{
  m_name = "TAN";
  const std::string projType("TAN");
  double crpix[] = {0, 90}, crval[] = {0,90}, cdelt[] = {1,1};
  initwcs (projType, crpix, crval, cdelt, 0, false);
}

TAN::TAN ( const TAN & t )
  :  WCSTransform ( t )
{
  const std::string projType("TAN");
  double crpix[] = {0, 90}, crval[] = {0,90}, cdelt[] = {1,1};
  initwcs (projType, crpix, crval, cdelt, 0, false);
}

TAN::~TAN ()
{
}

#ifdef CLONE_DEFECT
TransformBase * TAN::clone () const
#else
TAN    * TAN::clone () const
#endif
{
  return new TAN ( *this );
}



/* virtual */
double TAN::aspectRatio () const
{
  return 1.0;
}



