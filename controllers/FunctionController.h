/* -*- mode: c++ -*- */

/** @file

hippodraw::FunctionController class interface

Copyright (C) 2001-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FunctionController.h,v 1.144 2007/09/04 22:48:35 pfkeb Exp $

*/

#ifndef _FunctionController_H_
#define _FunctionController_H_

#include "axes/AxesType.h"

// for incomplete type warning
#ifdef _MSC_VER
#include "functions/FunctionBase.h"
#endif

#include <list>
#include <vector>

namespace hippodraw {

class DataRep;
class Fitter;
class FunctionBase;
class FunctionParameter;
class FunctionRep;
class NTuple;
class PlotterBase;
class ProjectorBase;
class Range;
class TupleCut;
class ViewBase;
class ViewFactory;

/** A singleton class is the interface between an application and the
    list of FunctionRep objects contained in a PlotterBase.

    @author Kaustuv <kaustuv@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket B. Malde <sanket@stanford.edu>
*/
class MDL_HIPPOPLOT_API FunctionController
{

private:
  
  /** The list of FunctionRep objects for the current plotter.
   */
  mutable std::vector < FunctionRep * > m_func_reps;

  /** The list of function names attached to the plotter. */
  std::vector < std::string > m_f_names;

  /** delta chi squared values for 99.99% confidence ellipsoids for
      mu = 1,2,3,4,5,6 */
  std::vector < double > m_deltaXSq;
  
  /** A pointer to the singleton object. */
  static FunctionController * s_instance;

  /** The current PlotterBase object. */
  PlotterBase * m_plotter;

  /** Index of the parameter that shall be bound to X axes while plotting
      the error / confidence ellipsoids. NOTE: m_x = n is the index of
      nth FREE parameter and NOT of nth parameter. */
  mutable int m_x;
  
  /** Index of the parameter that shall be bound to Y axes while plotting
      the error / confidence ellipsoids. NOTE: m_y = n is the index of
      nth FREE parameter and NOT of nth parameter. */
  mutable int m_y;
  
  /** A count of number of confidence contour plots created.
   */
  unsigned int m_confid_count;

  /** A private  default constructor for avoiding creation. */
  FunctionController ();

  /** A private copy constructor to avoid copying. */
  FunctionController ( const FunctionController & );

  /** Finds the FunctionRep objects contained by the @a plotter. Finds
      the %FunctionRep objects contained by the @a plotter and inserts
      the list m_functions.  If there are no functions contained, then
      the list will be empty.
   */
  void findFunctions ( const PlotterBase * plotter ) const;

  /** Sets the errors from the composite.  Sets the errors or each of
      the contained functions in the composite. 
  */
  void setErrorsFromComposite ( const PlotterBase * plotter, 
				const FunctionRep * composite );

  /** Clears and fills the vector the FunctionRep objects contained by
      the @a plotter that target the @a data_rep.  If @a data_rep is a
      null pointer, then all FunctionRep objects are appended.
  */
  void fillFunctionReps ( std::vector < FunctionRep * > & freps,
			  const PlotterBase * plotter,
			  const DataRep * data_rep ) const;

public:

  /** Returns the pointer to the singleton instance. */
  static FunctionController * instance ();

  /** The destructor. */
  ~FunctionController();

  /** Returns the list of Fitter names. 
   */
  const std::vector < std::string > & getFitterNames () const;

  /** Returns the name of the default Fitter object.
   */
  const std::string & getDefaultFitter () const;

  /** Returns a newly created FunctionRep object with function of type
      @a name and target @a rep. */
  FunctionRep * createFunctionRep ( const std::string & name,
				    DataRep * rep  );
    
  /** Returns a newly created FunctionRep object with FunctionBase pointer
      @a function and target @a rep. */
   FunctionRep * createFunctionRep ( FunctionBase * function,
				     DataRep * rep  );
    
