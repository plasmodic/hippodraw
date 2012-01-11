/* -*- mode: c++ -*- */

/** @file

Complementary Error function class interface (hippodraw::Erfc)


$Id: Erfc.h,v 1.8 2006/06/28 00:24:48 pfkeb Exp $
*/

#ifndef _Erfc_H_
#define _Erfc_H_

#include "FunctionBase.h"

namespace hippodraw {

/** A function that can be used with a fitter. 
    The function represents the Complementary Error Function distribution Erfc
    
    @f[ Erfc(x) = \frac{2}{\sqrt(\pi)} \int^{+\inf}_x \exp(-t^2)dt  @f]
    
    The algorithm is based on a Chebyshev fit as denoted in
    Numerical Recipes 2nd ed. on p. 214 (W.H.Press et al.).
    
    The fractional error is always less than 1.2e-7.
    
    
    This code also allows to fit a data set with the Erfc distribution:
    we define @f[ t = \frac{x - m}{s} @f], with x the data set
    variable.  m and s (mean and sigma) are fit parameters PAR(1) and
    PAR(2) respectively.  In this way the Erfc becomes related to the
    cumulative distribution function of a Gaussian.  As a result, the
    concrete implemented formula is @f[ Erfc(x) = PAR(3) *
    \int^{+\inf}_{t} \exp(-u^2)du @f].

    In other words, this implementation evaluates Erfc at the reduced
    value @f$t@f$ so that a fit is possible.  PAR(3) is a
    normalization parameter.
    
    @author Johann Cohen-Tanugi  <johann.cohen@pi.infn.it>
*/

class MDL_HIPPOPLOT_API Erfc : public FunctionBase
{
 private:
  enum{MEAN,SIGMA,NORM};

 protected:
  /** Initializes the function and parameter names. */
  virtual void initialize();

 public:
  /** The default constructor. */
  Erfc ();
  
  /** This constructor takes the values of the parameters as an argument. */
  Erfc ( double mean, double sigma, double norm );
  
  virtual FunctionBase * clone () const;
  
  /** The function call operator.  */
  virtual double operator () ( double x ) const;
  
  virtual void initialParameters ( const FunctionHelper * helper );
  
  //Help functions:
  double calcErfc(double x) const;

 protected:
  virtual double derivByParm ( int i, double x ) const;

  double derivByRed ( double value) const;

private:
  ///compute the reduced variable: @f$x = \frac{value-mean}{sigma}@f$
  inline double calcRed(double value) const 
  {
    if(m_parms[SIGMA] == 0.0) {
      Erfc * p = const_cast < Erfc * > ( this );
      p->m_parms[SIGMA] = 0.0001; //dummy lower value
    }

    return (value - m_parms[MEAN])/m_parms[SIGMA];
  }



};

} // namespace hippodraw

#endif// _Erfc_H_
