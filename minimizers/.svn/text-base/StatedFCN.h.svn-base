/* -*- mode: c++ -*- */

/** @file

hippodraw::StatedFCN class interface

Copyright (C) 2004-2007   The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: StatedFCN.h,v 1.39 2007/08/20 17:31:14 pfkeb Exp $

*/

#ifndef _StatedFCN_H_
#define _StatedFCN_H_

#ifdef _MSC_VER
#pragma warning(disable:4275) // non dll 9nterface coming from Minuit's FCNBase
#endif

// for have minuit
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_MINUIT
#include "pattern/libhippo.h"
#include "Minuit/FCNBase.h"
#else

#ifdef HAVE_MINUIT2
#include "pattern/libhippo.h"
#include "Minuit2/FCNBase.h"
#else
#include "FCNBase.h"
#endif

#endif

#include <string>

namespace hippodraw {

class DataSource;
class FunctionBase;
class TupleCut;

/** A derived class for FCNBase class. This class uses a stated
    function derived from FunctionBase.  That is, it assumes the
    function maintains the state of all its parameters.

    @note If @b HippoDraw is built with Minuit or Minuit2, then this
    class is derived from the Minuit's FCNBase class, otherwise it
    derives from @b HippoDraw's FCNBase.  The difference between
    Minuit and Minuit2 is the capitialization of the member function
    names.  There's no difference between HippoDraw's and Minuit's
    FCNBase class.  If you derive from this class, you need to supply
    a <em> config.h </em> file which contains a @#define HAVE_MINUIT or
    @#define HAVE_MINUIT2 depending on how HippoDraw was built.
    HippoDraw doesn't install its autoconf generated <em> config.h
    </em> file since it may conflict with a file of the same name from
    other packages.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
#ifdef HAVE_MINUIT2
class MDL_HIPPOPLOT_API StatedFCN : public ROOT::Minuit2::FCNBase 
#else
class MDL_HIPPOPLOT_API StatedFCN : public FCNBase 
#endif

{

private:

  /** The flags to indicated which parameters are to be held fixed
      during minimization of this objective function.

      @note The flags are held as int because it is more space and
      time efficient for the expected number of parameters.
  */
  std::vector < int > m_fixed_flags;

protected:

  /** A pointer to the model function to be used in fitting.  This
      object does not own the function.
   */
  FunctionBase * m_function;

  /** Needs derivatives flag.  A flag set to @c true, if the Fitter
      needs partial derivatives from the FunctionBase.
   */
  bool m_needs_derivs;

  /** The default constructor.  The default constructor initializes
      the pointer to the model function to the null pointer. 
  */
  StatedFCN ();

  /** The copy constructor.  The copy constructor makes a copy of the
      pointer to the FunctionBase object it uses.
   */
  StatedFCN ( const StatedFCN & );

 public:

  /** Makes a copy of the derived class object. 
   */
  virtual StatedFCN * clone ( ) const = 0;

  /** Makes a copy of the relevant attributes from @a other object.
      @note This is similar to a copy constructor, but can be used
      between objects from different derived classes.
   */
  virtual void copyFrom ( const StatedFCN * other );

  /** Returns @c true if model function has been set, otherwise
      returns @c false.
  */
  bool hasFunction () const;

  /** Sets the model function.  Sets the model function object to @a
      function.
  */
  void setFunction ( FunctionBase * function );

  /** Sets the data source.  Sets the data source and uses indices from
      hippodraw::DataPoint2DTuple.
   */
  virtual void setDataSource ( const DataSource * source ) = 0;

  /** Sets the data source, dimension, and indices into the columns.
      Sets the data source, dimension and indices into the column.
      For one dimensional data source, the order of the indices are as
      in The hippodraw::DataPoint2DTuple enumeration.  For two
      dimensional data source as with hippodraw::DataPoint3DTuple
      enumeration.  For higher dimensions, the pattern should repeat.
      A negative index value indicates that the variable is not used.
   */
  virtual void setDataSource ( const DataSource * source,
			       int dimension,
			       const std::vector < int > & ) = 0;

  /** Sets the FCN to use errors from the DataSource if they are
      available on all data points.
  */
  virtual bool setUseErrors ( bool yes = true ) = 0;

  /** Returns true if error data from the DataSource is to be used.
   */
  virtual bool getUseErrors () const = 0;

  /** Returns the names of the model function's parameters.
   */
  const std::vector < std::string > & getParmNames () const;

  /** Returns the current state of the model function's parameters.
   */
  const std::vector < double > & getParameters () const;

  /** Sets the values of the model function's parameters.
   */
  void setParameters ( const std::vector < double > & parms );

  /** Fills the vector with the values of the free parameters.
   */
  void fillFreeParameters ( std::vector < double > & free_parms ) const;

  /** Returns the number of free parameters.
   */
  unsigned int getNumberFreeParms () const;

  /** Returns the vector of which of the model function's parameters
      are considered fixed and not to be changed during
      minimization of this objective function.
  */
  const std::vector < int > & getFixedFlags () const;

  /** Sets the parameters that are to be held fixed during
      minimization of this objective function.
  */
  virtual void setFixedFlags ( const std::vector < int > & flags );

  /** Sets the free parameters from the vector @a parms. 
   */
  virtual void setFreeParameters ( const std::vector < double > & parms );

  /** Clears and fills the vector with the derivatives of the function
      a coordinate value x.
  */
  void fillFreeDerivatives ( std::vector < double > &, double x );

  /** Sets the model function parameters to @a parms and returns the
      objective value.
  */
  virtual double operator () ( const std::vector < double > & parms ) const;

  /** Returns the value of the objective function.  Returns the value
      of the objective function for the current state of the function.
      The state of the function's parameters is set by operator().
      Other states the function may have is set by calling member
      functions of the function directly.
  */
  virtual double objectiveValue () const = 0;

  /** Returns the number of degrees of freedom. 
   */
  virtual int degreesOfFreedom() const = 0;

  virtual void calcAlphaBeta ( std::vector < std::vector < double > > & alpha,
			       std::vector < double > & beta ) = 0;

  /** Returns @c true if data points were integrated.  Return @c true
      if the FCN function needs data points were integrated over a
      range.  An example would be a histogram.  Returns @c false if
      the integrated data points are not needed. */ 
  virtual bool needsIntegrated () const = 0;

  /** Sets the cut to limit range of fitting.
   */
  virtual void setFitCut ( TupleCut * cut ) = 0;

  /** Sets use of a fitting range on or off.
   */
  virtual void setFitRange ( bool yes = true ) = 0;

  /** Sets the needs derivatives flag.  Sets the needs derivatives
      flag to @c true if the Fitter needs partial derivatives from the
      FunctionBase object.
  */
  void setNeedsDerivatives ( bool yes );

  /** Returns @c true if function is compatible this objective
      function.  Return @c false if the function is incompatible with
      this objective function.  For example, if the fitter indicated
      it needs partial derivatives and the function can not provide
      them.
  */
  bool isCompatible ( const FunctionBase * ) const;

};

} // namespace hippodraw

#endif // _StatedFCN_H_
