/* -*- mode: c++ -*- */

/** @file

XYPlot classes interface.

Copyright (C) 2000-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: XYPlot.h,v 1.49 2007/07/26 21:44:34 pfkeb Exp $

*/

#ifndef _XYPlot_H_
#define _XYPlot_H_

#include "datareps/DataRep.h"

namespace hippodraw {

/** Displays X Y pairs with or without error bars.   An example is shown below

    @image html xyplot.png "XYPlot data representation"
    @image latex xyplot.eps "XYPlot data representation"

    @author Stephane Bonneaud <gandalf@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket B. Malde <sanket@stanford.edu>
*/

class MDL_HIPPOPLOT_API XYPlot
  : public hippodraw::DataRep
{
 public:

  /** The default constructor. */
  XYPlot ( );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual DataRep * clone();

  bool acceptFunction ( int num );

  /** Returns @c true.  Returns @c true SymbolPointRep class used by
      this class are capable of displaying error on the data points.
  */
  virtual bool hasErrorDisplay () const;

  /** Sets the point representation.  Sets the point representation to
      @ point_rep as DataRep::setRepresentation does.  Also sets the
      error display if appropriate.
   */
  virtual void setRepresentation ( RepBase * point_rep );

  /** Sets the axis bindings.  Sets binding of the axis with name axis
      to the NTuple column with label label.  If @a axis is one of the
      error bar axes, then also set the error display appropriately.
   */
  virtual void setAxisBinding ( const std::string & axis,
				const std::string & label );

  /** Sets the axis bindings.  Sets binding all axes to the NTuple
      column with label in the vector.  Also set the error display flag
      in the SymbolPointRep.
   */
  virtual 
  void setAxisBindings ( const std::vector < std::string > & bindings );

  virtual bool hasAxis ( hippodraw::Axes::Type ) const;

  /** Returns @c true to allow zooming on Y asis. */
  virtual bool hasZoomY () const;
};


} // namespace hippodraw

#endif // _XYPlot
