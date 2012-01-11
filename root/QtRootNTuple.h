/* -*- mode: c++ -*- */

/** @file

hippodraw::QtRootNTuple class interface.

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtRootNTuple.h,v 1.27 2007/03/27 22:23:30 pfkeb Exp $

*/

#ifndef _QtRootNTuple_H_
#define _QtRootNTuple_H_

#include "RootNTuple.h"

namespace boost {
  namespace python {
    namespace numeric {
      class array;
    }
  }
}

namespace hippodraw {

/** A wrapper for RootNTuple, so that when RootNTuple is used in Qt
    based application, the application object can be locked and
    unlocked before calling any ROOT functions.  ROOT is not thread
    safe, so this is necessary to avoid the application thread and
    Python thread from clashing.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class QtRootNTuple : public RootNTuple
{
private:

  /** The default constructor.

      @note The default constructor is private because the implement
      currently requires opening a ROOT file.
   */
  QtRootNTuple ( );

  /** Removes the row dimension, i.e. first index, from the shape
      vector.*/
  static void sliceRowDimension ( std::vector < int > & shape );

public:

  /** The public constructor requiring a TTree from a ROOT file.
   */
  QtRootNTuple ( TTree * tree );

  /** Returns the number of columns
   */
  unsigned int columns () const;

  /** Returns the number of rows.
   */
  unsigned int rows () const;

  /** Returns copy of TBranch data by @a name.
   */
  const std::vector < double > & getColumn ( const std::string & name ) const;

  /** Returns copy of TBranch data by @a name and @a indexes.
   */
  const std::vector < double > & 
  getColumn ( const std::string & name,
	      const std::vector < int> & indexes ) const;

  /** Returns copy of TBranch data by @a index.
   */
  const std::vector < double > & getColumn ( unsigned int index ) const;

  /** Returns the row indexed by @a index.
   */
  const std::vector < double > & getRow ( unsigned int index ) const;

  /** Adds a column to the object. The column is treated like the
      other columns and the original ROOT file is not modified.
  */
  int addColumn ( const std::string &, const std::vector < double > & );

  /** Returns the labels of the column, i.e. the TBranch names. */
  const std::vector < std::string > & getLabels () const;

  /** Returns true if the column named @a column, is an array, rather
      then simple type.
  */
  bool isMultiDimensional ( const std::string & column ) const;

  /** Returns the shape of multidimensional array excluding row dimension.
      @deprecated Should return shape of column including row index.
  
   */
  const std::vector < int > getColumnShape ( const std::string & column );


  /** Returns a numarray for @a variable in @a row of NTuple.

      @request Should let PyDataSource create the numarray so this
      class doesn't depend on numarray; as was done for FitsNTuple.
   */
  boost::python::numeric::array
  valueAt ( unsigned int row, const std::string & variable );

  /** Returns a numarray for @a variable 

      @request Should let PyDataSource create the numarray so this
      class doesn't depend on numarray; as was done for FitsNTuple.
   */
  boost::python::numeric::array
  getColumnAsArray ( const std::string & variable );


  void expandIfNeeded ( const std::vector < std::string > & labels ) const;

  std::string 
  createBinding ( const std::string & name, 
		  const std::vector < int > & indices ) const;
};

} // namespace hippodraw

#endif // _QtRootNTuple_H_
