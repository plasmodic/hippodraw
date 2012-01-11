/* -*- mode: c++ -*- */

/** @file

hippodraw::LogNormal  class interface

Copyright (C) 2004, 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: LogNormal.h,v 1.6 2007/08/24 22:19:16 pfkeb Exp $

*/

#ifndef _LogNormal_H_
#define _LogNormal_H_

#include "FunctionBase.h"

namespace hippodraw {

/** A function that can be used with a fitter. 
    The function represents the Log Normal distribution.


    @author Martin Kocian <kocian@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API LogNormal : public FunctionBase
{
 private:

  /** Index of the parameters. 
   */
  enum { norm = 0, mu = 1, sigma = 2 };

 protected:

  /** Initializes the function and parameter names. */
  virtual void initialize ();

  virtual double derivByParm ( int i, double x ) const;

public:

  /** The default constructor. */
  LogNormal ();

  /** This constructor takes the values of the parameters as an
      argument.
  */
  LogNormal ( double norm, double mu, double sigma );

  virtual FunctionBase * clone () const;

  /** The function call operator.  */
  virtual double operator () ( double x ) const;

  virtual void initialParameters ( const FunctionHelper * helper );

  /** Returns @c false as this function has not implemented
      calculation of partial derivatives.
      @sa FunctionBase::hasDerivatives.
   */
  virtual bool hasDerivatives () const;

};

} // namespace hippodraw

#endif // _LogNormal_H_
