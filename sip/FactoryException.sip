// -*- mode:c++ -*-
//
// This is the SIP interface definition for FactoryException
//
// Copyright (C)  2003  The Board of Trustees of  
// The Leland Stanford Junior University.  All Rights Reserved.
//
// This file is part of sihippo
//
// $Id: FactoryException.sip,v 1.4 2007/02/24 22:24:38 pfkeb Exp $
//
// Author: Paul_Kunz@slac.stanford.edu
//

class FactoryException 
{
%TypeHeaderCode
#include "pattern/FactoryException.h"
using hippodraw::FactoryException;
%End


public:

  FactoryException ( const std::string & );
  const char * what () const throw ();

  char * __str__();
%MethodCode
    return PyString_FromString ( sipCpp->what() );
%End

};
