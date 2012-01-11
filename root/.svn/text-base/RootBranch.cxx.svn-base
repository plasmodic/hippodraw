/** @file

hippodraw::RootBranch class implementation

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: RootBranch.cxx,v 1.57 2007/02/12 19:14:49 pfkeb Exp $

*/

#include "RootBranch.h"

#include "axes/Range.h"

#include <cassert>

using std::string;
using std::vector;

using namespace hippodraw;

/** @request @@@@@@ Should make this branch unuseable instead of
    throwing exception if the data type is not supported.
 */
RootBranch::
RootBranch ( TBranch * branch )
  : m_branch ( branch ),
    m_vector_double_data ( 0 ),
    m_vector_float_data ( 0 ),
    m_vector_int_data ( 0 ),
    m_vector_uint_data ( 0 ),
    m_vector_ulong64_data ( 0 ),
    m_releventIndex ( -1 ),
    m_branch_set ( false ),
    m_useable ( true )
{
  TObjArray * leaves = branch -> GetListOfLeaves ();
  m_number_leaves = leaves -> GetEntries ();

  if ( m_number_leaves == 1 )
    {
      TObject * object = leaves -> At ( 0 );
      m_leaf = dynamic_cast < TLeaf * > ( object );
      assert ( m_leaf != 0 );
      const string type = m_leaf -> GetTypeName ();

      if ( type == "Double_t" ) m_leaf_type = RootData::Double;
      else if ( type == "Float_t" ) m_leaf_type = RootData::Float;
      else if ( type == "Int_t" ) m_leaf_type = RootData::Int;
      else if ( type == "Short_t" ) m_leaf_type = RootData::Short;
      else if ( type == "UShort_t" ) m_leaf_type = RootData::UShort;
      else if ( type == "UInt_t" ) m_leaf_type = RootData::UInt;
      else if ( type == "Long64_t" ) m_leaf_type = RootData::Long64;
      else if ( type == "ULong64_t" ) m_leaf_type = RootData::ULong64;
      else {
	m_useable = false;
      }
      m_number_elements = m_leaf -> GetLen();
  } else {
	// XXX: should handle branches with more than one leaf
	m_useable = false;
	m_leaf = NULL;
	return;
  }

  try {
    initShape ( m_leaf -> GetTitle() );
  }
  catch ( ... ) {
    m_useable = false;
  }
}

RootBranch::
RootBranch ()
  : m_branch ( 0 ),
    m_releventIndex ( -1 ),
    m_branch_set ( false )
{
}

RootBranch::
~RootBranch()
{
}

unsigned int
RootBranch::
size () const
{
  assert ( false ); // branch doesn't know its size yet

  return 0;
}


bool
RootBranch::
empty () const
{
  return false;
}

bool
RootBranch::
isFilled ( ) const
{
  return true;
}

bool
RootBranch::
isMultiDimensional() const
{
  return ! ( m_number_elements == 1 );
}


int
RootBranch::
numberOfElements() const
{
  return m_number_elements;
}

TBranch *
RootBranch::
getTBranch()
{
  return m_branch;
}
  
void
RootBranch::
initShape ( const char* title )
{
  m_shape.clear ();

  string s( title );
  
  //Creating the list of dropped delimiters. 
  boost::char_separator< char > sep( "][" );  

  // A tokenizer with above dropped delimiters.
  typedef boost::tokenizer< boost::char_separator< char > >  tokenizer;
  tokenizer tok( s, sep );
  
  // Start extracting the dimension sizes.
  for( tokenizer::iterator tok_iter = tok.begin();
       tok_iter != tok.end();
       ++tok_iter )
    if( tok_iter != tok.begin() ) {
      unsigned int value = boost::lexical_cast< unsigned int >( *tok_iter );
      m_shape.push_back( value );
    }
}

