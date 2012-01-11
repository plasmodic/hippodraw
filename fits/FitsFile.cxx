/** @file

hippodraw::FitsFile implementation

Copyright (C) 2000, 2004-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: FitsFile.cxx,v 1.48 2007/05/17 17:57:10 pfkeb Exp $

*/

#include "FitsFile.h"

#include "pattern/string_convert.h"

#include <algorithm>
#include <stdexcept>

#include <cassert>
#include <cstring>

using std::string;
using std::vector;

using namespace hippodraw;

FitsFile::FitsFile ( const std::string & filename, bool write )
  : FitsFileBase ( filename, write )
{
}

void
FitsFile::
fillHDUNames ( std::vector < std::string > & names )
{
  names.clear ();
  int number = getNumberOfHDU ( );

  for ( int i = 0; i < number; i++ ) {
    moveToHDU ( i );
    string value = stringValueForKey ( "EXTNAME" );

    if ( value == "" ) {
      if ( i == 0 ) {
	int number = intValueForKey ( "NAXIS" );
	if ( number == 0 ) {
	  value = "Empty image";
	}
      }
    }

    if ( value == "" ) value = "<no name>";
    names.push_back ( value );
  }
}

int
FitsFile::
fillColumnNames ( std::vector < std::string > & labels )
{
  HduType type = getHduType ();
  if ( type == Image ) {
    m_status = fillColumnNamesFromImage ( labels );
  }
  else {
    m_status = fillColumnNamesFromTable ( labels );
  }

  return m_status;
}

int
FitsFile::
fillColumnNamesFromTable ( std::vector < std::string > & labels )
{
  labels.clear ();
  int columns = getNumberOfColumns ();

  if ( columns != 0 ) {
    m_status = 0; // must NOT be reset on each fits call, else matchs
		  // same column on each call
    for ( int i = 0; i < columns; i++ ) { // count like Fortran
      char colname [ FLEN_VALUE ];
      int col;
      fits_get_colname ( m_fptr, CASEINSEN, const_cast< char * > ("*"),
			 colname, &col, &m_status );
      labels.push_back ( string ( colname ) );
    }
  }
  else { // must be image
    labels.push_back ( string ( "none" ) );
  }

  return m_status;
}

int
FitsFile::
fillColumnNamesFromImage ( std::vector < std::string > & labels )
{
  labels.clear ();
  int dimension = getImageDimensions ();
  if ( dimension == 2 ) {
    labels.push_back ( "Values" );
  }
  else if ( dimension == 3 ) {
    const char type[] = "CTYPE3";
    bool yes = hasKey ( type );
    if ( yes ) {
      const string name = stringValueForKey ( type );
      int number = intValueForKey ( "NAXIS3" );
      for ( int i = 0; i < number; i++ ) {
	const string iv = String::convert ( i );
	labels.push_back ( name + " " + iv );
      }
    }
  }
    
  return m_status;
}

int
FitsFile::
fillDoubleVectorFromColumn ( std::vector < double > & vec,
			     int column )
{
  int retval = 0;

  HduType type = getHduType ();
  if ( type == Btable || type == Atable ) {
    retval = fillFromTableColumn ( vec, column );
  }
  else {
    retval = fillFromImage ( vec, column );
  }

  return retval;
}

int
FitsFile::
fillFromTableColumn ( std::vector < double > & vec, int column )
{
  int anynul;
  double nulval = 0;
  long nelements = vec.size();
  vector<double>::iterator it = vec.begin();
  double * ptr = &*it;
  // Careful, counting columns like Fortran
  m_status = 0;
  fits_read_col_dbl( m_fptr, column+1, 1, 1, nelements, nulval,
		     ptr, &anynul, &m_status);

  return m_status;
}

void
FitsFile::
fillShape ( std::vector < intptr_t > & shape, int column )
{
  m_status = 0;  // must always set

  string tdimn ( "TDIM" );
  tdimn += String::convert ( column+1 ); // a la Fortran
  char value[80]; // max length of keyword record
  fits_read_keyword ( m_fptr,
		      const_cast < char * > ( tdimn.c_str() ),
		      value,
		      NULL, & m_status );

  long rows = getNumberOfRows ();
  if ( value[0] != 0 ) {
    const string dims = value;
    unsigned int count = std::count ( dims.begin(), dims.end(), ',' );
    count += 1; // one for simple vector
    int icount = 0;
    vector < long > naxes ( count );
    fits_read_tdim ( m_fptr, column+1, count, // input
		     &icount,  &naxes[0], & m_status ); //output
    shape.resize ( count + 1 );
    shape [0] = rows;
    std::copy ( naxes.begin(), naxes.end(), shape.begin() + 1 );
  }
  else { // no array
    shape.resize ( 1, rows );
  }
}

