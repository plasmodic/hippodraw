/** @file 

hippodraw::BFGSFitter class implemenation.

Copyright (C) 2003-2006  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BFGSFitter.cxx,v 1.41 2006/09/22 13:31:37 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#ifdef _MSC_VER
#define isnan _isnan
#endif

//To have isnan.
#ifdef __APPLE__
#include <cstdlib>
#define _GLIBCPP_USE_C99 1
#endif

#include "BFGSFitter.h"

#include "NumLinAlg.h"
#include "StatedFCN.h"

//  see todo
#include <iostream>
using std::cout;
using std::endl;

#include <cfloat>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <ctime>

#ifdef __APPLE__
using std::isnan;
#endif


using std::pow;
using std::swap;
using std::min;
using std::abs;
using std::vector;
using std::vector;
using std::string;
using std::map;

using namespace hippodraw;

using namespace Numeric;

BFGSFitter::BFGSFitter(  const char * name )
  : Fitter ( name ),
   m_xinit( 1 ),
   m_grad_cutoff( 1e-6 ),
   m_step_cutoff( 1e-6 ),
   m_proj_cutoff( 1e-6 ),
   m_c1( 1e-4 ),
   m_c2( 0.9 ),
   m_alpha_max( 4 ),
   m_alpha1( 1 )
{
  m_iter_params[ "grad_cutoff" ] = & m_grad_cutoff;
  m_iter_params[ "step_cutoff" ] = & m_step_cutoff;
  m_iter_params[ "proj_cutoff" ] = & m_proj_cutoff;
  m_iter_params[ "c1" ] = & m_c1;
  m_iter_params[ "c2" ] = & m_c2;
  m_iter_params[ "alpha_max" ] = & m_alpha_max;
  m_iter_params[ "alpha1" ] = & m_alpha1;
}

Fitter * BFGSFitter::clone ( ) const
{
  return new BFGSFitter ( *this ); // uses compiler generated copy constructor
}

  /** @todo Use if ( x!=x ) to test for nan.
   */
bool
BFGSFitter::
calcBestFit()
{
  double Alpha_star;

  // Initialization
  vector < double > init_params;
  m_fcn -> fillFreeParameters ( init_params );
  setInitIter( init_params );
  
  // Allocate space for various matrices / vectors needed
  vector< double > xold = m_xinit;
  vector< double > xnew = m_xinit;
  vector< double > gk       = gradient( xold );
  vector< double > gkPlusUn = gradient( xnew );
  
  vector< double > pk( m_xinit.size() );
  vector< double > s( m_xinit.size() );
  vector< double > y( m_xinit.size() );

  // The standard quasi newton initialization 
  eye ( m_M, m_xinit.size() );
  m_M = ( 1.0 / norm( gk ) ) * m_M;
  
  vector< vector< double > > t1 , t2;
  eye( t1, m_xinit.size() );
  eye( t2, m_xinit.size() );

  double fx    = function( xnew );
  double fxold = fx;
  for( int k = 1; k <= m_max_iterations; k++ )
    {
      // Update the iterate.
      gk   = gkPlusUn;
      pk   = m_M * (-gk);
      Alpha_star = wolfeStep( xold, pk );
      
      do
	{
	  xnew = xold + Alpha_star * pk;
	  fx   = function( xnew );
	  Alpha_star /= 3.0;
	}
      while( isnan( fx ) );
      
      gkPlusUn = gradient( xnew );

      s = xnew - xold;
      y = gkPlusUn - gk;

      double ys = innerProduct( y, s );
      double yy = norm( y );
      double ss = norm( s );
      
      // Termination criteria
      if( (  abs( ys )           < m_proj_cutoff ) ||
	  (  abs( Alpha_star )   < DBL_EPSILON   ) ||
	  (  ss                  < m_step_cutoff ) ||
	  (  yy                  < m_grad_cutoff ) ||
	  (  fx                  >= fxold         )  )
	break;
      
      // DFP Update of inverse of approximate hessian.
      // M = M-(s*y'*M+M*y*s')/(y'*s)+(1+(y'*M*y)/(y'*s))*(s*s')/(y'*s);
      double temp = ( 1 + innerProduct( y, m_M * y ) / ys ) / ys;
      
      t1 = ( outerProduct(s, y) * m_M)/ys + ( m_M * outerProduct(y , s) ) / ys;
      t2 =  temp * outerProduct( s, s );
      m_M  =  m_M - t1 + t2;
      
      // one pass of the loop is over so refresh
      xold = xnew;
      fxold = fx;
    }
  
  m_fcn -> setFreeParameters ( xold );
  //write( xold );
    
  return true;
}

