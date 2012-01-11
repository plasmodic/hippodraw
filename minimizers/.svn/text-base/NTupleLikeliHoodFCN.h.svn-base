/* -*- mode: c++ -*- */

/** @file

hippodraw::NTupleLikeliHoodFCN class interface

Copyright (C) 2003-2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: NTupleLikeliHoodFCN.h,v 1.16 2007/07/02 18:17:15 pfkeb Exp $

*/
// for have minuit
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef _NTupleLikeliHoodFCN_H_
#define _NTupleLikeliHoodFCN_H_

#include "NTupleFCN.h"

namespace hippodraw {

/** A Likelihood functor class.  A Likelihood objective function
    functor class compatible with C++ version of @em Minuit.  This
    class gets its data points from a DataSource provided by a
    ProjectorBase object.  It uses functions derived from
    FunctionBase.

    @author Kaustuv <kaustuv@stanford.edu>
*/

class MDL_HIPPOPLOT_API NTupleLikeliHoodFCN : public NTupleFCN
{
private:

  /** The copy constructor. */
  NTupleLikeliHoodFCN ( const NTupleLikeliHoodFCN & );

public:

  /** The default constructor. */
  NTupleLikeliHoodFCN ();

  /** Makes a copy of the object. */
  virtual StatedFCN * clone () const;

  /** Calculates and returns the Likelihood function for the 1D binned
      data. This does not work with unbinned data. or 2D binned data. 
  */
  virtual double objectiveValue () const;

#ifdef HAVE_MINUIT2
  virtual double Up () const
#else
  virtual double up () const
#endif
  {
    return 1.0;
  }

  virtual bool needsIntegrated () const;

};

} // namespace hippodraw

#endif // _NTupleLikeliHoodFCN_H_
