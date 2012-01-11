/* -*- c++ -*- */

/** @file

CircularBuffer class interface

Copyright (C) 2003, 2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: CircularBuffer.h,v 1.11 2005/10/30 00:10:11 pfkeb Exp $

*/

#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include "NTuple.h"

namespace hippodraw {

/** CircularBuffer manager.  The is an NTuple that acts as a circular
    buffer.  That is, it fills rows to its maximum capacity, then
    start replacing rows from the first row to the end.  It repeats
    this behavior continuously.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API CircularBuffer : public NTuple
{

 private:

  /** The capacity.   The number of rows in the buffer. 
   */
  unsigned int m_capacity;

  /** The index to the next row to be added.
   */
  unsigned int m_next_row;

  /** A flog which when @c true, indicates the buffer has been filled
      to capacity. 
  */
  bool m_has_filled;

  /** The following assignment operator replicates the columns and the
      title to the left hand operand. */
  CircularBuffer & operator= ( const CircularBuffer & rhs );

  /** Increments the next row index.  Increments the next row index.
      If it reaches the capacity, then reset it to the first row and
      set the flag indicating the buffer has reached its capacity. 
  */
  void incrementRowIndex ();

 public:

  /** Constructs an CircularBuffer object with name @a name. 
   */
  CircularBuffer ( const std::string & name );

  /** Constructor taking a vector of column labels as argument. 
   */
  CircularBuffer ( const std::vector < std::string > & labels );

  /** The copy constructor creates an circular buffer that is a copy
      of an existing one. 
  */
  CircularBuffer ( const CircularBuffer & nt);

  /** The constructor creates an circular buffer with `n' columns. 
   */
  CircularBuffer ( unsigned int n );

  /** The default constructor creating an circular buffer  with 0 columns. 
   */
  CircularBuffer ();

  /** Clears the contents without changing the capacity.
   */
  virtual void clear();
  
  /** Adds a row to the buffer.  The size of the input vector must be
      the same as the width of the ntuple or an DataSourceException is
      thrown.  If the buffer has not yet been filled to capacity, add
      the row to the end of the buffer.  If the buffer has been filled
      to capacity, replaces the oldest row.
  */
  virtual void addRow ( const std::vector<double> & v );

  /** Sets the size, in rows, of the circular buffer.
  */  
  virtual void reserve ( unsigned int count );

};

} // namespace hippodraw

#endif // CIRCULARBUFFER_H
