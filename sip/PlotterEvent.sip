// -*- mode:c++ -*-
// This is the SIP interface definition for PlotterEvent
//
// Copyright (C)  2003  The Board of Trustees of  
// The Leland Stanford Junior University.  All Rights Reserved.
//
// This file is part of sihippo
//
// $Id: PlotterEvent.sip,v 1.5 2007/02/24 22:24:38 pfkeb Exp $
//
// Author: Paul_Kunz@slac.stanford.edu
//

class PlotterEvent : QCustomEvent
{
%TypeHeaderCode
#include "qt/PlotterEvent.h"
using hippodraw::PlotterEvent;
using hippodraw::PlotterBase;
%End

public:

  PlotterEvent ( PlotterBase * );

  /** The copy constructor. */
  PlotterEvent ( const PlotterEvent & );
  
  /** The required virtual destructor. */
  virtual ~PlotterEvent ();  

  /** Returns the PlotterBase object that created the update event. 
   */
  PlotterBase * plotter () const;


};
