/* -*- mode: c++ -*- */

/** @file

Copyright (C) 2000-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: StripChartProjector.h,v 1.28 2005/10/30 00:10:14 pfkeb Exp $

*/

#ifndef _StripChartProjector_H_
#define _StripChartProjector_H_

#include "projectors/Map2Projector.h"

namespace hippodraw {

/*** A derived class of Map2Projector that maps 2 DataSource columns
     to the projection with the initial x projection value being the
     smallest value in the data source column bound to the x
     projection.

     @bug If the Y axis points go out of range, the plot becomes in
     correct in that the lines to no go to the range border.

     @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
     @author Matan Shacham <matan@slac.stanford.edu>
 */
class MDL_HIPPOPLOT_API StripChartProjector : public Map2Projector
{

private:

  /** The copy constructor. */
  StripChartProjector ( const StripChartProjector & projector );

  /** Function to add the acceptable point reps. */
  virtual void addPointReps ();

 public:

  /** The following default constructor binds to the first two columns. */
  StripChartProjector();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  ProjectorBase * clone ();

  /** Fills the data source with projected values. Fills the NTuple with
      projected values starting with the one with the smallest X
      value.
   */
  virtual void fillProjectedValues ( DataSource * ntuple, 
				     bool in_range = false ) const;

};

} // namespace hippodraw

#endif // _StripChartProjector_H_
