/** @file

hippodraw::MapMatrixProjector class implementation

Copyright (C) 2003-2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: MapMatrixProjector.cxx,v 1.75 2007/04/23 21:57:21 pfkeb Exp $

*/

#include <limits.h>

#ifdef _MSC_VER
// Include max() and min() missing from MicroSoft Visual C++.
#include "msdevstudio/MSconfig.h"
#endif

#include "MapMatrixProjector.h"

#include "axes/AxisModelBase.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/RTuple.h"

#include <algorithm>
#include <numeric>

#include <cfloat>
#include <cmath>

#include <cassert>

using namespace hippodraw;

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::abs;
using std::accumulate;
using std::find;
using std::max;
using std::min;
using std::sqrt;
using std::string;
using std::vector;
#endif

MapMatrixProjector::MapMatrixProjector ( )
  : NTupleProjector ( 2 ),
    m_x_label ( "x" ),
    m_y_label ( "y" ),
    m_cols ( 0 ),
    m_rows ( 0 ),
    m_x_step ( 1.0 ),
    m_y_step ( 1.0 ),
    m_x_origin ( 0.0 ),
    m_y_origin ( 0.0 ),
    m_null_value ( 0.0 ),
    m_transpose ( false )
{
  m_binding_options.push_back ( "Z" );
  m_min_bindings = 1;
  addPointReps();
}

/** @note Must initialize with the copy constructor of ProjectorBase
    here, otherise the default constructor will be used.  This is
    because it is a virtual base class of the inherited classes.
 */
MapMatrixProjector::
MapMatrixProjector ( const MapMatrixProjector & projector )
  : ProjectorBase ( projector ),
    NTupleProjector( projector ),
    m_x_label ( projector.m_x_label ),
    m_y_label ( projector.m_y_label ),
    m_cols ( projector.m_cols ),
    m_rows ( projector.m_rows ),
    m_x_step ( projector.m_x_step ),
    m_y_step ( projector.m_y_step ),
    m_x_origin ( projector.m_x_origin ),
    m_y_origin ( projector.m_y_origin ),
    m_null_value ( projector.m_null_value ),
    m_transpose ( projector.m_transpose )
{  
  addPointReps();
}

ProjectorBase * MapMatrixProjector::clone()
{
  return new MapMatrixProjector( *this );
}

void
MapMatrixProjector::
setNumberOfBins ( hippodraw::Axes::Type axis, unsigned int number )
{
  assert ( axis == Axes::X || axis == Axes::Y );

  if ( axis == Axes::X ) m_cols = number;
  else m_rows = number;
}

int
MapMatrixProjector::
getNumberOfBins ( hippodraw::Axes::Type axis ) const
{
  assert ( axis != Axes::Z );
  int bins = axis == Axes::X ? m_cols : m_rows;

  return bins;
}

const Range &
MapMatrixProjector::
setBinWidth ( hippodraw::Axes::Type axis, double step )
{
  if ( axis == Axes::X ) {
    m_x_step = step;
  }
  else if ( axis == Axes::Y ) {
    m_y_step = step;
  }
  else if ( axis == Axes::Z ) {
    m_scale_factor = step;
  }

  return getRange ( axis );
}

double
MapMatrixProjector::
getBinWidth ( hippodraw::Axes::Type axis ) const
{
  if ( axis == Axes::X ) {
    return m_x_step;
  }
  if ( axis == Axes::Y ) {
    return m_y_step;
  }
  if ( axis == Axes::Z ) {
    return m_scale_factor;
  }
  assert ( false );
  return 0.0;
}

void 
MapMatrixProjector::
setOffset ( hippodraw::Axes::Type axis, double origin )
{
  if ( axis == Axes::X ) {
    m_x_origin = origin;
    return;
  }
  if ( axis == Axes::Y ) {
    m_y_origin = origin;
    return;
  }
  assert ( false );
}

double
MapMatrixProjector::
getOffset ( hippodraw::Axes::Type axis ) const
{
  if ( axis == Axes::X ) {
    return m_x_origin;
  }
  if ( axis == Axes::Y ) {
    return m_y_origin;
  }
  assert ( false );
  return 0.0;
}

bool
MapMatrixProjector::
inRange ( int row ) const
{
  return inRangeWithZ ( row, true );
}

