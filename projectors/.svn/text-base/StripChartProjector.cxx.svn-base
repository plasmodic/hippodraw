/** @file

StripChartProjector class implementation

Copyright (C) 2000-2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: StripChartProjector.cxx,v 1.50 2005/10/30 00:10:14 pfkeb Exp $

*/

#include <limits.h>

// For truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "StripChartProjector.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/DataSource.h"

#include <algorithm>

#include <cassert>

using std::distance;
using std::min_element;
using std::vector;

namespace hippodraw {

StripChartProjector::StripChartProjector( )
  : Map2Projector()
{
  addPointReps();
}

/** @note Must initialize with the copy constructor of ProjectorBase
    here, otherise the default constructor will be used.  This is
    because it is a virtual base class of the inherited classes.
 */
StripChartProjector::
StripChartProjector ( const StripChartProjector & projector )
  : ProjectorBase ( projector ), // already done in Map2Proj, but no harm
    Map2Projector( projector )
{
  addPointReps();
}

ProjectorBase * StripChartProjector::clone()
{
  return new StripChartProjector ( *this );
}

void StripChartProjector::addPointReps()
{
  m_pointreps.clear();
  m_pointreps.push_back ( "Line" );
  m_pointreps.push_back ( "Symbol" );
}

namespace dp = hippodraw::DataPoint2DTuple;

/** @bug bool Parameter is not used
 */
void
StripChartProjector::
fillProjectedValues ( DataSource * ntuple, bool ) const //in_range ) const
{
  ntuple -> clear();

  unsigned int x_col = m_columns[0];
  unsigned int y_col = m_columns[1];
  unsigned int x_err = m_columns[2];
  unsigned int y_err = m_columns[3];

  unsigned int min_row = m_ntuple -> indexOfMinElement ( x_col );

  unsigned int size = m_ntuple -> rows ();

  vector < double > row ( dp::SIZE );
  for ( unsigned int i = min_row; i < size; i++ ) {
//     if ( acceptRow ( i ) == false ||
// 	 ( in_range == true && inRange ( i ) == false ) ) continue;
    if ( acceptRow ( i, m_cut_list ) == false ) continue;

    row[dp::X] = m_ntuple -> valueAt ( i, x_col );
    row[dp::Y] = m_ntuple -> valueAt ( i, y_col );

    double xe 
      = x_err < UINT_MAX ? m_ntuple -> valueAt ( i, x_err ) : 0.0;
    double ye 
      = y_err < UINT_MAX ? m_ntuple -> valueAt ( i, y_err ) : 0.0;

    row[dp::XERR] = xe;
    row[dp::YERR] = ye;
    ntuple -> addRow ( row );
  }
  for ( unsigned int i = 0; i < min_row; i++ ) {
    if ( acceptRow ( i, m_cut_list ) == false ||
	 inRange ( i ) == false ) continue;

    row[dp::X] = m_ntuple -> valueAt ( i, x_col );
    row[dp::Y] = m_ntuple -> valueAt ( i, y_col );
    double xe 
      = x_err < UINT_MAX ? m_ntuple -> valueAt ( i, x_err ) : 0.0;
    double ye 
      = y_err < UINT_MAX ? m_ntuple -> valueAt ( i, y_err ) : 0.0;

    row[dp::XERR] = xe;
    row[dp::YERR] = ye;

    ntuple -> addRow ( row );
  }
}

} // namespace hippodraw

