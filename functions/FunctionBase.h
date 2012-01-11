/* -*- mode: c++ -*- */

/** @file

hippodraw::FunctionBase class interface

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FunctionBase.h,v 1.93 2007/03/27 18:53:05 pfkeb Exp $

*/

#ifndef _FunctionBase_H_
#define _FunctionBase_H_

#include "pattern/libhippo.h"

#include <string>
#include <vector>

namespace hippodraw {

class FunctionHelper;

/** A function that can be added to a DataRep and used
    in a fitter.

    This is the base class for functions that can be
    added to a %DataRep and used by a fitter.  The initial value of
    the functions parameters can be set by a call to the
    initialParameters() member function. This function takes as
    argument a FunctionHelper which maybe connected to the data set.
    The function can make calls to the %FunctionHelper to get some
    summaries from the data set and make an estimate of parameter
    values so that the function will at least appear on the %DataRep
    if not come close to fitting it.  This function must be
    implemented in a derived class.
    
    The value of the function at a given value of the one dimensional
    coordinate @c x, or multi-dimensional coordinates as vector of
    doubles is obtained by a call to the overloaded member function
    operator()().  This function must be implemented in a derived
    class.

    To support a user interface, the name of the function may be
    obtained by a call to the name() member function and the name of
    the function's parameters from the parmNames() member function.
    The values of the function's parameters can be obtained by a call
    to the getParameters() member function.  The number of parameters
    can be obtained by the size member function.

    Internally, setting and receiving the parameters described above
    is done by the functions that take an iterator as an argument.
    The iterator skips over fixed parameters in both setting and
    receiving functions.  This architecture is used to support the
    derived class LinearSumFunction.

    Derived classes need to implement a few pure virtual functions
    defined by this class. 

    @li initialize() Set the name of the function and initialize the
    vector of parameter names.  The size of other vectors can then be
    set by calling the resize() member function.  

    @li initialParameters() Initializes the parameters.  A call to
    this function is optional, but there must be an implementation.

    @li operator()() Returns the function's value a given x for the
    current value of the parameters.

    @li clone() Supports putting function prototypes in a
    FunctionFactory.

    @li derivByParm() Returns the derivative with respect to a
    parameter.

    Once there conditions are satisfied, the class maybe added to the
    FunctionFactory for use in applications.
    
    @note This class does not have any dependence on any other part of
    the @b HippoDraw class library.  If the function classes are
    useful for other applications or class library builders, it could
    be split off as a separate package.  Please let the author know if
    you would like to use this class outside the context of @b
    HippoDraw.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API FunctionBase
{

protected:

  /** The default constructor. */
  FunctionBase ();

  /** The copy constructor.
   */
  FunctionBase ( const FunctionBase & );

  /** The name of the function. */
  std::string m_name;

  /** The names of the function parameters. */
  std::vector<std::string> m_parm_names;

  /** The parameter values. */
  std::vector < double > m_parms;


  /** Sets the name of the function.
   */
  void setName ( const char * );

  /** Re-sizes the appropriate vectors maintained in this base class.
      Derived classes should call this member function @b after
      initializing the m_parm_names data member, as the new size is
      taken from that data member's size. */
  virtual void resize ();

  /** Initializes the function and parameter names.  The derived classes
      should initialize their names and the vector of parameter names and
      then invoke the resize() member function.

      @note Make this member pure when problem compiling
      python/FunctionWrap under Windows is solved.
	  */
  virtual void initialize ();
  
 public:

  
  /** The virtual destructor. */
  virtual ~FunctionBase();

  /** Returns the name of the function. */
  const std::string & name () const;

  /** Creates a new function object by copying an existing one.  The
      implementation raises an assertion as this member function
      should be reimplemented in s derived class.  

      @note There's an implementation here to satisfy the SIP
      interface.
  */
  virtual FunctionBase * clone () const;

  /** Returns the number of parameters. */
  virtual int size () const;

  /** Returns a reference to a vector of parameter names. */
  virtual const std::vector < std::string > & parmNames ( ) const;

  /** Sets the names of the parameters.
   */
  void setParmNames ( const std::vector < std::string > & names );

  /** Returns @c false.  Derived classes that are a composite of
      functions should override this member function and return @c
      true. */
  virtual bool isComposite () const;

  /** Does nothing. Derived classes that are a composite of functions
      should override this member function and add functions to the
      composite. */
  virtual void addToComposite ( FunctionBase * );

  /** Does nothing. Derived classes that are a composite of functions
      should override this member function and remove functions from the
      composite. */
  virtual void removeFromComposite ( FunctionBase * );

  /** Returns 0. Derived classes that are a composite of functions should
      override this member function to return the number of functions in
      the composite. */
  virtual int count ();

  /** Returns the values of the parameters as a vector.  The function
      is non-const because a derived class may need to create the
      vector. */
  virtual const std::vector< double > & getParameters () const;

  /** Sets the parameter values. Derived
      classes should implement this function by invoking
      incrementValues ( vector< double >::const_iterator ) so that
      they correctly work with the LinearSumFunction class.*/
  virtual void setParameters ( const std::vector< double > & incr );

  /** Sets the parameter values to the value pointed to by the
      iterator. Returns an iterator to a data element one past the
      last used data element. */
  virtual std::vector< double >::const_iterator 
  setParameters ( std::vector< double >::const_iterator it );

  /** Returns the function's derivative at the coordinate value @p x
      with respect to the @p i-th parameter.  Derived classes must
      implement this function. 

      @todo Provide a numeric method for doing this calculation that
      will be the default.  Derived classes should provide more
      efficient analytic method if available.

      @bug Make this function pure when problem compiling
      python/FunctionWrap under Windows is solved.
  */
  virtual double derivByParm ( int i, double x ) const;

  /** The function call operator.
	@bug Make this member pure when problem compiling python/FunctionWrap
	is solved.
	*/
  virtual double operator() ( double x ) const;

  /** The function call operator for multi-dimension coordinate
      variable.  Returns the value of the function at coordinate
      represented by @a v.  The implementation in this base class uses
      the first element of the vector and calls operator () ( double ).
  */
  virtual double operator () ( const std::vector < double > & v ) const;

  /** Sets the FunctionHelper so that the function can calculate a
      reasonable set of initial parameter values. */
  virtual void initialParameters ( const FunctionHelper * helper );

  /** Returns @c true if function can calculate its partial
      derivatives.  This base class implementation returns @c true.
      Derived class that can not calculate its partial derivatives
      should return @c false.  This allows an application to not
      attempt to use a minimizer that requires derivatives to use this
      function.
  */
  virtual bool hasDerivatives () const;

  /** Returns the integral of the function from the lower limit
      to the higher limit. As of now all the integration is done here
      numerically using the trapezoidal rule. Derived class may
      implement an analytical expression in place of this numerical method.
  */
  virtual double integrate ( double lower_limit, double upper_limit ) const;

  /** Returns the number of dimensions of the data coordinate.  This
      implementation in the base class returns @c 1.  Derived class
      should override this function if the data coordinate is
      multi-dimensional.
  */
  virtual unsigned int dimensions () const;

};

} // namespace hippodraw

#endif // _FunctionBase_H_
