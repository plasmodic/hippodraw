/* -*- mode: c++; -*- */

/** @file

hippodraw::AxisModelBase class interface

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: AxisModelBase.h,v 1.126 2007/01/05 03:03:26 xiefang Exp $ 

*/

#ifndef _AxisModelBase_H_
#define _AxisModelBase_H_

#include "AxisLoc.h"
#include "Range.h"

namespace hippodraw {

class AxisTick;

/** The AxisModelBase class maintains the Range and scaling of an axis.

    @author Stephane Bonneaud <gandalf@slac.stanford.edu>
    @author Kaustuv <kaustuv@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket Malde <sanket@stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/

class MDL_HIPPOPLOT_API AxisModelBase
{
protected:

  /** The current range of the axis. */
  Range m_range;

  /** The scaled range. */
  mutable Range m_scaled_range;

  /** The starting range before start of dragging events.  While
      dragging, the setLow or setHigh member functions use this Range
      value rather than the current value to set the new range. */
  Range m_start_range;

  /** The current dragging state. */
  bool m_is_dragging;

  /** The auto-range flag.  When set @c true, the range of the axis is
      set to include all projected values. */
  bool m_auto_range;

  /** A flag to indicate whether the range is empty. */
  bool m_empty;

  /** The scale factor.  For normal displays, the value is 1.0.
      However, for something like histogram displays, the value will
      be the bin width.  This is done so that instead of displaying
      the density ,the number of entries will be displayed. */
  double m_scale_factor;

  /** If @c true, the axis is being scaled. */
  bool m_scaling_on;

  /** The position of the first tick.
      @todo Maybe it is not needed. 
  */
  double m_first_tick;
  
  /** The distance between two consecutive ticks.  It has been scaled
      by the scale factor. */
  double m_tick_step;
  
  /** The maximum number of ticks. */
  const int m_max_ticks;

  //The actual program doesn't represent nor support any minor
  //ticks.
  int m_num_minor_ticks; // The Number of minor ticks between the major ones.

  AxisLoc m_label_location;
  AxisLoc m_scale_location;
  
  /** The value of the power of ten to be used to multiply the tick
      labels.  It has been scaled by the scale factor. */
  double m_pmag;

  /** A flag to indicate that one will using scientific notation.
      That is the member m_pmag becomes significant in the display of
      the tick labels. */
  bool m_use_pmag;
  
  /** The value of the power of ten of the range to be used to
      determine how many decimal places are needed. */
  double m_rmag;

  /** The list of generated ticks. */
  std::vector<AxisTick> m_ticks;

  /** Only work for non auto_ticks. Return the ticks in range.*/
  std::vector<AxisTick> m_ticks_in_range;
  
  /** A flag set to @c true if ticks are being automatically
      generated.   
  */
  bool m_auto_ticks;

  /** Sets the member m_start_dragging to the current range if
      dragging is starting. */
  void startDragging ( bool dragging );
  
public:

  /** The constructor sets the location of the ticks, labels, and
      scale. */
  AxisModelBase ( AxisLoc label, AxisLoc scale );

  /** The copy constructor. */
  AxisModelBase( const AxisModelBase & axis_model );
  
  /** The virtual destructor. */
  virtual ~AxisModelBase();

  /** Returns a copy of the object. */
  virtual AxisModelBase * clone () const = 0;


  /** Sets the tick step */ 
  void setTickStep( const double & t_step );
  
  /** Returns the tick step in the true coordinate system.  It may be
      scaled internally. */
  double getTickStep() const;

  /** Sets the value for first tick step */ 
  void setFirstTick( const double & first_tick );
 
  /** Returns the value for the first tick step */ 
  double getFirstTick() const;

  /** Returns the value for maximum number of ticks */
  double getMaxTicks() const;
  
  /** Sets the magnitude of the range */ 
  void setRMag ( const double & rmag );
  
  /** Sets the magnitude of the range */ 
  double getRMag() const;

  /** Sets the magnitude  of the power of ten for the tick labels.*/ 
  void setPMag( const double & pmag );
  
  /** Returns the magnitude of the power of ten for the tick labels. */
  double getPMag() const;