int
FitsFile::
fillAxisSizes ( std::vector < long > & vec ) const
{
  vec.clear ();
  int naxis = getImageDimensions ();
  vec.resize ( naxis );

  vector < long > ::iterator first = vec.begin();
  long * ptr = & *first;
  m_status = 0;
  fits_get_img_size ( m_fptr, naxis, ptr, & m_status );
  assert ( m_status == 0 );

  return m_status;
}

void
FitsFile::
fillImageDeltas ( std::vector < double > & deltas ) const
{
  deltas.clear ();
  int naxis = getImageDimensions ();
  deltas.reserve ( naxis );

  char key [ FLEN_KEYWORD];
  char * keyroot = const_cast < char * > ("CDELT");
  for ( int i = 0; i < naxis; i++ ) {
    m_status = 0;
    fits_make_keyn ( keyroot, i+1, key, & m_status );
    bool yes = hasKey ( key );

    if ( yes ) {
      double value = doubleValueForKey ( key );
      deltas.push_back ( value );
    }
    else { // no key, take default
      deltas.push_back ( 1.0 );
    }
  }
}

void
FitsFile::
fillRefPixelIndices ( std::vector < int > & indices ) const
{
  indices.clear ();
  int naxis = getImageDimensions ();
  indices.reserve ( naxis );

  char key [ FLEN_KEYWORD];
  char * keyroot = const_cast < char * > ( "CRPIX" );
  for ( int i = 0; i < naxis; i++ ) {
    m_status = 0;
    fits_make_keyn ( keyroot, i+1, key, & m_status );
    bool yes = hasKey ( key );

    if ( yes ) {
      int value = intValueForKey ( key );
      indices.push_back ( value );
    }
    else { // no key, take default
      indices.push_back ( 1 );
    }
  }
}

void
FitsFile::
fillRefPixelValues ( std::vector < double > & values ) const
{
  values.clear ();
  int naxis = getImageDimensions ();
  values.reserve ( naxis );

  char key [ FLEN_KEYWORD];
  char * keyroot = const_cast < char * > ( "CRVAL" );
  for ( int i = 0; i < naxis; i++ ) {
    m_status = 0;
    fits_make_keyn ( keyroot, i+1, key, & m_status );
    bool yes = hasKey ( key );

    if ( yes ) {
      double value = doubleValueForKey ( key );
      values.push_back ( value );
    }
    else { // no key, take default
      values.push_back ( 0. );
    }
  }
}

bool
FitsFile::
isHammerAitoff () const
{
  bool hammer = true;

  char key [FLEN_KEYWORD];
  char * keyroot = const_cast < char * > ( "CTYPE" );
  for ( int i = 0; i < 2; i++ ) {
    fits_make_keyn ( keyroot, i+1, key, & m_status );
    bool yes = hasKey ( key );

    if ( yes ) {
      string value = stringValueForKey ( key );
      if ( value.find ( "-AIT" ) == string::npos ) {
	hammer = false;
      }
    } else {
      hammer = false;
    }
  }

  return hammer;
}

int
FitsFile::
fillFromImage ( std::vector < double > & vec, unsigned int zplane )
{
  vector < long > naxes;
  fillAxisSizes ( naxes );

  int datatype = Double;
  long nelements = naxes[0] * naxes[1];
  double nulval = 0;
  vector < double >::iterator first = vec.begin();
  double * ptr = & *first;
  int anynul;
  m_status = 0;
  if ( naxes.size () == 2 ) {
    long fpixel[2] = { 1, 1 }; // index like Fortran
    fits_read_pix ( m_fptr, datatype, fpixel, nelements,
		    & nulval, ptr, & anynul, & m_status );
  } else {
    long fpixel[] = { 1, 1, 1 }; // index like Fortran
    fpixel[2] = zplane + 1; // like Fortran
    fits_read_pix ( m_fptr, datatype, fpixel, nelements,
		    & nulval, ptr, & anynul, & m_status );
  }

  return m_status;
}

