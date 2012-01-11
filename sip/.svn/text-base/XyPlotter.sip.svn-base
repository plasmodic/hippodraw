// -*- mode:c++ -*-

/** @file

This is the SIP interface definition for XyPlotter

Copyright (C) 2005, 2006  The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

This file is part of sihippo

$Id: XyPlotter.sip,v 1.6 2007/02/24 22:24:38 pfkeb Exp $

Author: Paul_Kunz@slac.stanford.edu

*/

class XyPlotter : PlotterBase
{
%TypeHeaderCode
#include "plotters/XyPlotter.h"
using hippodraw::XyPlotter;
using hippodraw::ViewBase;
using hippodraw::NTuple;
using hippodraw::DataRep;
using hippodraw::Color;
using hippodraw::RepBase;
%End

public:

 /** This default constructo. */
  XyPlotter ( );

  /** The copy constructor. */
  XyPlotter ( const XyPlotter &  );

  /** The destructor. */
  virtual ~XyPlotter ();

  /** This clone function returns an object of its own kind which
      is a copy of this object at this moment
      */
  PlotterBase * clone();

  virtual void addValues ( const std::vector < double > & );
  virtual NTuple * createNTuple () const /Factory/;
  virtual void drawIn ( ViewBase * );
  virtual DataRep * getDataRep ( int );
  virtual int getNumDataReps () const;
  virtual const Color & repColor () const;
  virtual RepBase * representation () const;
  virtual void setRepColor ( const Color & );
  virtual void setRepresentation ( RepBase * );
  virtual void update ();

};
