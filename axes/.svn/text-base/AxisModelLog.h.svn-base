/* -*- mode: c++ -*- */

/** @file

AxisModelLog class interface

Copyright (C) 2000-2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: AxisModelLog.h,v 1.54 2005/10/30 00:10:09 pfkeb Exp $ 

*/

#ifndef _AxisModelLog_H_
#define _AxisModelLog_H_

#include "AxisModelBase.h"

namespace hippodraw {

/** Given a range, calculates a model for an axis with a log scale.

    @author Stephane Bonneaud <gandalf@slac.stanford.edu> 
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu> 
    @author Oded Wurman <owurman@stanford.edu>
*/
class MDL_HIPPOPLOT_API AxisModelLog : public AxisModelBase
{

  /** Sets the tick step, and the magnitude power magnitude (the
      greatest magnitude that either the maximum or minimum reaches,
      as well as the magnitude in the range length. */
  virtual void setTickStep();

 public:

  /** The constructor sets the location of the ticks, labels, and
      scale. */
  AxisModelLog ( AxisLoc label, AxisLoc scale );

  /** The copy-like constructor allows conversion from the linear axis
      model. */
  AxisModelLog( const AxisModelBase & axis_model );

  /** The destructor. */
  ~AxisModelLog();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual AxisModelBase * clone() const;

  /** Returns true because this class represents a log scale. */
  bool isLog() const;

  virtual const Range & adjustValues ( const Range & );

  /** This function makes log values logical on a logarithmic scale. */
  virtual const Range & adjustLogValues();

  /** This returns the next step for the ticks. */
  double nextStep( double cur );

  /** This returns the previous step for the ticks. */
  double prevStep( double cur );

  virtual Range calcLow  ( int parm, bool dragging = false );
  virtual Range calcHigh ( int parm, bool dragging = false );

};

} // namespace hippodraw

#endif // _AxisModelLog_H_
