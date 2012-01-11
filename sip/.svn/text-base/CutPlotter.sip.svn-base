// -*- mode:c++ -*-

/** @file

This is the SIP interface definition for Cut1DPlotter

Copyright (C) 2006  The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

This file is part of sihippo

$Id: CutPlotter.sip,v 1.5 2007/02/24 22:24:38 pfkeb Exp $

Author: Paul_Kunz@slac.stanford.edu

*/


class CutPlotter : XyPlotter
{
%TypeHeaderCode
#include "graphics/ViewBase.h"
using hippodraw::ViewBase;
#include "plotters/CutPlotter.h"
using hippodraw::CutPlotter;
using hippodraw::NTuple;
using hippodraw::DataRep;
using hippodraw::RepBase;
using hippodraw::Color;
%End


public:

 /** This default constructo. */
//  CutPlotter ( );

  /** The copy constructor. */
  CutPlotter ( const CutPlotter &  );

  /** The destructor. */
  virtual ~CutPlotter ();

  /** This clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  PlotterBase * clone();

  /** Sets the cut Range to include the full range of data.  @todo The
      shading showing the cut range should go to the end of the last
      bin and not just to the end of the last data.
  */
  virtual void setCutRangeFull () = 0;

  /** Reverse the meaning of the cut Range. */
  virtual void toggleInverted () = 0;

  void setCutRange ( double, double );

};