/** @todo Take out error message and send signal to application.
 */
double BFGSFitter::wolfeStep( const std::vector< double >& x0,
			      const std::vector< double >& p ) const
{
  double step_fac = sqrt(2.0);          // Geometric step factor; must be > 1
  
  double phi0  = function( x0 );        // Function value at the initial point
  double dphi0 = gradp( x0, p );        // innner product of gradient and p 
  
  // dphi0 >= 0 means you are ascending. To avoid it set step = 0 and
  // terminate the iterations ( maybe after giving out a warning )
  // Algorithm ensures that such case will not arise but curse of finite
  // precision mathematics lead to some very small positive dphi0.
  if ( dphi0 >= 0 )
    return 0.0;
  
  double Alpha0  = 0;
  double Alphai  = m_alpha1;
  double Alphaim = Alpha0;
  double phiim   = phi0;
  int i = 1;
  int done = 0;
  int cnt  = 0;
  
  double phii, dphii;
  
  while ( !done )
    {
      phii = function( x0 + Alphai * p );
      
      if ( (phii > (phi0 + m_c1 * Alphai * dphi0)) ||
	   ( (phii >= phiim) && ( i > 1)) )
        return zoom( x0, p, phi0, dphi0, Alphaim, Alphai );
      
      dphii = gradp( x0 + Alphai * p , p );
      
      if ( abs( dphii ) <= -m_c2 * dphi0 )
        return Alphai;
      
      if (dphii >= 0)
        return zoom( x0, p, phi0, dphi0, Alphai, Alphaim );
      
      // Choose new Alphai in (Alphai, Alpha_max)
      Alphaim = Alphai;
      phiim = phii;
      Alphai = min( step_fac * Alphai, m_alpha_max );
      
      if ( Alphai == m_alpha_max )
	cnt += 1;
      
      if (cnt > 1)
	{
// 	  cout << "WARNING: Unable to bracket a strong Wolfe pt. in [ "
// 	       << m_alpha1 << ", " << m_alpha_max << " ]" << endl;
	  return m_alpha_max;
	}
      
      i += 1;
      //cout << "    Wolfe Iteration: " << i << endl;
    }

  return 0.0;
}

double BFGSFitter::zoom( const std::vector< double >& x0,
			 const std::vector< double >& p,
			 double phi0, double dphi0,
			 double Alphalo, double Alphahi ) const
{
  int MaxIter = 20; 
  int iter = 0;
  int done = 0;

  double philo, phij;
  double dphij;
  double Alphaj;
  double Alpha_star = 0.0;
  
  while ( !done  && iter < MaxIter )
    {
      iter += 1;
      
      philo = function( x0 + Alphalo * p );

      // Find a trial step length Alphaj between Alphalo and Alphahi
      Alphaj = interpolate( x0, p, Alphalo, Alphahi );
     
      // Evalaute phi( Alphaj )
      phij = function( x0 + Alphaj * p );

      if( (phij > phi0 + m_c1 * Alphaj * dphi0) || (phij >= philo) )
	Alphahi = Alphaj;
      else
	{
	  dphij = gradp( x0 + Alphaj * p , p );
	  
	  if ( abs( dphij ) <= -m_c2 * dphi0 )
	    Alpha_star = Alphaj;
	  return Alpha_star;
	
	  if ( dphij * ( Alphahi - Alphalo ) >= 0 )
	    Alphahi = Alphalo;
	  
	  Alphalo = Alphaj;
	}
      
    }

  // If above loop fails take the mid-point
  if (iter == MaxIter)
    Alpha_star = 0.5 * ( Alphahi + Alphalo );
  
  return Alpha_star;

}


double BFGSFitter::interpolate( const std::vector< double >& x0,
				const std::vector< double >& p,
				double Alphaim,
				double Alphai ) const
{
  
  if ( Alphaim > Alphai )
    swap( Alphaim, Alphai);
  
  double phiim  = function( x0 + Alphaim * p );
  double phii   = function( x0 + Alphai  * p );
  
  double dphiim = gradp( x0 + Alphaim * p, p );
  double dphii  = gradp( x0 + Alphai  * p, p );
  
  double d1     = dphiim + dphii - 3 * ( (phiim - phii)/(Alphaim - Alphai) );
  double d2     = sqrt( d1 * d1 - dphiim * dphii);
  
  double Alphaip = Alphai - (Alphai - Alphaim) * 
    ( (dphiim + d2 - d1) / (dphii - dphiim + 2 * d2)    );
  
  double lth = abs(Alphai - Alphaim);
  
  if( abs(Alphaip - Alphai)  < 0.05 * lth ||
      abs(Alphaip - Alphaim) < 0.05 * lth ||
      Alphaip < Alphaim ||
      Alphaip > Alphai )
    Alphaip = 0.5 * (Alphai + Alphaim);
  
  return Alphaip;
}

