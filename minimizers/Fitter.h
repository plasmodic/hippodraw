/* -*- mode: c++ -*- */

/** @file

hippodraw::Fitter class interface

Copyright (C) 2003-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Fitter.h,v 1.40 2006/04/12 19:16:01 pfkeb Exp $

*/

#ifndef _Fitter_H_
#define _Fitter_H_

#include "pattern/libhippo.h"

#include <string>
#include <vector>
#include <limits.h>
#include <stdlib.h>

namespace hippodraw {

  class DataSource;
  class FunctionBase;
  class StatedFCN;
  class TupleCut;

/** The base class for fitters.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API Fitter
{

private:

  /** Returns the index to the model function parameters @a name.  If
      @a name is not found, throws a std::runtime_error.
  */
  unsigned int getParameterIndex ( const std::string & name );

protected:

  /** The copy constructor. Makes a copy of the data members and a
      clone of the StatedFCN.
   */
  Fitter ( const Fitter & );

  /** The name of the fitter. */
  std::string m_name;

  /** The required constructor. 
   */
  Fitter ( const char * name );

  /** The objective function. 
   */
  StatedFCN * m_fcn;

  /** The maximum number of iterations allowed in attempting the fit. */
  int m_max_iterations;

 public:

  /** The virtual destructor. */
  virtual ~Fitter();

  /** Makes a copy of the receiving object.  Makes copy of receiving
      object by creating a new one with the only constructor.
      @attention Objects created with this function may not be
      complete until the setFCN member function is called.
  */
  virtual Fitter * clone () const = 0;

  /** Makes a copy of @a other.
   */
  virtual void copyFrom ( const Fitter * other );

  /** Returns the name of the fitter. 
   */
  const std::string & name () const;

  /** Sets the objective function object.  Sets the objective function
      object to be used and takes possession of it.  That is, will
      delete an existing object, if there is one, and will delete the
      object in this object's destructor.
      
      @attention A derived class that has special requirements, such as
      need for partial derivatives with respect to the parameters
      should override this method to set those requirements.
  */
  virtual void setFCN ( StatedFCN * fcn );

  /** Returns the objective function object. 
   */
  StatedFCN * getFCN ( );

  /** Sets the model function.
   */
  void setFunction ( FunctionBase * function );

  /** Returns @c true if the function is compatible with the objective
      function. */
  bool isCompatible ( const FunctionBase * ) const;

  /** Sets the source of data to be used.
   */
  void setDataSource ( const DataSource * source );

  /** Sets the fitter to use error data from the DataSource, if
      available.
   */
  void setUseErrors ( bool yes = true );

  /** Returns @c true if error data from the DataSource will be used
      if available.
  */
  bool getUseErrors () const;

  /** Returns @c true if the Fitter needs integrated intervals.
   */
  bool needsIntegrated () const;

  /** Fills the vector with the free parameters values.
   */
  void fillFreeParameters ( std::vector < double > & ) const;

  /** Sets the parameters that are to be held fixed during objective
      function minimization.
   */
  virtual void setFixedFlags ( const std::vector < int > & flags );

  /** Sets the limits of the model function parameter values.

      Attempts to set the limits of the model function parameter
      values.  This base class implementation throws a
      FitterException.  Derived classes that support setting limits of
      the parameter values should override this member function.
   */

  /** Returns a vector containing flags for which parameters are to be
      held fixed during objective function minimization.
  */
  virtual const std::vector < int > & getFixedFlags () const;

  /** Sets the limits for the parameter of the model function indexed
      by @a i.  This base class implementation throws a
      std::runtime_error. Derived classes that implement this function
      should override it.
  */
  virtual void setLimits ( unsigned int i, double lower, double upper );

  /** Sets the limits for the parameter of the model function with
      name @a name.
  */
  void setLimits ( const std::string & name,
		   double lower,
		   double upper );

  /** Calculates the best fit.  Returns @c true if the fit converged;
      otherwise it returns @c false. */
  virtual bool calcBestFit () = 0;

  /** Sets the step size for parameter of the minimization.  This base
      class implementation throws FitterException.  Derived classes
      that support setting step size should override this member
      function.
   */
  virtual void setStepSize ( unsigned int i, double size );

  /** Sets the minimization step size for model function parameter @a name.
   */
  void setStepSize ( const std::string & name, double size );

  /** Calculates the value of the objective function at the current
      set of parameters.
  */
  virtual double objectiveValue() const;

  /** Returns the number of degrees of freedom in the fit. 
   */
  virtual int calcDegreesOfFreedom () const;

  /** Calculates the covariance matrix. Returns EXIT_SUCCESS if a
      minima of the objective functions is found, returns EXIT_FAILURE
      in case algorithm converges on other stationary points (i.e. on
      saddle points). 
      
      This member function should be overridden by a derived class if
      supported.  The implementation in this base class returns
      EXIT_FAILURE.
  */
  virtual 
  int calcCovariance ( std::vector < std::vector < double > > & cov );

  /** Sets the cut to limit range of fitting.
   */
  virtual void setFitCut ( TupleCut * cut );

  /** Sets use of a fitting range on or off.
   */
  virtual void setFitRange ( bool yes = true );

};

} // namespace hippodraw

#endif // _Fitter_H_
