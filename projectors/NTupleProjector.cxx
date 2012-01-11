/** @file

hippodraw::NTupleProjector class implementation

Copyright (C) 2000-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: NTupleProjector.cxx,v 1.209 2007/04/23 21:57:21 pfkeb Exp $

*/

#include <limits.h>

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "NTupleProjector.h"

#include "axes/AxisModelBase.h"

#include "datasrcs/NTuple.h"
#include "datasrcs/TupleCut.h"

#include <algorithm>
#include <functional>
#include <stdexcept>

#include <cassert>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::distance;
using std::find;
using std::max;
using std::max_element;
using std::min;
using std::min_element;
using std::runtime_error;
using std::string;
using std::vector;
#endif

using namespace hippodraw;

NTupleProjector::NTupleProjector ( unsigned int columns )
  : ProjectorBase (),
    m_is_valid ( true ),
    m_columns ( columns, UINT_MAX ),
    m_cut_list(0), // Null pointer.
    m_min_bindings ( 0 )
{
  m_ntuple = new NTuple ( true );
}

NTupleProjector::NTupleProjector ( const NTupleProjector & projector )
  : ProjectorBase ( projector ),
    m_is_valid ( projector.m_is_valid ),
    m_binding_options ( projector.m_binding_options ),
    m_bindings ( projector.m_bindings),
    m_columns ( projector.m_columns ),
    m_ntuple ( projector.m_ntuple ),
    m_min_bindings ( projector.m_min_bindings )
{
  if ( m_ntuple->isNull () ) {
    m_ntuple = new NTuple ( true );
  }
  m_binding_options = projector.m_binding_options;
}

NTupleProjector::~NTupleProjector ()
{
  if ( m_ntuple->isNull () ) {
    delete m_ntuple;
  }
  else {
    DataSource * source = const_cast < DataSource * > ( m_ntuple );
    source -> removeObserver ( this );
  }
}

void  NTupleProjector::update ( const Observable * )
{
  setDirty ( true );
  notifyObservers ();
}

void
NTupleProjector::
willDelete ( const Observable * observee )
{
  if ( observee == m_ntuple ) {
    m_ntuple = new NTuple ( true ); // a null ntuple
  }
}

const vector< string > & NTupleProjector::bindingOptions () const
{
  return m_binding_options;
}

unsigned int NTupleProjector::
indexOfBindingOption ( const std::string & axis ) const
{
  vector< string >::const_iterator first 
    = find ( m_binding_options.begin(),
	     m_binding_options.end(),
	     axis );
  if ( first == m_binding_options.end () ) {
     std::string what = std::string("NTupleProjector::indexOfBindingOption: ")
        + std::string("no such binding option: ") + axis;
     throw runtime_error( what );
  }

#ifdef DISTANCE_DEFECT
  return first - m_binding_options.begin();
#else
  return distance ( m_binding_options.begin(), first );
#endif
}

const std::vector < std::string > & 
NTupleProjector::
getAxisBindings () const
{
  m_bindings.clear();
  size_t size = m_columns.size ();
  for ( size_t i = 0; i < size; i++ ) {
    int column = m_columns[i];
    if ( column >= 0 ) {
      const string & label = m_ntuple->getLabelAt ( column );
      m_bindings.push_back ( label );
    } else {
      const string label = "nil";
      m_bindings.push_back ( label );
    }
  }

  return m_bindings;
}

int 
NTupleProjector::
indexOf ( const std::string & label ) const
{
  return m_ntuple->indexOf ( label );
}

void NTupleProjector::setXErrorOption ( bool )
{
}

void NTupleProjector::setYErrorOption ( bool )
{
}

bool
NTupleProjector::
acceptRow ( unsigned int i, const CutList_t & cut_list ) const
{
  // measured with gprof to be faster this way then using iterators.
  bool accept = true;

  if ( cut_list.empty() == false ) {
    unsigned int size = cut_list.size();
    unsigned int j = 0;
    while ( j < size ) {
      const TupleCut * cut = cut_list[j++];
      if ( cut != 0 ) {
	if ( cut -> acceptRow ( m_ntuple, i ) == false ) {
	  accept = false;
	  break;
	}
      }
    }
  }

  return accept;
}

void 
NTupleProjector::
setAxisBinding ( int axis, const std::string & label )
{
  if ( label == "nil" ) {
    m_columns[axis] = UINT_MAX;
  }
  else {
    m_ntuple -> throwIfInvalidLabel ( label );
    int column = m_ntuple->indexOf ( label );
    m_columns[axis] = column;
  }
  m_is_valid = true;
  setDirty ( true );
}

void NTupleProjector::setAxisBinding ( const std::string & axis, 
				       const std::string & label )
{
  unsigned int index = indexOfBindingOption ( axis );

  setAxisBinding ( index, label );
}

