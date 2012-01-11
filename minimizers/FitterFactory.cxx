/** @file

hippodraw::FitterFactory class implementation

Copyright (C) 2003, 2005, 2006-2007   The Board of Trustees of The
Leland Stanford Junior University.  All Rights Reserved.

$Id: FitterFactory.cxx,v 1.21 2007/07/02 18:17:15 pfkeb Exp $

*/

// for have minuit
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "FitterFactory.h"

// List of default fitters
#include "LMFitter.h"
#include "BFGSFitter.h"

#if HAVE_MINUIT || HAVE_MINUIT2
#include "MinuitMigrad.h"
#endif

// List of default objective functions
#include "NTupleChiSqFCN.h"
#include "NTupleLikeliHoodFCN.h"
#include "NTuplePearsonFCN.h"

#include <stdexcept>

using std::string;
using std::vector;

using namespace hippodraw;

FitterFactory * FitterFactory::s_instance = 0;

FitterFactory::FitterFactory ()
{
}

FitterFactory * FitterFactory::instance ()
{
  if ( s_instance == 0 ) {
    s_instance = new FitterFactory ();
    s_instance->initialize ();
  }

  return s_instance;
}

void FitterFactory::initialize ()
{
  Fitter * fitter = new LMFitter ( "ChiSq: Levenberg Marquart" );
  StatedFCN * chifcn = new NTupleChiSqFCN ();
  fitter -> setFCN ( chifcn );
  add ( fitter  );

  fitter = new LMFitter ( "Pearson: BFGS" );
  StatedFCN * pearsonfcn = new NTuplePearsonFCN ();
  fitter -> setFCN ( pearsonfcn );
  add ( fitter  );

  fitter = new BFGSFitter   ( "MLEH: BFGS" );
  StatedFCN * likfcn = new NTupleLikeliHoodFCN ();
  fitter -> setFCN ( likfcn ); 
  add ( fitter );
 
#if HAVE_MINUIT || HAVE_MINUIT2
  fitter = new MinuitMigrad ( "ChiSq: Minuit(Migrad)" );
  chifcn = new NTupleChiSqFCN ();
  fitter -> setFCN ( chifcn );
  add ( fitter );

  fitter = new MinuitMigrad ( "MLEH: Minuit(Migrad)" );
  likfcn = new NTupleLikeliHoodFCN ();
  fitter -> setFCN ( likfcn ); 
  add ( fitter );
#endif
  
}

void
FitterFactory::
setDefault ( const std::string & name )
{
  bool yes = exists ( name );

  if ( yes == false ) {
    string what ( "FitterFactory: Fitter with name `" );
    what += name;
    what += "' does not exist.";
    throw std::runtime_error ( what );
  }

  m_default = name;
}

const string &
FitterFactory::
getDefault () const
{
  if ( m_default.empty () ) {
    const vector < string > & fitters = names ();
    return fitters.front ();
  }
  //else
  return m_default;
}
