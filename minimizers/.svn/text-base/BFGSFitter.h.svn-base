/* -*- mode: c++ -*- */

/** @file

BFGSFitter  class interface

Copyright (C) 2003-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BFGSFitter.h,v 1.22 2005/10/30 00:10:12 pfkeb Exp $

*/

#include <map>

#ifndef _BFGSFitter_H_
#define _BFGSFitter_H_

#include "Fitter.h"

namespace hippodraw {

/** The minimization algorithm used is a complementary quasi newton
    algorithm, with BFGS (Broyden-Fletcher-Goldfarb-Shanno) update
    and hence the name BFGSFitter. The entire minimization algorithm
    relies heavily on the description given in Numerical Optimization
    by Jorge Nocedal, Stephen J. Wright Publisher: Springer Verlag;
    (August 27, 1999) ISBN: 0387987932). In the following description
    we will refer to this book a number of times. 

    @author Kaustuv <kaustuv@stanford.edu>
*/
class MDL_HIPPOPLOT_API BFGSFitter: public Fitter
{
private:

  /* Using compiler generated copy constructor. 
     BFGSFitter ( const BFGSFitter & )
  */

  /** The inverse of the quasi-Hessian. */
  std::vector< std::vector< double > > m_M;
  
  /** The initial value to start the iteration from */
  std::vector< double > m_xinit;
  
  /** The gradient cut-off parameter.  If two successive iterations
      change the the norm of the gradient of Likelihood function less
      than this quantity, then iteration is terminated */
  double m_grad_cutoff;
  
  /** The step cut-off parameter.  If the two successive iterations
      result in the norm of difference of iterates less than this quantity,
      then iteration is terminated */
  double m_step_cutoff;
  
  /** The projection cut-off parameter.  If the projection of step on
      the gradient is less than this quantity, then iteration is terminated */
  double m_proj_cutoff;
  
  /** c1,c2   - constants such that 0 < c1 < c2 < 1 and they ensure that strong
      Wolfe conditions hold true.
      
      Strong Wolfe conditions:
      
      f(x_k + Alpha_k * p_k) <= f(x_k) + c1 * Alpha_k * grad f(x_k)'*p_k (1)
      |grad f(x_k + Alpha_k * p_k)'*p_k| >= c2 * grad f(x_k)' * p_k    (2)
      
      The lower the c2 value, the closer you are asking the
      algorithm  to get to an actual local minimum.
      The lower the c1 value, the sole demand being made is that
      the direction be a direction of descent.
      
      Nocedal and Wright suggest use of following:
      c1      = 1e-4;
      c2      = 0.9; */
  double m_c1, m_c2;
  
  /** Maximum step length to try, suggested value by Nocedal and Wright is 
      alpha_max = 4 */
  double m_alpha_max; 
  
  /** First step length to try and  this must be less than Alpha_max.
      (typically Alpha1 = 1 for Newton's method and relatives) */
  double m_alpha1;

  /** Map of the various iteration parameters to their name*/
  std::map< std::string, double * >  m_iter_params;
    
 public:
  /** The constructor taking name of fitter as argument.  */
  BFGSFitter ( const char * name );
  
  Fitter * clone () const;
  
  /** Calculates the covariance matrix. Returns EXIT_SUCCESS if a minima
      of the log likelihood functions is found, returns EXIT_FAILURE in case
      algorithm converges on other stationary points (i.e. on saddle points). */
  virtual int calcCovariance ( std::vector< std::vector < double > >& cov );
  
  /** Computes a step satisfying the Wolfe conditions.  Based on Algorithm
      3.2 (Line Search Algorithm) on pp. 58--60 of "Numerical Optimization"
      by Jorge Nocedal and Stephen J. Wright */
  double wolfeStep( const std::vector< double > & x0,
		    const std::vector< double > & p ) const;

  /** A function which helps out Wolfe in deciding the step length.
      Based on Algorithm 3.3 (zoom) on pg. 60 of "Numerical Optimization"
      by Jorge Nocedal and Stephen J. Wright */
  double zoom( const std::vector< double >& x0,
	       const std::vector< double >& p,
	       double phi0, double dphi0,
	       double Alphalo, double Alphahi ) const;

  /** A cubic interpolation routine */
  double interpolate( const std::vector< double >& x0,
		      const std::vector< double >& p,
		      double Alphaim, double Alphai ) const;
  
  /** The objective function */
  double function( const std::vector< double > & x ) const;

  /** The gradient of the objective function */
  std::vector< double > gradient( const std::vector< double > & x ) const;

  /** Efficient computation of gradient of the objective function
      with a vector p. */
  double gradp( const std::vector< double > & u,
		const std::vector< double > & p ) const;
  
  /** Returns the initial value of the iterate */
  const std::vector< double > & initIter() const;
  
  /** Sets the initial value of the iterate, assuming it is given as a vector*/
  int setInitIter( const std::vector< double > & xinit );

  /** Given a string, this function returns the value of the associated
      iteration parameter. Names of the various iteration parameters
      have been chosen according to the following simple rule. m_foo_bar
      is called foo_bar. */
  double iterParam( std::string name );

  /** Given a string and a double, this function sets the value of the
      associated iteration parameter. Names of the various parameters have
      been chosen according to the following simple rule. m_foo_bar is called
      foo_bar. */
  int setIterParam( std::string name, double value );  
  
  /** Main driver routine for BFGS algorithm which has been used in
      computing the  bets fit for the function.
  */
  virtual bool calcBestFit(); 

};

} // namespace hippodraw

#endif // _BFGSFitter_H_