  /** Sets the Range to the @a low and @a high values. */
  void setRange ( double low, double high, double pos );

  /** The function which changes the m_range member. The only possible
      user is the plotter object. It sets the range equal to the range
      of the projector object. */
  void setRange ( const Range &, bool scaled = false );

  /** Sets the Range to overlap of the two ranges. */
  void setIntersectRange ( const Range &, const Range & );

  /** The function which changes the pos member of m_range. */
  void setRangePos ( double );

  /** Sets the range to be the union of the existing range and @a
      range.  If the current range is empty, then set the range to @a
      range. */
  void setUnionRange ( const Range & range );

  /** Sets the range to empty. */
  void setEmpty ();

  /** Returns the range represented by this @c AxisModel. The range is
      multiplied by the scale factor if scaled is true. 

      @todo Really should have separate function, getScaledRange, so
      at the calling point it would be clear.
  */  
  const Range & getRange ( bool scaled ) const;

  /** Sets the auto-ranging flag to @a flag. */
  void setAutoRanging ( bool flag );

  /** Returns @c true if auto-ranging is enabled; otherwise, returns @c
      false. */
  bool isAutoRanging ( ) const;

  /** Sets the value of the scale factor. */
  void setScaleFactor ( double );

  /** Returns the scale factor.  Returns the scale factor if isScaling
      returns @c true, otherwise returns 1.0.  @sa #m_scale_factor. */
  double getScaleFactor () const;

  /** Returns @c true if the axis is being scaled.  Use this member
      instead of testing against @c 1.0 since 1.0 is a valid scale
      factor. */
  bool isScaling () const;

  /** Sets axis scaling on if @a on is @c true. */
  void setScaling ( bool on = true );

  /** The following functions are used by the AxisRepBase *
   *  object. They enable the user to access the member fields needed to do
   *  the actual drawing.  What they are doing is pretty obvious, so
   *  no (or very little) documentation is added. 
   */

  /** Returns @c true if the labels generated for the ticks assume that
      the magnitude of the exponent will be displayed.  The base class
      implementation returns @c false.*/
  virtual bool needPMag () const;

  /** Use to set the value of the member variable m_use_pmag */
  virtual void setUsePMag( const bool & use_p_mag );
  
  /** Returns a boolean describing the type of the scale of the
      axis. False for the linear scale and true for the logarithmic
      one. */
  virtual bool isLog() const = 0;

  AxisLoc getLabelLocation() const;
  AxisLoc getScaleLocation() const;

  /** Sets the ticks for plotting. */
  void setTicks( const std::vector<AxisTick> & ticks);

  /** Returns a reference of generated ticks. 
      If in non_auto_ticks mode, return only ticks in range.
  */
  const std::vector<AxisTick> & getTicks() const;

  /** Adjust ticks in non_auto_tick mode. This method should be called
      whenever m_range or m_ticks is changed AND m_auto_tick is false.
  */
  void adjustTicks();

  /** Sets flag to determine if tick positions should be automatically
      generated or not.
  */
  void setAutoTicks ( bool yes );

  /** Returns @a true if position of the ticks should be automatically
      generated.
  */
  bool isAutoTicks () const;

  /** Adjusts the range on the axis to nice values within the Range @a
      limit. */
  virtual const Range & adjustValues ( const Range & limit ) = 0;

  /** Adjust the range for nice logging. */
  virtual const Range & adjustLogValues() = 0;

  /** Sets the low end of the Range and returns the new Range.  The
      parameter @a parm should be in the range 0 to 100. A value of 0
      will leave the low end of the range unchanged, while a value of
      100 will set it to value of the high. A derived class will
      interpret the values in between so that the appearance is
      linear. */
  virtual Range calcLow ( int parm, bool dragging = false ) = 0;

  /** Sets the high end of the Range and returns the new Range.  The
      parameter @a parm should be in the range 0 to 100. A value of 0
      will high end of the range to the low end, while a value of
      100 will leave the value unchanged. A derived class will
      interpret the values in between so that the appearance is
      linear. */
  virtual Range calcHigh ( int parm, bool dragging = false ) = 0;
  
};

} // namespace hippodraw

#endif // _AxisModel_H_