void 
NTupleProjector::
setAxisBindings ( const std::vector< std::string > & labels )
{
  size_t size = labels.size();

  if ( size < m_min_bindings ) {
    string what ( "NTupleProjector::setAxisBindings: " );
    what += "insufficient number of labels";
    throw runtime_error ( what );
  }

  size_t cols = m_columns.size ();
  for ( unsigned int i = 0; i < cols; i++ ) {
    if ( i < size ) {
      const string & label = labels[i];
      setAxisBinding ( i, label );
    }
    else {
      const string nil ( "nil" );
      setAxisBinding ( i, nil );
    }
  }
  m_is_valid = true;
}

void NTupleProjector::setNTuple ( const DataSource * ntuple )
{
  assert ( ntuple != 0 );

  if ( m_ntuple->isNull () ) {
    delete m_ntuple;
    m_ntuple = 0;
  }

  if ( m_ntuple != 0 ) {
    DataSource * nt = const_cast < DataSource * > ( m_ntuple );
    nt->removeObserver ( this );
  }
  m_ntuple = ntuple;
  changedNTuple();
  m_is_valid = true;
  setDirty ( true );
}


const string & NTupleProjector::getTitle() const
{
  return m_ntuple->title();
}

const std::string & NTupleProjector::getXLabel() const
{
  return m_ntuple->getLabelAt( m_columns[0] );
}

const string & NTupleProjector::getYLabel ( bool ) const
{
  return m_ntuple->getLabelAt( m_columns[1] );
}

Range NTupleProjector::dataRangeWithError ( int data, int error ) const
{
  assert ( !( data < 0 ) &&
	   static_cast<size_t> ( data ) < m_ntuple->columns () );
  assert ( !( error < 0 ) &&
	   static_cast<size_t> ( error ) < m_ntuple->columns () );
  
  double lo = DBL_MAX;
  double hi = DBL_MIN;

  unsigned int size = m_ntuple -> rows ();
  for ( unsigned int row = 0; row < size; row++ ) {
    double value = m_ntuple -> valueAt ( row, data );
    double err = m_ntuple -> valueAt ( row, error );
    lo = min ( value - err, lo );
    hi = max ( value + err, hi );
  }

  double pos = getPosWithError( data, error );

  return Range ( lo, hi, pos );
}

Range
NTupleProjector::
dataRange ( int column ) const
{
  assert ( m_ntuple );
  assert ( !( column < 0 ) &&
	   static_cast<size_t> (column) < m_ntuple->columns() );

  Range range;
  bool isValid = m_ntuple -> fillRange ( column, range );
  m_is_valid &= isValid;

  return range;
}

double NTupleProjector::getPosWithError ( int data, int error ) const
{
  assert ( !( data < 0 ) &&
	   static_cast<size_t> ( data ) < m_ntuple->columns () );
  assert ( !( error < 0 ) &&
	   static_cast<size_t> ( error ) < m_ntuple->columns () );
  
  double pos = DBL_MAX;
 
  unsigned int size = m_ntuple -> rows ();
  for ( unsigned int row = 0; row < size; row++ ) {
    double value = m_ntuple -> valueAt ( row, data ); 
    double err   = m_ntuple -> valueAt ( row, error ); 
    if ( value > 0. ) {
      double x = value - err;
      if ( x > 0.0 ) {
	pos = min ( x, pos );
      }
      else {
	if ( value != 0.0 ) pos = min ( 0.1 * value, pos );
      }
    }
  }

  return pos;
}

double
NTupleProjector::
getPos ( int column ) const
{
  assert ( m_ntuple );
  assert ( !( column < 0 ) &&
	   static_cast<size_t> (column) < m_ntuple->columns() );

  double pos = DBL_MAX;

  unsigned int size = m_ntuple -> rows ();

  for ( unsigned int row = 0; row < size; row++ ) {
    double value = m_ntuple -> valueAt ( row, column );
    if ( value < pos && value > 0.0 ) pos = value;
  }

  return pos;
}

void NTupleProjector::addCut ( const TupleCut * cut )
{
  m_cut_list.push_back ( cut );
}

void NTupleProjector::removeCut ( const TupleCut * cut )
{
  CutList_t ::iterator first
    = find ( m_cut_list.begin(), m_cut_list.end(), cut );

  if (first == m_cut_list.end()) {
	std::string what ("Cut does not apply to currently selected plot");
	throw runtime_error(what);
  }

  m_cut_list.erase ( first );
}

const vector < const TupleCut * > & NTupleProjector::getCutList () const
{
  return m_cut_list;
}

int
NTupleProjector::
getNumberOfEntries () const
{
  unsigned int size = m_ntuple->rows ();
  int number = 0;
  for ( unsigned int i = 0; i < size; i++ ) {
    if ( acceptRow ( i, m_cut_list ) && 
	 inRange ( i ) )
      {
      number++;
      }
  }

  return number;
}

int
NTupleProjector::
getUnderflow () const
{
  return -1;
}

