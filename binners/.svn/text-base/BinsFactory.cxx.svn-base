/** @file

BinsFactory class implementation

Copyright (C) 2003, 2004   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: BinsFactory.cxx,v 1.4 2005/10/30 00:10:10 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "BinsFactory.h"

// List of default Bins container objects
#include "Bins1DHist.h"
#include "Bins1DProfile.h"
#include "Bins2DHist.h"
#include "Bins2DProfile.h"

namespace hippodraw {

BinsFactory * BinsFactory::s_instance = 0;

BinsFactory::BinsFactory ()
{
}

BinsFactory * BinsFactory::instance ()
{
  if ( s_instance == 0 ) {
    s_instance = new BinsFactory ();
    s_instance->initialize ();
  }
  return s_instance;
}

void BinsFactory::initialize ()
{
  add ( new Bins1DHist () );
  add ( new Bins1DProfile () );
  add ( new Bins2DHist () );
  add ( new Bins2DProfile () );
}

} // namespace hippodraw
