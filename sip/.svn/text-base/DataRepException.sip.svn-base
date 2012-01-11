/* -*- mode:c++ -*- */

/** @file

SIP interface definition for DataRepException

Copyright (C)  2003, 2005  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

$Id: DataRepException.sip,v 1.6 2007/02/24 22:24:38 pfkeb Exp $

@author <Paul_Kunz@slac.stanford.edu>

*/


class DataRepException 
{
%TypeHeaderCode
#include "datareps/DataRepException.h"
using hippodraw::DataRepException;
%End


public:
/** Constructor taking the bad binding lable @a type as argument.*/
  DataRepException ( const std::string & );

  /** The copy constructor.  @attention Although the implementation is
      the same as the default copy constructor, GCC 2.95.3 needs an
      implementation else the exception is not caught.  It is not
      needed by GCC 3.2.1.
  */
  DataRepException ( const DataRepException & );

 /** The virtual destructor.  Appears that the required declaration
      has to be the same as the base class, according to GCC 3.2.1. */
  virtual ~DataRepException () throw ();


 /** The standard method to return a string giving message about the
      cause of the exception. */
  const char * what () const throw ();

  char * __str__();
// %MemberCode
%MethodCode
    return PyString_FromString ( sipCpp->what() );
%End

};
