/* -*- mode: c++; -*- */

/** @file

hippodraw::BinningProjector class interface

Copyright (C) 2002, 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinningProjector.h,v 1.87 2006/10/22 00:20:27 pfkeb Exp $

*/

#ifndef _BinningProjector_H_
#define _BinningProjector_H_

#include "ProjectorBase.h"

namespace hippodraw {

class BinnerAxis;
class BinsBase;

/** The BinningProjector is an abstract class provides most of the
    functionality for a projector that does some from of binning along
    the one or more axes.

    @author Stephane Bonneaud <gandalf@slac.stanford.edu>
    @author Xie Fang <xiefang@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/

class MDL_HIPPOPLOT_API BinningProjector : virtual public ProjectorBase
{

private:

  /** The number of AxesType accepted by the binner.  If the value is
      1, then only X is accepted.  A value of 2 accepts X and Y,
      etc. */
  unsigned int m_binner_dim;

protected:

  /** The binner object.  Some derived classes need a BinsBase object
      to accumulate data before the projection can be accomplished.
      Such classes should create the appropriate type of binner and
      store a pointer to it here.  A null pointer value signals that
      the derived class projector does not need a binner. */
  BinsBase * m_binner;

  /** The following constructor takes the number of @a axes used by
      the BinsBase object. */
  BinningProjector ( unsigned int axes );

  /** The copy constructor. */
  BinningProjector ( const BinningProjector & projector );

  /** Provides the all the data to the binner. */
  virtual void execute ();

  /** Checks and sets if an AxisModelBase object on an axis should be
      scaled. This method maybe overridden by derived classes such as
      those that do histogramming.  */
  virtual void checkScaling ();

public:

  /** The destructor. */
  virtual ~BinningProjector();

  /* overrides base class. */
  virtual void setAxisModel ( hippodraw::Axes::Type, AxisModelBase * );

  virtual const Range & setBinWidth ( hippodraw::Axes::Type axis, 
				      double width );

  virtual const Range & setBinWidth ( hippodraw::Axes::Type axis,
				      int parm, 
				      bool dragging );

  double getBinWidth ( hippodraw::Axes::Type axis ) const;

  virtual void setOffset ( const std::string & axis,
			   int parm,
			   bool dragging );
  virtual void setOffset ( hippodraw::Axes::Type axis, double offset );

  virtual void reset ();

  double getOffset ( hippodraw::Axes::Type axis ) const;

  /** Returns reference to BinsBase object used by this projector. */
  const BinsBase * getBinner ( ) const;

  /** Sets the BinsBase object to be used by this projector. */
  void setBinner ( BinsBase * bins );

  /** Sets the bin edge calculator to @a binner on axis @a axis. */
  virtual void setBinnerOn ( BinnerAxis * binner, 
			     hippodraw::Axes::Type axis );

  /** Returns the number of bins.  Returns number of bins if @a axis
      is binned, otherwise returns 0. 
  */
  virtual int getNumberOfBins ( hippodraw::Axes::Type axis ) const;

  /** Get the z value at the specified point (x,y). */
  virtual double getZValue ( double x, double y ) const;

  /** Returns an data source representing the binned values. 
   */
  virtual DataSource * createNTuple () const;

  /** Prepares the projector for plotting by executing, if needed, 
      the binning procedure. */
  virtual void prepareValues();

  /** Sets the Range of the binner. See documentation for derived
      classes for behavior of this member function.
   */
  virtual void setBinnerRange ( hippodraw::Axes::Type axis,
				const Range & range,
				bool const_width ) = 0;

  /** Sets a scale factor on the output so that the number of entries
      appears to be @a number.  This feature might be used, for
      example, to compare two histograms that have different number of
      entries by normalizing one to the other.
  */
  virtual void normalizeTo ( double number );

  /** Sets the scaling number of entries on if @a on is @c true,
      otherwise turns if off.
  */
  virtual void setNormalizing ( bool on );

  /** Normalize the output of the receiving object's projected values
      to that of @a target.
  */
  virtual void normalizeTo ( const ProjectorBase * target );

  /** Normalizes the projector to its target.
   */
  virtual void normalize ();

  /** Updates the receiving projector.  If the projector is normalizing to
      a target projector and @a object is the target, then re-normalizes
      itself.  If not, then notifies its observers.
  */
  virtual void update ( const Observable * object );

  /** If @a object is the target of normalization, removes the target
      and sets normalization off.
  */
  virtual void willDelete ( const Observable * object );

  /** Sets the contents of the bins from the data source.  

      @note This method should be called for derived classes which are
      static version of projection.  If called on a dynamic version,
      the contents of the bins will be refreshed from the data source
      with which it is bound.
   */
  void setBinContents ( const DataSource * source );

  /** Set the minimum entries/bin.
   */
  virtual void setMinEntries( int entries );

  /** Get the minimum entries/bin.
   */
  virtual int getMinEntries ( );
  
  /** Returns @c true if projected values are convertable to image.
      Returns @c true if number of binned axis is equal to two and if
      bins have equal widths on both axes, otherwise returns false.
      
      @request Allow axis to be on log scale, but this will require
      generating the X and Y coordinates for the image.
   */
  virtual bool isImageConvertable () const;

protected:

  virtual void fillDataSource ( DataSource * ntuple, 
				bool in_range = false ) const;

};

} // namespace hippodraw

#endif // _BinningProjector_H_
