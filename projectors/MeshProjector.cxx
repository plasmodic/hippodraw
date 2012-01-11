/** @file

hippodraw::MeshProjector class implementation

Copyright (C) 2006, 2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: MeshProjector.cxx,v 1.191 2007/04/23 21:57:21 pfkeb Exp $

*/

#include <limits.h>

#ifdef _MSC_VER
// Include max() and min() missing from Microsoft Visual C++.
#include "msdevstudio/MSconfig.h"
#endif

#include "MeshProjector.h"

#include "axes/AxisModelBase.h"
#include "axes/Range.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/NTuple.h"

#include <algorithm>
#include <cfloat>

#include <cassert>

using namespace hippodraw;

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::find;
using std::max;
using std::min;
using std::string;
using std::vector;
#endif

MeshProjector::MeshProjector ( )
  : NTupleProjector ( 5 ),
    m_x_option ( "X width (optional)" ),
    m_y_option ( "Y width (optional)" )
{
  m_binding_options.push_back ( "X" );
  m_binding_options.push_back ( "Y" );
  m_binding_options.push_back ( "Z" );
  m_binding_options.push_back ( "X width" );
  m_binding_options.push_back ( "Y width" );
  m_min_bindings = 5;
  addPointReps();
}

/** @note Must initialize with the copy constructor of ProjectorBase
    here, otherise the default constructor will be used.  This is
    because it is a virtual base class of the inherited classes.
 */
MeshProjector::
MeshProjector ( const MeshProjector & projector )
  : ProjectorBase ( projector ),
    NTupleProjector ( projector )
{
  addPointReps();
}

// For some reason, implementing empty destructor decrease code size
// by 5 kbytes.
MeshProjector::~MeshProjector()
{
}

ProjectorBase * MeshProjector::clone()
{
  return new MeshProjector( *this );
}

void MeshProjector::setXErrorOption ( bool enable )
{
  const string name ( m_x_option );
  vector< string >:: iterator first 
    = find ( m_binding_options.begin (),
	     m_binding_options.end (),
	     name );

  if ( first != m_binding_options.end () && !enable ) {
    m_binding_options.erase ( first );
    m_columns[3] = UINT_MAX;
  }
  else if ( enable ) {
    m_binding_options.push_back ( name );
  }
}

/** @todo Remove duplication of code in this method with
    setXErrorOption. */
void MeshProjector::setYErrorOption ( bool enable )
{
  const string name ( m_y_option );
  vector< string >:: iterator first 
    = find ( m_binding_options.begin (),
	     m_binding_options.end (),
	     name );
  if ( first != m_binding_options.end () && !enable ) {
    m_binding_options.erase ( first );
    m_columns[4] = UINT_MAX;
  }
  else if ( enable ) {
    m_binding_options.push_back ( name );
  }
}

void MeshProjector::changedNTuple()
{
  unsigned int cols = m_ntuple->columns () - 1;
  if ( m_columns[0] > cols ) m_columns[0] = cols;
  if ( m_columns[1] > cols ) m_columns[1] = cols;
  if ( m_columns[2] > cols ) m_columns[2] = cols;
  if ( m_columns[3] > cols ) m_columns[3] = cols;
  if ( m_columns[4] > cols ) m_columns[4] = cols;
}

Range MeshProjector::valueRange () const
{
  return dataRangeOn ( Axes::Z );
}

Range
MeshProjector::
dataRangeOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  if ( axis == Axes::X ) {
    if ( m_columns[3] == UINT_MAX ) {
      return dataRange ( m_columns[0] );
    } else {
      return dataRangeWithError ( m_columns[0], m_columns[3] );
    }
  }
  if ( axis == Axes::Y ) {
    if ( m_columns[4] == UINT_MAX ) {
      return dataRange ( m_columns[1] );
    }
    else {
      return dataRangeWithError ( m_columns[1], m_columns[4] );
    }
  }
    // has to be Z
  return dataRangeOnValue ( );
}

namespace dp = hippodraw::DataPoint3DTuple;

