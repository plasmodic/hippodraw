/* -*- c++ -*- */

/** @file

hippodraw::FunctionWrap class interface.

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FunctionWrap.h,v 1.24 2007/03/27 18:53:05 pfkeb Exp $

*/

#ifndef FunctionWrap_H
#define FunctionWrap_H

#include "functions/FunctionBase.h"

namespace hippodraw {

/** A wrapper class for FunctionBase.  This class allows one to
    implement in Python a derived class of FunctionBase.
    
    @note This class needs Python 2.3 or later to be fully functional.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

    with a lot of help from David Abrahams <dave@boost-consulting.com>
 */
class FunctionWrap : public FunctionBase,
		     public boost::python::wrapper < FunctionBase >
{
private:

  /** The new ownership.
   */
  boost::python::object invert_ownership;

  /** Find the Python object that owns this object.
   */
  template < class T >
  boost::python::object get_owner ( T * me ) const;

public:

  /** Constructor.
   */
  FunctionWrap ( const FunctionBase & base );

  /** The copy constructor.
   */
  FunctionWrap ( const FunctionWrap & );

  /** The default constructor.
   */
  FunctionWrap ();

  /** The destructor.
   */
  virtual ~FunctionWrap ();


  /** Sets the name of the function.
   */
  void setName ( const std::string & name );

  /** Sets the names of the parameters and re-sizes the number of
      parameters available.
  */
  void setParmNames ( const std::vector < std::string > & names );
  double derivByParm ( int, double ) const;
  void initialize ();

  /** Make copy of object, including copy of Python object.  

      @note One can't call clone() if implemented in the Python class,
      because if it constructs a new object, it will also destroy it
      thus leaving a dangling reference for C++.  The attempting to do
      that is commented out in the implementation.  Since all the C++
      FunctionBase prototypes in the FunctionFactory are created with
      their default constructor, this implementation just calls the
      default constructor of the Python class.  The net effect should
      be the same as a call to clone().   See
      http://mail.python.org/pipermail/c++-sig/2005-July/009262.html

      
   */
  FunctionBase * clone () const;

  double operator () ( double ) const;
  double operator () ( const std::vector < double > & ) const;
  void initialParameters ( const FunctionHelper * );

  unsigned int dimensions () const;

  /** Returns @c true if the Python function has implemented returning
      partial derivatives.
  */
  virtual bool hasDerivatives () const;

};

} // namespace hippodraw


#endif // FunctionWrap_H
