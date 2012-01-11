/* -*- mode: c++ -*- */

/** @file

Gaussian  class interface

Copyright (C) 2000, 2003, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Gaussian.h,v 1.43 2007/08/24 22:19:16 pfkeb Exp $

*/

#ifndef _Gaussian_H_
#define _Gaussian_H_

#include "FunctionBase.h"

namespace hippodraw {

/** A function that can be used with a fitter. 
    The function represents a gaussian,

    @f[ \frac{N}{\sigma \sqrt {2\pi}}\exp(-\frac{(x-\mu)^{2}}{2\sigma
^{2}}) @f].


    @author Daniel Engovatov <engovatov@alumni.stanford.org>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API Gaussian : public FunctionBase
{

 private:

  /** Index of the parameters. 
   */
  enum { norm = 0, mean = 1, sigma = 2 };

protected:

  /** Initializes the function and parameter names. */
  virtual void initialize ();

public:

  /** The default constructor. */
  Gaussian ();

  /** This constructor takes the values of the parameters as an argument. */
  Gaussian ( double norm, double mean, double sigma );

  virtual FunctionBase * clone () const;

  /** The function call operator.  */
  virtual double operator () ( double x ) const;

  virtual void initialParameters ( const FunctionHelper * helper );

protected:

  virtual double derivByParm ( int i, double x ) const;

private:

  /** Calculates the derivative with respect to the normalization
      parameter. */
  double derivByNorm ( double x ) const;

  /** Calculates the derivative with respect to the mean
      parameter. */
  double derivByMean ( double x ) const;

  /** Calculates the derivative with respect to the sigma
      parameter. */
  double derivBySigma ( double x ) const;

};

} // namespace hippodraw

#endif // _Gaussian_H_
