/** @file

hippodraw::Map3Projector class implementation

Copyright (C) 2000-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: Map3Projector.cxx,v 1.148 2006/08/05 22:06:57 pfkeb Exp $

*/

#ifdef _MSC_VER
// Include max() and min() missing from Microsoft Visual C++.
#include "msdevstudio/MSconfig.h"
#include  <math.h>
#endif //_MSC_VER

#include "Map3Projector.h"

#include "axes/AxisModelBase.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/NTuple.h"

#include <algorithm>

#include <cmath>

#include <cassert>

using namespace hippodraw;

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::abs;
using std::max;
using std::max_element;
using std::min;
using std::min_element;
using std::string;
using std::vector;
#endif

Map3Projector::Map3Projector( )
  : NTupleProjector ( 3 )
{
  m_binding_options.push_back ( "X" );
  m_binding_options.push_back ( "Y" );
  m_binding_options.push_back ( "Z" );
  m_min_bindings = 3;
  addPointReps();
}

/** @note Must initialize with the copy constructor of ProjectorBase
    here, otherise the default constructor will be used.  This is
    because it is a virtual base class of the inherited classes.
 */
Map3Projector::
Map3Projector ( const Map3Projector & projector )
  : ProjectorBase ( projector ),
    NTupleProjector( projector )
{
  addPointReps();
}

ProjectorBase * Map3Projector::clone()
{
  return new Map3Projector( *this );
}

void Map3Projector::changedNTuple ()
{
  unsigned int cols = m_ntuple->columns () - 1;

  if ( m_columns[0] > cols ) m_columns[0] = cols;
  if ( m_columns[1] > cols ) m_columns[1] = cols;
  if ( m_columns[2] > cols ) m_columns[2] = cols;
}

/** @todo This method doesn't seem to be used yet is needed to satisfy
    pure virtual function in the base class.  Some client should be
    calling it.
*/
Range Map3Projector::valueRange() const
{
  Range range = dataRange ( m_columns[2] );

  return range;
}

Range
Map3Projector::
dataRangeOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  unsigned int index = 0;
  if ( axis == Axes::Y ) index = 1;
  if ( axis == Axes::Z ) index = 2;

  return dataRange ( m_columns[index] );

}

double
Map3Projector::
getPosOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  unsigned int index = 0;
  if ( axis == Axes::Y ) index = 1;
  if ( axis == Axes::Z ) index = 2;

  return getPos ( m_columns[index] );

}

const string & Map3Projector::getZLabel () const
{
  return m_ntuple->getLabelAt ( m_columns[2] );
}

namespace dp = hippodraw::DataPoint3DTuple;

/** @note The implemented algorithm is to take the last projected
    value that is close to the @a x and @a y since it is the one which
    would paint on top of the others.  Also, projected values are used
    so as to take into consideration any cuts that might be applied.
*/
double Map3Projector::getZValue ( double x, double y ) const
{
  
  double retval = 0;

  const Range & xr = m_x_axis->getRange ( true );
  const Range & yr = m_y_axis->getRange ( true );
  
  double xe = xr.length() * 0.01;
  double ye = yr.length() * 0.01;

  const vector < double > & xs = m_proj_values -> getColumn ( dp::X );
  const vector < double > & ys = m_proj_values -> getColumn ( dp::Y );
  const vector < double > & zs = m_proj_values -> getColumn ( dp::Z );
  unsigned int size = xs.size();
  for ( unsigned int i = 0; i < size; i++ ) {
    if ( abs ( x - xs[i] ) < xe  &&
	 abs ( y - ys[i] ) < ye      )  retval = zs[i]; // last one
  }

  return retval;
}

void Map3Projector::addPointReps()
{
  m_pointreps.push_back ( "ColorSymbol" );
}

DataSource *
Map3Projector::
createNTuple () const
{
  unsigned int ix = m_columns [ 0 ];
  unsigned int iy = m_columns [ 1 ];
  unsigned int iz = m_columns [ 2 ];

  unsigned int columns = 3;
  NTuple * ntuple = new NTuple ( columns );
  vector < string > labels;
  labels.push_back ( m_ntuple -> getLabelAt ( ix ) );
  labels.push_back ( m_ntuple -> getLabelAt ( iy ) );
  labels.push_back ( m_ntuple -> getLabelAt ( iz ) );
  ntuple -> setLabels ( labels );

  fillProjectedValues ( ntuple );

  return ntuple;
}

void
Map3Projector::
fillProjectedValues ( DataSource * ntuple, bool in_range ) const
{
  ntuple -> clear ();

  unsigned int size = m_ntuple -> rows ();
  ntuple -> reserve ( size );
  vector < double > row ( 3 ); // an exception, no errors

  unsigned int ix = m_columns [ 0 ];
  unsigned int iy = m_columns [ 1 ];
  unsigned int iz = m_columns [ 2 ];

  for ( unsigned int i = 0; i < size; i++ ) {
    if ( acceptRow ( i, m_cut_list ) == false ||
	 ( in_range == true && inRange ( i ) == false ) ) continue;

    row[dp::X] = m_ntuple -> valueAt ( i, ix );
    row[dp::Y] = m_ntuple -> valueAt ( i, iy );
    row[dp::Z] = m_ntuple -> valueAt ( i, iz );

    ntuple -> addRow ( row );
  }
}

void
Map3Projector::
prepareValues ()
{
  if ( m_proj_values == 0 ) {
    m_proj_values = createNTuple ();
  }
  else {
    fillProjectedValues ( m_proj_values, true );
  }

  setDirty ( false );
}