double BFGSFitter::function( const std::vector< double > &  u ) const
{
  // Gets the value of the objective function from pvfcn
  // first you have to convert the vector to vector
  vector< double > x( u.size() );

  for( unsigned int i = 0; i < u.size(); i++ )
    x[i] = u[i];
  
  m_fcn -> setFreeParameters ( x );
  
  return objectiveValue();

  // Following are a few standard test functions
  // which were used to test this minimizer. 

  //double x = u[0]; double y = u[1];
 
  // Rosenbrock's function
  //return 100 * (y - x * x) * (y - x * x)  + (1 - x) * (1 - x);

  // Freudenstein and Roth's Function
  //return pow(-13 + x  + ((5 - y)*y - 2 )*y, 2) + 
  //  pow(-29 + x  + ((y + 1)*y - 14)*y, 2);

  // Beale Function  
  //return pow(1.5   - x*(1 - y   ), 2) + 
  //  pow(2.25  - x*(1 - y*y ), 2) + 
  //  pow(2.625 - x*(1 - y*y*y ), 2);
}

vector< double >
BFGSFitter::gradient( const std::vector< double > & u ) const
{
  double h = 1e-5;
  vector< double > x( u.size(), 0.0 );
  vector< double > xph( u.size(), 0.0 );
  
  vector< double > g( u.size() );
  
  for( unsigned int i = 0; i < u.size(); i++ )
    x[i] = u[i];
  
  // Calculating the gradient by finite differencing
  m_fcn -> setFreeParameters ( x );
  double fx   = m_fcn -> objectiveValue ();
  double fxph = 0.0;
  for( unsigned int i = 0; i < u.size(); i++ )
    {
      for( unsigned int j = 0; j < u.size(); j++ ) {
	xph[j] = ( i == j ) ? ( x[j] + h ) : x[j];
      }
      m_fcn -> setFreeParameters ( xph );
      fxph =  m_fcn -> objectiveValue ();
      g[i] = ( fxph - fx ) / h;
    }
 
  // Following are a few gradients of standard test functions
  // which were used to test this minimizer.
  
  /* double x = u[0]; double y = u[1];
     vector< double > g(2);*/
  
  // Gradient of Rosenbrock's function
  /*  g[0] = -400 * x * ( y   - x * x )   - 2 * ( 1 - x );
      g[1] =  200 *     ( y   - x * x );*/
  
  // Gradient of Freudenstein and Roth's Function
  //g[0] = 2*(y*(y*(y+1)-14)+ x - 29) +
  //	 2*(y*((5-y)*y-2) + x - 13);
  //g[1] = 2*(y*(2*y+1) + y*(y+1)   - 14) * (y*(y*(y+1) - 14) + x - 29) + 
  //	 2*((5-y)*y   + (5-2*y)*y -  2) * (y*((5-y)*y -  2) + x - 13);

  // Gradient of Beale Function
  //g[0]= 2*(2.625 - x * (1 - y*y*y) ) * (y*y*y - 1)+
  //  2*(2.25  - x * (1 - y*y) ) * (y*y - 1)+
  //  2*(1.5   - x * (1 - y  ) ) * (y   - 1);
  //g[1]= 6*x*y*y*(2.625 - x * (1 - y*y*y)) + 
  //  4*x*y  *(2.25  - x * (1 - y*y)) + 
  //  2*x    *(1.5   - x * (1 - y  ));

  return g;
}