Range
MeshProjector::
dataRangeOnValue () const
{
  MeshProjector * mp = const_cast < MeshProjector * > ( this );
  mp -> prepareValues ();
  if ( m_proj_values -> empty () ) {
    return Range ( 0.0, 1.0, 0.5 );
  }
  const vector < double > & values = m_proj_values -> getColumn ( dp::Z );

  return  Range ( values );
}

double
MeshProjector::
getPosOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  if ( axis == Axes::X ) {
    if ( m_columns[3] == UINT_MAX ) {     // Was m_columns[2], should be a bug
      return getPos ( m_columns[0] );
    } else {
      return getPosWithError ( m_columns[0], m_columns[3] );
    }
  }
  if ( axis == Axes::Y ) {
    if ( m_columns[4] == UINT_MAX ) {     // Was m_coloumns[3], should be a bug
      return getPos ( m_columns[1] );
    }
    else {
      return getPosWithError ( m_columns[1], m_columns[4] );
    }
  }
  // has to be Z
  return getPos ( m_columns[2] );
}

void MeshProjector::addPointReps()
{
  m_pointreps.push_back ( "ColorBox" );
}

DataSource *
MeshProjector::
createNTuple () const
{

  unsigned int x_col = m_columns[0];
  unsigned int y_col = m_columns[1];
  unsigned int z_col = m_columns[2];        // Was 3, should be a bug

  unsigned int x_err = m_columns[3];
  unsigned int y_err = m_columns[4];

  unsigned int columns = dp::SIZE;
  NTuple * ntuple = new NTuple ( columns );

  vector < string > labels;
  labels.push_back ( m_ntuple -> getLabelAt ( x_col ) );
  labels.push_back ( m_ntuple -> getLabelAt ( y_col ) );
  labels.push_back ( m_ntuple -> getLabelAt ( z_col ) );

  if ( x_err < UINT_MAX ) {
    labels.push_back ( m_ntuple -> getLabelAt ( x_err ) );
  } else {
    labels.push_back ( dp::XWIDTH );
  }

  if ( y_err < UINT_MAX ) {
    labels.push_back ( m_ntuple -> getLabelAt ( y_err ) );
  } else {
    labels.push_back ( dp::YWIDTH );
  }
  labels.push_back ( " z error" ); // for z error

  ntuple->setLabels ( labels );

  unsigned int size = m_ntuple -> rows ();
  ntuple -> reserve ( size );

  fillProjectedValues ( ntuple );

  return ntuple;
}

/** Fills the DataSource the projected values.  Fills the %DataSource
    with the projected values from the data in the bound columns.  If
    either the x-error or y-error columns are not bound, and the
    %DataSource has columns that match one created by projector with
    width and error columns, then take that data points as the
    projected values.  @sa DataPoint2DTuple.
 */
void
MeshProjector::
fillProjectedValues ( DataSource * ntuple, bool in_range ) const
{
  ntuple -> clear ();

  unsigned int x_col = m_columns[0];
  unsigned int y_col = m_columns[1];
  unsigned int z_col = m_columns[2];

  unsigned int x_err = m_columns[3];
  unsigned int y_err = m_columns[4];

  const vector < string > & labels = m_ntuple -> getLabels ();
  unsigned int size = labels.size();
  if ( size > 3 ) {
    if ( x_err == UINT_MAX &&
	 labels [ dp::XERR ] == dp::XWIDTH ) x_err = dp::XERR;
    if ( size > 3 ) {
      if ( y_err == UINT_MAX &&
	   labels [ dp::YERR ] == dp::YWIDTH ) y_err = dp::YERR;
    }
  }
  size = m_ntuple -> rows ();
  vector < double > row ( dp::SIZE );
  for ( unsigned int i = 0; i < size; i++ ) {
    if ( acceptRow ( i, m_cut_list ) == false ||
	 ( in_range == true && inRange ( i ) == false ) ) continue;

    row[dp::X] = m_ntuple -> valueAt ( i, x_col );
    row[dp::Y] = m_ntuple -> valueAt ( i, y_col );
    row[dp::Z] = m_ntuple -> valueAt ( i, z_col );


    double xe 
      = x_err < UINT_MAX ? m_ntuple -> valueAt ( i, x_err ) : 0.0;
    double ye 
      = y_err < UINT_MAX ? m_ntuple -> valueAt( i, y_err ) : 0.0;

    row[dp::XERR] = xe;
    row[dp::YERR] = ye;

    ntuple -> addRow ( row );
  }
}

