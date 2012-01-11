/* -*- mode: c++ -*- */

/** @file

LMFitter class interface

Copyright (C) 2000, 2001, 2003, 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: LMFitter.h,v 1.52 2005/10/30 00:10:12 pfkeb Exp $

*/

#ifndef _LMFitter_H_
#define _LMFitter_H_

#include "Fitter.h"

namespace hippodraw {

/** A Fitter class.  A Fitter class that uses derivatives of functions
    for fitting.

    An implementation of the Levenberg Marquart.

    @request Make the fit() method a member function of the DataRep
    instead of the function in the Python interface.

    @author Daniel Engovatov <engo@slac.stanford.edu>
    @author Kaustuv <kaustuv@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API LMFitter : public Fitter
{

private:

  /** The copy constructor.
   */
  LMFitter ( const LMFitter & );

  /** The alpha matrix. */
  std::vector < std::vector < double > > m_alpha;

  /** The beta vector. */
  std::vector < double > m_beta;

  /** The Chi Squared cut-off parameter.  If the two iterations of a
      step change the Chi squared less than this quantity, then no more
      steps are taken. */
  double m_chi_cutoff;

  /** The starting lambda factor. */
  double m_start_lambda;

  /** The current lambda factor. */
  double m_lambda;

  /** The factor by which lambda is divided if it is too large  */
  double m_lambda_shrink_factor;

  /** The factor by which lambda is divided if it is too small */
  double m_lambda_expand_factor;
  
public:

  /** The constructor taking name of fitter as argument. 
   */
  LMFitter ( const char * name );

  Fitter * clone () const;

  /** Calculates the best fit.  Returns @c true if the fit converged;
      otherwise it returns @c false. */
  virtual bool calcBestFit ();

  /** Takes a step in the fitting and returns the change to the
      parameters of the function. */
  virtual bool calcStep ();

  /** Calculates the alpha matrix. */
  virtual void calcAlpha ();

  /** Calculates the covariance matrix. Returns EXIT_SUCCESS if a minima
      of the chi2 functions is found, returns EXIT_FAILURE in case algorithm
      converges on other stationary points (i.e. on saddle points). */
  virtual int calcCovariance ( std::vector< std::vector < double > >& cov );
        
  /** Solves the system. */
  virtual bool solveSystem ();

  /** Sets the objective function.  Sets the objective function and
      indicate to it that it needs partial derivatives.
  */
  virtual void setFCN ( StatedFCN * fcn );

};

} // namespace hippodraw

#endif // _LMFitter_H_
