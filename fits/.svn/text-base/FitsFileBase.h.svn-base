/* -*- mode:c++ -*- */

/** @file

hippodraw::FitsFileBase interface

Copyright (C) 2000, 2004-2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: FitsFileBase.h,v 1.27 2007/04/23 21:57:20 pfkeb Exp $

*/

#ifndef _FitsFileBase_h
#define _FitsFileBase_h

#include "fitsio.h"

#include <string>

namespace hippodraw {

/** A base class for the concrete classes that wrap the cfitsio library.

Implemented in this class are those items that do not handle arrays of
data.  See the derived class FitsFile for handling arrays with STL.
Other derived classes may use other classes for handling arrays.

@author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

*/
class FitsFileBase {

public:

  /** The type of HDU.
   */
  enum HduType
    { Image = IMAGE_HDU,   ///< Image
      Atable = ASCII_TBL,  ///< ASCII table
      Btable = BINARY_TBL, ///< Binary table
      Any = ANY_HDU        ///< Matchs any
    };

  /** The type of image in HDU.
   */
  enum ImageType
    { ByteImg = BYTE_IMG,     ///< 8 bit integer per pixel
      ShortImg = SHORT_IMG,   ///< 16 bit integer per pixel
      LongImg = LONG_IMG,     ///< 32 bit integer per pixel
      FloatImg = FLOAT_IMG,   ///< 32 bit floating point per pixel
      DoubleImg = DOUBLE_IMG, ///< 64 bit floating point per pixel
      NoImg = 0               ///< not an image HDU
    };

  /** The data types supported by cfitsio.
   */
  enum DataType {
    ByteU = TBYTE,        ///< unsigned char
    Short = TSHORT,       ///< signed short
    ShortU = TUSHORT,     ///< unsigned short
    Int = TINT,           ///< signed integer
    IntU = TUINT,         ///< unsigned integer
    Long = TLONG,         ///< signed long
    LongU = TULONG,       ///<  unsigned long
    LongLong = TLONGLONG, ///< signed long long
    Float = TFLOAT,       ///< 32 bit floating point
    Double = TDOUBLE      ///< 64 bit floating point
  };

private:

  /** Converts an integer to HduType.
   */
  static HduType convert ( int i );

  /** Return the number of existing keywords (not counting the END
      keyword). Ignores the parameter in the underlying call to the
      cfitsio function `fits_get_hdrspace()' dealing with the amount
      of space currently available for more keywords.  */
  int numKeywords() const;

protected:

  /** The status return code from the last cfitsio operation.
   */
  mutable int m_status;

  /// Pointer to the fits file data structure
  fitsfile *m_fptr;

  /// Protected construction taking a file name as argument
  FitsFileBase ( const std::string & filename, bool write = false );

  /** Returns the image type.
   */
  ImageType getImageType () const;

  /** Returns the number of dimensions (axes) of an image.
   */
  int getImageDimensions ( ) const;

  /** Read a specified keyword value and return it as a double.  Data
      type conversion will be performed for numeric values if the
      keyword value does not have the type double. If the value of the
      keyword is undefined (i.e., the value field is blank) then an
      undefined value will be returned.
  */
  double doubleValueForKey ( const char * key ) const;

  /** Returns the number of columns in a table.
   */
  int getNumberOfColumns ( ) const;

   /** Returns the number of HDU in the file.
    */
  int getNumberOfHDU () const;

  /** Returns @c true if the keyword @a key exists, otherwise returns
      @c false. */
  bool hasKey ( const char * key ) const;

  /** Read a specified keyword value and returns it as a int.  Data
      type conversion will be performed for numeric values if the
      keyword value does not have the type int. If the value of the
      keyword is undefined (i.e., the value field is blank) then an
      undefined value will be returned.  */
  int intValueForKey( const char * key ) const;

  /** Read a specified keyword value and returns it as a string.  Data
      type conversion will be performed for numeric values if the
      keyword value does not have the type int. If the value of the
      keyword is undefined (i.e., the value field is blank) then an
      undefined value will be returned.
  */
  std::string stringValueForKey ( const char * key ) const;

public:

  /** Virtual destructor. */
  virtual ~FitsFileBase();

  /** Clear the entire error message stack. */
  void clearErrorMessageStack( void );

  /** Returns the type of HDU.
   */
  HduType getHduType () const;

  /** Returns the current HDU number.  The first (primary) HDU has
      number value = 1.
  */
  int getHDUNumber ( ) const;

  /** Move to a specified absolute HDU number in the FITS file and
      return the cfitsio status. When a FITS file is first opened or
      created it is automatically positioned to the first HDU (the
      primary array) in the file which has hdunum = 0 ( = 1 in cfitsio
      call) . The hdutype parameter of the underlying call to cfitsio
      function `fits_movabs_hdu()' is ignored.
  */
  int moveToHDU ( int hdunum );

  /** Move to a specified absolute HDU with name @a name in the FITS
      file and return the cfitsio status. When a FITS file is first
      opened or created it is automatically positioned to the first
      HDU (the primary array) in the file which has hdunum = 1.
  */
  int moveToHDU ( const std::string & name );

  /** Returns the cfitsio status code for the last operation.
   */
  int status () const;

  /** Returns the number of rows in the table.
   */
  long getNumberOfRows ( ) const;

private:

  ///Disallow copying
  FitsFileBase( const FitsFileBase& );

  ///Disallow assignment
  FitsFileBase& operator=( const FitsFileBase&);

};

} // namespace hippodraw

#endif // _FitsFileBase_h
