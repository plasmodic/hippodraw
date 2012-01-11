/* -*- c++ -*- */

/** @file

RTuple class interface.

Copyright (C) 2000-2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: RTuple.h,v 1.9 2006/02/26 19:56:09 xiefang Exp $

*/

#ifndef RTUPLE_H
#define RTUPLE_H

#ifdef _MSC_VER
# pragma warning(disable:4290) // exception spec ignored except to indicate
#endif

#include "DataSource.h"

namespace hippodraw {

/** A DataSource class implemented with std::vector<double> to store
    the row data.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API RTuple : public DataSource
{

 private:

  /** The data rows of the ntuple table.  Note that the pointer to the
      rowss is used so that when rows are added, the row data doesn't
      need to be relocated.
  */
  std::vector< std::vector<double> *> m_data;

  /** The copy constructor creates an ntuple that is a copy of an
      existing one. 
  */
  RTuple ( const RTuple & nt);

 public:

  /** Constructor taking a vector of column labels as argument.
   */
  RTuple ( const std::vector < std::string > & labels );

  /** A constructor that creates an ntuple with @a n columns. Creates
      an RTuple with @a n columns.  The objects will be empty, i.e. 0
      rows.
   */
  RTuple ( unsigned int n );

  /** The destructor. 
   */
  virtual ~RTuple();

  virtual void copy ( const DataSource & other );

  /** Clears the RTuple.  Clears all the data elements of the
      %RTuple.  Does not change the other attributes or capacity.
  */
  virtual void clear();
    virtual void reserve ( unsigned int count );

  /** Returns @c true, if RTuple is empty, i.e. has no rows. 
   */
  virtual bool empty () const;

  /** Returns the number of rows.
   */
  virtual unsigned int rows () const;

  /** Adds a row to the end of the ntuple.  If the size of the input
      vector is the same as the width of the ntuple, adds the row to
      the end of the ntuple.  It also notifies its Observer objects
      and returns true.  Otherwise, throws a DataSourceException
      object.
  */
 virtual void addRow ( const std::vector<double> & v );

  /** Returns a vector of data elements in one row.  Returns
      a vector with data elements copied from the RTuple row
      indexed by @a index.  If @a index is out of range, throws a
      DataSourceException object.
  */
  virtual const std::vector < double > & getRow ( unsigned int index ) const;

  virtual double 
  operator [] (  std::vector < unsigned int > & indices ) const;

  virtual double valueAt ( unsigned int row, unsigned int column ) const;

};

} // namespace hippodraw

#endif // RTUPLE_H
