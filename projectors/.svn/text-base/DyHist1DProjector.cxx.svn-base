/** @file

hippodraw::DyHist1DProjector class implementation

Copyright (C) 2000-2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: DyHist1DProjector.cxx,v 1.312 2007/04/23 21:57:21 pfkeb Exp $

*/

#ifdef _MSC_VER
// Include max() and min() missing from MicroSoft Visual C++.
#include "msdevstudio/MSconfig.h"
#endif

#include "DyHist1DProjector.h"

#include "axes/AxisModelBase.h"
#include "binners/BinsBase.h"
#include "datasrcs/DataPointTuple.h"
#include "datasrcs/NTuple.h"

#include <cassert>
#include <limits.h>

using namespace hippodraw;

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::list;
using std::max;
using std::string;
using std::vector;
#endif

DyHist1DProjector::DyHist1DProjector( )
  : Hist1DProjImp ( ),
    NTupleProjector ( 2 )
{
  m_binding_options.push_back ( "X" );
  m_binding_options.push_back ( "Weight (optional)" );
  m_min_bindings = 1;
}

/** @note Must initialize with the copy constructor of ProjectorBase
    here, otherise the default constructor will be used.  This is
    because it is a virtual base class of the inherited classes.
 */
DyHist1DProjector::
DyHist1DProjector ( const DyHist1DProjector & projector )
  : ProjectorBase ( projector ),
    Hist1DProjImp ( projector ),
    NTupleProjector ( projector ),
    m_fixed ( projector.m_fixed )
{
}

ProjectorBase * DyHist1DProjector::clone()
{
    ProjectorBase * pb =  new DyHist1DProjector( *this );
  return pb;
}

void DyHist1DProjector::changedNTuple()
{
  unsigned int cols = m_ntuple->columns () - 1;
  if ( m_columns[0] > cols ) {
    m_columns[0] = cols;
  }
  if ( m_columns[1] > cols ) m_columns[1] = UINT_MAX;

  m_binner->setDirty ( );
}

/** @bug When restoring and refitting a function, it appears that this
    member function is called.  Should only be called once.
*/
void DyHist1DProjector::execute ()
{
  if ( m_ntuple -> isNull () ) return;

  // Get the data and the optional weight column. 
  unsigned int column = m_columns[0];
  unsigned int weight = UINT_MAX;
  bool have_weight = m_columns[1] < UINT_MAX;

  if ( have_weight ) {
    weight = m_columns[1];
  }

  // Use integer indexing to ensure that it will take everything from the
  // same row, including the cut values.
  
  m_binner->reset ();

  unsigned int size = m_ntuple -> rows ();
  for ( unsigned int i = 0; i < size; i++ ) 
    {
      if ( acceptRow ( i, m_cut_list ) == false ) continue;
      double x = m_ntuple -> valueAt ( i, column );
      double w = 1.0;
      if ( have_weight ) {
	w = m_ntuple -> valueAt ( i, weight );
      }
      m_binner->accumulate( x, w );
    }
}

double
DyHist1DProjector::
getPosOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y );

  if ( axis == Axes::X ) {
    return getPos ( m_columns[0] );
  }
  // Y

  return getPosOnValue ();
}

Range
DyHist1DProjector::
dataRangeOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y );

  if ( axis == Axes::X ) {
    return dataRange ( m_columns[0] );
  }
  // Y
  return dataRangeOnValue ();
}

const string & DyHist1DProjector::getYLabel ( bool density ) const
{
  if ( density == false ) {
    bool scaling = m_y_axis->isScaling ();
    if ( scaling ) {
      return m_y_label_entries;
    }
  }
  // else
 return m_y_label_density;
}

namespace dp = hippodraw::DataPoint2DTuple;

/** @todo Remove duplicated code that is also in NTupleProjector.
    @todo Fetch the column vector by binding instead of label.  
    @todo Should be able to reuse global functions instead of implementation
    here.
*/
double
DyHist1DProjector::
getAverage ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y );

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
  default:
    break;
  }
  
  // Get the NTuple.
  const DataSource * tuple = getNTuple();
  if ( tuple -> empty () || axis == Axes::Y ) {

    // The axis label is invalid.
    
    // This should not happen for DyHist1DProjector.
    if ( axis == Axes::X ) return 0.0;
    
    // Get the range.
    const Range & r = m_y_axis->getRange(false);

    double scale_factor = m_y_axis -> getScaleFactor ();
    double min = r.low()  * scale_factor;
      double max = r.high() * scale_factor;

    const vector < double > & values 
      = m_proj_values -> getColumn ( dp::Y );    

    for ( unsigned int i = 0; i < values.size(); i++ ) {
      double val = values[i] * scale_factor;
      // Add value to sum if its within the range.
      if(val >= min && val <= max){    
	sum += val;
	number ++;
      }
    }
  }
  else {

    // The axis label is valid. Reimplementation from NTupleProjector.
    
    unsigned int col = tuple -> indexOf ( label );
    unsigned int size = tuple -> rows ();    
    const Range & range = getRange ( axis );

    for ( unsigned int i = 0; i < size; i++ ) {
      
      if ( ! acceptRow ( i, m_cut_list ) ) continue;
      double value = tuple -> valueAt ( i, col );
      if ( range.includes ( value ) ) {	
	sum += value;
	number ++;
      }
      
    }
  
  }
  
  return (sum / number);

}

/* virtual */
bool DyHist1DProjector::isAxisBinned ( const std::string & axis ) const
{
  if ( axis == m_binding_options[0] ) {
    return true;
  }
  return false;
}

void
DyHist1DProjector::
setBinnerRange ( hippodraw::Axes::Type axis,
		 const Range & range,
		 bool const_width )
{
  m_binner -> setRange ( axis, range, const_width );
  if ( const_width == false ) {
    checkScaling ();
  }

  setDirty ( true );
}

void
DyHist1DProjector::
update ( const Observable * object )
{
  const DataSource * datasource 
    = dynamic_cast < const DataSource * > ( object );

  if ( datasource != 0 ) {
    NTupleProjector::update ( object );
  }
  else {
    BinningProjector::update ( object );
  }
}

void
DyHist1DProjector::
willDelete ( const Observable * object )
{
  const DataSource * datasource 
    = dynamic_cast < const DataSource * > ( object );

  if ( datasource != 0 ) {
    NTupleProjector::willDelete ( object );
  }
  else {
    BinningProjector::willDelete ( object );
  }
}

int
DyHist1DProjector::
getUnderflow () const
{
  int underflow = m_binner->getUnderflow ();
  return underflow;
}

int
DyHist1DProjector::
getOverflow () const
{
  int overflow = m_binner->getOverflow ();
  return overflow;
}
