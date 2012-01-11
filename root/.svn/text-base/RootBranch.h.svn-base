/* -*- c++ -*- */

/** @file

hippodraw::RootBranch class interface.

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: RootBranch.h,v 1.47 2007/02/12 19:14:49 pfkeb Exp $

*/

#ifndef RootBranch_H
#define RootBranch_H

#include "RootDataType.h"

#include "pattern/libhippo.h"

#include "TLeaf.h"
#include "TObjArray.h"

#include <string>
#include <vector>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

class TBranch;

namespace hippodraw {

/** A class to read data from a ROOT TBranch.

    @request @@@@ support ROOT Char_t and others.

    @author Kaustuv <kaustuv@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API RootBranch
{

private:
  
  /** The ROOT TBranch from which the data will be obtained.
   */
  mutable TBranch * m_branch;

  /** The ROOT TLeaf which described the data in this branch.
   */
  TLeaf * m_leaf;

  /** The ROOT type name for this branch if single TLeaf is on this
      branch.
  */
  hippodraw::RootData::Type m_leaf_type;
  
  /** The number of TLeaf objects on this TBranch.
   */
  int  m_number_leaves;

  /** The number of elements in the Leaf array or @c 0 if not an
      array.
  */
  int m_number_elements;
  
  /** The address of the following variable is given to ROOT TTree as
      the branch address for this branch if the type is ROOT Double_t.
      So root finally stores the double data in this variable.
  */
  mutable Double_t m_double_data;
  
  /** The address of the following variable is given to ROOT TTree as
      the branch address for this branch if the type is ROOT Float_t.
      So root finally stores the float data in this variable.
  */
  mutable Float_t m_float_data;
  
  /** The address of the following variable is given to ROOT TTree as
      the branch address for this branch if the type is ROOT Int_t.
      So root finally stores the integer data in this variable.
  */
  mutable Int_t m_int_data;
  
  /** The address of the following variable is given to ROOT TTree as
      the branch address for this branch if the type is ROOT UInt_t.
      So root finally stores the integer data in this variable.
  */
  mutable UInt_t m_uint_data;
  /** The address of the following variable is given to ROOT TTree as
      the branch address for this branch if the type is ROOT Short_t.
      So root finally stores the integer data in this variable.
  */
  mutable Short_t m_short_data;
  
  /** The address of the following variable is given to ROOT TTree as
      the branch address for this branch if the type is ROOT UShort_t.
      So root finally stores the integer data in this variable.
  */
  mutable UShort_t m_ushort_data;
  
  /** The address of the following variable is given to ROOT TTree as
      the branch address for this branch if the type is ROOT Long64_t.
      So root finally stores the integer data in this variable.
  */
  mutable Long64_t m_long64_data;
  
  /** The address of the following variable is given to ROOT TTree as
      the branch address for this branch if the type is ROOT ULong64_t.
      So root finally stores the integer data in this variable.
  */
  mutable ULong64_t m_ulong64_data;
  
  /** The address of the following variable is given to ROOT TTree as
      the branch address for this branch if the type is ROOT Double_t.
      So root finally stores the data in this address pointed by this
      variable.

      @request Could this be a vector instead of pointer.
  */
  mutable Double_t * m_vector_double_data;

  /** The address of the following variable is given to ROOT TTree as
      the branch address for this branch if the type is ROOT Float_t.
      So root finally stores the data in this address pointed by this
      variable.
  */
  mutable Float_t * m_vector_float_data;

  /** The address of the following variable is given to ROOT TTree as
      the branch address for this branch if the type is ROOT Int_t.
      So root finally stores the data in this address pointed by this
      variable.
  */
  mutable Int_t * m_vector_int_data;

  /** The address of the following variable is given to ROOT TTree as
      the branch address for this branch if the type is ROOT UInt_t.
      So root finally stores the data in this address pointed by this
      variable.
  */
  mutable UInt_t * m_vector_uint_data;


  /** The address of the following variable is given to ROOT TTree as
      the branch address for this branch if the type is ROOT Short_t.
      So root finally stores the data in this address pointed by this
      variable.
  */
  mutable Short_t * m_vector_short_data;

