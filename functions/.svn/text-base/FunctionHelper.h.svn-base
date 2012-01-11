/* -*- mode: c++ -*- */

/** @file

FunctionHelper class interface

Copyright (C) 2003   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FunctionHelper.h,v 1.7 2005/10/30 00:10:11 pfkeb Exp $

*/

#ifndef _FunctionHelper_H_
#define _FunctionHelper_H_

#include "pattern/libhippo.h"

namespace hippodraw {

/** An abstract base class to help FunctionBase objects perform some
    operations.

    The class helps %FunctionBase objects perform some operations that
    require access to the data set upon which the function will be
    used.  In particular, for a function to estimate its initial
    parameters, it requires some summary information about the data
    set, such as the mean value for X or Y.   

    The use pattern is as follows.  The client creates a function via
    its default constructor, maybe via the FunctionFactory.  It then
    calls initialParameters() with a pointer to a concrete
    implementation of this class that has access to the data set.  The
    %FunctionBase object will then call one or more of the member
    functions declared by this class to estimate its initial
    parameters.

    All the member functions declared in this class are pure virtual
    and there are no data member declared.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API FunctionHelper
{
protected:

  /** The default constructor.  The default constructor is protected
      to force creation of derived class. */
  FunctionHelper () { }

public:

  /** The virtual destructor.  No implementation is needed in this
      base Derived classes may need to release resources This
      declaration also keeps some compilers from issuing warnings.*/
  virtual ~FunctionHelper() { }

   /** Returns the number of points in the data set. */
  virtual int size () const = 0;

  /** Returns the sum of the values of the data set. */
  virtual double getTotal () const = 0;
  
  /** Returns the smallest coordinate value along the X
      axis in the data set. */
  virtual double minCoord () const = 0;
  
  /** Returns the largest coordinate value along the X
      axis in the data set. */
  virtual double maxCoord () const = 0;
  
  /** Returns the smallest value in the data set. */
  virtual double minValue () const = 0;
  
  /** Returns the largest value in the data set. */
  virtual double maxValue () const = 0;

  /** Returns the mean of the values in the data set. */
  virtual double meanValue () const = 0;
  
  /** Returns the mean of the coordinates along a X axis
      in the data set. */
  virtual double meanCoord () const = 0;

  /** Returns the standard deviation of the coordinates along a X
      axis in the data set. */
  virtual double stdCoord () const  = 0;
  
  /** Returns the value at a given coordinate. */
  virtual double valueAt( double x ) const = 0;
  
};

} // namespace hippodraw

#endif // _FunctionHelper_H_