double BFGSFitter::gradp( const std::vector< double > & u,
			  const std::vector< double > & p ) const
{
  double h = 1e-5;
  vector< double > x( u.size() );
  
  // Calculating the gradient in direction of p by finite differencing
  for ( unsigned int i = 0; i < u.size(); i++ ) {
    x[i]  = u[i];
  }
//   double fx   = m_fcn -> operator()( x );
  m_fcn -> setFreeParameters ( x );
  double fx   = m_fcn -> objectiveValue ( );
  
  for ( unsigned int i = 0; i < u.size(); i++ ) {
    x[i] += h * p[i] ;
  }
  m_fcn -> setFreeParameters ( x );
  double fxph = m_fcn -> objectiveValue ();
    
  return ( fxph - fx ) / h;
    
  // Following are a directional derivative of standard test functions
  // which were used to test this minimizer.
  
  /* double x = u[0]; double y = u[1];
     vector< double > g(2);*/
  
  // Gradient of Rosenbrock's function
  /*  g[0] = -400 * x * ( y   - x * x )   - 2 * ( 1 - x );
      g[1] =  200 *     ( y   - x * x );*/
  
  // Gradient of Freudenstein and Roth's Function
  //g[0] = 2*(y*(y*(y+1)-14)+ x - 29) +
  //	 2*(y*((5-y)*y-2) + x - 13);
  //g[1] = 2*(y*(2*y+1) + y*(y+1)   - 14) * (y*(y*(y+1) - 14) + x - 29) + 
  //	 2*((5-y)*y   + (5-2*y)*y -  2) * (y*((5-y)*y -  2) + x - 13);

  // Gradient of Beale Function
  //g[0]= 2*(2.625 - x * (1 - y*y*y) ) * (y*y*y - 1)+
  //  2*(2.25  - x * (1 - y*y) ) * (y*y - 1)+
  //  2*(1.5   - x * (1 - y  ) ) * (y   - 1);
  //g[1]= 6*x*y*y*(2.625 - x * (1 - y*y*y)) + 
  //  4*x*y  *(2.25  - x * (1 - y*y)) + 
  //  2*x    *(1.5   - x * (1 - y  ));

  // return g[0] * p[0] + g[1] * p[1];
}

const vector< double > & BFGSFitter::initIter() const
{
  return m_xinit;
}

int BFGSFitter::setInitIter( const std::vector< double > & xinit )
{
  m_xinit.resize( xinit.size() );
  
  // Provide  a random perturbations to the initial value
  //srand( (unsigned) time( NULL ) );
  //for( unsigned int i = 0; i < xinit.size(); i++ )
  //m_xinit[i] = xinit[i] * (1 + 0.025 * ( 0.5 - rand() / ( RAND_MAX + 1.0 )));

  m_xinit = xinit;
  
  return EXIT_SUCCESS;
}

int BFGSFitter::calcCovariance ( std::vector < std::vector < double > >& cov )
{
  cov.resize( m_xinit.size() );
  for( unsigned int i = 0; i < m_xinit.size(); i++ )
    cov[i].resize( m_xinit.size(), 0.0 );

  for( unsigned int i = 0; i < m_xinit.size(); i++ )
    for( unsigned int j = 0; j < m_xinit.size(); j++ )
      cov[i][j] = m_M[i][j];

  // set return flag as EXIT_SUCCESS if cov is Positive Definite,
  // and to EXIT_FAILURE otherwise.
  int flag = cholFactor( cov  );
  
  for( unsigned int i = 0; i < m_xinit.size(); i++ )
    for( unsigned int j = 0; j < m_xinit.size(); j++ )
      cov[i][j] = m_M[i][j];
  
 return flag;
}


double BFGSFitter::iterParam ( std::string name )
{
  // First check if user is attempting to access the max_iterations
  // This is a hack, but it was necessary to ensure a uniform interface
  if( name == "max_iterations" )
    return m_max_iterations;
  
  // Don't use map::operator[]() to find the name and its
  // associated value, as it will create one if it doesn't exist.
  map< string, double * >::const_iterator it 
    = m_iter_params.find ( name );

  if ( it == m_iter_params.end () )
    cout << name << " is not a valid iteration parameter name" << endl;
  else
    return *m_iter_params[name];

  return 0.0;
}

int BFGSFitter::setIterParam ( std::string name, double value )

{

  // First check if user is attempting to modify the max_iterations
  // This is a hack, but it was necessary to ensure a uniform interface
  if( name == "max_iterations" )
    {
      m_max_iterations = ( int ) value;
      return EXIT_SUCCESS;
    }

  // Now start worrying about the other parameters. 
  // Don't diretly use map::operator[]() to find the name and its
  // associated value, as it will create one if it doesn't exist.
  map< string, double * >::const_iterator it 
    = m_iter_params.find ( name );
  
  if ( it == m_iter_params.end () )
    {
      cout << name << " is not a valid iteration parameter name" << endl;
      return EXIT_FAILURE;
    }
  else
    {
      *m_iter_params[name] = value;
      return EXIT_SUCCESS;
    }
  
  return EXIT_FAILURE;
}
