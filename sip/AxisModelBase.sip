/* -*- mode:c++ -*- */

/** @file

This is the SIP interface definition for AxesModelBase

Copyright (C)  2002, 2004  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

$Id: AxisModelBase.sip,v 1.4 2007/02/24 22:24:38 pfkeb Exp $

@author Paul_Kunz@slac.stanford.edu

*/

class AxisModelBase /Abstract/
{
%TypeHeaderCode
#include "axes/AxisModelBase.h"
using hippodraw::AxisModelBase;
%End

public:

  /// AxisModelBase ();
  virtual ~AxisModelBase ();

};
