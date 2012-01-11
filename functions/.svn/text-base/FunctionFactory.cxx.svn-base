/** @file

hippodraw::FunctionFactory class implementation

Copyright (C)  2000-2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: FunctionFactory.cxx,v 1.40 2005/12/13 17:52:05 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "functions/FunctionFactory.h"

// List of default functions
#include "functions/BrokenPowerLaw.h"
#include "functions/ConstantF.h"
#include "functions/Erfc.h"
#include "functions/Exponential.h"
#include "functions/Gamma.h"
#include "functions/Gaussian.h"
#include "functions/Landau.h"
#include "functions/Linear.h"
#include "functions/LinearSumFunction.h"
#include "functions/LogNormal.h"
#include "functions/LogParabola.h"
#include "functions/Novosibirsk.h"
#include "functions/PowerLaw.h"
#include "functions/Quadratic.h"
#include "functions/Quadratic2.h"
#include "functions/Weibull.h"
#include "functions/Chi2Dist.h"

using namespace hippodraw;

FunctionFactory * FunctionFactory::s_instance = 0;

FunctionFactory::FunctionFactory ()
{
}

FunctionFactory * FunctionFactory::instance ()
{
  if ( s_instance == 0 ) {
    s_instance = new FunctionFactory ();
    s_instance->initialize ();
  }
  return s_instance;
}

void FunctionFactory::initialize ()
{
  add ( new Gaussian () );
  add ( new Gamma () );
  add ( new Erfc () );
  add ( new Exponential () );
  add ( new Landau () );
  add ( new Linear () );
  add ( new LinearSumFunction () );
  add ( new LogNormal () );
  add ( new LogParabola () );
  add ( new Novosibirsk () );
  add ( new PowerLaw () );
  add ( new BrokenPowerLaw () );
  add ( new Quadratic () );
  add ( new Quadratic2 () );
  add ( new Weibull () );
  add ( new ConstantF () );
  add ( new Chi2Dist () );
}
