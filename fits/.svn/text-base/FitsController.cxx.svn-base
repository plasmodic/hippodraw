/** @file

hippodraw::FitsController class implementation

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FitsController.cxx,v 1.70 2007/01/23 20:23:44 pfkeb Exp $

*/

#include "FitsController.h"

#include "FitsFile.h"
#include "FitsNTuple.h"

#include "controllers/CutController.h"
#include "datasrcs/DataSourceController.h"
#include "datasrcs/TupleCut.h"
#include "pattern/string_convert.h"
#include "plotters/PlotterBase.h"

#include <algorithm>
#include <fstream>


#include <cassert>

using std::ifstream;
using std::runtime_error;
using std::string;
using std::vector;

using namespace hippodraw;

/** Short cut to iterator type.
 */
typedef std::map < std::string, FitsFile * >::iterator FileMapIterator;

FitsController * FitsController::s_instance = 0;

FitsController *
FitsController::
instance ()
{
  if ( s_instance == 0 ) {
    s_instance = new FitsController ();
  }
  return s_instance;
}

FitsController::
~FitsController()
{
}

const std::string &
FitsController::
version () const
{
  float version = 0.0;
  fits_get_version ( & version );

  m_version = String::convert ( version );

  return m_version;
}

FitsFile *
FitsController::
openFile ( const std::string & name )
{
  FitsFile * file = 0;
  FileMapIterator first = m_file_map.find ( name );

  if ( first == m_file_map.end() ) {
    ifstream test ( name.c_str (), std::ios::in );
    if ( test.is_open () == false ) {
      string what ( "FitsController: File `" );
      what += name;
      what += "' not found";
      throw std::runtime_error ( what );
    }

    file = new FitsFile ( name );
    int status = file -> status();

    if ( status != 0 ) {
      string what ( "cfitsio: Error opening file `" );
      what += name;
      what += "'";
      delete file;
      throw runtime_error ( what );
    }
    m_file_map [ name ] = file;
  }
  else {
    file = first -> second;
  }

  return file;
}

/** @todo Why is this commented out?
 */
void
FitsController::
closeFile ( const std::string & )
{
//   FileMapIterator where = m_file_map.find ( name );
//   if ( where != m_file_map.end () ) {
//     m_file_map.erase ( where );
//     fitsfile * file = where -> second;
//     fits_close_file ( file, & status );
//   }
}

const vector < string > &
FitsController::
getNTupleNames ( const std::string & file_name )
{
  m_ntuple_names.clear ();
  FitsFile * file = openFile ( file_name );
  if ( file != 0 ) {
    file -> fillHDUNames ( m_ntuple_names );
  }

  return m_ntuple_names;
}


DataSource *
FitsController::
createNTuple ( const std::string & filename,
	       const std::string & name )
{
  int index = -1;

  const vector < string > & names 
    = getNTupleNames ( filename );
  unsigned int size = names.size ();

  for ( unsigned int i = 0; i < size; i++ ) {
    if ( name == names[i] ) {
      index = i;
      break;
    }
  }

  if ( index == -1 ) {
    string what ( "FitsController: File `" );
    what += filename; 
    what += "' ";
    what += "does not have extensison with name `";
    what += name;
    what  += "'";
    throw runtime_error ( what );
  }

  return createNTuple ( filename, name, index );
}

DataSource *
FitsController::
createNTuple ( const std::string & filename, 
	       const std::string & name,
	       int index )
{
  DataSource  * ntuple = 0;

  string ds_name = filename;
  ds_name += ": ";
  ds_name += name;

  FitsFile * file =  openFile ( filename );
  if ( file == NULL ) return NULL;

  int retval = file -> moveToHDU ( index );
  if ( retval == 0 ) {
    try {
      ntuple = new FitsNTuple ( file );
    }
    catch ( const runtime_error & e ) {
      throw e;
    }

    ntuple -> setTitle ( name );
    ntuple -> setName ( ds_name );
    DataSourceController * controller = DataSourceController::instance();
    controller -> registerNTuple ( ds_name, ntuple );
    controller -> registerDataSourceFile ( ntuple );
  }

  return ntuple;
}

