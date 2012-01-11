/* -*- mode:c++ -*- */

/** @file

SIP interface definition for FunctionBase

Copyright (C)  2002, 2003, 2005  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

$Id: FunctionBase.sip,v 1.15 2007/02/24 22:24:38 pfkeb Exp $

Author: Paul_Kunz@slac.stanford.edu

*/

// class FunctionHelper;

class FunctionBase
{
%TypeHeaderCode
#include "functions/FunctionBase.h"
using hippodraw::FunctionBase;
%End

protected:

  FunctionBase ();
  FunctionBase ( const FunctionBase & );
  virtual double derivByParm ( int , double  ) const = 0;
  void setName ( const char * );
  virtual void initialize () = 0;

public:

  virtual ~FunctionBase();

  /** Returns the name of the function. */
  const std::string & name () const;

  /** Creates a new function object by copying an existing one.  

      @note This needed special implementatin otherwise Python would
      garbage collect the created Python object since it thinks
      there's no reference to it.  The call to sipTransfer transfers
      the ownership to C++ and increments a reference to the Python
      object.  See archives of the <a
      href="http://mats.imk.fraunhofer.de/mailman/listinfo/pykde">
      mailing list</a> around March 14, 2005 for discussion.
   */
  virtual FunctionBase * clone () const;
%VirtualCatcherCode
    PyObject *sipResObj = sipCallMethod ( 0, sipMethod, "" );
    if ( !sipResObj || 
	 sipParseResult ( 0,
			  sipMethod,
			  sipResObj,
			  "L",
			  sipForceConvertTo_FunctionBase,
			  &sipRes) < 0) {
		PyErr_Print();
    }
  sipTransfer ( sipResObj, 1 );
%End

  /** Returns the number of parameters. */
  virtual int size () const;

//   /** Returns the number of free parameters.
//    */
//   virtual unsigned int numberFreeParms () const;

  /** Returns a reference to a vector of parameter names. */
  virtual const std::vector < std::string > & parmNames ( );

  /** Sets the parameter names.
   */
  void setParmNames ( const std::vector < std::string > & );

//   /** Returns a vector of free parameter names. Names are appended
//       at the back of the vector.*/
//   void freeParmNames ( std::vector< std::string > &  ) const;
    
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
  virtual const std::vector< double > & getParameters ();

  /** Sets the parameter values. Derived
      classes should implement this function by invoking
      incrementValues ( vector< double >::const_iterator ) so that
      they correctly work with the LinearSumFunction class.*/
  virtual void setParameters ( const std::vector< double > & );

//   /** Returns the fixed parameter flags. */
//   virtual const std::vector < bool > & fixedFlags () const;

//   /** Sets the fixed flag for each function parameter.  A fixed
//       parameter is not returned or set by the values() or setValues()
//       member functions. */
//   virtual void setFixedFlags ( const std::vector < bool > & );

//   /** Sets the parameter values to the value pointed to by the
//       iterator. Returns an iterator to a data element one past the
//       last used data element. */
//   virtual std::vector< double >::const_iterator 
//   setParameters ( std::vector< double >::const_iterator it );

//   /** Sets the fixed flag for the parameter values.  Returns an
//       iterator to the end of the sequence used. */
//   virtual std::vector < bool >:: const_iterator
//   setFixedFlags ( std::vector < bool >:: const_iterator it );

  /** The function call operator.  */
  virtual double operator () ( double ) const = 0;

// %MethodCode
// sipRes = sipCpp -> operator () ( 0. );
// %End


//   virtual double __call__ ( double );
//   /** The function call operator.  @bug Temporary fix until we
//    undertand how to get SIP to accept operator()().*/
//   virtual double valueAt ( double ) const;
  
//   /** The function call operator for multi-dimension coordinate
//       variable.  Returns the value of the function at coordinate
//       represented by @a v.  The implementation in this base class uses
//       the first element of the vector and calls operator () ( double ).
//   */
//   virtual double operator () ( const std::vector < double > & v ) const;

//   /** Sets the FunctionHelper so that the function can calculate a
//       reasonable set of initial parameter values. */
//   virtual void initialParameters ( const FunctionHelper *  );

//   /** Returns the value of the free parameters.  For each free
//       parameter, its value is push_back to the @p parms vector */
//   virtual void getFreeParameters ( std::vector < double > & ) const;

//   /** Returns the value of the k-th free parameters. */
//   virtual double getFreeParameter( unsigned int ) const;
  
//   /** Sets the values of the free parameters. */
//   void setFreeParameters ( const std::vector< double > &  );

//   /** Sets the values of the free parameters.  For each free
//       parameter, its value is set from the value pointed by the
//       iterator, and the iterator is advanced.  Returns the advanced
//       iterator. */
//   virtual std::vector< double >:: const_iterator 
//   setFreeParameters ( std::vector< double >:: const_iterator it );

  /** Returns @c true if function can calculate its partial
      derivatives.  This base class implementation returns @c true.
      Derived class that can not calculate its partial derivatives
      should return @c false.  This allows an application to not
      attempt to use a minimizer that requires derivatives to use this
      function.
  */
  virtual bool hasDerivatives () const;

//   /** Returns the function's derivatives.  Calculates the function's
//       derivatives for each free parameter at the coordinate value @p
//       x.  Appends the derivatives at the end of the sequence @a
//       derives. */
//   virtual void getFreeDerivatives ( std::vector < double > & , 
// 				    double ) const;

  /** Returns the integral of the function from the lower limit
      to the higher limit. As of now all the integration is done here
      numerically using the trapeziodal rule. Derived class may
      implement an analytical expression in place of this numerical method */
  virtual double integrate ( double , double  ) const;

};
