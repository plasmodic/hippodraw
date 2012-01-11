/* -*- mode: c++ -*- */

/** @file

FunctionParameter class interface

Copyright (C) 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FunctionParameter.h,v 1.3 2005/10/30 00:10:10 pfkeb Exp $

*/

#ifndef _FunctionParameter_H_
#define _FunctionParameter_H_

#include "pattern/libhippo.h"

#include <string>

namespace hippodraw {

/** A class to collect function parameters from a FunctionBase object
    but also the constraints and errors from the Fitter object  Objects
    of this class can then be delivered to client code so that such
    code does not need to know how to collect what it needs itself.
    This class is equivalent to Minuit's MinuitParameter class.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API FunctionParameter
{

private:

  /** The name of the parameter.
   */
  std::string m_name;  

  /** The value of the parameter.
   */
  double m_value;

  /** The error on the value after fitting.
   */
  double m_error;

  /** The lower limit allowed during fitting.
   */
  double m_lower_limit;

  /** the upper limit allowed during fitting.
   */
  double m_upper_limit;

  /** The fixed flag used by the minimizer.
   */
  bool m_fixed;

  /** The has low limit flag.
   */
  bool m_has_lower;
  /** The has uppper limit flag.
   */
  bool m_has_upper;

public:

  /** The constructor.
   */
  FunctionParameter ( const std::string & name,
		      double value,
		      double error,
		      bool fixed,
		      double low_limit,
		      double high_limit,
		      bool has_low,
		      bool has_high );

  /* Taking default copy constructor.
   *   FunctionParameter ( const FunctionParameter & );
   */

  /** Returns the name of the parameter
   */
  const std::string & name () const;

  /** Returns the value of the parameter.
   */
  double value () const;

  /** Returns the error on the value of the parameter.
   */
  double error () const;

  /** Returns the fixed flag.
   */
  bool isFixed () const;

  /** Returns the lower limit, if any on the parameter.
   */
  double lowerLimit () const;

  /** Returns the upper limit, if any on the parameter.
   */
  double upperLimit () const;

  /** Returns @c true if lower limit on parameters being used.
   */
  bool hasLowerLimit () const;

  /** Returns @c true if upper limit on parameters being used.
   */
  bool hasUpperLimit () const;

};

} // namespace hippodraw

#endif // _FunctionParameter_H_
