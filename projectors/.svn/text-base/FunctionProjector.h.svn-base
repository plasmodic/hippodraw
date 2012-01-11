/* -*- mode: c++ -*- */

/** @file

hippodraw::FunctionProjector class interface

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FunctionProjector.h,v 1.136 2006/08/05 22:06:57 pfkeb Exp $

*/

#ifndef _FunctionProjector_H_
#define _FunctionProjector_H_

#include "ProjectorBase.h"

namespace hippodraw {

class BinsFunction;
class Fitter;
class FunctionBase;
class TupleCut;

/** A projector that plots one function.  

    @author Kaustuv <kaustuv@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket B. Malde <sanket@stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/
class MDL_HIPPOPLOT_API FunctionProjector : public ProjectorBase
{

private:

  /** The error covariance matrix of the errors associated with
      the parameters of the function */
  mutable std::vector< std::vector < double > > m_covariance;

  /** The principle errors of the errors associated with the parameters
      of the function */
  mutable std::vector< double >  m_principleErrors;
    
  /** An X label that might be used. */
  static std::string s_x_label; 

  /** A vector used to save parameters so that they can be
      restored.
      @request @@@@ Use a std::stack for this.
  */
  std::vector< double > m_save_parms;

  /** Dummy member so getZLabel member can return reference. */
  std::string m_z_label;

  /** The ProjectorBase object containing the data points for the
      function and fitter. 
  */
  ProjectorBase * m_target;

  /** The list of function objects to be projected. */
  FunctionBase * m_function;

  /** A fitter object for the function.  @todo Decide if
      FunctionProjector or the FunctionRep should be the owner of the
      Fitter.
   */
  Fitter * m_fitter;

  /** Function to add the acceptable point reps. */
  virtual void addPointReps ();

public:

   /** This constructor takes a FunctionBase object and its target
       data projector.
    */
   FunctionProjector ( FunctionBase * function,
                       ProjectorBase * targetProjector );

  /** The copy constructor. */
  FunctionProjector( const FunctionProjector & );

  /** The virtual destructor.  It is responsible for deleting the @c
      Function objects. */
  virtual ~FunctionProjector();

  virtual ProjectorBase * clone();

  /** Implements ProjectorBase. */
  virtual bool isEmpty () const;

  /** Returns the objective value (chi2 or likelihood) between the
      function and the data. */
  double objectiveValue () const;

  /** Returns the covariance matrix.
   */
  const std::vector < std::vector < double > > &  covariance ( ) const;

  /** Calculates the principle errors from the fit.
   */
  void calcPrincipleErrors() const;

  /** Returns the principle diagonal of the covariance matrix */
  const std::vector< double > & principleErrors () const;
  
  /** Sets the principle errors.  Sets the principle errors of the
      parameters.  This function may be used, for example, to set the
      errors found from a fitting procedure.
  */
  void setPrincipleErrors ( std::vector < double > ::const_iterator first,
			    std::vector < double > ::const_iterator last );

  /** Sets the Fitter to be used. 
   */
  void setFitter ( Fitter * fitter );

  /** Returns the Fitter that is in use. 
   */
  Fitter * getFitter ( ) const;

  /** Returns the number of degrees of freedom in the fit. */
  int degreesOfFreedom () const; 

  /** Returns a pointer to the contained function. */
  FunctionBase * function () const;

  /** Returns true if the function's parameters have been saved. */
  bool hasSavedParameters () const;

  /** Makes a copy of the function's parameter values. */
  void saveParameters ();

  /** Restores the function's parameter values from the previously
      saved values. */
  void restoreParameters ();

  /** Gives the function object initial values based on a given data
      projector. */
  void initializeFunction ( ProjectorBase * projector );

  /** Fits the function. Attempts to fit the function to the
      data. Returns true if the fit converged, otherwise returns
      false.  Creates a new fitter object if necessary. 
   */
  bool fitFunction ( );

  virtual Range valueRange() const;

  virtual const std::string & getTitle() const;

  virtual void setRange( hippodraw::Axes::Type, bool );
  virtual double getPosOn ( hippodraw::Axes::Type ) const;

  /** Finds the X axis label of the plot.  Normally the label is taken
      from the data projector that is used in conjunction with the
      function projector.  If called, the function returns "X". */
  virtual const std::string & getXLabel() const;

  /** Finds the Y axis label of the plot.  Normally the label is taken
      from the data projector that is used in conjunction with the
      function projector.  If called, the function returns the name of
      the function. The argument @a density is not used.
  */
  virtual const std::string & getYLabel ( bool density = false ) const;

  /** A call to this member function is meaningless. Thus, it raises
      an assertion.
  */
  int getNumberOfEntries () const;

  /** Returns the number of underflow. It is meaningless. */
  int getUnderflow () const;

  /** Returns the number of overflow. It is meaningless. */
  int getOverflow () const;


  virtual Range dataRangeOn ( hippodraw::Axes::Type ) const;

  /** Is meaningless for this projector. */
  const std::string & getZLabel() const;

  /** Returns a null pointer.
   */
  virtual DataSource * createNTuple () const;
  
  /** Set the parameter values by hand. */
  void setParameters( const std::vector<double> &params );

  /** Sets the cut to limit range of fitting.
   */
  virtual void setFitCut ( TupleCut * cut );

  /** Sets use of a fitting range on or off.
   */
  virtual void setFitRange ( bool yes = true );

protected:

  /** Does nothing.
   */
  virtual void fillProjectedValues ( DataSource * ntuple, 
				     bool in_range = false ) const;

};

} // namespace hippodraw

#endif // _FunctionProjector_H_
