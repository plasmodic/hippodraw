/* -*- mode: c++ -*- */

/**

SIP interface definition for  FunctionFactory

Copyright (C) 2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: FunctionFactory.sip,v 1.2 2007/02/24 22:24:38 pfkeb Exp $

*/

class FunctionFactory
{
%TypeHeaderCode
#include "functions/FunctionFactory.h"
using hippodraw::FunctionFactory;
%End


 private:

  /** A private copy constructor to avoid copying. */
  FunctionFactory  ( );

 public:

  static FunctionFactory * instance ();

  const std::vector < std::string > & names ();
  void add ( FunctionBase * );

};