  /** Returns the FunctionRep.  Returns a %FunctionRep object
      contained by the @a plotter.  If more than one exists, returns
      the %FunctionRep corresponding the the composite function.  If
      none exists, return a null pointer.

      @bug If multiple DataRep exists, each with its own functions,
      then this function will probably not return the right one.  Use
      datareps.py to demonstrate this.  The only use of this function
      is Inspector itself, thus should be possible to remove this
      function altogether.
  */
  FunctionRep * getFunctionRep ( const PlotterBase * plotter ) const;

  /** Returns the FunctionRep.  Returns the %FunctionRep object
      contained by the @a plotter whose target is @a data.  If more
      than one %FunctionRep object has @a data as target, then
      return the one which holds the composite function, or if no
      composite is found, then return the last %FunctionRep object.*/
  FunctionRep * getFunctionRep ( const PlotterBase * plotter,
				 const DataRep * data ) const;
  

  /** Clears and fills the vector with the top level FunctionRep
      objects that target @a data_rep and are contained in @a plotter.
      A top level FunctionRep object is one that is not a member of a
      CompositeFunctionRep.
   */
  void fillTopLevelFunctionReps ( std::vector < FunctionRep * > & reps,
				  const PlotterBase * plotter,
				  const DataRep * data_rep ) const;

  /** Returns the CompositeFunctionRep for which @a rep is a member if
      it is one, otherwise returns rep. */
  FunctionRep * getComposite ( const PlotterBase * plotter,
			       FunctionRep * rep );

  /** Adds a function to the first DataRep object of the plotter.
   */
  FunctionBase *  addFunction ( PlotterBase * plotter, 
				const std::string & name );

  /** Adds the function named by the string to the designated DataRep
      @a data_rep contained in the @a plotter.  If @a frep is not a null
      pointer, then the newly created function and @a frep become part
      of the same CompositeFunctionRep, otherwise the newly created
      function is stand-a-lone.  Returns either the
      CompositeFunctionRep or the stand-a-lone FunctionRep.
  */
  FunctionRep * addFunction ( PlotterBase * plotter,
			      const std::string & name,
			      FunctionRep * frep,
			      DataRep * data_rep );

  /** Adds the @a func_rep to the @a plotter. Find the applicable
      DataRep in the plotter, then adds the function using it.
  */
  void addFunction ( PlotterBase * plotter, FunctionRep * func_rep );

  /** Adds DataRep @a rep to the PlotterBase @a plotter.  If @a rep is
      not a FunctionRep, then just add the @a rep to the @a plotter.
      If @a rep is a FunctionRep object, then add it to the plotter
      if it is not a composite function.

      @note This function is intended to be use as part of the
      document reading.
  */
  void addDataRep ( PlotterBase * plotter, DataRep * rep );

  /** Adds the @a func_rep to the DataRep contained by the @a plotter.
      If @a frep is not a null pointer, then @a func_rep and @a frep
      become part of of a CompositeFunctionRep which is created if one
      doesn't already exist.  Otherwise, @a func_rep because a
      standalone object.

      Returns either the CompositeFunctionRep or the standalone
      FunctionRep.
   */
  FunctionRep * addFunctionRep ( PlotterBase * plotter,
				 DataRep *,
				 FunctionRep * frep,
				 FunctionRep * func_rep );

  /** Removes the designated function from the plotter.
   */
  void removeFunction ( PlotterBase * plotter, FunctionRep * function );

  /** Returns true if @a rep contained by @a plotter has a function
      attached.  If @a rep is non-zero, returns @c true if the @a rep
      contained by @a plotter is a target of a function, otherwise
      returns @c false.  If @a rep is a @c null pointer, returns @c
      true, if @a plotter contains at least one DataRep object that is
      a target of a function, otherwise returns @c false.
  */
  bool hasFunction ( const PlotterBase * plotter,
		     const DataRep * rep );