int 
NTupleProjector::
getOverflow () const
{
  return -1;
}


bool
NTupleProjector::
inRange ( int row ) const
{
  unsigned int size = m_columns.size(); // number of bound columns
  bool yes = true;
  for ( unsigned int i = 0; i < size; i++ ) {
    if ( m_columns[i] == UINT_MAX ) break;

    AxisModelBase * axis_model = 0;
    if ( i == 0 ) axis_model = m_x_axis;
    else if ( i == 1 ) axis_model = m_y_axis;
    else if ( i == 2 ) axis_model = m_z_axis;
    if ( axis_model == 0 ) break;

    unsigned int column = m_columns[i]; 
    const Range & range = axis_model->getRange ( false );
    double value = m_ntuple -> valueAt ( row, column );
    if ( range.excludes ( value ) ) return false;
  }

  return yes;
}

/** @todo Should be changed to getDatSource(). 
 */
const DataSource * 
NTupleProjector::
getNTuple () const
{
  return  m_ntuple;
}

/** @todo Should be changed to getDatSource(). 
 */
DataSource * 
NTupleProjector::getNTuple ()
{
  return const_cast < DataSource * > ( m_ntuple );
}

const string & NTupleProjector::getNTupleName () const
{
  return m_ntuple->getName ();
}

double
NTupleProjector::
getAverage ( hippodraw::Axes::Type axis ) const
{

  double sum = 0.0;
  double number = 0.0;

  string label = "";
  
  // Get the axis label.
  switch ( axis ) {
  case Axes::X:
    label = getXLabel();
    break;
  case Axes::Y:
    label = getYLabel();
    break;
  case Axes::Z:
    label = getZLabel();
    break;
  default:
    break;
  }
  
  // Get the NTuple for this NTupleProjector.
  const DataSource * tuple = getNTuple();
  if ( tuple -> empty () ) {
    return 0.0;
  }
  // Check if the label is valid for the NTuple.
//   if(!tuple->isFilled(label)) return 0.0;
   
  // Get the column corresponding to the label.
  unsigned int column = tuple -> indexOf ( label );
  // Get the range of projected values.
  const Range & r = getRange ( axis );
    unsigned int size = m_ntuple -> rows ();
    for  ( unsigned int i = 0; i < size; i++ ) {
    // Check if row i is eliminated because of a cut.
    if ( !acceptRow ( i, m_cut_list ) )continue;
    double value = m_ntuple -> valueAt ( i, column );
    // Add the current value only if it is within the range.
    if ( r.includes ( value ) ) {
	sum += value;
	number ++;
    }
  
  }

  return (sum / number);
}

bool
NTupleProjector::
isEmpty () const
{
  return m_ntuple -> empty ();
}

NTuple *
NTupleProjector::
createEmptyNTuple () const
{
  unsigned int columns = m_ntuple->columns();
  
  NTuple * ntuple = new NTuple ( columns );

  const vector < string > & labels = m_ntuple->getLabels();
  ntuple->setLabels ( labels );

  unsigned int size = m_ntuple->rows();
  ntuple -> reserve ( size );
  return ntuple;
}

void
NTupleProjector::
fillNTuple ( NTuple * ntuple, const CutList_t & cut_list ) const
{
   unsigned int size = m_ntuple->rows();

   for ( unsigned int i = 0; i < size; i++ ) {
     if ( acceptRow ( i, cut_list )  ) { //&& inRange(i) ) {
	 const vector < double > & row = m_ntuple -> getRow ( i );
	 ntuple -> addRow ( row );
      }
   }
}

NTuple * 
NTupleProjector::
getNTupleAfterCuts () const
{
  NTuple * ntuple = createEmptyNTuple ();
  fillNTuple ( ntuple, m_cut_list );

  return ntuple;
}

void
NTupleProjector::
fillColumnAfterCuts(const std::string & column,
                    std::vector<double> & columnData) const {

   const std::vector<double> & coldata(m_ntuple->getColumn(column));

   size_t nrows = m_ntuple->rows();
   for ( size_t i = 0 ; i < nrows ; i++ ) {
      if ( acceptRow( i, m_cut_list ) ) {
         columnData.push_back( coldata[i] );
      }
   }
}

NTuple *
NTupleProjector::
createNTupleWith ( const std::vector < TupleCut > & tuple_cuts ) const
{
  NTuple * ntuple = createEmptyNTuple ();
  vector < const TupleCut * > cut_list;
  for ( unsigned int i = 0; i < tuple_cuts.size(); i++ ) {
    const TupleCut & cut = tuple_cuts[i];
    cut_list.push_back ( & cut );
  }
  fillNTuple ( ntuple, cut_list );

  return ntuple;
}

bool
NTupleProjector::
isDataValid () const
{
  return m_is_valid;
}

bool
NTupleProjector::
hasDataSourceBindings () const
{
  return true;
}