bool
MapMatrixProjector::
inRangeWithZ ( int row, bool use_z ) const
{
  bool accept = true;

  std::size_t cindex = calcColumnIndex ( row );
  double lvalue = m_x_origin + cindex * m_x_step;
  const Range & x_range = m_x_axis -> getRange ( false );
  bool in = x_range.includes ( lvalue ) ||
            x_range.includes ( lvalue + m_x_step );
  accept &= in;

  if ( accept ) {
    std::size_t rindex = calcRowIndex ( row );
    double bvalue = m_y_origin + rindex * m_y_step;
    const Range & y_range = m_y_axis -> getRange ( false );
    in = y_range.includes ( bvalue ) ||
         y_range.includes ( bvalue + m_y_step );
    accept &= in;
  }

  if ( accept && use_z == true ) {
    const Range & z_range = m_z_axis->getRange ( false );
    double value = m_ntuple -> valueAt ( row, m_columns[0] );
    accept &= z_range.includes ( value );
  }

  return accept;
}

void MapMatrixProjector::changedNTuple()
{
  unsigned int cols = m_ntuple->columns () - 1;
  if ( m_columns[0] > cols ) m_columns[0] = cols;
  if ( m_columns[1] > cols ) m_columns[1] = UINT_MAX;
}

Range MapMatrixProjector::valueRange () const
{
  return dataRangeOn ( Axes::Z );
}

namespace dp = hippodraw::DataPoint3DTuple;

Range
MapMatrixProjector::
dataRangeOnValue () const
{
  MapMatrixProjector * mmp = const_cast < MapMatrixProjector *> ( this );
  mmp -> prepareValues ();
  if ( m_proj_values -> empty () ) {
    return Range ( 0.0, 1.0, 0.5 );
  }
  const vector < double > & values = m_proj_values -> getColumn ( dp::Z );

  return Range ( values );
}

Range
MapMatrixProjector::
dataRangeOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  Range range;

  if ( axis == Axes::X ) {
    double len = m_x_origin + m_cols * m_x_step;
    if ( m_x_step < 0. ) {
      range.setRange ( len, m_x_origin, - m_x_step );
    } else {
      range.setRange ( m_x_origin, len, m_x_step );
    }
  }

  if ( axis == Axes::Y ) {
    double len = m_y_origin + m_rows * m_y_step;
    if ( m_y_step < 0. ) {
      range.setRange ( len, m_y_origin, - m_y_step );
    }
    else {
      range.setRange ( m_y_origin, len, m_y_step );
    }
  }

  if ( axis == Axes::Z ) {
    range = dataRangeOnValue ();
  }

  return range;
}

/* @bug @@@@ This method can create low range > high range from
   transform log log */
Range
MapMatrixProjector::
preferredRange ( Axes::Type axis ) const
{
  Range range;
  double low = DBL_MAX;
  double pos = DBL_MAX;
  double high = -DBL_MIN;
 
  if ( axis == Axes::Z ) {
    std::size_t rows = m_ntuple -> rows ();
    unsigned int used = 0;
    for ( unsigned int row = 0; row < rows; row++ ) {
      bool accept = inRangeWithZ ( row, false );
      if ( accept ) {
	double value = m_ntuple -> valueAt ( row, m_columns[0] );
	low = std::min ( low, value );
	if ( value > 0 ) {
	  pos = std::min ( pos, value );
	}
	high = std::max ( high, value );
	used++;
      }
    }
    range.setRange ( low, high, pos );
  }
  else {
    range = ProjectorBase::preferredRange ( axis );
  }

  return range;
}
double
MapMatrixProjector::
getPosOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  if ( axis == Axes::X ) {
    return 0.5 * std::abs ( m_x_step );
  }
  if ( axis == Axes::Y ) {
    return 0.5 * std::abs ( m_y_step );
  }
  if ( m_columns[1] == UINT_MAX ) {
    return getPos ( m_columns[0] );
  }
  //It has to be Y with an error.
  return getPosWithError ( m_columns[0], m_columns[1] );
}

const string & MapMatrixProjector::getXLabel() const
{
  return m_x_label;
}

const string & MapMatrixProjector::getYLabel ( bool ) const
{
  return m_y_label;
}

const string &
MapMatrixProjector::
getZLabel () const
{
  return m_ntuple->getLabelAt( m_columns[0] );
}


/** @bug Averge value might be wrong, or data is bad.  In
    test_FitsImage.py, the left had EGRET plot gets overage x and y of
    0.0, while the right hand plot is 0.25 (TO REMOVE: not a bug)
*/
double
MapMatrixProjector::
getAverage ( hippodraw::Axes::Type axis ) const
{
  MapMatrixProjector * p = const_cast < MapMatrixProjector * > ( this );
  p -> prepareValues ();

  unsigned int col = 3;    // bad value
  switch ( axis ) {
      
  case Axes::X:
    col = dp::X;
    break;
    
  case Axes::Y:
    col = dp::Y;
      break;
    
  case Axes::Z:
    col = dp::Z;
    break;

  default:
    break;
  }
  assert ( col < 3 );

  const DataSource * ntuple = getProjectedValues ();
  const vector < double > & data = ntuple -> getColumn ( col );

  unsigned int size = ntuple -> rows ();

  double sum = 0.0;
  sum = accumulate ( data.begin(), data.end(), sum );

  return sum / size;
}