  /** The address of the following variable is given to ROOT TTree as
      the branch address for this branch if the type is ROOT UShort_t.
      So root finally stores the data in this address pointed by this
      variable.
  */
  mutable UShort_t * m_vector_ushort_data;

  /** The address of the following variable is given to ROOT TTree as
      the branch address for this branch if the type is ROOT Long64_t.
      So root finally stores the data in this address pointed by this
      variable.
  */
  mutable Long64_t * m_vector_long64_data;

  /** The address of the following variable is given to ROOT TTree as
      the branch address for this branch if the type is ROOT ULong64_t.
      So root finally stores the data in this address pointed by this
      variable.
  */
  mutable ULong64_t * m_vector_ulong64_data;

  /** In case the data quantity we are dealing with is a vector/matrix
      store its dimensions of each axis in this vector.
  */
  std::vector < int > m_shape;

  /** In case the data quantity is vector/matrix  we would like just to take
      one element out of it. This index represents that particular relevant
      entry of the vector/matrix when flattened out as a 1-d vector.
  */
  int m_releventIndex;
  
  /** Set @c true when branch address has been set.
   */
  mutable bool m_branch_set;

  /** Set to @c false if the branch is not usable.  A branch is not
      usable.  It is not usable if it contains a array what has
      variable length.  In such a case, the DataSource does not have a
      fixed number of columns.  It is also not usable if it contains
      a data type that is not supported.
  */
  bool m_useable;

 public:

  /** The constructor.
   */
  RootBranch ( TBranch * );

private:

  /** The copy constructor.
   */
  RootBranch ( const RootBranch & );  

  /** The default constructor.
   */
  RootBranch ();

public:

  /** The destructor. 
   */
  ~RootBranch();


  /** Returns @c true if data sitting in the rows of this branch is and array*/
  bool isMultiDimensional() const;

  /** Number of elements in this branch. Since we are just dealing with the
      single leaf branches this entry in case of array shows total number of
      elements in that array. */
  int numberOfElements() const;
  
  /** Returns the size of the slice for the next to last dimension.
   */
  unsigned int size () const;

  /** Returns @c true, if RootBranch is empty, i.e. has no data.
   */
  bool empty () const;

  /** Returns the value of the leaf at index @a row.
   */
  double valueAt ( unsigned int row ) const;

  /** Returns the type of the data in this branch.
   */
  hippodraw::RootData::Type getType () const;

  /** Returns pointer to a @c double array in given @a row. Returns
      pointer to a @c double array in given @a row.  Converts from the
      ROOT type to @c double if necessary.

      @request Add the conversion feature to other xxxArrayAt member
      functions.
   */
  double * doubleArrayAt ( unsigned int row );

  /** Returns pointer to a float array type in given @a row.
   */
  float * floatArrayAt ( unsigned int row );

  /** Returns pointer to a int array type in given @a row.
   */
  int * intArrayAt ( unsigned int row );

  /** Returns pointer to a unsigned int array type in given @a row.
   */
  unsigned int * uintArrayAt ( unsigned int row );

  bool isFilled ( ) const;
  
  /** Gives the dimensionality of the data stored in each row of this branch.
     A scalar entry in each row means we're dealing with rank 0 data,
     A Vector entry in each row means we're dealing with rank 1 data,
     A Matrix entry in each row means we're dealing with rank 2 data,
     so on and so forth. 
  */
  unsigned int getRank () const;
  
  /** In case we are dealing with multidimensional data in rows of this branch
      we would like to deal with on one entry of this multidimensional data.
      So here we set this relevant entry */ 
  void setReleventIndex( const std::vector< unsigned int >& index );

  /** Vector of the number of entries in the multidimensional data.
      @note Chose vector of int to be compatible num_util.
   */
  const std::vector < int > & getShape ();

  /** Returns the TBranch which was used to initialize this branch */
  TBranch * getTBranch();

  /** Returns @c true if the branch is usable.
   */
  bool isUseable () const;
  
private:

  /** Sets the address where the ROOT TBranch will put its data.
   */
  void setBranchAddress () const;

  /** From the root title, which is of format "name[x][y][z]",
      gets the sizes of various dimension.  Stores the information as
      private data member.
  */
  void initShape ( const char* title );

};

} // namespace hippodraw

#endif // RootBranch_H
