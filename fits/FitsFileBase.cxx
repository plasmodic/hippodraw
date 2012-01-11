/** @file

hippodraw::FitsFileBase implemenation

Copyright (C) 2000, 2004, 2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

@author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

$Id: FitsFileBase.cxx,v 1.35 2007/01/16 23:16:38 pfkeb Exp $

*/

#include "FitsFileBase.h"

#include <iostream>

#include <cassert>

using std::string;

using namespace hippodraw;

FitsFileBase::FitsFileBase ( const std::string & filename, bool write) 
{
  if (!write)
    {
      m_status = 0;
      fits_open_file( &m_fptr, filename.c_str(), READONLY, &m_status );
    }
  else
    {
      m_status = 0;

      // Overwrite the FITS file.
      std::string fn = "!";
      fn += filename;
      fits_create_file( &m_fptr, fn.c_str(), &m_status );
    }
}

FitsFileBase::~FitsFileBase()
{
  if ( m_fptr != 0 ) {
    m_status = 0;
    fits_close_file ( m_fptr, &m_status );
  }
}

void FitsFileBase::clearErrorMessageStack( void )
{
  fits_clear_errmsg();
}


FitsFileBase::HduType
FitsFileBase::
convert ( int i )
{
  static HduType table[] = { Image, Atable, Btable, Any };

  assert ( i < 3 && i >= 0 );

  return table [ i ];
}

FitsFileBase::HduType
FitsFileBase::
getHduType () const
{
  int hdutype;
  m_status = 0;
  fits_get_hdu_type ( m_fptr, &hdutype, &m_status );

  return convert ( hdutype );
}

FitsFileBase::ImageType
FitsFileBase::
getImageType () const
{
  ImageType type = NoImg;
  static ImageType table[] 
    = { ByteImg, ShortImg, LongImg, FloatImg, DoubleImg };

  int bitpix;
  m_status = 0;
  fits_get_img_type ( m_fptr, & bitpix, & m_status );
  if ( bitpix < 5 && bitpix >= 0 ) {
    type = table [ bitpix ];
  }

  return type;
}

int
FitsFileBase::
getImageDimensions () const
{
  int naxis = 0;
  m_status = 0;
  fits_get_img_dim ( m_fptr, & naxis, & m_status );
  assert ( m_status == 0 );

  return naxis;
}

int
FitsFileBase::
getNumberOfHDU () const
{
  int hdunum = 0;
  m_status = 0;
  fits_get_num_hdus ( m_fptr, &hdunum, &m_status );

  return hdunum;
}

int
FitsFileBase::
moveToHDU ( int hdunum )
{
  int hdutype;
  m_status = 0;
  fits_movabs_hdu ( m_fptr, hdunum + 1, // count like Fortran
		    &hdutype, &m_status );

  return m_status;
}

int
FitsFileBase::
moveToHDU ( const std::string & name )
{
  char * extname = const_cast < char * > ( name.c_str() );
  m_status = 0;
  fits_movnam_hdu ( m_fptr, ANY_HDU, extname, 0, &m_status );

  return m_status;
}

int
FitsFileBase::
getHDUNumber () const
{
  int number = 0;
  //int retval = 
  m_status = 0;
  fits_get_hdu_num ( m_fptr, &number );

  return number - 1; // count like C++
}

int FitsFileBase::numKeywords() const
{
  int keyexist, morekeys;
  m_status = 0;
  fits_get_hdrspace( m_fptr, &keyexist, &morekeys, &m_status );
  return keyexist;
}

double
FitsFileBase::
doubleValueForKey ( const char * key ) const
{
  double value;
  m_status = 0;
  fits_read_key( m_fptr, TDOUBLE, 
		 const_cast<char *>(key), &value, 0, &m_status );
  return value;
}

bool
FitsFileBase::
hasKey ( const char * key ) const
{
  char value [ FLEN_VALUE ];
  m_status = 0;
  fits_read_keyword ( m_fptr, const_cast < char * > ( key ),
		      value, 0, & m_status );

  return m_status == 0;
}

int
FitsFileBase::
intValueForKey ( const char * key ) const
{
  int value;
  m_status = 0;
  fits_read_key( m_fptr, TINT, 
		 const_cast<char *>(key), &value, 0, &m_status );

  return value;
}

string
FitsFileBase::
stringValueForKey ( const char * key ) const
{
  char value [ FLEN_VALUE ];
  m_status = 0;
  fits_read_key ( m_fptr, TSTRING,
		 const_cast<char *>(key), &value, 0, &m_status );

  return string ( value );
}

int
FitsFileBase::
status () const
{
  return m_status;
}

long
FitsFileBase::
getNumberOfRows () const
{
  long number = 0;

  int hdutype;
  m_status = 0;
  fits_get_hdu_type ( m_fptr, &hdutype, &m_status );

  if ( hdutype == IMAGE_HDU ) {
    int nx = intValueForKey ( "NAXIS1" );
    int ny = intValueForKey ( "NAXIS2" );
    number = nx * ny;
  }
  else { // table
    m_status = 0;
    fits_get_num_rows ( m_fptr, & number, & m_status );
  }

  return number;
}

int
FitsFileBase::
getNumberOfColumns () const
{
  int ncols = 0;
  m_status = 0;
  fits_get_num_cols ( m_fptr, &ncols, &m_status );

  return ncols;
}