void
FitsController::
checkForImage ( PlotterBase * plotter, const DataSource & source )
{
  const FitsFile * file = 0;
  try {
    const FitsNTuple & ntuple 
      = dynamic_cast < const  FitsNTuple & > ( source );
    file = ntuple.getFile ();
  }
  catch ( ... ) {
    // do nothing
  }
  if ( file == 0 ) return;

  FitsFileBase::HduType type = file -> getHduType ();
  if ( type != FitsFileBase::Image ) return;

  vector < long > sizes;
  file -> fillAxisSizes ( sizes );
  if ( sizes.size () > 3 ) {
    string what ( "FitsController: greater then 3D images not supported" );
    throw std::runtime_error ( what );
  }

  plotter -> setNumberOfBins ( Axes::X, sizes[0] );
  plotter -> setNumberOfBins ( Axes::Y, sizes[1] );

  vector < double > deltas;
  file -> fillImageDeltas ( deltas );
  plotter -> setBinWidth ( Axes::X, deltas[0] );
  plotter -> setBinWidth ( Axes::Y, deltas[1] );

  vector < double > ref_values;
  file -> fillRefPixelValues ( ref_values );
  vector < int > ref_indices;
  file -> fillRefPixelIndices ( ref_indices );

  // Process fits file with PIXCENT keyword.
  double pixToRef;
  if ((file ->pixCenter ())==true) pixToRef=0.5;
  else pixToRef=0.0;

  double x_orig = - deltas[0] * ( ref_indices[0]-1+pixToRef ) + ref_values[0];
  double y_orig = - deltas[1] * ( ref_indices[1]-1+pixToRef ) + ref_values[1];
  plotter ->setOffset ( Axes::X, x_orig );
  plotter ->setOffset ( Axes::Y, y_orig );


  // Fix out of range bug.
  Range range;
  if ( deltas[0] < 0. ) {
    range.setRange ( x_orig +  sizes[0]  * deltas[0], x_orig, -deltas[0] );
  }
  else {
    range.setRange ( x_orig, x_orig + (sizes[0] ) * deltas[0], 1. );
  }
  plotter -> setRange ( Axes::X, range, false, true );
  range.setLow ( y_orig );
  range.setLength ( ( sizes[1]  ) * deltas[1] );
  plotter -> setRange ( Axes::Y, range, false, true );

  plotter -> matrixTranspose ( true );

  bool yes = file -> isHammerAitoff ();
  if ( yes ) {
    plotter ->setAspectRatio ( 2.0 );
    plotter ->setFitsTransform ("HammerAito");
  }
}

/** @bug Needs to avoid duplicated code.
 */
void
FitsController::
writeNTupleToFile ( const DataSource * ntuple,
		    const std::string & filename )
{
  FitsFile * file = new FitsFile ( filename, true );

  long row = static_cast<long> ( ntuple -> rows() );
  int column = static_cast<int> (ntuple -> columns());
  const vector <string> labels = ntuple -> getLabels();
  vector < vector < int > > shapes;

  for ( int i = 0; i < column; i++ ){
    vector < int > shape;
    ntuple -> fillShape ( shape, i );
    shapes.push_back ( shape );
  }
  string extname;
  const string fullname = ntuple -> getName();
  string::size_type pos1 = fullname.find_last_of ( ":" );
  string::size_type pos2 = fullname.find_last_of ( "/" );
 
  if ( pos1 != string::npos && pos1!=1 ) {
    extname = fullname.substr ( pos1+2 );
  }
  else if (pos2 != string::npos) {   // only filename
    extname = "<no name>";
  }
  else {  // only title
    extname = fullname; 
  } 
  
  file -> writeHDU ( row, column, labels, shapes, extname );
  for ( int i = 0; i < column; i++ ) {
      const vector < double > & col = ntuple -> getColumn (i );
    file -> writeColumn ( i, col );
  }

  file -> writeCloseFile ();
  
  delete file;

}

