/* -*- mode: c++ -*- */

/** @file

Linear function class interface

Copyright (C) 2000, 2003   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Linear.h,v 1.34 2005/10/30 00:10:12 pfkeb Exp $
*/

#ifndef _Linear_H_
#define _Linear_H_

#include "FunctionBase.h"

namespace hippodraw {

/** A function can be used with a fitter. 
    This function computes @f$ f(x) = p_0 + p_1 x @f$.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API Linear : public FunctionBase
{

protected:

  /** Initializes the function and parameters names. */
  virtual void initialize ();

public:

  /** The default constructor. */
  Linear ();

  /** The constructor takes the values of parameters as arguments. */
  Linear ( double slope, double intercept );

  virtual FunctionBase * clone () const;

  /** The function call operator.  */
  virtual double operator () ( double x ) const;

  virtual void initialParameters ( const FunctionHelper * helper );

protected:

  virtual double derivByParm ( int i, double x ) const;

};

} // namespace hippodraw

#endif // _Linear_H_
