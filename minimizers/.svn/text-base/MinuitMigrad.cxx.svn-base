/** @file

hippodraw::MinuitMigrad class implemenation.

Copyright (C) 2003-2006  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: MinuitMigrad.cxx,v 1.45 2007/01/08 22:13:24 pfkeb Exp $

*/

#include "MinuitMigrad.h"

#include "StatedFCN.h"

#include "pattern/string_convert.h"

#ifdef HAVE_MINUIT
#include <Minuit/FunctionMinimum.h>
#include <Minuit/MinuitParameter.h>
#include <Minuit/MnMigrad.h>
#include <Minuit/MnUserParameters.h>
#endif

#ifdef HAVE_MINUIT2
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MinuitParameter.h>
#include <Minuit2/MnMigrad.h>
#include <Minuit2/MnUserParameters.h>
using namespace ROOT::Minuit2;
#endif

#include <stdexcept>

using std::string;
using std::vector;

using namespace hippodraw;

MinuitMigrad::
MinuitMigrad ( const char * name )
  : Fitter ( name ),
    m_minimizer ( 0 ) // null pointer
{
}

MinuitMigrad::
MinuitMigrad ( const MinuitMigrad & mm )
  : Fitter ( mm ),
    m_minimizer ( 0 )
{
//   if ( mm.m_minimizer != 0 ) {
//     m_minimizer = new MnMigrad ( * mm.m_minimizer );
//   }
}

Fitter *
MinuitMigrad::
clone ( ) const
{
  return new MinuitMigrad ( *this );
}
void
MinuitMigrad::
initialize ()
{
  m_minimizer = 0;
  bool yes = m_fcn -> hasFunction ();
  if ( yes ) {
    const vector < double > & parms = m_fcn -> getParameters ();
    const vector < int > & fixes = m_fcn -> getFixedFlags ();
    assert ( fixes.size() == parms.size() );
    unsigned int size = parms.size ();
    MnUserParameters mn_parms;

    // Minuit has limit of 11 characters in parameter names.  So we
    // don't give the real names which could be longer.
    for ( unsigned int i = 0; i < size; i++ ) {
      const string name = String::convert ( i );
      double value = parms [ i ];
#ifdef HAVE_MINUIT
      mn_parms.add ( name.c_str(), value, 0.1 );
#else
      mn_parms.Add ( name.c_str(), value, 0.1 );
#endif

      if ( fixes [ i ] != 0 ) {
#ifdef HAVE_MINUIT
	mn_parms.fix ( i );
#else
	mn_parms.Fix ( i );
#endif
      }
    }

    m_minimizer = new MnMigrad ( *m_fcn, mn_parms );

    if ( m_limits.empty () == false ) {
      for ( unsigned int i = 0; i < m_limits.size (); i++ ) {
	if ( m_limits[i].set == true ) {
#ifdef HAVE_MINUIT
	  m_minimizer -> setLimits ( i, m_limits[i].lower, m_limits[i].upper );
#else
	  m_minimizer -> SetLimits ( i, m_limits[i].lower, m_limits[i].upper );
#endif
	}
      }
    }

  }
}

void
MinuitMigrad::
checkIndex ( unsigned int i )
{
  if ( m_minimizer == 0 ) initialize ();

  if ( m_minimizer == 0 ) {
    string what ( m_name );
    what += ": model function no yet set";
    throw std::runtime_error ( what );
  }
  const vector < double > & parms = m_fcn -> getParameters ();
  unsigned int size = parms.size();
  if ( i < size == false ) {
    string what ( m_name );
    what += ": index to parameter out of range";
    throw std::runtime_error ( what );
  }
}

void
MinuitMigrad::
initLimits ()
{
  const vector < double >  & parms = m_fcn -> getParameters ();
  for ( unsigned int i = 0; i < parms.size(); i ++ ) {
    limit l;
    l.set = false;
    m_limits.push_back ( l );
  }
}

void
MinuitMigrad::
setLimits ( unsigned int i, double lower, double upper )
{
  if ( m_limits.empty() == true ) {
    initLimits ();
  }

  checkIndex ( i );

  m_limits[i].lower = lower;
  m_limits[i].upper = upper;
  m_limits[i].set = true;
}

void
MinuitMigrad::
setStepSize ( unsigned int i, double size )
{
  checkIndex ( i );
#ifdef HAVE_MINUIT2
  m_minimizer -> SetError ( i, size );
#else
  m_minimizer -> setError ( i, size );
#endif
}

/** @note The implementation always creates a new MinuitMigrad object
    as there appears to be no way to add additional parameters once
    the MinuitMigrad object is created.  Within HippoDraw, we allow
    adding of functions to the composite LinearSumFunction, which
    effectively adds parameters.
 */
bool
MinuitMigrad::
calcBestFit ()
{
  initialize ();
  FunctionMinimum fun_min = m_minimizer -> operator () ();
  // in the above call, hidden parameters are maxfcn=0, edm=1.e-5

#ifdef HAVE_MINUIT2
  bool yes = fun_min.IsValid ();
#else
  bool yes = fun_min.isValid ();
#endif

  if ( yes ) {

#ifdef HAVE_MINUIT2
    std::vector < double > cur_parms = m_minimizer -> Params();
#else
    std::vector < double > cur_parms = m_minimizer -> params();
#endif
    m_fcn -> setParameters ( cur_parms );
  }
  return yes;
}

int 
MinuitMigrad::
calcCovariance ( std::vector < std::vector < double > >& covar )
{ 
  if ( m_minimizer == 0 ) initialize ();

#ifdef HAVE_MINUIT2
  const MnUserCovariance & covar_m = m_minimizer ->Covariance ();
  unsigned int size = covar_m.Nrow ();
#else
  const MnUserCovariance & covar_m = m_minimizer ->covariance ();
  unsigned int size = covar_m.nrow ();
#endif
  covar.resize ( size );
  for ( unsigned int i = 0; i < size; i++ ) {
    covar[i].resize ( size, 0.0 );
  }

  for ( unsigned int i = 0; i < size; i++ ) {
    for ( unsigned int j = 0; j < size; j++ ) {
      covar[i][j] = covar_m ( i, j );
    }
  }

  return EXIT_SUCCESS;
}
