/* -*- mode: c++ -*- */

/** @file

hippodraw::ErrorBarRep class interface

Copyright (C) 2000-2003, 2005, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: ErrorBarRep.h,v 1.30 2006/06/10 17:18:41 pfkeb Exp $

*/

#ifndef _ErrorBarRep_H_
#define _ErrorBarRep_H_

#include "RepBase.h"

#include "axes/Range.h"

namespace hippodraw {

/** A rep to draw error bars, owned by point reps.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu> 
*/
class MDL_HIPPOPLOT_API ErrorBarRep : public RepBase
{

private:

  /** A cache of the X errors. @sa SymbolPointRep#m_x */
  std::vector< double > m_x_err;

  /** A cache of the Y errors. @sa SymbolPointRep#m_x */
  std::vector< double > m_y_err;

  /** The X error flag. If set @c true, error bars along the X axis
      are generated. */
  bool m_x_flag;

  /** The Y error flag. If set @c true, error bars along the Y axis
      are generated. */
  bool m_y_flag;

  /** The X axis range for cap calculation. */
  Range m_x_range;

  /** The Y axis range for cap calculation. */
  Range m_y_range;

 
  /** Creates the line segment for the X error bar.
   */
  void drawXError ( double x, double y, double error,
		    const TransformBase & t );

  /** Creates the line segment for the Y error bar.
   */
  void drawYError ( double x, double y, double error,
		    const TransformBase & t );

 public:

  /** The default constructor.  @note The copy constructor does not
      need to be implemented as the compiler create one works and
      saves 1.4KB in the object code.
  */
  ErrorBarRep ( );

//  Take the default copy constructor
//   ErrorBarRep ( const ErrorBarRep & point_rep );

  /** The destructor. */
  ~ErrorBarRep();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  RepBase * clone ();

  /** Sets the flag to plot the errors on the X axis. */
  void setXError ( bool );

  /** Sets the flag to plot the errors on the Y axis. */
  void setYError ( bool );

  virtual void drawProjectedValues ( const DataSource * ntuple,
				     TransformBase * transform,
				     ViewBase * view );

private:

  /** Gets the ranges of the plotter on X and Y axis.
   */
  void getRanges ( const ViewBase * view );

};

} // namespace hippodraw

#endif // _ErrorBarRep_H_
