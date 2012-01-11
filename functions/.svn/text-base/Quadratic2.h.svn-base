/* -*- mode: c++ -*- */

/** @file

Quadratic2 class interface

Copyright (C) 2000, 2003   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Quadratic2.h,v 1.3 2005/10/30 00:10:12 pfkeb Exp $

*/

#ifndef _Quadratic2_H_
#define _Quadratic2_H_

#include "FunctionBase.h"

namespace hippodraw {

/** This function represents a quadratic, i.e., 
    @f$ f(x) = y_{\rm scale} (x - x_0)^2 + y_0@f$.

    @author J. Chiang <jchiang@slac.stanford.edu>

*/
class MDL_HIPPOPLOT_API Quadratic2 : public FunctionBase
{

protected:

  /** Initializes the function and parameters names. */
  virtual void initialize ();

public:

  /** The default constructor. */
  Quadratic2 ();

  /** The following constructor takes the values of the parameters as
      an argument. */
  Quadratic2 ( double yscale, double y0, double x0 );

  virtual FunctionBase * clone () const;

  /** The function call operator.  */
  virtual double operator () ( double x ) const;

  virtual void initialParameters ( const FunctionHelper * helper );

protected:

  virtual double derivByParm ( int i, double x ) const;

};

} // namespace hippodraw

#endif // _Quadratic2_H_
