/* -*- mode: c++ -*- */

/** @file

hippodraw::LineProjector class interface

Copyright (C) 2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: LineProjector.h,v 1.10 2007/05/16 18:15:51 pfkeb Exp $

*/

#ifndef _LineProjector_H_
#define _LineProjector_H_

#include "ProjectorBase.h"

#include "datasrcs/DataPointTuple.h"

namespace hippodraw {

/** A projector that plots a value as a horizontal or vertical line

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API LineProjector : public ProjectorBase
{

private:

  /** The value represented.
   */
  double m_value;

  /** The axis for the value.
   */
  hippodraw::Axes::Type m_axis;

  /** A label for the line.
   */
  std::string m_title;

  /** The copy constructor. */
  LineProjector( const LineProjector & );

public:

  /** The constructor taking the represented value as the argument.
   */
  LineProjector ( hippodraw::Axes::Type, double value );

  /** The virtual destructor.  It is responsible for deleting the @c
      Function objects. */
  virtual ~LineProjector();

  virtual ProjectorBase * clone();

  /* The required virtual functions. */
  virtual void addPointReps ();
  virtual bool isEmpty () const;
  virtual Range dataRangeOn ( hippodraw::Axes::Type ) const;
  virtual const std::string & getTitle() const;
  virtual Range valueRange () const;
  virtual double getPosOn ( hippodraw::Axes::Type ) const;
  virtual const std::string & getXLabel() const;
  virtual const std::string & getYLabel ( bool flag = false ) const;
  virtual int getNumberOfEntries () const;

  /** Returns the number of underflow. It is meaningless. */
  int getUnderflow () const;

  /** Returns the number of overflow. It is meaningless. */
  int getOverflow () const;

  virtual DataSource * createNTuple () const;

  virtual void prepareValues ();

  /** Sets the value that will be projected.
   */
  void setValue ( double value );

protected:

  virtual void fillProjectedValues ( DataSource * ntuple, 
				     bool in_range = false ) const;

};

} // namespace hippodraw

#endif // _LineProjector_H_
