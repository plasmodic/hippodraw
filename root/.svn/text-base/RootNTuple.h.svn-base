/* -*- c++ -*- */

/** @file

hippodraw::RootNTuple class interface.

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: RootNTuple.h,v 1.68 2007/01/31 23:04:20 pfkeb Exp $

*/

#ifndef RootNTuple_H
#define RootNTuple_H

#include "RootDataType.h"

#include "datasrcs/DataSource.h"

class TTree;
class TBranch;

namespace hippodraw {

class RootBranch;

/** A DataSource class implemented with a ROOT TBranch objects from a
    ROOT TTree to store the column data.

    @bug @@@@ Unsigned short is not supported.


    @author Kaustuv <Kaustuv@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API RootNTuple : public DataSource
{

 private:

  /** The type of container holding pointers to the ROOT branches.
   */
  typedef std::vector < RootBranch * > BranchList_t;

  /** The RootBranch objects that contains the data. 
   */
  BranchList_t m_data;

  /** The type of container holding pointers to the cached data vectors.
   */
  typedef std::vector < std::vector < double > * > CacheList_t;
  /** A cache of data already read.
   */
  CacheList_t m_data_cache;

  /** Answers if rows of column are composed of the
      multidimensional data.
   */ 
  std::vector < bool > m_multiDimensional;
  
  /** The ROOT TTree object which is the source of the data.
   */
  TTree * m_tree;

  /** Index into the current position in the branches.
   */
  mutable unsigned int m_cur_entry;

  /** A hack to generate labels */
  bool rotateWheels( std::vector< int > & wheels,
		     const std::vector < int > & sz,
		     unsigned int d );

  /** Adds a column to the end of the RootNTuple.  If the size of the
      input array is the same as the size of the existing columns adds
      input array to then end of the RootNTuple . Gives the column the
      label @a label.  Returns the index of the newly created column.
      If the size of the input array differs from the size of existing
      columns or if the label already exists, then throws a
      std::runtime_error object.   

      @note This method is private because ROOT files are intended to
      be read only.
  */
  int addColumn ( const std::string & label, RootBranch * );

  /** Get the TBranch associated with this column.
   */
  TBranch * getTBranch(  const std::string & column );
  

protected:

  /** The default constructor.  

      @note The default constructor is protected because implementation
      depends on opening ROOT file before adding columns.
  */
  RootNTuple ();

  /** The copy constructor.
   */
  RootNTuple ( const RootNTuple & );

 public:

  /** A constructor taking TTree from ROOT file as argument. */
  RootNTuple ( TTree * tree );

  /** The destructor. 
   */
  virtual ~RootNTuple();

 /** Notifies observers. 
      @todo remove this method when Observable is exposed to Python.
   */
  virtual void notifyObservers ( ) const;

  /** Returns the size of first dimension, i.e. the number of rows.
   */
  virtual unsigned int rows () const;

  /** Returns @c true, if RootNTuple is empty, i.e. has no data.
   */
  virtual bool empty () const;

  /** Removes all the cached data vectors, does not change the
      contained ROOT file.
  */
  virtual void clear ();

  /** Does nothing as the contained ROOT file controls the size.
   */
  virtual void reserve ( unsigned int );

  /** Raises an assertion, as a RootNTuple does not really have
      shape.
  */
  virtual double 
  operator [] (  std::vector < unsigned int > & indices ) const;

  virtual double valueAt ( unsigned int row, unsigned int column ) const;

  virtual double valueAtNoCache ( unsigned int row,
				  unsigned int column ) const;

  /** Returns the type of data in @a column.
   */
  hippodraw::RootData::Type getType ( unsigned int column ) const;

  /** Returns pointer to a double array type in given @a row.
   */
  virtual double * doubleArrayAt ( unsigned int row,
				   unsigned int column ) const;

  /** Returns pointer to a float array type in given @a row.
   */
  virtual float * floatArrayAt ( unsigned int row,
				 unsigned int column ) const;

  /** Returns pointer to a int array type in given @a row.
   */
  virtual int * intArrayAt ( unsigned int row,
			     unsigned int column ) const;

