/* -*- mode: c++ -*- */

/** @file

PowerLaw class interface

Copyright (C) 2000, 2003   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: PowerLaw.h,v 1.5 2005/10/30 00:10:12 pfkeb Exp $
*/

#ifndef _PowerLaw_H_
#define _PowerLaw_H_

#include "FunctionBase.h"

namespace hippodraw {

/** A function can be used with a fitter. 
    This function computes @f[ f(x) = p_0 x^{p_1} @f].

    @author J. Chiang <jchiang@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API PowerLaw : public FunctionBase
{

protected:

  /** Initializes the function and parameters names. */
  virtual void initialize ();

public:

  /** The default constructor. */
  PowerLaw ();

  /** The constructor takes the values of parameters as arguments. */
  PowerLaw ( double prefactor, double index );

  virtual FunctionBase * clone () const;

  /** The function call operator.  */
  virtual double operator () ( double x ) const;

  virtual void initialParameters ( const FunctionHelper * helper );

protected:

  virtual double derivByParm ( int i, double x ) const;

};

} // namespace hippodraw

#endif // _PowerLaw_H_
