/** @file

hippodraw::DataSourceController class implementation

Copyright (C) 2001-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DataSourceController.cxx,v 1.29 2007/04/23 21:57:20 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "DataSourceController.h"

#include "DataSource.h"

#include "pattern/string_convert.h"

#include <algorithm>
#include <fstream>
#include <utility>

#include <cassert>

using std::runtime_error;
using std::string;
using std::vector;

using namespace hippodraw;

DataSourceController * DataSourceController::s_instance = 0;

DataSourceController::DataSourceController ( )
  : m_base_name ( "in-memory" ),
    m_suffix ( 0 ),
    m_current_index ( -1 )
{
}

/** The copy constructor.  Only needed for SIP interface and should
    never be called. */
DataSourceController::
DataSourceController ( const DataSourceController & )
  : Observer ()
{
  assert ( false );
}

DataSourceController * DataSourceController::instance ( )
{
  if ( s_instance == 0 ) {
    s_instance = new DataSourceController ( );
  }
  return s_instance;
}

void
DataSourceController::
registerDataSourceFile ( DataSource * ds )
{
  m_ds_files.push_back ( ds );
}

DataSource * 
DataSourceController::
findDataSource ( const std::string & name ) const 
//   throw ( DataSourceException )
  throw ( runtime_error )
{
  DataSource * source = getDataSource ( name );

  if ( source == 0 ) {
    string what ( "DataSourceController: No NTuple with name `" );
    what += name;
    what += "' has been registered";
    throw runtime_error ( what );
  }

  return source;
}

int
DataSourceController::
indexOfDataSource ( const std::string & name ) const
{
  int index = -1;
  DataSourceList_t::size_type size = m_sources.size ();
  DataSourceList_t::size_type i = 0;

  for ( ; i < size; i++ ) {
    DataSource * ds = m_sources [ i ];
    const string & ds_name = ds -> getName ();
    if ( ds_name == name ) {
      index = i;
      break;
    }
  }

  return index;
}

DataSource * 
DataSourceController::
getDataSource ( const std::string & name ) const
{
  DataSource * source = 0;

  int index = indexOfDataSource ( name );
  if ( index >= 0 ) {
    source = m_sources [ index ];
  }

  return source;
}

void
DataSourceController::
changeName ( const std::string & old_name, 
	     const std::string & new_name )
{
  DataSource * ntuple = getDataSource ( old_name );
  if ( ntuple != 0 ) {
    ntuple -> setName ( new_name );
  }
}

bool
DataSourceController::
isFromFile ( const DataSource * source ) const
{
  DataSourceList_t::const_iterator first 
    = find ( m_ds_files.begin (), m_ds_files.end (), source );

  return first != m_ds_files.end ();
}

void
DataSourceController::
getDataSources ( std::vector < DataSource * > & sources,
		 bool all ) const
{
  sources.clear();
  unsigned int size = m_sources.size ();

  for ( unsigned int i = 0; i < size; i++ ) {
    DataSource * ds = m_sources [ i ];
    if ( all == false ) {
      bool yes = isFromFile ( ds );
      if ( yes ) continue;
    }
    sources.push_back ( ds );
  }
}


const vector < string > &
DataSourceController::
getNTupleNames () const
{
  m_names.clear ();

  DataSourceList_t::const_iterator i = m_sources.begin ();
  while ( i != m_sources.end() ) {
    const DataSource * source = *i++;
    const string & name = source -> getName ();
    m_names.push_back ( name );
  }

  return m_names;
}

string
DataSourceController::
registerNTuple ( DataSource * ds )
{
  string text = ds ->getName ();
  if ( text.empty () ) {
    text += "<";
    text += m_base_name;
    text += String::convert ( ++m_suffix );
    text += ">";
  }

  ds->setName ( text );
  registerNTuple ( text, ds );

  return text;
}

void
DataSourceController::
registerNTuple ( const std::string & key, DataSource * ntuple )
{
  ntuple -> setName ( key );
  ntuple -> addObserver ( this );
  m_sources.push_back ( ntuple );
  m_current_index = m_sources.size () - 1;
}

void
DataSourceController::
unregisterNTuple ( const DataSource * ntuple )
{
  DataSourceList_t::iterator i 
    = std::find ( m_sources.begin(), m_sources.end(), ntuple );
  if ( i != m_sources.end () ) {
    m_sources.erase ( i );
  }
  int size = static_cast < int > ( m_sources.size() );
  if ( m_current_index >= size ) {
    m_current_index = m_sources.size () -1;
  }
}

void
DataSourceController::
update ( const Observable * )
{
}

void
DataSourceController::
willDelete ( const Observable * observee )
{
  const DataSource * ntuple 
    = dynamic_cast < const DataSource * > ( observee );
  if ( ntuple == 0 ) return; // not NTuple

  unregisterNTuple ( ntuple );
}

DataSource *
DataSourceController::
getCurrent () const
{
  DataSource * source = 0;
  if ( m_current_index >= 0 ) {
    source = m_sources [ m_current_index ];
  }

  return source;
}

void
DataSourceController::
setCurrentIndex ( int i )
{
  m_current_index = i;
}
