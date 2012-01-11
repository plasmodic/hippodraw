/* -*- mode: c++ -*- */

/** @file

hippodraw::Bins2DHist class interface

Copyright (C)  2000-2006  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: Bins2DHist.h,v 1.73 2006/08/23 18:14:11 pfkeb Exp $

*/

#ifndef _Bins2DHist_H_
#define _Bins2DHist_H_

#include "binners/Bins2DBase.h"

namespace hippodraw {

/** The class for the 2D histogram binning. 

    @author Stephane Bonneaud <gandalf@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Matan Shacham <matan@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu> 
*/
class MDL_HIPPOPLOT_API Bins2DHist : public Bins2DBase
{

private:

  /** The accumulated square of the weights.  Use [X][Y]. */
  std::vector<std::vector<double> > m_variance;

  /** The X moments. */
  double m_x_moments[3];

  /** The Y moments. */
  double m_y_moments[3];
 
public:

  /** The default constructor. */
  Bins2DHist ( );

  /** The copy constructor. */
  Bins2DHist ( const Bins2DHist & binner );

  /** The destructor. */
  ~Bins2DHist ();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  BinsBase * clone () const;

  virtual void setNumberOfBins ( hippodraw::Axes::Type axis, int number );

  /** Resets the accumulation to zero. */
  virtual void reset();

  /** Accumulates the data point (@a x, @a y) with weight @a w.  The
      remaining argument is ignored.*/
  virtual void accumulate ( double x, double y, 
			   double w = 1.0, double dummy = 0. );

  /** Get the z value at the specified point (x,y). */
  virtual double getZValue ( double x, double y ) const;

  virtual NTuple * createNTuple () const;
  virtual void fillDataSource ( DataSource * ntuple ) const;
  virtual void setBinContents ( const DataSource * ntuple );

};

} // namespace hippodraw

#endif // _Bins2DHist_H_
