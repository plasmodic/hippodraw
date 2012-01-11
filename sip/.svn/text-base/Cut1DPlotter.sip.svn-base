// -*- mode:c++ -*-

/** @file

This is the SIP interface definition for Cut1DPlotter

Copyright (C) 2003, 2005  The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

This file is part of sihippo

$Id: Cut1DPlotter.sip,v 1.8 2007/02/24 22:24:38 pfkeb Exp $

Author: Paul_Kunz@slac.stanford.edu

*/


class Cut1DPlotter : CutPlotter
{
%TypeHeaderCode
#include "graphics/ViewBase.h"
using hippodraw::ViewBase;
#include "plotters/Cut1DPlotter.h"
using hippodraw::Cut1DPlotter;
using hippodraw::DataRep;
using hippodraw::RepBase;
using hippodraw::Color;
using hippodraw::NTuple;
%End


public:

 /** This default constructo. */
  Cut1DPlotter ( );

  /** The copy constructor. */
  Cut1DPlotter ( const Cut1DPlotter &  );

  /** The destructor. */
  virtual ~Cut1DPlotter ();

  /** This clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  PlotterBase * clone();

  virtual void setAxisBinding ( const std::vector < std::string > & );

  /** Sets the cut Range to include the full range of data.  @todo The
      shading showing the cut range should go to the end of the last
      bin and not just to the end of the last data.
  */
  void setCutRangeFull ();

  /** Reverse the meaning of the cut Range. */
  virtual void toggleInverted ();

  /** Returns the label for the Cut.
   */
  virtual const std::string & getCutLabel () const;

  void setCutRange ( double, double );

};
