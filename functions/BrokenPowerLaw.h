/* -*- mode: c++ -*- */

/** @file

BrokenPowerLaw class interface

Copyright (C) 2000, 2003   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BrokenPowerLaw.h,v 1.3 2005/10/30 00:10:11 pfkeb Exp $
*/

#ifndef _BrokenPowerLaw_H_
#define _BrokenPowerLaw_H_

#include "FunctionBase.h"

namespace hippodraw {

/** A function can be used with a fitter. 
    This function computes 
    @f[ f(x) = p_0 (x/p_3)^{p_1} @f] for @f[ x < p_3 @f]
    @f[ f(x) = p_0 (x/p_3)^{p_2} @f] for @f[ x \ge p_3 @f]

    @author J. Chiang <jchiang@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API BrokenPowerLaw : public FunctionBase
{

protected:

  /** Initializes the function and parameters names. */
  virtual void initialize ();

public:

  /** The default constructor. */
  BrokenPowerLaw ();

  /** The constructor takes the values of parameters as arguments. */
  BrokenPowerLaw ( double prefactor, double index1, 
                    double index2, double x_break );

  virtual FunctionBase * clone () const;

  /** The function call operator.  */
  virtual double operator () ( double x ) const;

  virtual void initialParameters ( const FunctionHelper * helper );

protected:

  virtual double derivByParm ( int i, double x ) const;

};

} // namespace hippodraw

#endif // _BrokenPowerLaw_H_
