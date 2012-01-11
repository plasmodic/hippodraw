/* -*- mode: c++ -*- */

/** @file

hippodraw::AxisModelLinear class interface

Copyright (C) 2000, 2002-2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: AxisModelLinear.h,v 1.56 2005/12/05 18:47:47 pfkeb Exp $ 
*/

#ifndef _AxisModelLinear_H_
#define _AxisModelLinear_H_

#include "AxisModelBase.h"

namespace hippodraw {

/** Given a range, calculates a model for an axis with a linear scale.

    @author Stephane Bonneaud <gandalf@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Oded Wurman <oded@slac.stanford.edu>
 */

class MDL_HIPPOPLOT_API AxisModelLinear : public AxisModelBase
{
public:

  /** The constructor sets the location of the ticks, labels, and
      scale. */
  AxisModelLinear ( AxisLoc label, AxisLoc scale );

  /** The copy-like constructor allows conversion from the log axis
      model. */
  AxisModelLinear( const AxisModelBase & axis_model );

  /** The destructor. */
  ~AxisModelLinear();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual AxisModelBase * clone() const;

  virtual bool needPMag () const;

  /** Returns false because this class represents a linear scale. */
  virtual bool isLog() const;

  /** Sets the number of minor ticks that are between the major
     ones.  */
  virtual const Range & adjustValues ( const Range & );

  /** Adjusts the range for nice logging. */
  virtual const Range & adjustLogValues();

  virtual Range calcLow  ( int parm, bool dragging = false );
  virtual Range calcHigh ( int parm, bool dragging = false );

};

} // namespace hippodraw

#endif // _AxisModelLinear_H_
