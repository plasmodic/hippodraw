/** @file

hippodraw::NTupleController class implementation

Copyright (C) 2001-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: NTupleController.cxx,v 1.131 2007/04/23 21:57:20 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "NTupleController.h"

#include "CircularBuffer.h"
#include "DataSourceController.h"
#include "TupleCut.h"

#include "pattern/string_convert.h"



#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <utility>

#include <cassert>

using std::endl;
using std::ofstream;
using std::runtime_error;
using std::string;
using std::vector;

using namespace hippodraw;

NTupleController * NTupleController::s_instance = 0;

NTupleController::NTupleController ()
{
}

/** The copy constructor.  Only needed for SIP interface and should
    never be called. */
NTupleController::NTupleController ( const NTupleController & )
{
  assert ( false );
}

NTupleController * NTupleController::instance ( )
{
  if ( s_instance == 0 ) {
    s_instance = new NTupleController ( );
  }
  return s_instance;
}

string::size_type
NTupleController::
findWhite ( const std::string & line, unsigned int left, bool tabs_only )
{
  string::size_type right = line.find( '\t', left );

  if( line.find( '\n', left ) < right ) right = line.find( '\n', left );

  if ( tabs_only == false ) { 
    if( line.find( ' ', left ) < right ) right = line.find( ' ', left );
  }

  return right;
}

void
NTupleController::
splitAndFill ( const std::string & line,
	       std::vector < std::string > & values )
{
  static string  white ( " \n\r\t" );
  values.clear ();

  string::size_type start = line.find_first_not_of ( white, 0 );
  while ( start != string::npos ) {
    string::size_type end = line.find_first_of ( white, start );
    if ( end != string::npos ) {
      values.push_back ( string ( line, start, end - start ) );
      start = line.find_first_not_of ( white, end );
    }
    else {
      values.push_back ( string ( line, start ) ); // to end
      break;
    }
  }
}

/** 
    @request If the label for a column is very long, then it is
    probably an error in the file format.  One should raise a dialog
    if attempting to read from the GUI, or throw an exception if from
    Python to asked the user to continue or not. 

    @request If line of labels contains no tabs at all, then should
    probably assume that any white space separates the labels.

    @request If bad format detected, try to say what was bad.

*/
int
NTupleController::
readAsciiNTuple ( NTuple * ntuple, const std::string & filename )
{
  // Two local flags to define if name or labels are missing
  bool hasTitle = true;
  bool hasLabel = true;

  string dir_name = filename;

  string::size_type pos = dir_name.find_last_of( '/' );
  if ( pos == string::npos ) {
    dir_name = ".";
  } else {
    dir_name.erase( pos );
  }
  dir_name += "/";

  std::ifstream infile( filename.c_str() );
  if( !infile.is_open() ) {
    string what ( "NTupleController: Could not open file\n`" );
    what += filename.c_str();
    what += "'\n";
    what += "Files doesn't exist or is not readable.";
    throw runtime_error ( what );
  }
  string line;
  if( !std::getline( infile, line ) ) {
    return -2;
  }

  // ignore spaces at the beginning of the line
  while( findWhite( line, 0, false ) == 0 ) line = line.substr( 1 );
  string::size_type firstTab = line.find( '\t', 0 );

  // fist line with tab, no name
  if ( firstTab != line.npos ) { 
    hasTitle = false;
    ntuple->setTitle ("<none>");
    string firstword = line.substr ( 0, firstTab );
    
    // first line are numbers, no labels
    if ( std::atof ( firstword.c_str() ) != 0.0 ) { 
      hasLabel = false;
    }
  }
  else {
    // first line is a single number, no name, no labels
    if ( std::atof ( line.c_str() ) != 0.0 ) {
      hasTitle = false;
      ntuple->setTitle("<none>");
      hasLabel = false;
    }
    // name exist
    else ntuple->setTitle ( line );
  }

  // second line missing
  if ( hasTitle ){
    if ( !std::getline( infile, line ) ) 
    return -2;
  }

  string::size_type size = line.size ();
  if ( line[size-1] == '\r' ) { // CR on Windows written file
    line.erase ( size-1 );
  }
  line += "\n";
  string::size_type right = findWhite( line, 0, true );
  string firstlabel = line.substr( 0, right );
  // current line start with number, no labels
  if ( std::atof ( firstlabel.c_str() ) != 0.0 ) hasLabel=false;

  vector< string > labels;
  string::size_type left = 0;
  // label index, used when labels in the file are missing
  int i=0; 

  while( right != line.npos ){
    if( right == left ){
      left++;
      right = findWhite( line, left, true );
      continue;
    }

    string label;
    if (!hasLabel) { 
      // make a label name when label name is missing 
      label="Column"+String::convert(i); 
      i++;
    }
    // read label name from the file
    else label = line.substr( left, right - left );
    labels.push_back ( label );
    left = right + 1;
    right = findWhite( line, left, true );
  }
  if ( labels.size () == 0 ) {
    return -2;
  }

  ntuple->setLabels ( labels );

  unsigned int columns = labels.size ();
  vector < double > vals ( columns );

  if ( hasLabel ) {
    std::getline ( infile, line );
  }

  do {
    // else use current line
    vector < string > values;
    splitAndFill ( line, values );

    if ( values.size() != columns ) {
      return -2;
    }
    for ( unsigned int i = 0; i < columns; i++ ) {
      vals[i] = atof ( values[i].c_str() );
    }
    ntuple -> addRow ( vals );

  } while ( std::getline ( infile, line ) );

  vector < unsigned int > shape ( 2 );
  shape[0] = ntuple -> rows();
  shape[1] = ntuple -> columns ();
  ntuple -> setShape ( shape );

  return 0;
}

