/* -*- mode: c++ -*- */

/** @file

Landau function class interface

Copyright (C) 2003 The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: Landau.h,v 1.15 2007/09/30 15:57:09 pfkeb Exp $
*/

#ifndef _Landau_H_
#define _Landau_H_

#include "FunctionBase.h"

#include <cmath>

namespace hippodraw {

/** The function represents the Landau distribution

This class represents a Landau distribution, as approximated by the
Moyal formula

@f[ Moyal(\lambda) 
= \frac{\exp{-0.5(\lambda+\exp{-\lambda})}}{\sqrt{2\pi}}  @f]

See J.E. Moyal, Theory of ionization fluctuations, Phil. Mag. 46 (1955) 263.

Note that this analytical approximation is too low in the tail.


In order to allow for a fit, we define @f[ \lambda = \frac{x - m}{s}
@f] with x the dataset variable. @f$m@f$ and @f$s@f$ are fit
parameters PAR(1) and PAR(2) respectively, and correspond to the mode
and the width of the distribution. PAR(3) is a normalization factor,
so that the concrete implemented formula is: @f$ Landau(\lambda(x)) =
PAR(3) * Moyal(\lambda(x)) @f$.


@author From Goddard GLAST ACD team (Fortran version)
@author Paul F. Kunz <Paul_Kunz@slac.stanford.edu> (C++ conversion)
@author Johann Cohen-Tanugi <johann.cohen@pi.infn.it> (fixed normalization)
*/
class MDL_HIPPOPLOT_API Landau : public FunctionBase
{
private:

  /** Indices to the parameter vectors. */
  enum { peak, /**< Index to the peak parameter */
	 norm, /**< Index to the normalization parameter */
	 sigma /**< Index to the sigma parameter */
  };

protected:

  /** Initializes the function and parameter names. */
  virtual void initialize ();

public:

  /** The default constructor. */
  Landau ();

  /** This constructor takes the values of the parameters as an argument. */
  Landau ( double peak, double counts, double sigma );

  virtual FunctionBase * clone () const;

  /** The function call operator.  */
  virtual double operator () ( double x ) const;

  virtual void initialParameters ( const FunctionHelper * helper );

  /** Returns @c false since the derivatives have not been implemented. */
  virtual bool hasDerivatives () const;

protected:

  //  virtual double derivByParm ( int i, double x ) const;

private:

  ///compute the reduced variable: @f$Y = \frac{x - mode}{sigma}@f$
  inline double calcY ( double x ) const 
  {
    return ( x - m_parms[peak] ) / m_parms[sigma];
  }

  /** Compute the helper variable for derivation: @f$Z = -0.5\times (
      1.0 - \exp{-y} ) @f$
  */
  inline double calcZ ( double x ) const
  {
    double y = calcY ( x );

	return -0.5 * ( 1.0 - std::exp ( -y ) );
  }

  /** Calculates the derivative with respect to the counts
      parameter. */
  double derivByNorm ( double x ) const;

  /** Calculates the derivative with respect to the peak
      parameter. */
  double derivByPeak ( double x ) const;

  /** Calculates the derivative with respect to the sigma
      parameter. */
  double derivBySigma ( double x ) const;

};

} // namespace hippodraw

#endif // _Landau_H_
