/* -*- mode: c++ -*- */

/** @file

StripChart class interface

Copyright (C)  2000-2005, 2007  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: StripChart.h,v 1.44 2007/07/26 21:44:34 pfkeb Exp $
*/

#ifndef _StripChart_H_
#define _StripChart_H_

#include "datareps/DataRep.h"

namespace hippodraw {

/** A derived class of DataRep which functions as a strip chart.  It
    differs from XYPlot in that the initial points are taken from the
    row with the smallest value in the ntuple column that is bound to
    the x projection.  By connecting the points with a line, it
    simulates a strip chart.  It thus depends that its @ref
    overview_projector will return the @ref overview_projectedvalue
    objects in order of increasing X value. 

    An example is shown below

    @image html stripchart.png "StripChart data representation"
    @image latex stripchart.eps "StripChart data representation"

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket B. Malde <sanket@stanford.edu>
*/
class MDL_HIPPOPLOT_API StripChart
  : public hippodraw::DataRep
{
 public:

  /** The default constructor. */
  StripChart ( );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual DataRep * clone ();

  virtual bool acceptFunction ( int num );

  virtual bool hasAxis ( hippodraw::Axes::Type ) const;

  /** Returns @c true to allow zooming on Y axis. */
  virtual bool hasZoomY () const;

};

} // namespace hippodraw

#endif // _StripChart
