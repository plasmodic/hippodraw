/* -*- mode: c++ -*- */

/** @file

chi-squared distribution class interface

Copyright (C) 2000, 2003   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Chi2Dist.h,v 1.7 2005/10/30 00:10:11 pfkeb Exp $
*/

#ifndef _Chi2Dist_H_
#define _Chi2Dist_H_

#include "FunctionBase.h"

namespace hippodraw {

/** A function can be used with a fitter. 
    This function computes the chi-squared distribution for 
    the number of degrees-of-freedom @f$\nu = p_1 @f$

    @f[ f(x) = p_0 exp( -x/2/p_2 ) ( x/2/p_2 )^{p_1/2 - 1} @f].

    @author J. Chiang <jchiang@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API Chi2Dist : public FunctionBase
{

protected:

  /** Initializes the function and parameters names. */
  virtual void initialize ();

public:

  /** The default constructor. */
  Chi2Dist ();

  /** The constructor takes the values of parameters as arguments. */
  Chi2Dist ( double norm, double nu, double scale );

  virtual FunctionBase * clone () const;

  /** The function call operator.  */
  virtual double operator () ( double x ) const;

  virtual void initialParameters ( const FunctionHelper * helper );

protected:

  virtual double derivByParm ( int i, double x ) const;

};

} // namespace hippodraw

#endif // _Chi2Dist_H_
