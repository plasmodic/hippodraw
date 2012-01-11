/** @file

STG class implementation

Copyright (C) 2002-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: STG.cxx,v 1.3 2005/11/10 23:43:01 xiefang Exp $

*/

#include "STG.h"
#include "UnaryTransform.h"

using namespace hippodraw;


STG::STG ( UnaryTransform * z )
  : WCSTransform ( z, true, true, false, true)
{
  m_name = "STG";
  const std::string projType("STG");
  double crpix[] = {0, 90}, crval[] = {0,90}, cdelt[] = {1,1};
  initwcs (projType, crpix, crval, cdelt, 0, false);
}

STG::STG ( const STG & t )
  :  WCSTransform ( t )
{
  const std::string projType("STG");
  double crpix[] = {0, 90}, crval[] = {0,90}, cdelt[] = {1,1};
  initwcs (projType, crpix, crval, cdelt, 0, false);
}

STG::~STG ()
{
}

#ifdef CLONE_DEFECT
TransformBase * STG::clone () const
#else
STG    * STG::clone () const
#endif
{
  return new STG ( *this );
}



/* virtual */
double STG::aspectRatio () const
{
  return 1.0;
}



