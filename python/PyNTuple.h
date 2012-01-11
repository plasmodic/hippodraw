/* -*- mode: c++ -*- */

/** @file

hippodraw::PyNTuple class interface.

Copyright (C) 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: PyNTuple.h,v 1.3 2006/10/03 18:07:41 pfkeb Exp $

*/

#ifndef _PyNTuple_H_
#define _PyNTuple_H_

#include "datasrcs/NTuple.h"

namespace hippodraw {

/** A wrapper for NTuple, so that when NTuple is used in Qt based
    application, the application object can be locked and unlocked
    before calling any NTuple member function that would modify it.
    This is needed for multiple processor machines.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class PyNTuple : public NTuple
{
private:

public:

  /** Constructor taking a vector of column labels as argument.
   */
  explicit PyNTuple ( const std::vector < std::string > & labels );

  /** A constructor that creates an NTuple with @a n columns. Creates
      an NTuple with @a n columns.  The objects will be empty, i.e. 0
      rows.
   */
  PyNTuple ( unsigned int n );

  /** The copy constructor.
   */
  PyNTuple ( const PyNTuple & );

  /** Make a new object by copying data from existing NTuple.
   */
  PyNTuple ( const NTuple & );

  /** The default constructor.
   */
  PyNTuple ( );

  /** Sets the title of the DataSource.
   */
  virtual void setTitle ( const std::string & title );

  virtual int addColumn ( const std::string & label,
			  const std::vector < double > & column );

  virtual void replaceColumn ( unsigned int i,
			       const std::vector < double > & column );

  virtual void replaceColumn ( const std::string & label,
			       const std::vector < double > & column );

  virtual void addRow ( const std::vector < double > & row );

  virtual void clear ();

  virtual void append ( const DataSource * source );

};

} // namespace hippodraw

#endif // _PyNTuple_H_