DataSource * 
NTupleController::
createNTuple ( const std::string & filename )
{
  unsigned int columns = 0;
  NTuple * nt = new NTuple ( columns );
  try {
    fillFromFile ( filename, nt );
  }
  catch ( const runtime_error & e ) {
    delete nt;
    throw e;
  }

  return nt;
}

void
NTupleController::
fillFromFile ( const std::string & filename, NTuple * nt )
{
  int retval = readAsciiNTuple ( nt, filename );

  if ( retval < 0 ) {
    std::string what ( "NTupleController: File:\n `" );
    what += filename;
    what += "'\n ";
    if ( retval == -1 ) {
      what += "could not be found.";
    }
    else {
      what += "had bad format.";
    }
    throw std::runtime_error ( what );
  }

  DataSourceController * controller = DataSourceController::instance ();
  DataSource * ds = controller -> getDataSource ( filename );

  // Register the NTuple name if it's not in memory.
  // Alway register that it came from a file.
  nt -> setName ( filename ); // before registering.
  if ( ds == 0 ) controller -> registerNTuple ( filename, nt );
  controller -> registerDataSourceFile ( nt );
}

NTuple * 
NTupleController::
createNTuple ( const std::vector < std::string > & labels )
{
  NTuple * nt = new NTuple ( labels );
  DataSourceController * controller = DataSourceController::instance ();
  controller -> registerNTuple ( nt );

  return nt;
}

CircularBuffer * 
NTupleController::
createCircularBuffer ( const std::vector < std::string > & labels )
{
  CircularBuffer * nt = new CircularBuffer ( labels );
  DataSourceController * controller = DataSourceController::instance ();
  controller -> registerNTuple ( nt );

  return nt;
}

NTuple * 
NTupleController::
createNTuple ( unsigned int columns )
{
  NTuple * nt = new NTuple ( columns );
  DataSourceController * controller = DataSourceController::instance ();
  controller -> registerNTuple ( nt );

  return nt;
}

CircularBuffer * 
NTupleController::
createCircularBuffer ( unsigned int columns )
{
  CircularBuffer * nt = new CircularBuffer ( columns );
  DataSourceController * controller = DataSourceController::instance ();
  controller -> registerNTuple ( nt );

  return nt;
}

NTuple * 
NTupleController::
createNTuple ( )
{
  NTuple * nt = new NTuple ();
  DataSourceController * controller = DataSourceController::instance ();
  controller -> registerNTuple ( nt );

  return nt;
}

CircularBuffer * 
NTupleController::
createCircularBuffer ( )
{
  CircularBuffer * nt = new CircularBuffer ();
  DataSourceController * controller = DataSourceController::instance ();
  controller -> registerNTuple ( nt );

  return nt;
}

DataSource * 
NTupleController::
findDataSource ( const std::string & name ) const
{
  DataSourceController * controller = DataSourceController::instance ();

  return controller -> findDataSource ( name );
}

void
NTupleController::
changeName ( DataSource * ntuple, const std::string & new_name )
{
  ntuple -> setName ( new_name );
}

int 
NTupleController::
writeNTupleToFile ( const std::string & name,
		    const std::string & filename )
{
  DataSourceController * controller = DataSourceController::instance ();
  DataSource * ntuple 
    = controller -> findDataSource ( name );
  // throws exception if not found
  if ( ntuple == 0 ) return -1;

  return writeNTupleToFile ( ntuple, filename );
}

/** @todo Define return codes for why the file was not written. */
int 
NTupleController::
writeNTupleToFile ( DataSource * ntuple, 
		    const std::string & filename )
{
  ofstream file ( filename.c_str() );
  if ( file.is_open () == false ) {
    return 1;
  }
  file << ntuple->title() << endl;

  const vector < string > & labels = ntuple->getLabels ();
#ifdef ITERATOR_MEMBER_DEFECT
  std::
#endif
  vector < string > ::const_iterator first = labels.begin ();
  string label = *first++;
  file << label;
  while (  first != labels.end() ) {
    label = *first++;
    file << "\t" << label;
  }
  file << endl;

  unsigned int rows = ntuple->rows ();
  for ( unsigned int i = 0; i < rows; i++ ) {
    const vector < double > & row = ntuple->getRow ( i );

#ifdef ITERATOR_MEMBER_DEFECT
    std::
#endif
      vector < double > ::const_iterator first = row.begin();
    while ( first != row.end() ) {
      file << "\t" << *first++;
    }
    file << endl;
  }

  DataSourceController * controller = DataSourceController::instance ();
  controller -> registerDataSourceFile ( ntuple );

  return 0;
}

