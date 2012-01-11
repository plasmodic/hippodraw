/* -*- mode: c++ -*- */

/** @file

Cut1DPlotter class interface

Copyright (C) 2000-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Cut1DPlotter.h,v 1.28 2006/02/17 19:58:07 pfkeb Exp $

*/

#ifndef _Cut1DPlotter_H_
#define _Cut1DPlotter_H_

#include "CutPlotter.h"

namespace hippodraw {

/** A @c Plotter class that plots one-dimensional TupleCut values. It
    servers two purposes: to display the accepted range of the cut and
    to maintain the cut itself. This class adds the cut acceptance
    range to the background of the inherited XyPlotter drawing. It
    maintains a list of DataRep objects which are the target of the
    cut.  If the cut parameters change, it sets the target DataRep
    objects dirty so that they will re-display themselves. It
    contains a single TupleCut object and is the public interface for
    setting its properties.

    @author Kaustuv <Kaustuv@stanford.edu>
    @author Paul_Kunz@slac.stanford.edu 
*/
class MDL_HIPPOPLOT_API Cut1DPlotter : public CutPlotter
{

public:

  /** This default constructor. */
  Cut1DPlotter ( );

  /** The copy constructor. */
  Cut1DPlotter ( const Cut1DPlotter & plotter );

  /** The destructor. */
  virtual ~Cut1DPlotter ();

  /** This clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  PlotterBase * clone();

  /** Adds a data representation.  Adds the data representation that
      will display the cut.
  */
  virtual void addDataRep ( hippodraw::DataRep * rep );

  /** Adds tuple cut after adding a data representation. */
  virtual void addTupleCut( hippodraw::DataRep * rep );

  virtual void setAxisBinding ( const std::vector < std::string > & bindings );

  /** Sets the cut Range to include the full range of data.  @todo The
      shading showing the cut range should go to the end of the last
      bin and not just to the end of the last data.
  */
  void setCutRangeFull ();

  /** Reverse the meaning of the cut Range. */
  virtual void toggleInverted ();

  /** Returns the label for the Cut.
      @deprecated Use fillCuts instead.
  */
  virtual const std::string & getCutLabel () const;

};

} // namespace hippodraw

#endif // _Cut1DPlotter_H_
