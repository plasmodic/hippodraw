/* -*- mode: c++ -*- */

/** @file

Quadratic class interface

Copyright (C) 2000, 2003   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Quadratic.h,v 1.31 2005/10/30 00:10:12 pfkeb Exp $

*/

#ifndef _Quadratic_H_
#define _Quadratic_H_

#include "FunctionBase.h"

namespace hippodraw {

/** A function that can be used with a fitter.
    This function represents a quadratic, i.e., 
    @f$ f(x) = p_0 + p_1 x + p_2 x^2 @f$.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

*/
class MDL_HIPPOPLOT_API Quadratic : public FunctionBase
{

protected:

  /** Initializes the function and parameters names. */
  virtual void initialize ();

public:

  /** The default constructor. */
  Quadratic ();

  /** The following constructor takes the values of the parameters as
      an argument. */
  Quadratic ( double intercept, double linear, double quad );

  virtual FunctionBase * clone () const;

  /** The function call operator.  */
  virtual double operator () ( double x ) const;

  virtual void initialParameters ( const FunctionHelper * helper );

protected:

  virtual double derivByParm ( int i, double x ) const;

};

} // namespace hippodraw

#endif // _Quadratic_H_
