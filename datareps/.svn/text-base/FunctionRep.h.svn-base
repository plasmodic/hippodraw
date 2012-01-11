/* -*- mode: c++ -*- */

/** @file

hippodraw::FunctionRep class interface

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FunctionRep.h,v 1.73 2006/02/04 22:09:07 pfkeb Exp $

*/

#ifndef _FunctionRep_H_
#define _FunctionRep_H_

#include "DataRep.h"

namespace hippodraw {

class Fitter;
class FunctionBase;
class FunctionParameter;

/** A derived class of DataRep which is a base class for displaying a
    function.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

    @bug @@@@ The function should be drawn in blue so it can be see on
    filled histogram.

*/
class MDL_HIPPOPLOT_API FunctionRep
  : public hippodraw::DataRep
{

private:

  /** A flag which when set @c true indicates the receiving object is
      a member of a composite.
   */
  bool m_in_composite;

protected:

  /** The target for the function.  The DataRep object from which the
      function receives its data sets.  This information is redundant,
      but needed for the archiving and unarchiving.
   */
  DataRep * m_target;

  /** The constructor taking the target DataRep object for the data.
   */
   FunctionRep ( FunctionBase * function, DataRep * rep );

  /** The copy constructor.
   */
  FunctionRep ( const FunctionRep & );

public:

  /** The virtual destructor.  Removes itself as an Observer of the
      target DataRep object.
  */
  virtual ~FunctionRep ();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual DataRep * clone ();

  /** Sets the target and Initializes the function with the projected
      values of @a the DataRep. */
  void initializeWith ( DataRep * );

  /** Returns @c false as this class does not have DataSource bindings.
   */
  virtual bool hasNTupleBindings () const;

  /** Returns @c true if represented function is a composite of two or
      more functions.
  */
  bool isComposite () const;

  /** Returns @c true if receiving object is member of a composite.
   */
  bool isInComposite () const;

  /** Informs receiving object that it is a member of a composite.
   */
  void setInComposite ( bool yes );

  /** Returns the actual function, a FunctionBase derived class. */
  FunctionBase * getFunction () const;

  /** Returns the DataRep object upon which the function is applied. */
  DataRep * getTarget () const;

  /** Sets the target for the FunctionRep.  Does not change the
      parameters.  @sa initializeWith. */
  void setTarget ( DataRep * rep );

  /** Sets the Fitter to @a fitter.  Returns @c true if @a fitter is
      compatible, otherwise returns @c false.
   */
  bool setFitter ( Fitter * fitter );

  /** Returns the Fitter object currently in use. */
  Fitter * getFitter ( ) const;

  /** Returns the name of the Fitter object. */
  const std::string & getFitterName () const;

  /** Fits the contained function to the data points from the target
      projector. */
  bool fitFunction ( );

  /** Returns the objective value of the function to the target DataSource.
   */
  double objectiveValue () const;

  /** Returns the covariance matrix of the function to the target
      DataSource.
   */
  const std::vector < std::vector < double > > & covarianceMatrix () const;

  /** Returns the number of degrees of Freedom of the function fitting
      to the target DataSource.
   */
  int degreesOfFreedom () const;

  /** Returns the names of the parameters for the function represented
      by this object. */
  const std::vector < std::string > & parmNames () const;

  /** Returns a set of flags indicating which function parameters are
      fixed during a fit.
  */
  const std::vector < int > & getFixedFlags () const;

  /** Returns the current function parameter values. */
  const std::vector < double > & parameters () const;

  /** Returns the principle errors associated with the parameter values. */
  const std::vector< double > & principleErrors() const;
  
  /** Sets the principle errors.
   */
  void setPrincipleErrors ( std::vector < double >::const_iterator begin, 
			    std::vector < double >::const_iterator end );

  /** Saves the function parameters in a cache so that they can be restored. */
  void saveParameters ();

  /** Restores the saved parameters. */
  void restoreParameters ();

  /** Set the parameter values by hand. */
  void setParameters ( const std::vector<double> & params );

  /** Set the parameters that are to be held fixed during a fit.
   */
  virtual void setFixedFlags ( const std::vector < int > & flags );

  /** Implements Observer pattern by removing the DataRep from its target.
   */
  void willDelete ( const Observable * observed );

  /** Returns @c false.  A function is not meant to be a target of
      another DataRep object.
  */
  virtual bool isTargetable () const;

  /** Sets the cut to limit range of fitting.
   */
  void setCutRange ( const Range & );

  /** Sets up the region over which fitting will be done.
   */
  void setTupleCut ();

  /** Removes the cut region from the fitting.
   */
  void removeCut ();

  /** Sets use of a fitting range.
   */
  void setCutRange ( bool yes = true );

  /** Returns @c true.  Returns @c true so that it will not turn off
      the Y zooming properties of the DataRep it is attached to.
  */
  bool hasZoomY () const;

  /** Set ignore errors.
   */
  void setIgnoreError(bool ignore);

  /** Get ignore errors.  Return true when there is no errors in the
      data points or ignore error is selected.  Return false when 
      there is error and ignore error is not selected.
  */
  bool getIgnoreErrors() const;

  /** Fills the vector with the parameters and constraints of the
      function.
   */
  virtual
  void fillFunctionParameters ( std::vector < FunctionParameter > & ) const;

  /** Adds the function representation to composite if receiving
      object is a CompositeFunctionRep, otherwise does nothing.
   */
  virtual void addToComposite ( FunctionRep * frep );

  /** Removes the function representation from composite if receiving
      object is a CompositeFunctionRep, otherwise does nothing.
   */
  virtual void removeFromComposite ( FunctionRep * frep );

  /** Returns the name of the contained function.
   */
  const std::string & functionName () const;

};

} // namespace hippodraw

#endif // _FunctionRep_H_