  /** Returns the index to an unique DataRep object that is not a
      FunctionRep.  Returns the index to the DataRep object contained by the
      plotter if only one is not a FunctionRep, otherwise returns -1 */
  int getUniqueNonFunctionIndex ( const PlotterBase * plotter ) const;

  /** Returns function names on the plotter.  Returns a reference to a
      vector of function names on the designated plotter that are
      applied to the DataRep @a rep.  If @a rep is a null pointer,
      returns all functions, otherwise returns only those whose target
      is @a rep.
  */
  const std::vector < std::string > & 
  functionNames ( PlotterBase *, DataRep * rep );

  /** Returns the list of available functions.
   */
  const std::vector < std::string > &
  getFunctionNames () const;

  /** Returns the Fitter object used by the plotter.
  */
  Fitter * getFitter ( const PlotterBase * plotter );

  /** Returns the name of the Fitter object as known by the
      FitterFactory. 
  */
  const std::string & getFitterName ( const PlotterBase * plotter );

  /** Sets the Fitter.  Sets the Fitter object for the function
      contained by the @a plotter.  The fitter is created from the
      FitterFactory using the name @a name.  Returns @c true if the
      fitter is compatible with the target @a plotter, otherwise
      returns @c false.
  */
  bool setFitter ( const PlotterBase * plotter, const std::string & name );

  /** Changes the fitter.
   */
  bool changeFitter ( const PlotterBase * plotter,
		      const DataRep * datarep,
		      const std::string & name );

  /** Sets the Fitter.  Sets the Fitter for FunctionRep @a frep to a
      fitter in the FitterFactory with name @a name.  Returns @c true
      if the fitter is compatible with the target of the FunctionRep,
      otherwise returns false.  

      @sa setFitter ( FunctionRep *, Fitter * )
   */
  bool setFitter ( FunctionRep * frep, const std::string & name );

  /** Sets the default Fitter object that will be used for newly
      created FunctionRep objects.
   */
  void setDefaultFitter ( const std::string & name );

  /** Attempts to fit the function contained by the @a plotter.  If a
      composite function exists, the fit is done to the composite.
      Otherwise, the fit is done the function contained by @a rep.
      @note This is the member function that implements the
      application logic.
  */
  bool fitFunction ( PlotterBase * plotter, FunctionRep * rep );

  /** Attempts to fit the function.  If function converges and the
      resulting chi squared is smaller than that of the starting
      point, keeps the functions parameters at the fitted value.
      Otherwise, restore the function parameters to their initial
      value.  Returns @a true if the function converged, otherwise
      returns @c false.
  */
  bool tryFitFunction ( PlotterBase * plotter, FunctionRep * );

  /** Attempts to fit the function to the data in the DataRep of the
      plotter.  The int parameter is not used.

      @deprecated This member function is maintained only for existing
      programs written in Python and using the SIP interface.
  */
  bool fitFunction ( PlotterBase * plotter, unsigned int );

  /** Saves the parameters so they can be restored. With the exception
      of cases in which they contain a composite function, the message
      is forwarded to all FunctionProjector objects.

      @request @@@@ Should save parameters in a stack, so that more
      than one set can be saved and restored.
  */
  void saveParameters ( PlotterBase * plotter );

  /** Restores the parameters from previously stored values. With the
      exception of cases in which they contain a composite function,
      the message is forwarded to all FunctionProjector objects. 
  */
  void restoreParameters ( PlotterBase * plotter );


  /** Returns the value of the objective function.  Returns the value
      of the objective function, one derived from FCNBase, for the
      function whose target is the DataRep.
  */
  double getObjectiveValue ( const PlotterBase *, const DataRep * );

  /** Returns the covariance matrix of the objective function.
   */
  const std::vector < std::vector < double > > &
  getCovarianceMatrix ( const PlotterBase * );

  /** Returns the Chi-Squared.  Uses getObjectiveValue to return the
      Chi-Squared.  This is only meaningful if the objective function,
      class derived from FCNBase, is a Chi-Squared calculator. 
  */
  double getChiSquared ( const PlotterBase * );

