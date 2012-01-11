/* -*- mode: c++ -*- */

/** @file

ProjectorHelper class interface

Copyright (C) 2003   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: ProjectorHelper.h,v 1.8 2005/10/30 00:10:14 pfkeb Exp $

*/

#ifndef _ProjectorHelper_H_
#define _ProjectorHelper_H_

#include "functions/FunctionHelper.h"

namespace hippodraw {

class DataSource;

/** A concreate implementation of the FunctionHelper class.

   This has access to the ntuple of the dataset via which
   it gets informations about various aspects of the datasets
   asked for.
   
   @sa FunctionHelper.

   @author Kaustuv <kaustuv@stanford.edu>
*/
class MDL_HIPPOPLOT_API ProjectorHelper : public FunctionHelper
{

private:
  /** The pointer to the ntuple (of the appropriate data set)
      for which it is called to help.
  */
  const DataSource * m_ntuple;

public:

  /** The constructor taking ntuple pointer as arguments. */
  ProjectorHelper ( const DataSource * ntuple );

  /** Returns the number of points in the data set. */
  virtual int size () const;

  /** Returns the sum of the values of the data set. */
  virtual double getTotal () const;

  /** Returns the smallest coordinate value along the X
      axis in the data set. */
  virtual double minCoord () const;

  /** Returns the largest coordinate value along the X
      axis in the data set. */
  virtual double maxCoord () const;

  /** Returns the smallest value in the data set. */
  virtual double minValue () const;

  /** Returns the largest value in the data set. */
  virtual double maxValue () const;

  /** Returns the mean of the values in the data set. */
  virtual double meanValue () const;
  
  /** Returns the mean of the coordinates along the X axis
      in the data set. */
  virtual double meanCoord () const;

  /** Returns the standard deviation of the coordinates 
      along the X axis in the data set. */
  virtual double stdCoord () const;
  
  /** Returns the value at a given coordinate. */
  virtual double valueAt( double x ) const;
  
};

} // namespace hippodraw

#endif // _ProjectorHelper_H_
