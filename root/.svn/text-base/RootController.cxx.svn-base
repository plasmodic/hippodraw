/** @file

hippodraw::RootController class implementation

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: RootController.cxx,v 1.53 2006/12/19 19:12:40 pfkeb Exp $

*/

#include "RootController.h"

#include "RootNTuple.h"

#include "datasrcs/DataSourceController.h"

#include "TFile.h"
#include "TKey.h"
#include "TTree.h"

#include "TH2.h"

#include <fstream>
#include <stdexcept>

#include <cassert>

using std::string;
using std::vector;

using namespace hippodraw;

/** Short cut to iterator type.
 */
typedef std::map < std::string, TFile * > ::iterator FileMapIterator_t;

RootController * RootController::s_instance = 0;

RootController *
RootController::
instance ()
{
  if ( s_instance == 0 ) {
    s_instance = new RootController ();
  }
#ifdef _MSC_VER
  TTree * tree = new TTree (); // force loading of dll
  delete tree;
#endif
  TH2::Class(); // This is needed to avoid ROOT crashing upon reading
		// a second file that contains a TH2 object.
  return s_instance;
}

RootController::
~RootController()
{
}

const std::string &
RootController::
version () const
{
  m_version = ROOT_RELEASE;
  return m_version;
}

TFile *
RootController::
openFile ( const std::string & name )
{
  TFile * file = 0;
  FileMapIterator_t first = m_file_map.find ( name );

  if ( first == m_file_map.end() ) {
    ifstream test ( name.c_str (), std::ios::in );
    if ( test.is_open () == false ) {
      string what ( "RootController: File `" );
      what += name;
      what += "' was not found.";

      throw std::runtime_error ( what );
    }
    file = new TFile ( name.c_str() );
    m_file_map [ name ] = file;
  }
  else {
    file = first -> second;
  }

  return file;
}

void
RootController::
closeFile ( const std::string & name )
{
  FileMapIterator_t where = m_file_map.find ( name );
  if ( where != m_file_map.end () ) {
    TFile * file = where -> second; // do before following for VC++ debugger.
    m_file_map.erase ( where );
    delete file;
  }
}

/** @bug @@@@@@ If can not open file, should thrown an exception.
    When fixing this bug, check also the FitsController for same bug.
 */
const vector < string > & 
RootController::
getNTupleNames ( const std::string & file_name )
{
  m_ntuple_names.clear();

  TFile * file = openFile ( file_name );

  if ( file != 0 ) {
    TList * keys = file -> GetListOfKeys ();
    Int_t size = keys -> GetSize ();

    for ( Int_t i = 0; i < size; i++ ) {
      TObject * obj = keys -> At ( i );
      TKey * key = dynamic_cast < TKey * > ( obj );
      const string class_name = key -> GetClassName ();
      if ( class_name == "TTree" || 
	   class_name == "TNtuple" ||
	   class_name == "TNtupleD" ) {
	const string name = key -> GetName ();
	m_ntuple_names.push_back ( name );
      }
    }
  }
  closeFile ( file_name );

  return m_ntuple_names;
}

TTree *
RootController::
getTree ( const std::string & filename,
	  const std::string & treename )
{
  TFile * file =  openFile ( filename );
  if ( file == NULL ) return NULL;

  TObject * object = file -> Get ( treename.c_str() );
  TTree * tree = dynamic_cast < TTree * > ( object );

  return tree;
}

DataSource *
RootController::
createNTuple ( const std::string & filename, const std::string & treename )
{
  TTree * tree = getTree ( filename, treename );
  if ( tree == NULL ) return NULL;

  DataSource * ntuple = new RootNTuple ( tree );

  return initNTuple ( ntuple, filename, treename );
}

DataSource *
RootController::
initNTuple ( DataSource * ntuple, const std::string & filename,
	     const std::string & treename )
{
  string ds_name = filename;
  ds_name += ": ";
  ds_name += treename;

  ntuple -> setTitle ( treename );
  ntuple -> setName ( ds_name );
  DataSourceController * controller = DataSourceController::instance();
  controller -> registerNTuple ( ds_name, ntuple );
  controller -> registerDataSourceFile ( ntuple );

  m_tuple_map [ ntuple ] = filename;
  ntuple -> addObserver ( this );

  return ntuple;
}

DataSource *
RootController::
createNTuple ( const std::string & name )
{
  DataSource * rtuple = 0;

  string::size_type pos = name.find_last_of ( ':' );
  if ( pos == string::npos ) {
    const vector < string > & tree_names = getNTupleNames ( name );
    rtuple = createNTuple ( name, tree_names[0] );
  }
  else {
    const string filename = name.substr ( 0, pos );
    string tree_name = name.substr ( pos + 1 );
    pos = tree_name.find_first_not_of ( ' ' );
    tree_name.erase ( 0, pos );
    rtuple = createNTuple ( filename, tree_name );
  }

  return rtuple;
}

void
RootController::
fillDimSize ( std::vector < int > & dims, 
	      const DataSource * source,
	      const std::string & column )
{
  dims.clear();

  const RootNTuple * rtuple = dynamic_cast < const RootNTuple * > ( source );
  if ( rtuple != 0 ) {
    int index = rtuple -> indexOf ( column );
    vector < int > shape; // complete shape include events
    rtuple -> fillShape ( shape, index );
    for ( unsigned int i = 1; i < shape.size(); i++ ) {
      dims.push_back ( shape [ i] ); // just shape of variable
    }
  }
}

bool
RootController::
smartExpandRootNTuple ( DataSource * source, std::string & column )
{
  bool yes = false;
  RootNTuple * rtuple = dynamic_cast < RootNTuple * > ( source );
  if ( rtuple != 0 ) {
    rtuple -> smartExpandRootNTuple ( column );
    yes = true;
  }

  return yes;
}

void
RootController::
update ( const Observable * )
{
  // nothing to be done.
}

/** @todo This method does get called with obs != RootNTuple, find out
    why.
 */
void
RootController::
willDelete ( const Observable * obs )
{
  const RootNTuple * tuple = dynamic_cast < const RootNTuple * > ( obs );
//   assert ( tuple != 0 ); // only observing this type
  if ( tuple != 0 ) {
    TupleToFileMap_t::iterator first = m_tuple_map.find ( tuple );
    assert ( first != m_tuple_map.end () ); // should be there.

    const string & filename = first -> second;
    // Are there any others
    int count = 0;
    first = m_tuple_map.begin();
    while ( first != m_tuple_map.end () ) {
      const string & name = first -> second;
      if ( name == filename ) {
	count ++;
      }
      ++first;
    }

    if ( count == 1 ) {
      closeFile ( filename );
    }
  }
}
