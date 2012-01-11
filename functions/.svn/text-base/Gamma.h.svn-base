/* -*- mode: c++ -*- */

/** @file

Gamma class interface

Copyright (C) 2000, 2003   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Gamma.h,v 1.1 2007/11/29 22:58:57 pfkeb Exp $
*/

#ifndef _Gamma_H_
#define _Gamma_H

#include "FunctionBase.h"

namespace hippodraw {

/** A function can be used with a fitter. 
    This function computes <a href=http://en.wikipedia.org/wiki/Gamma_distribution> Gamma distribution </a>.

    @author Nathan Whitehorn <nwhitehorn@icecube.wisc.edu>
*/
class MDL_HIPPOPLOT_API Gamma : public FunctionBase
{

protected:

  /** Initializes the function and parameters names. */
  virtual void initialize ();

public:

  /** The default constructor. */
  Gamma ();

  /** The constructor takes the values of parameters as arguments. */
  Gamma ( double prefactor, double scale, double shape );

  virtual FunctionBase * clone () const;

  /** The function call operator.  */
  virtual double operator () ( double x ) const;

  virtual void initialParameters ( const FunctionHelper * helper );
  virtual bool hasDerivatives () const; /* false */

protected:

  virtual double derivByParm ( int i, double x ) const;

};

} // namespace hippodraw

#endif // _Gamma_h

