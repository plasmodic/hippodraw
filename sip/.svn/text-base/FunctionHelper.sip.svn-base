// -*- mode:c++ -*-
// This is the SIP interface definition for FunctionHelper
//
// Copyright (C)  2003  The Board of Trustees of  
// The Leland Stanford Junior University.  All Rights Reserved.
//
// This file is part of sihippo
//
// $Id: FunctionHelper.sip,v 1.2 2007/02/24 22:24:38 pfkeb Exp $
//
// Author: stuvi@slac.stanford.edu
//

class FunctionHelper
{
%TypeHeaderCode
#include "functions/FunctionHelper.h"
using hippodraw::FunctionHelper;
%End

protected:

  FunctionHelper ();
  //virtual ~FunctionHelper ();

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
  virtual double valueAt ( double ) const = 0;
  

};
