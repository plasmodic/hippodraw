/* -*- mode: c++ -*- */

/** @file

hippodraw::PyFunctionRep class interface.
 
Copyright (C) 2002, 2003, 2005, 2006  The Board of Trustees of The
Leland Stanford Junior University.  All Rights Reserved.

$Id: PyFunctionRep.h,v 1.31 2006/04/14 23:28:11 pfkeb Exp $

*/

#ifndef PyFunctionRep_H
#define PyFunctionRep_H

#include <string>
#include <vector>

namespace hippodraw {

class DataRep;
class PyDataRep;
class FunctionRep;
class FunctionBase;
class PlotterBase;
class QtDisplay;

/** This class is the public Python interface wrap for the FunctionRep
    class. Its purpose to to lock the application thread, when needed,
    before invoking methods in the FunctionRep.

    @request @@@@  Return covariance matrix.
    
    @todo Could make this a derived class of FunctionRep so that
    methods that do not need wrapping could be called directly.

    @author James Chiang <jchiang@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class PyFunctionRep
{

private:

  /** The wrapped FunctionRep object. */
  FunctionRep * m_rep;

  /** The PlotterBase on which the FunctionRep is displayed. */
  PlotterBase * m_target;

public:

  /** Constructor taking function name and target DataRep.
   */
  PyFunctionRep ( const std::string & function_name, PyDataRep * rep );

  /** Constructor taking function and target DataRep.
   */
  PyFunctionRep ( FunctionBase * function, PyDataRep * rep );

  /** Constructor taking function and no target.
   */
  PyFunctionRep ( FunctionBase * function );

  /** Adds the function on to the @a display. */
  void addTo ( QtDisplay * display );

  /** Returns the wrapped FunctionRep object. */
  hippodraw::DataRep * getRep () const;

  /** Returns the names of the parameters. */
  const std::vector < std::string > &  parmNames () const;

  /** Returns the current values of the parameters. 
      @todo Enable access to parameters of composite functions via
      function indexes.  Do this for parameters(), setParameters(),
      and setFixedFlags() methods.
   */
  const std::vector < double > &  parameters () const;

  /** Returns the principle error values associated with the parameters. */
  const std::vector < double > & principleErrors () const;
  
  /** Fits the function to the target DataRep.  Fits the function to
      this object's target DataRep.  Uses the
      FunctionController::fitFunction member function to apply the
      application logic.
   */
  bool fitFunction ();

   /** Set the parameter values. */
   void setParameters( const std::vector<double> & params );

  /** Returns the function value at a given @a x. 
   */
  double operator () ( double x );

   /** Returns the value of objective value. */
   double objectiveValue();

  /** Returns the covariance matrix from the last fit.
   */
  const std::vector < std::vector < double > > & covarianceMatrix ();

   /** Returns the number of degrees-of-freedom. */
   int degreesOfFreedom();

   /** Set the fixed flags of the parameters. */
   void setFixedFlags( const std::vector<int> & flags );

  /** Sets the Fitter with @a name in the FitterFactory. 
   */
  void setFitter ( const std::string & name );

  /** Returns the name of the Fitter. 
   */
  const std::string & getFitterName () const;
      
  /** Returns an XY Plot of the residuals between the function and the
      data.
  */
  QtDisplay * createResidualsDisplay () const;

  /** Sets the cut to limit range of fitting.
   */
  void setFitRange ( double low, double high );

  /** Sets use of a fitting range on or off.
   */
  void setFitRangeEnabled ( bool yes = true );

};

} // namespace hippodraw

#endif // PyFunctionRep_H
