/* -*- mode:c++ -*- */

/** @file 

Declaration of GammaQ functions

$Id: Gammaq.h,v 1.5 2006/01/25 00:18:53 pfkeb Exp $

*/

#ifndef GAMMAQ_CXX_
#define GAMMAQ_CXX_

#define ITMAX 100
#define EPS 3.0e-7
#define FPMIN 1.0e-30
#define MAXSTR 80

#include <cstdio>
#include <cstdlib>

namespace hippodraw {
  namespace Numeric {

/** Calculates Q, the probability that the chi2 should exceed a
    particilar chi2 (input)  by chance. It internally calls
    gser and gcf depending on the input range.*/
double gammq(double a, double x);

/** Calculates natural log of a gamma function */ 
double gammln( double xx );

/** Calculates Q, the probability that the chi2 should exceed a
    particilar chi2 (input)  by chance, by using series approximation*/
void gser(double *gamser, double a, double x, double *gln);

/** Calculates Q, the probability that the chi2 should exceed a
    particilar chi2 (input)  by chance, by using continued fractions */
void gcf( double *gammcf, double a, double x, double *gln );

  } // end namespace Numeric
} // end namespace hippodraw

#endif
