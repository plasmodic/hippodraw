/*-*- mode: c++ -*- */

/** @file

hippodraw::Bins2DProfile  class interface

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Bins2DProfile.h,v 1.43 2006/08/23 18:14:11 pfkeb Exp $

*/

#ifndef _Bins2DProfile_H_
#define _Bins2DProfile_H_

#include "Bins2DBase.h"

#include "axes/Range.h"

namespace hippodraw {

class DataSource;

/** The class for the 2D profile binner hierarchy.

    @author Matan Shacham <matan@slac.stanford.edu> 
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API Bins2DProfile : public Bins2DBase
{

protected:

  /** The accumulated sum of the z variable in each bin.
      The matrix structure is identical to m_data.*/
  std::vector<std::vector<double> > m_sumZ;

  /** The accumulated square of the weights.  Use [X][Y]. */
  std::vector<std::vector<double> > m_variance;
  
  /** The range of the z axis.  Note that the z axis does not need a
      binner axis object because it is not binned.*/
  Range m_z_range;


public:

  /** The default constructor. */
  Bins2DProfile ( );

  /** The copy constructor. */
  Bins2DProfile( const Bins2DProfile & binner );

  /** The destructor. */
  virtual ~Bins2DProfile ();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  BinsBase * clone () const;

  virtual void setNumberOfBins ( hippodraw::Axes::Type axis, int nb );

  /** Resets the accumulation to zero. */
  virtual void reset();

  /** Accumulates the data point (@a x,@a y, @a z) with weight @a w. */
  virtual void accumulate( double x, double y, double z, double w = 1.0 );

  /** Get the z value at the specified point (x,y). */
  virtual double getZValue ( double x, double y ) const;

  virtual NTuple * createNTuple () const;
  virtual void fillDataSource ( DataSource * ntuple ) const;
  virtual void setBinContents ( const DataSource * ntuple );

};

} // namespace hippodraw

#endif // _Bins2DProfile_H_