  /** Returns the degrees of freedom.  Returns the degrees of freedom
      of the function with the DataRep using the current function
      parameters.
  */
  int getDegreesOfFreedom ( const PlotterBase * );

  /** Creates a textual view of function of type @a type. */

  ViewBase * createFuncView ( const ViewFactory * factory,
			      PlotterBase * plotter,
			      const std::string & type );

  /** Returns an NTuple from the FunctionRep and its target DataRep.
   */
  NTuple * createNTuple ( const PlotterBase * plotter,
			  const FunctionRep * rep );

  /** Returns residuals display.  Creates and returns a PlotterBase
      object displaying the residuals of the function to its target.

      @request @@ If target is histogram, then scale of residuals
      should take into account the bin width to make labels in counts.
  */
  PlotterBase * createResidualsDisplay ( PlotterBase * plotter,
					 const FunctionRep * );

  /** This returns in @a nt the percentage distribution of the confidence
      in a rectangle bounded by xmin, ymin, xmax and ymax. This bounding
      rectangle is just large enough that is captures 99.9% confidence
      ellipsoid between the two selected parameters (whose indexes are
      given by m_x and m_y.

      @request @@@@ Make sure they are being calculated correctly by
      hold one parameter constant and refitting.

 */
  void ellipsoidNTuple ( PlotterBase * plotter, FunctionRep * ,
			 NTuple* nt, int n,
			 double & xmin, double & xmax,
			 double & ymin, double & ymax );
  
  /** Returns a new Ellipsoid display.  Creates and returns a PlotterBase
      object displaying the ellipsoids of the function parameters
      of the function representation @a rep in @a plotter.
  */
  PlotterBase * createNewEllipsoidDisplay ( PlotterBase * plotter,
					    FunctionRep * rep );

  /** Returns a new Ellipsoid display.  Creates and returns a PlotterBase
      object displaying the ellipsoids of the function parameters
      in @a plotter.
  */
  PlotterBase * refreshEllipsoidDisplay ( PlotterBase * plotter,
					  FunctionRep * );
    
  /** Given an @ SigmaInv this function returns the coordinates of
      an ellipse (in 2 dimension) centered at @a xbar. Equation
      of ellipse is (x-xbar)' * SigmaInv * (x-xbar) = Csquare 
  */
  NTuple * ellipse( const std::vector< double > & xbar,
		    std::vector< std::vector < double > > & SigmaInv,
		    double Csq );
  
  /** Sets the which parameter shall be bound to which axes while plotting
      the error / confidence ellipsoids. */
  int setEllpsoidParamIndex ( Axes::Type axes, int index );

  /** Returns @c true if the @a function is compatible with the @a
      fitter.  Checks if the function is compatible with the fitter
      and return @c true if so, otherwise returns @c false.  A
      function maybe incompatible with the fitter, for example, if
      the fitter needs the partial derivatives from the function, but
      the function is not capable of supplying them.
  */
  bool isCompatible ( const std::string & function,
		      const std::string & fitter );

  /** Returns @c true if function with name @a name is available.
   */
  bool functionExists ( const std::string & name );

  /** Sets the range used for fitting. */
  void setFitRange ( PlotterBase * plotter, const Range & range );

  /** Sets the range used for fitting. */
  void setFitRange ( PlotterBase * plotter, double low, double high );

  /** Sets the function representation up to obey TupleCut on target,
      if any.
  */
  void setTupleCut ( FunctionRep * rep );

  /** Sets the TupleCut, if any for the FunctionRep contained by @a
      plotter whose target is @a data_rep.
  */
  void setTupleCut ( const PlotterBase * plotter,
		     DataRep * data_rep );

  /** Removes the region cut from both the target and function.
   */
  void removeTupleCut ( const PlotterBase * plotter,
			DataRep * data_rep );

};

} // namespace hippodraw

#endif // _FunctionController_H_
