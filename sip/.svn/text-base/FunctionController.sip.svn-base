/* -*- mode:c++ -*- */

/** @file 

SIP interface definition for FunctionController

Copyright (C)  2002-2005  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

$Id: FunctionController.sip,v 1.20 2007/02/24 22:24:38 pfkeb Exp $

Author: Paul_Kunz@slac.stanford.edu

*/

class FunctionController
{
%TypeHeaderCode
#include "controllers/FunctionController.h"
using hippodraw::FunctionController;
using hippodraw::PlotterBase;
using hippodraw::NTuple;
using hippodraw::FunctionRep;
%End

private:

  FunctionController ();

public:

  static FunctionController * instance ();

  FunctionBase * addFunction ( PlotterBase *, const std::string & )
    throw ( FactoryException );

  bool fitFunction ( PlotterBase *, unsigned int = 0 );
  double getChiSquared ( const PlotterBase * );
  int getDegreesOfFreedom ( const PlotterBase * );
//  const std::vector < double > parameters ( PlotterBase *, 
// 					    unsigned int = 0 );
//  const std::vector < double > principleErrors ( PlotterBase *, 
//						 unsigned int = 0 );
//  const std::vector < double > getErrors ( const PlotterBase * );
  void saveParameters ( PlotterBase * );
  void restoreParameters ( PlotterBase * );
//  void removeFunction ( PlotterBase *, FunctionRep * function );
//  NTuple * createNTuple ( PlotterBase * ) /Factory/;

  /** Returns the list of Fitter names. 
   */
  const std::vector < std::string > & getFitterNames () const;

  /** Returns the Fitter object used by the plotter.
  */
  Fitter * getFitter ( const PlotterBase * );

  /** Returns the name of the Fitter object as known by the
      FitterFactory. 
  */
  const std::string & getFitterName ( const PlotterBase * );

  /** Sets the Fitter.  Sets the Fitter object for the function
      contained by the @a plotter.  The fitter is created from the
      FitterFactory using the name @a name.  Returns @c true if the
      fitter is compatible with the target @a plotter, otherwise
      returns @c false.
  */
  bool setFitter ( const PlotterBase *, const std::string & );

  /** Sets the range used for fitting.
      Arguments are...
      @p PlotterBase object,
      @p Low end of range,
      @p High end of range.
   */
  void setFitRange ( PlotterBase *, double, double );

};
