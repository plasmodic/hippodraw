/** @file

hippodraw::RootNTuple class implementation

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: RootNTuple.cxx,v 1.114 2007/07/02 18:17:16 pfkeb Exp $

*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "RootNTuple.h"

#include "RootBranch.h"

#include "axes/Range.h"
#include "pattern/Observer.h"
#include "pattern/string_convert.h"

#include "TBranch.h"
#include "TTree.h"

#ifdef SSTREAM_DEFECT
#include <strstream>
#else
#include <sstream>
#endif

#ifdef _MSC_VER
#define isinf _isinf
#endif

#include <cmath>
#include <cassert>
#include <climits>

using std::string;
using std::vector;
using std::runtime_error;

using namespace hippodraw;

RootNTuple::
RootNTuple ( TTree * tree )
  : DataSource ( false ),
    m_tree ( tree ),
    m_cur_entry ( UINT_MAX )
{
  initBranches ();
}

RootNTuple::
RootNTuple ( )
  : DataSource ( false ),
    m_tree ( 0 ),
    m_cur_entry ( UINT_MAX )
{
}

RootNTuple::~RootNTuple()
{
  Observable::notifyObservers ( &hippodraw::Observer::willDelete );

  BranchList_t::iterator first = m_data.begin();
  while ( first != m_data.end() ) {
    delete *first++;
  }

  CacheList_t::iterator cache = m_data_cache.begin ();
  while ( cache != m_data_cache.end() ) {
    vector< double > * p = *cache++;
    if ( p != 0 ) {
      delete p;
    }
  }
}

/** @todo If the name contains '[]' it should be removed.  The '[]' in
    the title is used to get the dimensions of the variable.
 */
void 
RootNTuple::
initBranches( )
{
  m_dups.clear ();

  TObjArray * branches = m_tree -> GetListOfBranches ();
  Int_t number = branches -> GetEntries ();
  
  for ( Int_t i = 0; i < number; i++ )
    {
      TObject * object = branches -> At ( i );
      TBranch * branch = dynamic_cast < TBranch * > ( object );
      if ( branch != 0 )
	{
	  branch -> SetBasketSize ( 320000 );
	  
	  RootBranch * h_branch = new RootBranch ( branch );
	  const string name = object -> GetName ();
	  int index = indexOf ( name );
	  if ( index < 0 ) {
	    addColumn ( name, h_branch );
	    m_multiDimensional.push_back( h_branch -> isMultiDimensional() );
	  }
	  else {
	    m_dups.push_back ( name );
	    RootBranch * rb = m_data [ index ];
	    m_data [ index ] = h_branch;
	    delete rb;
	  }
	}
    }
}

bool
RootNTuple::
rotateWheels( std::vector < int > & wheels,
	      const std::vector < int > & sz,
	      unsigned int d )
{
  bool full = false;
  
  if( d < sz.size() - 1 ) // Still not at the last wheel...
    {
      if( rotateWheels( wheels, sz, d + 1 ) == true )
	{
	  wheels[d]++;

	  full = ( wheels[d] == static_cast< int >( sz[d] )  ) ? 
	    true : false;

	  if( full )
	    wheels[d] = 0;
	  
	  return full;
	}
    }
  else // Finally at the last wheel. Phew!
    {
      wheels[ d ]++;
      full = ( wheels[d] == static_cast< int >( sz[d] )  ) ? true : false;

      if( full )
	wheels[ d ] = 0;
      return full;
    }
  
  return full;
}

void
RootNTuple::
notifyObservers ( ) const
{
  Observable::notifyObservers ();
}

unsigned int
RootNTuple::
rows() const
{
  Double_t number =  m_tree -> GetEntries ();
  return static_cast < unsigned int > ( number );
}

