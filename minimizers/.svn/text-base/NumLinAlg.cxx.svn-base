/** @file

Implemenation of linear algrebra functions.

Copyright (C) 2003-2005  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: NumLinAlg.cxx,v 1.13 2005/05/16 18:39:20 pfkeb Exp $

@author Kaustuv <kaustuv@stanford.edu>

*/

#include "NumLinAlg.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <cstdlib>

using std::ofstream;
using std::ifstream;
using std::setw;
using std::setprecision;
using std::cout;
using std::endl;
using std::vector;
using std::abs;

namespace hippodraw {
  namespace Numeric {

std::vector< double >
operator + ( const std::vector< double >& x, 
	     const std::vector< double >& y  )
{
  int nrows = x.size();
  vector< double > z;
  
  z.resize( nrows, 0.0 );
  
  for ( int i = 0; i < nrows; i++)
      z[i] = x[i] + y[i];

  return z;
}

std::vector< double >
operator - ( const std::vector< double >& x,
	     const std::vector< double >& y  )
{
  int nrows = x.size();
  vector< double > z;
  
  z.resize( nrows, 0.0 );
  
  for ( int i = 0; i < nrows; i++)
      z[i] = x[i] - y[i];

  return z;
}

std::vector< double >
operator - ( const std::vector< double >& y  )
{
  int nrows = y.size();
  vector< double > z;
  
  z.resize( nrows, 0.0 );
  
  for ( int i = 0; i < nrows; i++ )
      z[i] = - y[i];

  return z;
}

std::vector< vector< double > >
operator + ( const std::vector< std::vector< double > >&A,
	     const std::vector< std::vector< double > >&B )
{
  int nrows = A.size();
  int ncols = A[0].size();
   
  vector< vector< double > > C( nrows );
  for( int i = 0; i < nrows; i++ )
    C[i].resize( ncols, 0.0 );
  
  for (int i = 0; i < nrows; i++) 
    for (int j = 0; j < ncols; j++)
      C[i][j] = A[i][j] +  B[i][j];

  return C;
}

std::vector< vector< double > >
operator - ( const std::vector< std::vector< double > >&A,
	     const std::vector< std::vector< double > >&B )
{
  int nrows = A.size();
  int ncols = A[0].size();
   
  vector< vector< double > > C( nrows );
  for( int i = 0; i < nrows; i++ )
    C[i].resize( ncols, 0.0 );
  
  for (int i = 0; i < nrows; i++) 
    for (int j = 0; j < ncols; j++)
      C[i][j] = A[i][j] -  B[i][j];

  return C;
}


std::vector< double >
operator * ( double a, const std::vector< double >& x )
{
  int nrows = x.size();
  vector< double > y;
  
  y.resize( nrows, 0.0 );
      
  for ( int i = 0; i < nrows; i++)
      y[i] = a * x[i];

  return y;
}

std::vector< double >
operator / ( const std::vector< double >& x, double a )
{
  int nrows = x.size();
  vector< double > y;
  
  assert( abs( a ) > DBL_EPSILON );

  y.resize( nrows, 0.0 );
  
  for ( int i = 0; i < nrows; i++)
      y[i] = x[i] / a;

  return y;
}

std::vector< std::vector< double > >
operator * ( double a, const std::vector< std::vector< double > >&A )
{
  int nrows = A.size();
  int ncols = A[0].size();
    
  vector< vector< double > > B( nrows );
  for( int i = 0; i < nrows; i++ )
    B[i].resize( ncols, 0.0 );
    
  for (int i = 0; i < nrows; i++) 
    for (int j = 0; j < ncols; j++)
      B[i][j] = a * A[i][j];

  return B;
}

std::vector< std::vector< double > >
operator / ( const std::vector< std::vector< double > >& A, double a )
{
  int nrows = A.size();
  int ncols = A[0].size();

  assert( abs( a ) > DBL_EPSILON );
  
  vector< vector< double > > B( nrows );
  for( int i = 0; i < nrows; i++ )
    B[i].resize( ncols, 0.0 );
  
  for (int i = 0; i < nrows; i++) 
    for (int j = 0; j < ncols; j++)
      B[i][j] = A[i][j]/a;

  return B;
}

std::vector< double >
operator * ( const std::vector< std::vector< double > >& A,
	     const std::vector< double >& x )
{
  int nrows = A.size();
  int ncols = A[0].size();
  vector< double > y;
  
  y.resize( nrows, 0.0 );
  
  for (int i = 0; i < nrows; i++) 
    for (int j = 0; j < ncols; j++)
      y[i] += A[i][j] * x[j];

  return y;
}

std::vector< double >
operator * ( const std::vector< double >& x,
	     const std::vector< std::vector< double > >& A )
{
  int nrows = A.size();
  int ncols = A[0].size();
  vector< double > y;
  
  y.resize( ncols, 0.0 );
    
  for (int j = 0; j < ncols; j++)
    for (int i = 0; i < nrows; i++) 
      y[j] += A[i][j] * x[i];

  return y;
}

std::vector< vector< double > >
operator * ( const std::vector< std::vector< double > >&A,
	     const std::vector< std::vector< double > >&B )
{
  int m = A.size();
  int r = A[0].size();
  int n = B[0].size();
  
  vector< vector< double > > C( m );
  for( int i = 0; i < m; i++ )
    C[i].resize( n, 0.0 );
  
  for (int i = 0; i < m; i++) 
    for (int j = 0; j < n; j++)
      for (int k = 0; k < r; k++)
	C[i][j] += A[i][k] * B[k][j];

  return C;
}

double
innerProduct( const std::vector< double > & a,
	      const std::vector< double > & b )
{
  double prod = 0.0;
  
  for ( unsigned int i = 0; i < a.size(); i++ ) 
    prod += a[i] * b[i];

  return prod;
}


vector< vector< double > >
outerProduct ( const std::vector< double >& a,
	       const std::vector< double >& b )
{
  vector< vector< double> > C( a.size() );
  for( unsigned int i = 0; i < a.size(); i++ ) 
    C[i].resize( b.size() );
  
  for( unsigned int i = 0; i < a.size(); i++ )
    for( unsigned int j = 0; j < b.size(); j++ )
      C[i][j] = a[i] * b[j];
  
  return C;
}


double
quadraticProduct( const std::vector< std::vector< double >  > & A,
		  const std::vector< double > x )
{
  double prod = 0.0;
  unsigned int n = A.size();
  
  assert ( x.size() == n );

  for ( unsigned int i = 0; i < n; i++ )
    for ( unsigned int j = 0; j < n; j++ )
      prod += x[i] * A[i][j] * x[j];
      
  return prod;
}


double
norm ( const std::vector< double > & a )
{
  return sqrt( innerProduct( a, a ) );
}


int
cholFactor ( std::vector < std::vector< double > > & A )
{
  unsigned int n = A.size();
  
  for ( unsigned int i  = 0; i  < n ; ++i )
    for ( unsigned int j = 0; j <= i ; ++j)
      {
	double  sum = A[i][j];
	
	A[j][i] = 0;
	
	for ( unsigned int k  = 0; k  < j; ++k )
	  sum -= A[i][k] * A[j][k];     
	
	if (i  == j)
	  {
	    if (sum < 0) return EXIT_FAILURE;    

	    sum = sqrt (sum);
	    
	    if ( fabs(sum) < DBL_EPSILON ) return EXIT_FAILURE;
	    
	    A[i][j] = sum;
	  }
	else
	  A[i][j] = sum / A[j][j];
      }
  
  return EXIT_SUCCESS;
}


int
cholBackSolve ( const std::vector < std::vector< double >  > &  L,
		std::vector< double > & x,
		const std::vector< double > & b )
{
  unsigned int n = L.size();
  
  double sum;

  // Solving L y = b 
  for ( unsigned int i  = 0; i  < n ; i++)
    {
      sum = b [i];
      for ( unsigned int j = 0; j < i ; ++j)
       sum -= x[j] * L[i][j];
     x[i] = sum / L[i][i];
   }
  
  // Solving L' x = y
  for ( int i = n - 1; i >= 0; i-- )
    {
      sum = x[i];
      for ( unsigned int j = i + 1; j < n ; j++)
      	sum -= x[j] * L[j][i];
      x[i] = sum / L[i][i]; 
    }

  return EXIT_SUCCESS;
}


int
invertMatrix ( const std::vector < std::vector< double >  > &  A,
	       std::vector < std::vector < double >  > &  Ainv )
{
  unsigned int n = A.size();
  vector< double > x, b;
  vector< vector< double >  > L;
  
  // Set appropriate sizes for the vectors and matrices 
  x.resize( n, 0.0 );
  b.resize( n, 0.0 );
  
  L.resize( n );
  Ainv.resize( n );
  
  for ( unsigned int i = 0; i < n; i++ )
    {
      L[i].clear ();
      L[i].resize ( n, 0.0 );

      Ainv[i].clear ();
      Ainv[i].resize ( n, 0.0 );
    }
  
  for ( unsigned int i = 0; i < n; i++ )
    for ( unsigned int j = 0; j < n; j++ )
      L[i][j] = A[i][j];
  
  // Do a cholesky factorization writing the lower triangular factor
  // in the lower triangular part of L and setting the upper part as 0
  cholFactor( L );
    
  for ( unsigned int j = 0; j < n; j++ )
    {
      // Set up right hand side i.e. set b = ei  
      for ( unsigned int i = 0; i < n; i++ )
	b[i] = ( i == j ) ? 1 : 0;
      
      // LL'x= b is being solved here
      cholBackSolve( L, x, b );
      
      // This x constitutes the j th coloumn of the inverse
      for ( unsigned int i = 0; i < n; i++ )
      	Ainv[i][j] = x[i] ;
    }

  return EXIT_SUCCESS;
}

int
eye ( std::vector < std::vector < double > >& I, int n )
{
  I.resize( n );
  for( int i = 0; i < n; i++ )
    {
      I[i].clear();
      I[i].resize( n, 0.0 );
    }

  for( int i = 0; i < n; i++ )
    I[i][i] = 1.0; 

  return EXIT_SUCCESS;
}

int
write ( const std::vector < double >  &  a )
{
  unsigned int n = a.size();
  
  for ( unsigned int i = 0; i < n; ++i )
    cout <<  setprecision( 15 ) <<  a[i]  << endl;
  cout << endl;
  
  return EXIT_SUCCESS;
}


int
write ( const std::vector < std::vector < double >  > &  A )
{
  unsigned int n = A.size();

  for ( unsigned int i = 0; i < n; ++i )
    {
      for ( unsigned int j = 0; j < n; ++j )
	cout << setw( 8 ) <<  setprecision( 4 ) << A[i][j];
      cout << endl;
    }

  cout << endl;
  
  return EXIT_SUCCESS;
}


int
allocateMatrix ( std::vector < std::vector < double > > & A,
		 int m, int n )
{
  A.resize( m );
  for( int i = 0; i < m; i++ )
    {
      A[i].clear();
      A[i].resize( n, 0.0 );
    }
  
  return EXIT_SUCCESS;
}


int
allocateVector ( std::vector < double > & x, int n )
{
  x.clear();
  x.resize( n, 0.0 );
  
  return EXIT_SUCCESS;
}


/* The driver main subroutine to check a few of above function.
   Test with the following matlab code:
   n = 4; L = tril( randn(n,n), -1 ) + eye(n,n); A = L' * L;  B =  randn(n , n); x =  randn(n , 1);  y = randn(n , 1); save test.txt -ascii A B x y;
   Then perform the operations as stated in the following program.
*/
/*int main()
{
  int n = 4;
  vector< double > x, y, z;
  vector< vector < double > > A, B, Ainv;
  ifstream fin( "test.txt" );

  if( !fin )
    {
      cout << " Could not open the file for reading " << endl;
      return EXIT_SUCCESS;
    }

  allocateMatrix( A, n, n );
  allocateMatrix( B, n, n );
  allocateVector( x, n );
  allocateVector( y, n );
  
  for( int i = 0; i < n; i++ )
    for( int j = 0; j < n; j++ )
      fin >> A[i][j];
  
  for( int i = 0; i < n; i++ )
    for( int j = 0; j < n; j++ )
      fin >> B[i][j];
  
  for( int i = 0; i < n; i++ )
    fin >> x[i];
  
  for( int i = 0; i < n; i++ )
    fin >> y[i];
  
  fin.close();
  
  //cout << "x + 3.14 * x + y /1.4141 - 2.8 * A * x - y + (y' * A)'= " << endl;
  //write( x + 3.141 * x + y / 1.4141 - 2.8 * A * x - y + y * A );
  
  //cout << "A + 3.14 * B + A /1.4141 - A * B + 65.0  * x * y' - B = " << endl;
  //write( A + 3.14 * B + A /1.4141 - A * B + 65.0  * x * y ) - B );

  //cout << "inv(A) = " << endl;
  //invertMatrix( A, Ainv );
  //write( Ainv );

  
  double temp = ( 1 + innerProduct( y, B * y ) / ys ) / ys;
      
  t1 = ( outerProduct(s, y) * B)/ys + ( m_M * outerProduct(y , s) ) / ys;
  t2 =  temp * outerProduct( s, s );
  B  =  B - t1 + t2;
  
  
  return EXIT_SUCCESS;
}
*/

} // namespace Numeric
} // namespace hippodraw
