/* -*- c++ -*- */

/** @file

hippodraw::DataColumn class interface.

Copyright (C) 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DataColumn.h,v 1.7 2007/05/17 22:58:21 pfkeb Exp $

*/

#ifndef DataColumn_H
#define DataColumn_H

#include "pattern/libhippo.h"

#include <vector>

#ifndef _MSC_VER
#include <inttypes.h>
#endif

namespace hippodraw {

/** A class to hold data and its attributes

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API DataColumn
{
public:

  enum Type {
    Double, ///< double type.
    Float,  ///< float type.
    Int,    ///< int type.
    UInt,   ///< unsigned int type.
    ULong64 ///< unsigned 64 bit long.
  };

private:

  /** The data.
      @request Support data types other than double.
   */
  std::vector < double > m_double_data;

  /** In case the data quantity we are dealing with is a vector/matrix
      store its dimensions of each axis in this vector.
  */
  std::vector < intptr_t > m_shape;

  /** The type for this column.
  */
  Type m_type;
  
private:

  /** The copy constructor not usable.
   */
  DataColumn ( const DataColumn & );  

public:

  /** The default constructor.
   */
  DataColumn ();

  /** The destructor. 
   */
  ~DataColumn();

  /** Returns the data.
   */
  std::vector < double > & getData ();

  /** Returns the data.
   */
  const std::vector < double > & getData () const;

  /** Gives the dimensionality of the data stored in each row of this branch.
     A scalar entry in each row means we're dealing with rank 0 data,
     A Vector entry in each row means we're dealing with rank 1 data,
     A Matrix entry in each row means we're dealing with rank 2 data,
     so on and so forth. 
  */
  unsigned int getRank () const;
  
  /** Vector of the number of entries in the multidimensional data.
      @note Chose vector of int to be compatible num_util.
   */
  const std::vector < intptr_t > & getShape ();

  /** Sets the shape of the data column.
      @note Chose vector of int to be compatible num_util.
   */
  void setShape ( const std::vector < intptr_t > & shape );


  /** Returns the size of the column.  Returns the total size of the
      column.  If the column contains an array object, then the size
      is the product of its dimensions, otherwise it is the number of
      elements.
   */
  std::size_t size () const;

  /** Clears the data array.
   */
  void clear ();

  /** Returns pointer to the sub-array at row @a row.
   */
  double * doubleArrayAt ( unsigned int row );

};

} // namespace hippodraw

#endif // DataColumn_H