void
RootNTuple::
fillDataCache ( unsigned int column )
{
  unsigned int size = RootNTuple::rows ();
  vector < double > * cache = new vector < double > ( size );
  RootBranch * data = m_data [ column ];
  for ( unsigned int row = 0; row < size; row++ ) {
    double value = data -> valueAt ( row );
    cache -> operator [] ( row ) = value;
  }
  m_data_cache [ column ] = cache;
}

bool
RootNTuple::
empty () const
{
  return rows () == 0;
}

double
RootNTuple::
valueAt ( unsigned int row, unsigned int column ) const
{
  if ( m_data_cache [ column ] == 0 ) {
    RootNTuple * rtuple = const_cast < RootNTuple * > ( this );
    rtuple -> fillDataCache ( column );
  }
  const vector < double > & cache = *m_data_cache [ column ];

  return cache[row];
}

double
RootNTuple::
valueAtNoCache ( unsigned int row, unsigned int column ) const
{
  double value = 0;
  if ( column < m_data.size () ) { // within original ROOT file
    RootBranch * data = m_data [ column ];
    value = data -> valueAt ( row );
  }
  else { // added column
    value = valueAt ( row, column );
  }
  return value;
}

RootData::Type
RootNTuple::
getType ( unsigned int column ) const
{
  RootBranch * data = m_data [ column ];

  return data -> getType ();
}

double *
RootNTuple::
doubleArrayAt ( unsigned int row, unsigned int column ) const
{
  RootBranch * data = m_data [ column ];

  return data -> doubleArrayAt ( row );
}

float *
RootNTuple::
floatArrayAt ( unsigned int row, unsigned int column ) const
{
  RootBranch * data = m_data [ column ];

  return data -> floatArrayAt ( row );
}

int *
RootNTuple::
intArrayAt ( unsigned int row, unsigned int column ) const
{
  RootBranch * data = m_data [ column ];

  return data -> intArrayAt ( row );
}

unsigned int *
RootNTuple::
uintArrayAt ( unsigned int row, unsigned int column ) const
{
  RootBranch * data = m_data [ column ];

  return data -> uintArrayAt ( row );
}


void
RootNTuple::
clear ()
{
  for ( unsigned int i = 0; i < m_data_cache.size (); i++ ) {
    if ( m_data_cache[i] != 0 ) {
      delete m_data_cache [i];
    }
    m_data_cache[i] = 0;
  }
}

void
RootNTuple::
reserve ( unsigned int )
{
  // does nothing.
}

double
RootNTuple::
operator [] ( std::vector < unsigned int > & ) const // indices ) const
{
  assert ( false );
  return 0.;
}

/** @bug @@@@@@ If a column has an array varible, then get seg fault.
    @todo Could bypass calling valueAt if performance is shown to be
    an issue.
*/
const vector < double > &
RootNTuple::
getRow ( unsigned int row ) const
{
  const vector < string > & labels = getLabels ();
  unsigned int size = labels.size();
  for ( unsigned int column = 0; column < size; column++ ) {
    const string & label = labels [ column ];
    if ( isUseable ( label ) ) {
      smartExpandRootNTuple ( label );
    }
    else {
      std::string what ( "RootNTuple: can not export data source\n"
			 "because some column(s) cantain\n"
			 " variable length data" );
      throw runtime_error ( what );
    }
  }

  size = m_data.size ();
  m_array.resize ( size );
  for ( unsigned int column = 0; column < size; column++ ) {
    // Use this method to avoid reading whole column
    RootBranch * branch = m_data [ column ];
    m_array [ column ] = branch -> valueAt ( row );
  }

  return m_array;
}

int
RootNTuple::
addColumn ( const std::string & label,
	    RootBranch * branch )
{
  // Check if label already exists.
  int index = indexOf ( label );
  if ( index >= 0 ) {
    string what ( "RootNTuple Attempt to add a column whose label"
		  " is same as other column." );
    throw runtime_error ( what );
  }

  m_data.push_back ( branch );
  m_data_cache.push_back ( 0 );
  addLabel ( label );
  
  return m_data.size() - 1;
}


