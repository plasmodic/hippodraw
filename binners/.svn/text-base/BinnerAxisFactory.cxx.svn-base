/*
 * HippoPlot BinnerAxisFactory class implementation
 *
 * Copyright (C) 2003   The Board of Trustees of The Leland Stanford
 * Junior University.  All Rights Reserved.
 *
 * $Id: BinnerAxisFactory.cxx,v 1.4 2005/10/30 00:10:09 pfkeb Exp $
 *
 */

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "BinnerAxisFactory.h"

// List of default Binner container objects
#include "BinnerAxisLinear.h"
#include "BinnerAxisLog.h"

namespace hippodraw {

BinnerAxisFactory * BinnerAxisFactory::s_instance = 0;

BinnerAxisFactory::BinnerAxisFactory ()
{
}

BinnerAxisFactory * BinnerAxisFactory::instance ()
{
  if ( s_instance == 0 ) {
    s_instance = new BinnerAxisFactory ();
    s_instance->initialize ();
  }
  return s_instance;
}

void BinnerAxisFactory::initialize ()
{
  add ( new BinnerAxisLinear () );
  add ( new BinnerAxisLog () );
}

} // namespace hippodraw