/** @note if array is of length one, then use the address of
    corresponding data member for non array variable of this type,
    else allocate memory for the data.
*/
void
RootBranch::
setBranchAddress () const
{
  if( m_number_elements == 1 )
    {
      switch ( m_leaf_type )
	{
	case RootData::Double:
	  m_branch -> SetAddress ( & m_double_data );
	  m_vector_double_data = & m_double_data;
	  break;
	case RootData::Float:
	  m_branch -> SetAddress ( & m_float_data );
	  m_vector_float_data = & m_float_data;
	  break;
	case RootData::Int:
	  m_branch -> SetAddress ( & m_int_data );
	  m_vector_int_data = & m_int_data;
	  break;
	case RootData::UInt:
	  m_branch -> SetAddress ( & m_uint_data );
	  m_vector_uint_data = & m_uint_data;
	  break;
	case RootData::ULong64:
	  m_branch -> SetAddress ( & m_ulong64_data );
	  m_vector_ulong64_data = & m_ulong64_data;
	  break;
	default:
	  assert ( false );
	  break;
	}
    }

  else if( m_number_elements > 1 )
    {
     switch ( m_leaf_type )
       {
       case RootData::Double:
	  m_vector_double_data = new Double_t [ m_number_elements ];
	  m_branch -> SetAddress ( m_vector_double_data );
	  break;
       case RootData::Float:
	  m_vector_float_data = new Float_t [ m_number_elements ];
	  m_branch -> SetAddress ( m_vector_float_data );
	  break;
      case RootData::Int:
	  m_vector_int_data = new Int_t [ m_number_elements ];
	  m_branch -> SetAddress ( m_vector_int_data );
	  break;
      case RootData::UInt:
	  m_vector_uint_data = new UInt_t [ m_number_elements ];
	  m_branch -> SetAddress ( m_vector_uint_data );
	  break;
       case RootData::Short:
	  m_vector_short_data = new Short_t [ m_number_elements ];
	  m_branch -> SetAddress ( m_vector_short_data );
	  break;
      case RootData::UShort:
	  m_vector_ushort_data = new UShort_t [ m_number_elements ];
	  m_branch -> SetAddress ( m_vector_ushort_data );
	  break;
       case RootData::Long64:
	  m_vector_long64_data = new Long64_t [ m_number_elements ];
	  m_branch -> SetAddress ( m_vector_long64_data );
	  break;
       case RootData::ULong64:
	  m_vector_ulong64_data = new ULong64_t [ m_number_elements ];
	  m_branch -> SetAddress ( m_vector_ulong64_data );
	  break;
       default:
	 assert ( false );
	 break;
       }

    }
  
  m_branch_set = true;
}

unsigned int 
RootBranch::
getRank () const
{
  unsigned int size = m_shape.size();

  return size;
}

const vector < int > &
RootBranch::
getShape ()
{
  return m_shape;
}

void
RootBranch::
setReleventIndex( const std::vector< unsigned int > & index )
{
  // This function makes sense only for multi dimenstional data
  assert( getRank () > 0 );
  
  // Index should completely specify the entry of the data in the matrix
  // So it should have as many enteries as the dimensions of the data
  assert( getRank () == index.size() );
  
  // Clear old relevent entry and put in this new ones.
  m_releventIndex = index[0];
  for ( unsigned int d = 1; d < getRank (); d++ ) {
    m_releventIndex = m_releventIndex * m_shape [ d ] + index[ d ];
  }
}

