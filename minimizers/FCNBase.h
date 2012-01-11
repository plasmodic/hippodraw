/* -*- mode: c++ -*- */

/** @file

hippodraw::FCNBase class interface

@note This file is only used if HAVE_MINUIT and HAVE_MINUIT2 are not defined.

Copyright (C) 2003, 2006   The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: FCNBase.h,v 1.5 2006/01/08 19:21:48 pfkeb Exp $

*/

#ifndef _FCNBase_H_
#define _FCNBase_H_

#include "pattern/libhippo.h"

#include <vector>

namespace hippodraw {

/** The base class for objective functions of a minimizer. 

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API FCNBase 
{

 public:

  /** The virtual destructor. */
  virtual ~FCNBase() {}

  /** Calculates and returns the objective function to be minimized.
      This objective function could be a Chi-Squared function or
      could be a Likelihood function.  
  */
  virtual 
  double operator () ( const std::vector < double > & parms ) const = 0;

  virtual double errorDef () const {
    return up ();
  }

  virtual double up () const = 0;

};

} // namespace

#endif // _FCNBase_H_