int
FitsFile::
fillIntVectorFromColumn( std::vector < int > & vec, int column )
{
  int anynul, status = 0;
  int nulval = 0;
  long nelements = vec.size();
  vector<int>::iterator it = vec.begin();
  int * ptr = new int [ nelements ];
  // Careful, counting columns like Fortran
  m_status = 0;
  fits_read_col_int( m_fptr, column+1, 1, 1, nelements, nulval,
		     ptr, &anynul, &status);
  copy ( ptr, ptr+nelements, it );

  return status;
}

void
FitsFile::
writeHDU ( long rows, int columns, 
	   const std::vector < std::string > & names,
	   const std::vector < std::vector < int > > & shapes,
	   const std::string & extname )
{
  char ** types = new char * [ columns ];
  char ** tform = new char * [ columns ];
  char ** tunits = 0;
  char * m_extname;

  vector < string > forms;  
  for ( int i = 0; i < columns; i ++ ) {
    types[i]=const_cast<char*> (names[i].c_str());
    int size = 1;
    const vector < int > & shape = shapes [ i ];
    unsigned int rank = shape.size();
    if ( rank > 1 ) {
      for ( unsigned int j = 1; j < rank; j++ ) { // strip row index
	int dim = shape [ j ];
	size *= dim;
      }
    }
    string form = String::convert ( size );
    form += "D";
    forms.push_back ( form );
//    unsigned int n = form.size();
    tform[i] = strdup ( form.c_str() );

//    tform [i] = const_cast <char *> ( forms[i].c_str() ); // will stay in scope
  }

  m_extname = const_cast<char*> (extname.c_str());

  fits_create_tbl( m_fptr, BINARY_TBL, 0, columns, 
		   types, tform, tunits, m_extname, &m_status);
//fits_report_error ( stdout, m_status );
  for ( int i = 0; i < columns; i ++ ) {
    const vector < int > & shape = shapes [ i ];
    unsigned int rank = shape.size();
    if ( rank > 1 ) {
      vector < long > naxes;
      for ( unsigned int j = 1; j < rank; j++ ) { // strip row index
	naxes.push_back ( shape [ j ] );
      }
      fits_write_tdim ( m_fptr, i+1, // like Fortran
			rank -1, &naxes [0], &m_status );
    }
  }    
  // clean up
  delete types;
  delete tform;
}

void
FitsFile::
writeImageHDU ( long x, long y )
{
  long naxes[2]={x,y};
  fits_create_img ( m_fptr, DOUBLE_IMG, 2, naxes, &m_status );
}


void
FitsFile::
writeColumn ( int col, const std::vector < double > & data )
{
  LONGLONG nelements = data.size ();
LONGLONG firstrow = 1;
LONGLONG firstelem = 1;
  fits_write_col( m_fptr, TDOUBLE, col+1, firstrow, firstelem, nelements,
		  const_cast < double * > ( &data[0] ), &m_status); 
}

void
FitsFile::
writePix ( long x, long y, const std::vector < double > & data )
{
  long fpixel[2]={1,1};
  long nelements = x * y;
  fits_write_pix ( m_fptr, TDOUBLE, fpixel, nelements,
		   const_cast < double * > ( &data[0] ), &m_status ); 
}

void
FitsFile::
writeCloseFile ()
{
  fits_close_file( m_fptr, &m_status );
  m_fptr = 0; // to remember its already closed.
}

bool
FitsFile::
pixCenter () const
{
  
  char * key = const_cast < char * > ( "PIXCENT" );
  bool yes = hasKey ( key );
  if ( yes ) 
    {
      if ( stringValueForKey ( key ) == "T" ) return true;
    }

  return false;

}

void
FitsFile::
writeRefPixelValues ( double value1, double value2 )
{
  char * key1 = const_cast < char * > ( "CRVAL1" );
  char * key2 = const_cast < char * > ( "CRVAL2" );

  fits_write_key( m_fptr, TDOUBLE, key1, &value1, NULL, &m_status );
  fits_write_key( m_fptr, TDOUBLE, key2, &value2, NULL, &m_status );
}
