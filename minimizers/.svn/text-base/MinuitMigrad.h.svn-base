/* -*- mode: c++ -*- */

/** @file

hippodraw::MinuitMigrad class interface

Copyright (C) 2003-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: MinuitMigrad.h,v 1.22 2007/07/02 18:17:15 pfkeb Exp $

*/

// for have minuit
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef _MinuitMigrad_H_
#define _MinuitMigrad_H_

#include "Fitter.h"

#ifdef HAVE_MINUIT
class MnMigrad;
#endif

#ifdef HAVE_MINUIT2
namespace ROOT {
  namespace Minuit2 {
    class MnMigrad;
  }
}
#endif

namespace hippodraw {

/** A Fitter class.  A Fitter class that uses the Simple version of
    Minuit for fitting.  

    The implementation here follows the example given in
    seal/Mathlibs/Minuit/tests/MnSim/SimpleGaussSim.cpp.  However, one
    change is to not to use explicitly the MnUserParameter, but give
    the the initial vector of parameters and errors directly.

    @bug @@ when fixxing a parameter, and doing a fit the tixed parameter
    doesn't show up as fixed in GUI, because the selected parameter
    changes.

    @request Add support for using Minos to get asymmetric errors.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API MinuitMigrad : public Fitter
{

private:

  /** Private copy constructor to prevent user copying. 
   */
  MinuitMigrad ( const MinuitMigrad & );

  /** Structure to hold limit values, if any.  
      
      @note This duplicates info stored in the Minuit minimizer, but
      it looks comples to dig it out, so we duplicate it.
   */
  struct limit {
    double lower; ///< Lower limit
    double upper; ///< Upper limit
    bool set; ///< @c true if limit has been set
  };

  /** The limit parameters.
   */
  std::vector < limit > m_limits;

  /** Initializes the limits.
   */
  void initLimits ();

  /** The Minuit Migrad minimizer.  

      @note Needs to be held by pointer because it does not have
      default constructor.
   */
#ifdef HAVE_MINUIT
  MnMigrad * m_minimizer;
#endif
#ifdef HAVE_MINUIT2
  ROOT::Minuit2::MnMigrad * m_minimizer;
#endif

  /** Initializes the Minuit Migrad minimizer.
   */
  void initialize ();

  /** Checks the @a index and throws exception if out of range.
   */
  void checkIndex ( unsigned int index );

public:

  /** The constructor. */
  MinuitMigrad ( const char * name );

  virtual Fitter * clone () const;

  /** Sets the upper and lower limits of the parameter.
   */
  virtual void setLimits ( unsigned int i, double lower, double upper );

  /** Sets the step size used by MnMigrad.
   */
  virtual void setStepSize ( unsigned int i, double size );

  /** Calculates the best fit.  Returns @c true if the fit converged;
      otherwise it returns @c false. */
  virtual bool calcBestFit ();

  /** Calculates the covariance matrix */
  virtual int calcCovariance ( std::vector< std::vector < double > >& cov );

};

} // namespace hippodraw

#endif // _MinuitMigrad_H_