double
RootBranch::
valueAt ( unsigned int row ) const
{
  if ( m_branch_set == false ) setBranchAddress ();
  
  Int_t entry = row;
  //Int_t bytes =
    m_branch -> GetEntry ( entry, 1 );
  
  double value = -1;
  if( m_number_elements == 1 )
    {
      switch ( m_leaf_type )
	{
	case RootData::Double:
	  value = m_double_data;
	  break;
	case RootData::Float:
	  value = m_float_data;
	  break;
	case RootData::Int:
	  value = m_int_data;
	  break;
	case RootData::UInt:
	  value = m_uint_data;
	  break;
	case RootData::Short:
	  value = m_short_data;
	  break;
	case RootData::UShort:
	  value = m_ushort_data;
	  break;
	case RootData::Long64:
	  value = m_long64_data;
	  break;
	case RootData::ULong64:
	  value = m_ulong64_data;
	  break;
	default:
	  assert ( false );
	  break;
	}
    }
  else
    {
      if ( m_releventIndex < 0 ||  m_number_elements <= m_releventIndex )
	{
	  // Temporary patch so other columns can be wriiten
// 	  std::string what ( "RootBranch: `" );
// 	  what += m_leaf -> GetTitle ();
// 	  what += "' indices not set properly.";
// 	  throw DataSourceException ( what );
	  return 0.0;
	}

      // The basic conversion formulae
      switch ( m_leaf_type )
	{
	case RootData::Double:
	  value = m_vector_double_data [ m_releventIndex ];
	  break;
	case RootData::Float:
	  value = m_vector_float_data [ m_releventIndex ];
	  break;
	case RootData::Int:
	  value = m_vector_int_data [ m_releventIndex ];
	  break;
	case RootData::UInt:
	  value = m_vector_uint_data [ m_releventIndex ];
	  break;
	case RootData::Short:
	  value = m_vector_short_data [ m_releventIndex ];
	  break;
	case RootData::UShort:
	  value = m_vector_ushort_data [ m_releventIndex ];
	  break;
	case RootData::Long64:
	  value = m_vector_long64_data [ m_releventIndex ];
	  break;
	case RootData::ULong64:
	  value = m_vector_ulong64_data [ m_releventIndex ];
	  break;
	default:
	  assert ( false );
	}
    }

  return value;
}

double *
RootBranch::
doubleArrayAt ( unsigned int row )
{
  if ( m_branch_set == false ) setBranchAddress ();
  Int_t entry = row;
  // Int_t bytes =
  m_branch -> GetEntry ( entry, 1 );

  if ( m_leaf_type != RootData::Double ) { // need to convert
    if ( m_vector_double_data == 0 ) { // memory not yet allocated
      m_vector_double_data = new Double_t [ m_number_elements ];
    }
    switch ( m_leaf_type ) {
    case RootData::Float :
      std::copy ( m_vector_float_data,
		  m_vector_float_data + m_number_elements,
		  m_vector_double_data );
      break;
    case RootData::Int :
      std::copy ( m_vector_int_data,
		  m_vector_int_data + m_number_elements,
		  m_vector_double_data );
      break;
    case RootData::UInt :
      std::copy ( m_vector_uint_data,
		  m_vector_uint_data + m_number_elements,
		  m_vector_double_data );
      break;
    case RootData::Short :
      std::copy ( m_vector_short_data,
		  m_vector_short_data + m_number_elements,
		  m_vector_double_data );
      break;
    case RootData::UShort :
      std::copy ( m_vector_ushort_data,
		  m_vector_ushort_data + m_number_elements,
		  m_vector_double_data );
      break;
    case RootData::Long64 :
      std::copy ( m_vector_long64_data,
		  m_vector_long64_data + m_number_elements,
		  m_vector_double_data );
      break;
    case RootData::ULong64 :
      std::copy ( m_vector_ulong64_data,
		  m_vector_ulong64_data + m_number_elements,
		  m_vector_double_data );
      break;
    default:
      break;
    }
  }

  return m_vector_double_data;
}

float *
RootBranch::
floatArrayAt ( unsigned int row )
{
  if ( m_branch_set == false ) setBranchAddress ();
  Int_t entry = row;
  // Int_t bytes =
  m_branch -> GetEntry ( entry, 1 );

  return m_vector_float_data;
}

int *
RootBranch::
intArrayAt ( unsigned int row )
{
  if ( m_branch_set == false ) setBranchAddress ();
  Int_t entry = row;
  // Int_t bytes =
  m_branch -> GetEntry ( entry, 1 );

  return m_vector_int_data;
}

unsigned int *
RootBranch::
uintArrayAt ( unsigned int row )
{
  if ( m_branch_set == false ) setBranchAddress ();
  Int_t entry = row;
  // Int_t_bytes =
  m_branch -> GetEntry ( entry, 1 );

  return m_vector_uint_data;
}

RootData::Type
RootBranch::
getType () const
{
  return m_leaf_type;
}

bool
RootBranch::
isUseable () const
{
  return m_useable;
}