int
RootNTuple::
addColumn ( const std::string & label,
	    const std::vector < double > & col )
{
  // Check if label already exists.
  int index = indexOf ( label );
  if ( index >= 0 ) {
    string what ( "RootNTuple Attempt to add a column whose label"
		  " is same as other column." );
    throw runtime_error ( what );
  }

  unsigned int new_size = col.size ();
  //  Check if column has right size.
    if ( m_data.empty () == false ) {
      unsigned int old_size = rows ();
  
      if ( old_size != 0 && old_size != new_size ) {
        string what ( "RootNTuple Attempt to add a column whose size"
  		    " is not equal to other columns." );
        throw runtime_error ( what );
      }
    }
  vector < double > * vec = new vector < double > ( col );
  m_data_cache.push_back ( vec );
  m_multiDimensional.push_back ( false );
    
  addLabel ( label );

  return m_data.size() - 1;
}

void
RootNTuple::
replaceColumn ( unsigned int col,
		const std::vector < double > & data )
{
  unsigned int size = columns ();
  if ( col >= size ) {
    const string what
      ( "RootNTuple: Attempt to replace non-existant column." );
    throw runtime_error ( what );
  }

  size = rows ();
  unsigned int new_size = data.size ();
  if ( size != 0 && size != new_size ) {
    const string what
      ( "RootNTuple: Attempt to replace column with one whose "
	"size is not equal to other columns." );
      throw runtime_error ( what );
  }
  m_data_cache [ col ] -> resize ( new_size );
  std::copy ( data.begin(), data.end(), m_data_cache [ col ] -> begin() );

  notifyObservers ();
}

void
RootNTuple::
setShape ( std::vector < unsigned int > & shape )
{
  assert ( false ); // not tested
  m_shape = shape;
}

const vector < unsigned int > &
RootNTuple::
getShape () const
{
  assert ( false ); // not tested
  return m_shape;
}

bool
RootNTuple::
isMultiDimensional ( const std::string & column ) const
{
  unsigned int colIndex = indexOf( column );

  return m_multiDimensional[ colIndex ];
}

bool
RootNTuple::
isUseable ( const std::string & column ) const
{
  unsigned int colIndex = indexOf( column );
  RootBranch * branch = m_data [ colIndex ];

  return branch -> isUseable ();
}

/** @bug If a column is an added one, shape is assumed to be rank 1.
    Need to add support for rank > 1 added columns.
 */
void
RootNTuple::
fillShape ( std::vector < int > & shape, unsigned int column ) const
{
  shape.clear ();
  shape.push_back ( RootNTuple::rows () ); // avoid QtRootNTuple
  BranchList_t::size_type size = m_data.size ();
  if ( column < size ) { // do nothing for added columns
    RootBranch * col = m_data [ column ];
    const vector < int > & sub_shape = col -> getShape ();
    unsigned int rank = sub_shape.size ();

    for ( unsigned int i = 0; i < rank; i++ ) {
      shape.push_back ( sub_shape [ i ] );
    }
  }
}

bool
RootNTuple::
setReleventIndex( const std::string & column,
		  const std::vector< unsigned int >& index )
{
  // Do not call this function on scalars
  assert( isMultiDimensional( column ) );
  
  unsigned int colIndex = indexOf ( column );
  RootBranch * col = m_data [ colIndex ];

  col -> setReleventIndex( index );

  return true;
}


/** Returns the TBranch which was used to initialize this branch */
TBranch *
RootNTuple::
getTBranch( const std::string & column )
{
  unsigned int colIndex = indexOf ( column );
  RootBranch * col = m_data [ colIndex ];
  
  return col -> getTBranch();
}


/** @todo Replace use of ostrstream with String::convert
 */
