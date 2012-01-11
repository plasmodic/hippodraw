/* -*- mode:c++ -*- */

/** @file

hippodraw::FitsFile interface

Copyright (C) 2000, 2004-2006  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: FitsFile.h,v 1.34 2007/05/17 22:58:21 pfkeb Exp $

*/

#ifndef _FitsFile_h
#define _FitsFile_h

#include "FitsFileBase.h"

#ifndef _MSC_VER
#include <inttypes.h>
#endif

#include <vector>

namespace hippodraw {

/** Wrapper class to CFITSIO

Encapsulates the low level C interface to a higher level C++
interface.  Does not currently handle everything, only those that were
needed in some simple applications.  This version handles arrays with
the Standard Template Library (STL).  

@author Xie Fang <xiefang@stanford.edu>
@author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

*/

class FitsFile : public FitsFileBase {

private:

  /** Fills the vector @a v from a table column.  The first column
      index is 0, unlike CFITSIO
   */
  int fillFromTableColumn ( std::vector < double > & v, 
			    int column );
  
  /** Fills the vector @a vec from an image HDU.  Fills the vector @a
      vec from an image HDU.  If the image is three dimensional, then
      selects only the @a axis for the third dimension.
   */
  int fillFromImage ( std::vector < double > & vec, unsigned int axis );

  /** Clears and fills the vector with the column labels of binary or
      ASCII table.
  */
  int fillColumnNamesFromTable ( std::vector < std::string > & labels );

  /** Clears and fills the vector with the column labels of a image.
  */
  int fillColumnNamesFromImage ( std::vector < std::string > & labels );

  /** Read elements from an ASCII or binary table column (in the CDU)
      and fill STL vector<int> with its contents.  

      \param vec The STL vector of doubles provided by the caller.
      \param column The column index which starts a 1.
      \return 0 if successful, else the cfitsio status word.

      Data is converted to type int if necessary. This routines
      returns the values of the table column array elements.  Data is
      taken from the first row and first element for a length equal to
      the size of the vector<int>.  Undefined array elements will be
      returned with a value `0'.  The ANYF parameter of the underlying
      call to the cfitsio function `fits_read_col_dbl()' is
      ignored.
  */
  int fillIntVectorFromColumn ( std::vector < int > & vec, int column );

 public:

  /// Public construction taking a filename as argument
  FitsFile ( const std::string & filename , bool write = false );

  /** Clears and fills the vector with the HDU names.  If an HDU
      doesn't have a name, one is generated for it.
   */
  void fillHDUNames ( std::vector < std::string > & names );

  /** Clears and fills the vector with the column labels of the
      current HDU table.
  */
  int fillColumnNames ( std::vector < std::string > & labels );

  /** Read elements from an ASCII or binary table column (in the CDU)
      and fill the vector with its contents.
      
      \param vec The vector of doubles provided by the caller.

      \param column The index to the column number starting at 0.

      \return 0 if successful, otherwise returns the cfitsio status word.

      Data is converted to type double if necessary. This method
      returns the values of the table column array elements.  Data is
      taken from the first row and first element for a length equal to
      the size of the vector<double>.  Undefined array elements will
      be returned with a value `0'.  The ANYF parameter of the
      underlying call to the cfitsio function `fits_read_col_dbl()'
      is ignored.
  */
  int fillDoubleVectorFromColumn ( std::vector < double > &vec,
				   int column );

  /** Clears and fills the vector @a vec with the size of each
      dimension of an image.  The size of the returned vector gives
      the number of dimensions.
  */
  int fillAxisSizes ( std::vector < long > & vec ) const;

  /** Fills the vector with the shape of the column.  Fills the vector
      @a v with the shape of the column @a c of a binary table.  If
      the column contains an array, the first member of the shape
      vector is the number of rows of the Binary table.
  */
  void fillShape ( std::vector < intptr_t > & shape, int column );

  /** Clears and fills the vector with the delta values of an image.
      If the delta parameter is not give in the image's header, then
      use the value 1.0. */
  void fillImageDeltas ( std::vector < double > & deltas ) const;

  /** Clears and fills the vector with the reference pixel's
      indexes.
  */
  void fillRefPixelIndices ( std::vector < int > & indices ) const;

  /** Clears and fills the vector with the reference pixel's
      values.
  */
  void fillRefPixelValues ( std::vector < double > & values ) const;

  /** Returns @c true if coordinate system of the axis is should
      undergo Hammer-Aitoff transformation.
  */
  bool isHammerAitoff () const;

  void writeHDU ( long rows, int columns, 
		  const std::vector < std::string > & names,
		  const std::vector < std::vector < int > > & shapes,
		  const std::string & extname );

  void writeImageHDU ( long x, long y );

  /** Writes the vector to FITS table.  Writes the contents of the
      vector @a data to a FITS table at column @a c. */
  void writeColumn ( int c, const std::vector < double > & data );

  void writePix ( long x, long y,
		  const std::vector <double > & data );

  void writeCloseFile ();

  bool pixCenter () const;

  void writeRefPixelValues ( double value1, double value2 );

};

} // namespace hippodraw

#endif // _FitsFile_h
