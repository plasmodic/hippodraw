/* -*- mode: c++ -*- */

/** @file

Weibull class interface

Copyright (C) 2000, 2003   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Weibull.h,v 1.1 2007/11/29 22:58:57 pfkeb Exp $
*/

#ifndef _Weibull_H_
#define _Weibull_H_

#include "FunctionBase.h"

namespace hippodraw {

/** A function can be used with a fitter. 
    This function computes <a href=http://en.wikipedia.org/wiki/Weibull_distribution > Weibull distribution </a>.

    @author Nathan Whitehorn <nwhitehorn@icecube.wisc.edu>
*/
class MDL_HIPPOPLOT_API Weibull : public FunctionBase
{

protected:

  /** Initializes the function and parameters names. */
  virtual void initialize ();

public:

  /** The default constructor. */
  Weibull ();

  /** The constructor takes the values of parameters as arguments. */
  Weibull ( double prefactor, double scale, double shape );

  virtual FunctionBase * clone () const;

  /** The function call operator.  */
  virtual double operator () ( double x ) const;

  virtual void initialParameters ( const FunctionHelper * helper );

protected:

  virtual double derivByParm ( int i, double x ) const;

};

} // namespace hippodraw

#endif // _Weibull_H_