void MapMatrixProjector::addPointReps()
{
  m_pointreps.push_back ( "ColorBox" );
  m_pointreps.push_back ( "Contour" );
}

void
MapMatrixProjector::
setNTuple ( const DataSource * ntuple ) 
{
  NTupleProjector::setNTuple ( ntuple );

  unsigned int size = ntuple->rows ();
  double s = static_cast < double > ( size );
  double side = sqrt ( s );

  m_rows = static_cast < unsigned int > ( side );
  m_cols = static_cast < unsigned int > ( side );

  setDirty ();
}

void
MapMatrixProjector::
matrixTranspose ( bool yes )
{
  m_transpose = yes;
}

/** @bug Not sure we're picking up the right bin.  This might be cause
    of last two plots in test_FitsImage.py not having the same average
    value. 
*/
double
MapMatrixProjector::
getZValue ( double x, double y ) const
{
  double xx = ( x - m_x_origin ) / m_x_step;
  double yy = ( y - m_y_origin ) / m_y_step;

  unsigned int i_x = static_cast < unsigned int> ( xx );
  unsigned int i_y = static_cast < unsigned int> ( yy );

  unsigned int row;
  if ( m_transpose ) {
    row = i_x + m_cols * i_y; 
  }
  else {
    row = i_x * m_rows + i_y;
  }

  unsigned int size = m_ntuple -> rows ();
  double value = 0.0;

  if ( row < size ) {
    value = m_ntuple -> valueAt ( row, m_columns[0] );
  }
  return value;
}

DataSource *
MapMatrixProjector::
createNTuple () const
{
  unsigned int z_err = m_columns[1];

  unsigned int columns = 6;
  RTuple * ntuple = new RTuple ( columns );

  // using setLabelAt save 2KB compared to setLabels()
  ntuple -> setLabelAt ( getXLabel (), 0 );
  ntuple -> setLabelAt ( getYLabel ( false ), 1 );
  ntuple -> setLabelAt ( getZLabel (), 2 );
  ntuple -> setLabelAt ( "Width",      3 );
  ntuple -> setLabelAt ( "Height",     4 );
  if ( z_err < UINT_MAX ) {
    ntuple -> setLabelAt ( m_ntuple -> getLabelAt ( z_err ), 5 );
  }
  else {
    ntuple -> setLabelAt ( "Error", 5 );
  }

  fillProjectedValues ( ntuple );

  return ntuple;
}

/** @bug Should be able to uncomment the commented out code now that
    things got fixed in implemnting MeshProjector.
 */
void
MapMatrixProjector::
fillProjectedValues ( DataSource * ntuple, bool ) const // in_range ) const
{
  ntuple -> clear();

  double width_x = m_x_step;
  double next_x = m_x_origin + 0.5 * width_x;

  vector < double > row ( dp::SIZE );
  row[dp::XERR] = abs ( m_x_step );
  row[dp::YERR] = abs ( m_y_step );
  row[dp::ZERR] = 1.0;

  unsigned int l = 0;
  for ( unsigned int i = 0; i < m_cols; i++ ) {

    double x = next_x;
    next_x += width_x;

    double width_y = m_y_step;
    double next_y = m_y_origin + 0.5 * width_y;

    for ( unsigned int j = 0; j < m_rows; j++ ) {
      double y = next_y;
      next_y += width_y;

      // Calculate our own inex to get value because calling
      // getValueAt takes too long
      int index;
      if ( m_transpose ) {
	index = i + m_cols * j;
      }
      else {
	index = i * m_rows + j;
      }
      double value = m_ntuple -> valueAt ( index, m_columns [0] );
   
//       if ( acceptRow ( l ) == false ||
// 	   ( in_range == true && inRange ( l ) == false ) ) {
// 	l++;o
// 	continue;
//       }
      if ( acceptRow ( l, m_cut_list ) == true ) {
	row[dp::Z] = value;
      }
      else {
	row[dp::Z] = m_null_value;
      }
      row[dp::X] = x;
      row[dp::Y] = y;

      ntuple -> addRow ( row );
      l++;
    }
  }

  vector < unsigned int > shape ( 3 );
  shape[0] = m_cols;
  shape[1] = m_rows;
  shape[2] = dp::SIZE;

  ntuple -> setShape ( shape );
}

void
MapMatrixProjector::
prepareValues ()
{
  if ( m_proj_values == 0 ) {
    m_proj_values = createNTuple ();
  }
  else {
    if ( isDirty () ) {
      fillProjectedValues ( m_proj_values, true );
    }
  }

  setDirty ( false );
}

bool
MapMatrixProjector::
isImageConvertable () const
{
  return true;
}
