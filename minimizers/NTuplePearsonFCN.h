/* -*- mode: c++ -*- */

/** @file

hippodraw::NTuplePearsonFCN class interface

Copyright (C) 2003-2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: NTuplePearsonFCN.h,v 1.7 2007/07/02 18:17:15 pfkeb Exp $

*/

// for have minuit
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef _NTuplePearsonFCN_H_
#define _NTuplePearsonFCN_H_

#include "NTupleFCN.h"

namespace hippodraw {

/** A Pearson's Chi-squared functor class.  A Pearson's Chi-squared
    objective function functor class compatible with C++ version of
    @em Minuit.  This class gets its data points from a DataSource
    provided by a ProjectorBase object.  It uses functions derived
    from FunctionBase.

    @author kaustuv <kaustuv@stanford.edu>
*/

class MDL_HIPPOPLOT_API NTuplePearsonFCN : public NTupleFCN
{

private:

  /** The copy constructor. */
  NTuplePearsonFCN ( const NTuplePearsonFCN & );

public:

  /** The default constructor.
   */
  NTuplePearsonFCN ();

  /** Makes a copy of the object. */
  virtual StatedFCN * clone () const;

#ifdef HAVE_MINUIT2
  virtual double Up () const;
#else
  virtual double up () const;
#endif
  /** Calculates and returns the Pearson's Chi-Squared. This objective function
      object compares the function with the data points and returns
      the Pearson's Chi-Squared. 
   */
  virtual double objectiveValue () const;
  virtual bool needsIntegrated () const;

};

} // namespace hippodraw

#endif // _NTuplePearsonFCN_H_
