/* -*- mode: c++ -*- */

/** @file

hippodraw::Novosibirsk  class interface

Copyright (C) 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Novosibirsk.h,v 1.6 2007/08/24 22:19:16 pfkeb Exp $

*/

#ifndef _Novosibirsk_H_
#define _Novosibirsk_H_

#include "FunctionBase.h"

namespace hippodraw {

/** A function that can be used with a fitter. 
    The function represents the Novosibirsk distribution.


    @author Martin Kocian <kocian@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API Novosibirsk : public FunctionBase
{

 private:

  enum { norm = 0, mean = 1, sigma = 2, //< FWHM/2.36
	 tail = 3 };

 protected:

  /** Initializes the function and parameter names. */
  virtual void initialize ();

  virtual double derivByParm ( int i, double x ) const;

 public:

  /** The default constructor. */
  Novosibirsk ();

  /** This constructor takes the values of the parameters as an
      argument.
  */
  Novosibirsk ( double norm, double mean, double sigma, double tail );

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

#endif // _Novosibirsk_H_