void
MeshProjector::
prepareValues ()
{
  if ( m_proj_values == 0 ) {
    m_proj_values = createNTuple ();
  }
  else if ( isDirty () ) {
    fillProjectedValues ( m_proj_values, true );
  }

  setDirty ( false );
}

bool
MeshProjector::
inRangeWithZ ( int row, bool with_z ) const
{
  bool accept = true;

  for ( unsigned int i = 0; i < 2; i++ ) {
    AxisModelBase * model = i == 0 ? m_x_axis : m_y_axis;
    const Range & range = model -> getRange ( false );
    unsigned int vcolumn = m_columns[i];
    unsigned int wcolumn = m_columns[i+3];
    double value = m_ntuple -> valueAt ( row, vcolumn );
    double width = m_ntuple -> valueAt ( row, wcolumn );
    bool in = range.includes ( value + width ) ||
      range.includes ( value - width );
    accept &= in;
  }
  if ( with_z ) {
    const Range & range = m_z_axis -> getRange ( false );
    double value = m_ntuple -> valueAt ( row, m_columns[2] );
    bool in = range.includes ( value );
    accept &= in;
  }

  return accept;
}

bool
MeshProjector::
inRange ( int row ) const
{
  return inRangeWithZ ( row, true );
}

Range
MeshProjector::
preferredRange ( Axes::Type axis ) const
{
  Range range;
  double low = DBL_MAX;
  double pos = DBL_MAX;
  double high = -DBL_MIN;
  if ( axis == Axes::Z ) {
    std::size_t rows = m_ntuple -> rows ();
    for ( unsigned int row = 0; row < rows; row++ ) {
      bool accept = inRangeWithZ ( row, false );
      if ( accept ) {
	double value = m_ntuple -> valueAt ( row, m_columns[2] );
	low = std::min ( low, value );
	if ( value > 0 ) {
	  pos = std::min ( pos, value );
	}
	high = std::max ( high, value );
      }
    }
    range.setRange ( low, high, pos );
  }
  else {
    range = ProjectorBase::preferredRange ( axis );
  }

  return range;
}


const string & MeshProjector::getZLabel () const
{
  return m_proj_values->getLabelAt ( dp::Z );
}


/** @note The implemented algorithm is to get the value of the mesh that
    include x, y. We assume that any point (x,y) is covered by only one
    color box.
*/
double MeshProjector::getZValue ( double x, double y ) const
{
  
  double retval = 0;

  const vector < double > & xs = m_proj_values -> getColumn ( dp::X );
  const vector < double > & ys = m_proj_values -> getColumn ( dp::Y );
  const vector < double > & zs = m_proj_values -> getColumn ( dp::Z );
  const vector < double > & xerr = m_proj_values -> getColumn ( dp::XERR );
  const vector < double > & yerr = m_proj_values -> getColumn ( dp::YERR );

  unsigned int size = xs.size();
  for ( unsigned int i = 0; i < size; i++ ) {
    if ( x>xs[i]-xerr[i] && x<xs[i]+xerr[i] && 
	 y>ys[i]-yerr[i] && y<ys[i]+yerr[i] ) {
      retval = zs[i];

      // Assume (x,y) can not be in another box
      break;
    }
  }

  return retval;
  
  
  /* TO REMOVE: Old algorithm, search for the nestest data point.
     
  const Range & xr = m_x_axis->getRange ( true );
  const Range & yr = m_y_axis->getRange ( true );
  
  double xe = 0.1 * xr.length();
  double ye = 0.1 * yr.length();
  double distanceSquare = xe*xe+ye*ye;
  
  if ( (x-xs[i])*(x-xs[i])+(y-ys[i])*(y-ys[i]) < distanceSquare)  {
  // Update the nearest point info.
  distanceSquare = (x-xs[i])*(x-xs[i])+(y-ys[i])*(y-ys[i]); 
  retval = zs[i]; 
  }
  */
}