void
RootNTuple::
smartExpandRootNTuple ( const std::string & column ) const
{
  // Expansion is --SURELY NOT-- needed if the data is not multidimensional
  if ( isMultiDimensional( column ) == true )
    {
      // Has the expansion of this column already taken place?
      bool flag = false;
      const vector< string > & labels = getLabels();
      
      for( unsigned int i = 0; i < labels.size(); i++ ) {
	const string & label = labels[i];
	if( label.find( column + "[" ) != string::npos ) {
	  flag = true;
	  break;  // No need to check all the labels
	}
      }
      
      // If no then go ahead and expand
      if( flag == false )
	{
	  unsigned int colIndex = indexOf ( column );
	  RootBranch * colRootBranch = m_data [ colIndex ];
	  TBranch * colTBranch = colRootBranch -> getTBranch();
	  
	  unsigned int dims = colRootBranch -> getRank ();
	  const vector < int > & sz =colRootBranch -> getShape ();
	  int nelems = colRootBranch -> numberOfElements();

	  vector< unsigned int > indx( dims, 0);
	  
	  vector< int > wheels( dims, 0);
	  wheels[ dims - 1 ] = -1;
	  RootNTuple * rtuple = const_cast < RootNTuple * > ( this );
	  
	  for( int i = 0; i < nelems; i++ )
	    {
#ifdef SSTREAM_DEFECT
	      std::ostrstream namestream;
#else
	      std::ostringstream namestream;
#endif
	      namestream << column;
	      
	      rtuple ->  rotateWheels( wheels, sz, 0 );
	      for ( unsigned int j = 0; j < dims; j++ ) {
		namestream << "[" << wheels[ j ] << "]";
	      }
	      
	      colTBranch -> SetBasketSize ( 320000 );
	      RootBranch * h_branch = new RootBranch ( colTBranch );
	      
	      for( unsigned int k = 0; k < wheels.size(); k++ )
		indx[ k ] = static_cast< unsigned int > ( wheels[ k ] );
	      h_branch -> setReleventIndex( indx );
	      
	      rtuple -> addColumn ( namestream.str(), h_branch );
	      rtuple -> m_multiDimensional.push_back( false );
	    }
	}
    }
}

void
RootNTuple::
expandIfNeeded ( const std::string & column ) const
{
  string::size_type pos = column.find_first_of ( '[' );

  if ( pos != string::npos ) {
    const string label = column.substr ( 0, pos );

    if (isMultiDimensional ( label ) ) {
      smartExpandRootNTuple ( label );
    }
  }
}

void
RootNTuple::
expandIfNeeded ( const std::vector < std::string > & labels ) const
{
  unsigned int size = labels.size ();

  for ( unsigned int i = 0; i < size; i++ ) {
    const string & column = labels [ i ];
    expandIfNeeded ( column );
  }
}

const vector < double > &
RootNTuple::
getColumn ( const std::string & name ) const
{
  expandIfNeeded ( name );
  throwIfInvalidLabel ( name );

  int index = indexOf ( name );

  return RootNTuple::getColumn ( index );
}

const vector < double > &
RootNTuple::
getColumn ( const std::string & name,
	    const std::vector < int > & indices ) const
{
  const string label = createBinding ( name, indices );

  return RootNTuple::getColumn ( label );
}

const vector < double > &
RootNTuple::
getColumn ( unsigned int index ) const
{
  isValidColumn ( index );

  if ( m_data_cache [ index ] == 0 ) {
    RootNTuple * rtuple = const_cast < RootNTuple * > ( this );
    rtuple -> fillDataCache ( index );
  }

  return *m_data_cache [ index ];
}

std::string
RootNTuple::
createBinding ( const std::string & label, 
		const std::vector < int > & indices ) const
{
  string text ( label );

  unsigned int size = indices.size();
  for ( unsigned int i = 0; i < size; i++ ) {
    text += "[";
    text += String::convert ( indices[i] );
    text += "]";
  }

  return text;
}
