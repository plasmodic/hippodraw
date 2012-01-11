// -*- mode:c++ -*-

/** @file

This is the SIP interface definition for LineDataRep

Copyright (C)  2004, 2005  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

This file is part of sihippo

$Id: LineDataRep.sip,v 1.4 2007/02/24 22:24:38 pfkeb Exp $

Author: Paul_Kunz@slac.stanford.edu

*/

class LineDataRep : DataRep
{
%TypeHeaderCode
#include "datareps/LineDataRep.h"
using hippodraw::LineDataRep;
using hippodraw::RepBase;
using hippodraw::DataSource;
using hippodraw::NTuple;
%End

protected:

public:
//  /** A constructor taking the value and axis arguments.
//    */
//   LineDataRep ( hippodraw::Axes::Type, double );

  /** The default constructor, should never be called.
   */
  LineDataRep ();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual DataRep * clone ();

  /** Returns @c false as this class does not have NTuple bindings. */
  virtual bool hasNTupleBindings () const;

  /** Sets the value that will be displayed.
   */
  void setValue ( double );



};
