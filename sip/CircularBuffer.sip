/* -*- mode: c++ -*- */

/** @file

This is the SIP interface definition for CircularBuffer

Copyright (C)  2003, 2004, 2006  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

This file is part of sihippo

$Id: CircularBuffer.sip,v 1.11 2006/10/07 17:58:09 pfkeb Exp $

@author Paul_Kunz@slac.stanford.edu

*/

class CircularBuffer : NTuple
{
%TypeHeaderCode
#include "datasrcs/CircularBuffer.h"
#include <string>
using hippodraw::CircularBuffer;

%End

public:
/** Constructs an CircularBuffer object with name @a name. 
   */
  CircularBuffer ( const std::string & );
  /** Constructor taking a vector of column labels as argument. 
   */
  CircularBuffer ( std::vector<std::string> & );
  /** The copy constructor creates an circular buffer that is a copy
      of an existing one. 
  */
  CircularBuffer ( const CircularBuffer & );
/** The constructor creates an circular buffer with `n' columns. 
   */
  CircularBuffer ( unsigned );
 /** The default constructor creating an circular buffer  with 0 columns. 
   */
  CircularBuffer ();

 /** Adds a row to the buffer.  The size of the input vector must be
      the same as the width of the ntuple or a std::runtime_error is
      thrown.  If the buffer has not yet been filled to capacity, add
      the row to the end of the buffer.  If the buffer has been filled
      to capacity, replaces the oldest row.
  */
  virtual void addRow ( const std::vector<double> & );

  /** Sets the size, in rows, of the circular buffer.
  */ 
  virtual void reserve ( unsigned );
  /** Clears the contents without changing the capacity.
   */
  virtual void clear ();

};
