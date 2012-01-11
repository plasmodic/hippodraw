/** @file

hippodraw::LMFitter class implemenation.

Copyright (C) 2000-2007  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: LMFitter.cxx,v 1.94 2007/04/23 21:57:21 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "LMFitter.h"

#include "NumLinAlg.h"
#include "StatedFCN.h"

#include <algorithm>

#include <cmath>
#include <cassert>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::abs;
using std::distance;
using std::swap;
using std::vector;
#endif

using namespace hippodraw::Numeric;

using namespace hippodraw;

LMFitter::
LMFitter ( const char * name )
  : Fitter ( name ),
    m_chi_cutoff ( 0.000001 ),
    m_start_lambda ( 0.001 ),
    m_lambda_shrink_factor( 9.8 ),
    m_lambda_expand_factor( 10.2 )
{
}


LMFitter::
LMFitter ( const LMFitter & fitter )
  : Fitter ( fitter ),
    m_chi_cutoff ( fitter.m_chi_cutoff ),
    m_start_lambda ( fitter.m_start_lambda ),
    m_lambda_shrink_factor ( fitter.m_lambda_shrink_factor ),
    m_lambda_expand_factor ( fitter.m_lambda_expand_factor )
{
}


Fitter *
LMFitter::
clone ( ) const
{
  return new LMFitter ( *this );
}

/** @todo Should probably move this implementation into the FCN. 
 */
void LMFitter::calcAlpha () 
{
  m_fcn -> calcAlphaBeta ( m_alpha, m_beta );
  unsigned int num_parms = m_beta.size();

  unsigned int j = 0; // for MS VC++
  for ( ; j < num_parms; j++ ) {
    for ( unsigned int k = 0; k < j; k++ ) {
      m_alpha[k][j] = m_alpha[j][k];
    }
  }

  j = 0; // for MS VC++
  for ( ; j < num_parms; j++ ) {
    m_alpha[j][j] *= ( 1.0 + m_lambda );
  }
}

/** @note Thecovarience matrix  is inverse of half the hessian
   at the optimal solution, and this semihessian matrix is nothing but the
   m_alpha matrix as calculated above BUT w/o the diagonal elements
   being scaled by the factor of (1+lambda). So we can set lamba = 0 to
   calculate it. */
int LMFitter::calcCovariance ( std::vector < std::vector < double > >& cov )
{
  m_lambda = 0;
  calcAlpha ();
    
  // Invert the semi hessian to get the error covarience matrix
  // Since we use cholesky factorisation we can conclude if the
  // given matrix is positive definite or not. So the return value
  // is EXIT_SUCCESS if m_alpha is Positive Definite, EXIT_FAILURE otherwise. 
  return invertMatrix ( m_alpha, cov ); 
}
  
bool LMFitter::solveSystem ()
{
  unsigned int num_parms = m_beta.size ();

  vector< int > ipiv ( num_parms, 0 );

  vector< int > indxr ( num_parms, -1 );
  vector< int > indxc ( num_parms, -1 );

  unsigned int irow = UINT_MAX;
  unsigned int icol = UINT_MAX;

  for ( unsigned int i = 0; i < num_parms; i++ ) {
    double big = 0.0;

    for ( unsigned int j = 0; j < num_parms; j++ ) {
      if ( ipiv[j] != 1 ) {
	
	for ( unsigned int k = 0; k < num_parms; k++ ) {
	  if ( ipiv[k] == 0 ) {
	    if ( abs ( m_alpha[j][k] ) >= big ) {
	      big = abs ( m_alpha[j][k] );
	      irow = j;
	      icol = k;
	    }
	  }
	  else if ( ipiv[k] > 1 ) {
	    return false;
	  }
	}
      }
    }

    if ( irow == UINT_MAX ) { // something is wrong, can't do fit.
      return false;
    }

    ++ipiv[icol];
    if ( irow != icol ) {
      for ( unsigned int l = 0; l < num_parms; l++ ) {
	swap ( m_alpha[irow][l], m_alpha[icol][l] );
      }
      swap ( m_beta[irow], m_beta[icol] );
    }
    indxr[i] = irow;
    indxc[i] = icol;
    if ( m_alpha[icol][icol] == 0.0 ) {
      return false;
    }
    double pivinv = 1.0 / m_alpha[icol][icol];
    m_alpha[icol][icol] = 1.0;

    for ( unsigned int l = 0; l < num_parms; l++ ) {
      m_alpha[icol][l] *= pivinv;
    }
    m_beta[icol] *= pivinv;

    for ( unsigned int ll = 0; ll < num_parms; ll++ ) {
      if ( ll != icol ) {
	double dum = m_alpha[ll][icol];
	m_alpha[ll][icol] = 0.0;

	for ( unsigned int l = 0; l < num_parms; l++ ) {
	  m_alpha[ll][l] -= m_alpha[icol][l] * dum;
	}
	m_beta[ll] -= m_beta[icol] * dum;
      }
    }
  }

  for ( int l = num_parms - 1; l >= 0; l-- ) {
    if ( indxr[l] != indxc[l] ) {

      for ( unsigned int k = 0; k < num_parms; k++ ) {
	swap ( m_alpha[k][indxr[l]], m_alpha[k][indxc[l]] );
      }
    }
  }
  return true;
}

bool LMFitter::calcStep ()
{
  calcAlpha ();
  bool ok = solveSystem ();

  return ok;
}

bool LMFitter::calcBestFit ()
{
  m_lambda = m_start_lambda;

  int i = 0;
  for ( ; i < m_max_iterations; i++ ) {

    double old_chisq = objectiveValue ();

    vector< double > old_parms;
    m_fcn -> fillFreeParameters ( old_parms );

    bool ok = calcStep ();
    assert ( old_parms.size() == m_beta.size() );

    vector< double > new_parms ( old_parms );
    vector< double >::iterator pit = new_parms.begin ( );
    vector< double >::iterator dit = m_beta.begin ( );

    while ( pit != new_parms.end () ) {
      *pit++ += *dit++;
    }
    m_fcn -> setFreeParameters ( new_parms );

    double new_chisq = objectiveValue ();

    if ( abs ( old_chisq - new_chisq ) < m_chi_cutoff ) break;

    if ( new_chisq < old_chisq ) {
      m_lambda /= m_lambda_shrink_factor; 
    }
    else {
      m_lambda *= m_lambda_expand_factor;
      m_fcn -> setFreeParameters ( old_parms );
    }

    if ( ! ok ) return ok;
  }

  return i < m_max_iterations;
}


void
LMFitter::
setFCN ( StatedFCN * fcn )
{
  Fitter::setFCN ( fcn );
  fcn -> setNeedsDerivatives ( true );
}