void
FitsController::
writeNTupleToFile ( const std::string & name, const std::string & filename )
{
  DataSourceController * controller = DataSourceController::instance ();
  DataSource * ntuple 
    = controller -> findDataSource ( name ); // throws exception if not found
  if ( ntuple == 0 ) return;

  writeNTupleToFile ( ntuple, filename );  
}

void
FitsController::
writeImageToFile ( unsigned int x, unsigned int y,
		   const std::vector <double> & data,
		   const std::string & filename )
{

  FitsFile * file = new FitsFile ( filename, true );

  long xx = static_cast<long> ( x );
  long yy = static_cast<long> ( y );

  double xlow = 0.0 - static_cast<double> (x) / 2 ;
  double ylow = 0.0 - static_cast<double> (y) / 2;

  file -> writeImageHDU ( xx,yy );
  file -> writeRefPixelValues ( xlow, ylow );
  file -> writePix ( xx, yy, data );
  
  file -> writeCloseFile ();

  delete file;
  
}

size_t
FitsController::
calcColumnWidth ( const DataSource * source, unsigned int column ) const
{
  vector < int > shape;
  source -> fillShape ( shape, column );
  size_t rank = shape.size ();
  size_t count = 1;

  for ( size_t i = 1; i < rank; i++ ) {
    count *= shape[i];
  }
  return count;
}

int
FitsController::
writeNTupleToFile ( const DataSource * ds,
		    const std::string & filename,
		    const std::string & dsname, 
		    const std::vector < std::string > & column_list,
		    const std::vector < const TupleCut * > & cut_list)
{
  if ( column_list.empty() ) return 1;

  FitsFile * file = new FitsFile ( filename, true );

  unsigned int columnNumber = column_list.size();
  unsigned int size = ds->rows();

  vector < bool > acceptArray;
  CutController::fillAcceptedRows ( acceptArray, ds, cut_list );

  long rows = std::count ( acceptArray.begin(), acceptArray.end(), true );
  int columns = static_cast<int> (columnNumber);

  // Following collected to create FITS table before writing.

  vector < int > col_indices ( columnNumber );
  
  for ( unsigned int i = 0; i < columnNumber; i++ ) {
    const string & label = column_list [ i ];
    int index = ds -> indexOf ( label );
    if ( index < 0 ) { 
     ds -> throwIfInvalidLabel ( label );
    }
    col_indices [i] = index;
  }

  vector < size_t > sizes;
  vector < vector < int > > shapes;
  for ( int i = 0; i < columns; i++ ) {
    unsigned int size = calcColumnWidth ( ds, col_indices[i] );
    sizes.push_back ( size );
    vector < int > shape;
    ds -> fillShape ( shape, col_indices[i] );
    shapes.push_back ( shape );
  }

  file -> writeHDU ( rows, columns, column_list,
		     shapes, dsname );


  for ( unsigned int k = 0; k < columnNumber; k++ ) {
    std::vector < double > tempColumn;
    if ( sizes [ k ] == 1 ) {
      tempColumn.reserve (size );
    }
    else {
      tempColumn.reserve ( size * sizes[k] );
    }
    for ( unsigned int i = 0; i<size; i++ ) {
      if ( acceptArray[i]==true ) {
	int index = col_indices[k];
	if ( sizes [ k ] == 1 ) {
	  tempColumn.push_back ( ds -> valueAtNoCache (i, index) );
	}
	else { // array variable
	  double * array = ds -> doubleArrayAt ( i, index );
	  for ( std::size_t l = 0; l < sizes [ k ]; l++ ) {
	    tempColumn.push_back ( array[l] );
	  }
	}
      }
    }
    file -> writeColumn ( k, tempColumn );
      
    tempColumn.clear();
  }
 
  file -> writeCloseFile ();
  
  delete file;
  
  return 0;
}
