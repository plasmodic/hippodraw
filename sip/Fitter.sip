/* -*- mode:c++ -*- */

/** @file

SIP interface definition for Fitter

Copyright (C)  2005  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

$Id: Fitter.sip,v 1.4 2007/02/24 22:24:38 pfkeb Exp $

Author: Paul_Kunz@slac.stanford.edu

*/

class Fitter
{
%TypeHeaderCode
#include "minimizers/Fitter.h"
using hippodraw::Fitter;
%End

protected:

  Fitter ( const Fitter & );
  
public:

  virtual ~Fitter();

  /** Copy the receiving object.
   */
  virtual Fitter * clone () const = 0;

  /** Returns the name of the fitter.
   */
  const std::string & name () const;

  /** Sets the parameters that are to be held fixed during objective
      function minimization.
   */
  virtual void setFixedFlags ( const std::vector < int > & );

  /** Returns a vector containing flags for which parameters are to be
      held fixed during objective function minimization.
  */
  virtual const std::vector < int > & getFixedFlags () const;

  /** Sets the limits for the parameter of the model function indexed
      by i.
  */
  virtual void setLimits ( unsigned int, double, double );

  /** Sets the limits of the model function parameter values.

      Attempts to set the limits of the model function parameter
      values.  This base class implementation throws a
      FitterException.  Derived classes that support setting limits of
      the parameter values should override this member function.
   */
  void setLimits ( const std::string & /* name */,
		   double /* lower */, 
		   double /* upper */ );


  /** Calculates the best fit.  Returns @c true if the fit converged;
      otherwise it returns @c false.
  */
  virtual bool calcBestFit () = 0;

//   /** Calculates the covariance matrix. Returns EXIT_SUCCESS if a
//       minima of the objective functions is found, returns EXIT_FAILURE
//       in case algorithm converges on other stationary points (i.e. on
//       saddle points). 
//   */
//    virtual 
//   int calcCovariance ( std::vector < std::vector < double > > & ) = 0;


};