/** @bug If writing the DataSource to a file fails, no error is
    reported.
*/
void
NTupleController::
saveNTuples ( const std::string & fileprefix, const std::string & filesuffix )
{
  string::size_type pos = fileprefix.find_last_of ( '/' );
  const string path = fileprefix.substr ( 0, pos + 1 );
  const string basename = fileprefix.substr ( pos + 1 );

  DataSourceController * controller = DataSourceController::instance ();
  vector < DataSource * > tuples;
  controller->getDataSources ( tuples, false ); // only non files
  unsigned int  size = tuples.size();

  for ( unsigned int i = 0; i < size; i++ ) {
    DataSource * ntuple = tuples[i];
    const string & tuple_name = ntuple->getName ();

    string filename ( basename );
    filename += String::convert ( i );
    filename += filesuffix;

    string tuple_file ( path );
    tuple_file += filename;

    writeNTupleToFile ( tuple_name, tuple_file );
    controller -> changeName ( tuple_name, filename );
  }
}

string
NTupleController::
registerNTuple ( DataSource * ds )
{
  DataSourceController * controller = DataSourceController::instance ();
  controller -> registerNTuple ( ds );

  return ds -> getName ();
}

void
NTupleController::
registerNTuple ( const std::string & key, DataSource * ntuple )
{
  DataSourceController * controller = DataSourceController::instance ();
  controller -> registerNTuple ( key, ntuple );
}


NTuple *
NTupleController::
createNTuple( const std::vector < std::string > & column_list,
	      const std::vector < const TupleCut * > & cut_list, 
	      DataSource * ds )
{
  if ( column_list.empty() ) return NULL;

  unsigned int columnNumber = column_list.size();
  unsigned int cutNumber = cut_list.size();
  unsigned int size = ds->rows();

  NTuple * nt = new NTuple( column_list );
 
  // Check all the rows.
  for ( unsigned int i = 0; i < size; i++ )
    {
      // If cut is not selected, default is accept.
      bool accept = true;

      // Check all the cuts.
      for ( unsigned int j = 0; j < cutNumber; j++ ) 
	{
	  const TupleCut * tc = cut_list[j];
	  accept = tc -> acceptRow ( ds, i );
	  if (!accept) break;
	}
      
      // Add the row when all cuts accept the row.
      if (accept) {
	vector <double> row;
	for ( unsigned int k = 0; k < columnNumber; k++ ) 
	  {
	    row.push_back(ds->valueAtNoCache(i,ds->indexOf(column_list[k])));
	  }
	nt->addRow(row);
      }
    }

  return nt;

}

int
NTupleController::
createNTupleToFile( const std::vector < std::string > & column_list,
		    const std::vector < const TupleCut * > & cut_list, 
		    DataSource * ds,
		    const std::string & filename,
		    const std::string & dsname)
{
  if ( column_list.empty() ) return 1;

  ofstream file ( filename.c_str() );
  if ( file.is_open () == false ) {
    return 1;
  }
  // Check the column list and create indices for inner loop
  unsigned int columnNumber = column_list.size();
  vector < int > col_indices ( columnNumber );
  
  for ( unsigned int i = 0; i < columnNumber; i++ ) {
    const string & label = column_list [ i ];
    int index = ds -> indexOf ( label );
    if ( index < 0 ) { 
     ds -> throwIfInvalidLabel ( label );
    }
    col_indices [i] = index;
  }

  file << dsname << endl;

#ifdef ITERATOR_MEMBER_DEFECT
  std::
#endif
  vector < string > ::const_iterator first = column_list.begin ();
  string label = *first++;
  file << label;
  while (  first != column_list.end() ) {
    label = *first++;
    file << "\t" << label;
  }
  file << endl;

  unsigned int cutNumber = cut_list.size();
  unsigned int size = ds->rows();

   // Check all the rows.
  for ( unsigned int i = 0; i < size; i++ )
    {
      // If cut is not selected, default is accept.
      bool accept = true;

      // Check all the cuts.
      for ( unsigned int j = 0; j < cutNumber; j++ ) 
	{
	  const TupleCut * tc = cut_list[j];
	  accept = tc -> acceptRow ( ds, i );
	  if (!accept) break;
	}
      
      // Add the row to the file when all cuts accept the row.
      if (accept) {
	
	for ( unsigned int k = 0; k < columnNumber; k++ ) 
	  {
	    int index = col_indices [ k ];
	    file << "\t" << ds -> valueAtNoCache (i, index );	    
	  }
	file << endl;
      }
    }
  return 0; 
}
