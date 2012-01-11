/* -*- c++ -*- */

/** @file

NTupleSorter class interface

Copyright (C) 2003   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: NTupleSorter.h,v 1.12 2005/10/30 00:10:11 pfkeb Exp $

*/

#ifndef NTUPLESORTER_H
#define NTUPLESORTER_H

#include "pattern/libhippo.h"

#include <vector>

namespace hippodraw {

class NTuple;

/** A helper class to sort and keep sorted an NTuple.  

    A class to maintain a sorted NTuple.  Sorting is done by the value
    of elements in the selected column.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API NTupleSorter
{

private:

  /** Private assignment operator to prevent copying. 
   */
  NTupleSorter & operator = ( const NTupleSorter & );

  /** The column used for sorting, or -1 if sorting is disabled.
   */
  int m_column;

  /** The direction of the sort. 
   */
  bool m_increasing;

  /** The NTuple object that will be used. 
   */
  NTuple * m_ntuple;

  /** Makes a row-wise copy of the NTuple object.  Clears the vector
      @a row_copy and fills it with a pointers to vectors containing
      the elements of each row.
   */
  void rowwiseCopy ( std::vector < std::vector < double > * > & row_copy );

public:

  /** The copy constructor.  Initializes the newly constructed object
      with save value of the data members except for m_ntuple which is
      set to null pointer.  It is intended that the copy only be used
      as predicate to the STL sort function.
   */
  NTupleSorter ( const NTupleSorter & );

  /** A constructor taking the NTuple to be sorted as the parameter.
      Constructs the object and initialize it to sort column 0 in
      increasing order.   Does not sort the %NTuple object.
  */
  NTupleSorter ( NTuple * ntuple );

  /** The virtual destructor.  The virtual distructor which is needed
      only to avoid warning from some compilers such as Gcc 4.0.0.
  */
  virtual ~NTupleSorter () {}

  /** Returns the NTuple being managed by the sorter. 
   */
  NTuple * getNTuple () const;

  /** Sets the sorting column.  Sets the sorting column and resets the
      direction.  If @a column is different from the current one, sets
      the sorting direction to be increasing.  If @a column is the
      same as previously used, toggles the sorting direction to be the
      opposite of the current state.
  */
  void setSorting ( int column );

  /** Sorts the NTuple. */
  void sort ( );

  /** Clears the NTuple. */
  void clear ();

  /** Adds a row to the NTuple.  Adds a row to the NTuple object.
      Maintains the sorted order with the current sorting column and
      its direction. 

      @note The implementation of this function has some fine
      examples of the use of STL template functions from @b algorithm,
      @b functional, and @b iterator.  However, workarounds may be
      needed for compilers weak in the handling of templates.
  */
  void addRow ( const std::vector < double > & row );

  /** Erases a row from the NTuple.  Removes the row @a index from
      the %NTuple.  @sa NTuple::eraseRow.
  */
  virtual void eraseRow ( unsigned int index );

  /** Returns the number of columns of the NTuple. 
   */
  unsigned int columns ( )  const;

  /** Returns the number of rows of the NTuple. 
   */
  unsigned int rows ( ) const;

  /** Returns a reference to the @a index row of the sorted NTuple. 
   */
  const std::vector < double > & getRow ( unsigned int index ) const;

  /** A predicate function to be used for comparing two rows.  Returns
      @c true if the element of the column used for sorting of row @a
      x is less than the element of row @a y.
  */
  bool operator () ( std::vector < double > * x,
		     std::vector < double > * y ) const;

};

} // namespace hippodraw

#endif // NTUPLESORTER_H