  /** Returns pointer to a unsigned int array type in given @a row and
      @a column.
   */
  virtual unsigned int * uintArrayAt ( unsigned int row,
				       unsigned int column ) const;

  /** Adds a column to the end of the RootNTuple.  If the size of the
      input array is the same as the size of the existing columns adds
      input array to then end of the RootNTuple . Gives the column the
      label @a label.  Returns the index of the newly created column.
      If the size of the input array differs from the size of existing
      columns or if the label already exists, then throws a
      DataSourceException object.   
  */
  int addColumn ( const std::string & label,
		  const std::vector < double > & column );

  /** Replaces the data in column @a index.  If @a index is in a valid
      range and if the size of @a data is the same as existing
      columns, then replaces the specified column with a copy of data
      in @a data.  If @a index is not a valid range or the sizes
      differ, then throws a DataSourceException.  In neither case is
      the ROOT file modified.
   */
  void replaceColumn ( unsigned int index,
		       const std::vector < double > & data );

  virtual const std::vector < double > &
  getColumn ( const std::string & name ) const;

  /** Returns the column with label @a name.  The column should be one
      that has an array on the TBranch.  The vector @a indexes is used
      as the indexes into the array.
  */
  virtual const std::vector < double > &
  getColumn ( const std::string & name,
	      const std::vector < int > & indices ) const;

  virtual const std::vector < double > &
  getColumn ( unsigned int index ) const;

  /** Returns a const reference to slice along the axis known as a
      row.  @note This method will read one row from all columns
      without expanding the columns to the the cache.
   */
  virtual const std::vector < double > & getRow ( unsigned int i ) const;

  /** Sets the shape of the data elements. Sets the shape data
      elements from the vector @a shape.  The rank is given by the
      size of the vector.  A rank of 2 correspond to table.  If the
      total number of elements in the RootNTuple is not equal to the
      product of the shape, then an assertion is raised.
  */
  virtual void setShape ( std::vector < unsigned int > & shape );

  /** Returns the shape of the data elements. 
   */
  const std::vector < unsigned int > & getShape () const;

  /** Answers if rows of column are composed of the multidimensional data.
   */ 
  virtual bool isMultiDimensional ( const std::string & column ) const;
    
  /** Returns @c true if the column is usable in the context of a
      DataSource.  A column may not be usable if it contains a
      multidimensional array that varies in size from row to row.
   */
  virtual bool isUseable ( const std::string & label ) const;

  /** In case we are dealing with multidimensional data in rows of this column
      we would like to deal with on one entry of this multidimensional data.
      So here we set this relevant entry. */
  virtual bool setReleventIndex( const std::string & column,
				 const std::vector< unsigned int >& index );
  
  /** Fills the vector with the shape of a column.  Returns via the
      vector @a v, the shape of the contained TBranch at the column
      @a column. If the TBranch contains an array of @em n dimensions,
      the size of shape vector will be <em> n+1 </em> since the first index
      is always the number of rows.
  */
  virtual void fillShape ( std::vector < int > & v,
			   unsigned int column ) const;

  /**
     Suppose we encounter a RootNTuple which has multi-dimensional
     data sitting in the rows of the concerned column. We expand this
     new NTuple to have additional columns with labels of form
     array[i][j][k] provided it has not been done before.
  */
  void smartExpandRootNTuple( const std::string & column ) const;

  /** Expand the labels if needed.  For each label in @a labels,
      expand the variable name if it is an array.   

      @note Although the object changes its labels, it is still
      logically a @c const function as the state of the data doesn't
      change.
   */  
  void expandIfNeeded ( const std::vector < std::string > & labels ) const;

  /** Creates complete binding string from the array variable name and
      indexes.
  */
  std::string 
  createBinding ( const std::string & name, 
		  const std::vector < int > & indices ) const;

private:

  void expandIfNeeded ( const std::string & label ) const;

  /** Initializes the columns of the NTuple.
   */
  void initBranches ();

  /** Fills the data cache for column @a column.
   */
  void fillDataCache ( unsigned int column );
  
};

} // namespace hippodraw

#endif // RootNTuple_H
