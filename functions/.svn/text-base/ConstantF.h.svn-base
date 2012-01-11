/* -*- mode: c++ -*-
 *
 * HippoPlot ConstantF  class interface
 *
 * Copyright (C) 2000, 2003   The Board of Trustees of The Leland
 * Stanford Junior University.  All Rights Reserved.
 *
 * $Id: ConstantF.h,v 1.32 2005/10/30 00:10:11 pfkeb Exp $
 *
 */

#ifndef _ConstantF_H_
#define _ConstantF_H_

#include "FunctionBase.h"

namespace hippodraw {

/** A ConstantF function can be used with a fitter.
    The function is a constant, i.e., computes @f$ f(x) = p_0 @f$.

    This function has `F' in its name, unlike the others,
    because it fixed some strange problem in trying to call its
    clone() member function from the FunctionFactory. 

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API ConstantF : public FunctionBase
{
protected:

  /** Initializes the function and parameters names. */
  virtual void initialize ();

 public:

  /** The default constructor. */
  ConstantF ();

  /** The following constructor takes the values of the parameters as
      an argument. */
  explicit ConstantF ( double value );

  virtual FunctionBase * clone () const;

  /** The function call operator.  */
  virtual double operator () ( double x ) const;

  virtual void initialParameters ( const FunctionHelper * helper );

protected:

  virtual double derivByParm ( int i, double x ) const;

};

} // namespace hippodraw

#endif // _ConstantF_H_
