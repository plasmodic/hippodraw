/* -*- mode:c++ -*- */

/** @file

Collection of linear algebra functions.

Copyright (C) 2003, 2004  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: NumLinAlg.h,v 1.12 2005/06/13 23:34:37 pfkeb Exp $

@author Kaustuv <kaustuv@stanford.edu>

*/

#ifndef NUMLINALG_H__
#define NUMLINALG_H__

#include <iosfwd>
#include <vector>

namespace hippodraw {

  /** Collection of linear algebra functions. 
   */
  namespace Numeric {

/** Given two vectors x and y this function performs operation  z = x + y.
 */
std::vector< double >
operator + ( const std::vector< double >& x, const std::vector< double >& y  );

/** Given two vectors x and y this function performs operation  z = x - y.
 */
std::vector< double >
operator - ( const std::vector< double >& x, const std::vector< double >& y  );

/** Given a vector y this function performs operation  z = - y.
 */
std::vector< double >
operator - ( const std::vector< double >& y  );

/** Given two matrices A and B this function performs operation  C = A + B.
 */
std::vector< std::vector< double > >
operator + ( const std::vector< std::vector< double > >&A,
	     const std::vector< std::vector< double > >&B );

/** Given two matrices A and B this function performs operation  C = A - B.
 */
std::vector< std::vector< double > >
operator - ( const std::vector< std::vector< double > >&A,
	     const std::vector< std::vector< double > >&B );

/** Given a scalar a and a vector x this  function performs operation y = ax.
 */
std::vector< double >
operator * (  double a, const std::vector< double >& x );

/** Given a scalar and a vector x this  function performs operation y = x/a.
 */
std::vector< double >
operator / ( const std::vector< double >& x, double a );

/** Computes the scalar matrix product B = aA */
std::vector< std::vector< double > >
operator * ( double a, const std::vector< std::vector< double > > & A ) ;

/** Computes the scalar matrix product B = A/a */
std::vector< std::vector< double> >
operator / ( const std::vector< std::vector< double> > & A, double a );

/** Computes the Matrix vector product y = Ax */
std::vector< double >
operator * ( const std::vector< std::vector< double > > & A,
	     const std::vector< double > & x );

/** Computes the vector matrix product y = x'A */
std::vector< double >
operator * ( const std::vector< double > & x,
	     const std::vector< std::vector< double > > & A );

/** Computes Matrix Matrix Product C=A*B */
std::vector< std::vector< double > >
operator * ( const std::vector< std::vector< double > >&A,
	     const std::vector< std::vector< double > >&B );

/** Computes the dot or the inner product of two vectors(i.e. a'b) */
double innerProduct( const std::vector< double > & a,
		     const std::vector< double > & b );

/** Computes the outer product of two vectors (i.e. C = ab') */
std::vector< std::vector< double > >
outerProduct ( const std::vector< double > & a,
	       const std::vector< double > & b );

/** Calculates the vector-matrix-vector product x'*A*x */
double quadraticProduct( const std::vector< std::vector< double >  > & A,
			 const std::vector< double > x );

/** Computes the two norm of the  vector */
double norm(  const std::vector< double > & a );

/** The subroutine which does cholesky factorization of a given Symmetric
    positive definite matrix (say) A. It over-writes A by resulting lower
    triangular matrix */
int cholFactor( std::vector< std::vector< double > > & A );

/** Solves the equation LL'x = b where L is lower triangular matrix */
int cholBackSolve( const std::vector< std::vector< double >  > &  L,
		   std::vector< double > & x,
		   const std::vector< double > & b );

/** Inverts a SPD matrix a to get inverse Ainv using the cholesky
    factorization. It inverts the matrix by repeatedly solving Ax = ei */
int invertMatrix( const std::vector< std::vector< double >  > &  A,
		  std::vector< std::vector< double >  > &  Ainv );

/** Creates an n x n identity matrix for M */
int eye( std::vector< std::vector< double > > & M, int n );

/** Given the vector it writes it to std stream */
int write( const std::vector< double >  & x );

/** Given the matrix it writes it to std stream */
int write( const std::vector< std::vector< double >  > & A );

/** Allocates a matrix of size m x n */
int allocateMatrix( std::vector< std::vector< double > >& A, int m, int n );

/** Allocates a vector of size n */
int allocateVector( std::vector< double >& x, int n );

  } // namespace Numeric
} // namespace hippodraw

#endif
