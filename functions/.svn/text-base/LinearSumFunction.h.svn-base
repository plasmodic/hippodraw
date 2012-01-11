/* -*- mode: c++ -*- */

/** @file

hippodraw::LinearSumFunction class interface

Copyright (C) 2000, 2003-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: LinearSumFunction.h,v 1.37 2006/05/21 18:18:15 pfkeb Exp $

*/

#ifndef _LinearSumFunction_H_
#define _LinearSumFunction_H_

#include "functions/FunctionBase.h"

namespace hippodraw {

/** A function that can be used with a fitter.  This function is a
    composite of other functions and computes the linear sum of them.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API LinearSumFunction : public FunctionBase
{

private:

  typedef std::vector < FunctionBase * > FunctionList_t;
  FunctionList_t m_functions;

protected:

  /** Initializes the function and parameter names. */
  virtual void initialize ();

public:

  /** The default constructor. */
  LinearSumFunction ( );

  /** The copy constructor.   Makes a deep copy. */
  LinearSumFunction ( const LinearSumFunction & );
  
  virtual FunctionBase * clone () const;
  
  const std::vector < std::string > & parmNames() const;

  void freeParmNames ( std::vector< std::string > & names ) const;
  
  virtual const std::vector< double > & getParameters () const;
  
  virtual std::vector< double >::const_iterator 
  setParameters ( std::vector< double >::const_iterator it );
  
  /** Returns the number of functions that are a part of this
      composite. */
  virtual int count ();
  
  virtual int size () const;

  /** Returns @c true. */
  virtual bool isComposite () const;
  
  /** Adds the function to the linear sum of functions. */
  virtual void addToComposite ( FunctionBase * );
  
  /** Removes the function from the linear sum of functions. */
  virtual void removeFromComposite ( FunctionBase * );
  
  /** The function call operator.  */
  virtual double operator () ( double x ) const;

  virtual void initialParameters ( const FunctionHelper * helper );

  virtual double derivByParm ( int i, double x ) const;

};

} // namespace hippodraw

#endif // _LinearSumFunction_H_
