/* -*- mode: c++ -*- */

/** @file

hippodraw::NTupleChiSqFCN class interface

Copyright (C) 2003-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: NTupleChiSqFCN.h,v 1.13 2006/01/08 19:21:48 pfkeb Exp $

*/

#ifndef _NTupleChiSqFCN_H_
#define _NTupleChiSqFCN_H_

#include "NTupleFCN.h"

namespace hippodraw {

/** A Chi-squared functor class.  A Chi-squared objective function
    functor class compatible with C++ version of @em Minuit.  This
    class gets its data points from a DataSource provided by a
    ProjectorBase object.  It uses functions derived from
    FunctionBase.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API NTupleChiSqFCN : public NTupleFCN
{

private:

  /** The copy constructor. */
  NTupleChiSqFCN ( const NTupleChiSqFCN & );

public:

  /** The default constructor.
   */
  NTupleChiSqFCN ();

  /** Makes a copy of the object. */
  virtual StatedFCN * clone () const;

#ifdef HAVE_MINUIT2
  virtual double Up () const;
#else
  virtual double up () const;
#endif
  /** Calculates and returns the Chi-Squared.  This objective function
      object compares the function with the data points and returns
      the Chi-Squared.  If any data points has error measurement, then
      data points with zero error measurement are ignored.  If none of
      the data points have error measurement, then an error of 1.0 is
      used.
   */
  virtual double objectiveValue () const;
  virtual bool needsIntegrated () const;

};

} // namespace hippodraw

#endif // _NTupleChiSqFCN_H_
