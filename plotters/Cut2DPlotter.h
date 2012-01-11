/* -*- mode: c++ -*- */

/** @file

hippodraw::Cut2DPlotter class interface

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Cut2DPlotter.h,v 1.25 2006/10/10 19:22:50 pfkeb Exp $

*/

#ifndef _Cut2DPlotter_H_
#define _Cut2DPlotter_H_

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

    @request @@@@@@ Need better way to show 2D cut when the 2D color
    plot is full of points with no empty squares.  Perhaps draw
    rectangle on top of plot.

    @request @@@@@@ Put 1 pt wide rectangle around selected region so it can
    be seen even if background is full.

    @author Paul_Kunz@slac.stanford.edu 
*/
class MDL_HIPPOPLOT_API Cut2DPlotter: public CutPlotter
{

public:

  /** This default constructor. */
  Cut2DPlotter ( );

  /** The copy constructor. */
  Cut2DPlotter ( const Cut2DPlotter & plotter );

  /** The destructor. */
  virtual ~Cut2DPlotter ();

  /** This clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  PlotterBase * clone();


  /** Adds a data representation.  Adds the data representation that
      will display the cut.
  */
  virtual void addDataRep ( hippodraw::DataRep * rep );

  /** Adds tuple cut after adding a data representation. */
  virtual void addTupleCut( hippodraw::DataRep * rep );

  virtual void setAxisBinding ( const std::vector < std::string > & bindings);

  /** Sets the cut Range to include the full range of data.  @todo The
      shading showing the cut range should go to the end of the last
      bin and not just to the end of the last data.
  */
  void setCutRangeFull ();

  /** Reverse the meaning of the cut Range on the give axis. */
  virtual void toggleInverted (  );

  /** Cut the range of the cut at Y axis. */
  Range getCutRangeY ( );

};

} // namespace hippodraw

#endif // _Cut2DPlotter_H_
