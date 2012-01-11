/* -*- mode: c++ -*- */

/** @file

ContourFunctionRep class interface

Copyright (C) 2005   The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: ContourFunctionRep.h,v 1.4 2005/10/30 00:10:17 pfkeb Exp $

*/

#ifndef _ContourFunctionRep_H_
#define _ContourFunctionRep_H_

#include "ContourPointRep.h"

namespace hippodraw {

class FunctionBase;
class NTuple;
class TransformBase;
class ViewBase;

/** A class that draws a function as contour lines.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API ContourFunctionRep : public ContourPointRep
{

private:

  /** The table of function values from which to build the contours.
   */
  NTuple * m_ntuple;

  /** The number of steps taken to calculate function values.
   */
  unsigned int m_steps;

  /** The copy constructor. */
  ContourFunctionRep( const ContourFunctionRep & point_rep );

public:

  /** The default constructor. */
  ContourFunctionRep();
  
  /** The destructor. */
  ~ContourFunctionRep();

  /** Draws a two dimension; function as contour lines.
   */
  void drawProjectedValues ( const Range & x_range, const Range & y_range,
			     const FunctionBase * function, 
			     TransformBase * transform,
			     ViewBase * view );

};

} // namespace hippodraw

#endif // _ContourFunctionRep_H_
