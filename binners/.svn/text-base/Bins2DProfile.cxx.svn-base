/** @file

hippodraw::Bins2DProfile  class implementation

Originally from C code written at SLAC by Jonas 
Karlsson in August 1990. Modified substantially 
since August 1990 by Gravina, Kunz, Pavel, and 
Rensing.

Copyright (C) 2000-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Bins2DProfile.cxx,v 1.79 2007/04/23 21:57:20 pfkeb Exp $

*/


#ifdef _MSC_VER
// Include max() and min() missing from MicroSoft Visual C++.
#include "msdevstudio/MSconfig.h"
#endif

#include "Bins2DProfile.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/NTuple.h"

#include <cmath>

#include <cassert>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::fill;
using std::list;
using std::sqrt;
using std::vector;
#endif

using namespace hippodraw;

Bins2DProfile::Bins2DProfile ( )
  : Bins2DBase ( "Bins2DProfile" )
{
}

Bins2DProfile::Bins2DProfile( const Bins2DProfile & binner )
  : Bins2DBase( binner ),
    m_sumZ( binner.m_sumZ ),
    m_variance( binner.m_variance ),
    m_z_range ( binner.m_z_range )
{
  m_num_bins = binner.m_num_bins;
}

Bins2DProfile::~Bins2DProfile ()
{
}

BinsBase *
Bins2DProfile::clone () const
{
  return new Bins2DProfile ( *this );
}

void
Bins2DProfile::
setNumberOfBins ( hippodraw::Axes::Type axis, int nb ) 
{
  assert ( axis == Axes::X || axis == Axes::Y );

  Bins2DBase::setNumberOfBins ( axis, nb );
  if ( axis == Axes::X ) {
    int number = numberOfBins ( Axes::X );
    m_data.resize( number + 2 );
    m_sumZ.resize( number + 2 );
    m_variance.resize( number );
  }
  else { // Y
    int number_y = numberOfBins ( Axes::Y );
    unsigned int i = 0; // For Visual Studio for scope bug.
    for ( ; i < m_data.size(); i++ ) {
      m_data[i].resize( number_y + 2 );
      m_sumZ[i].resize( number_y + 2 );
    }
    for ( i = 0; i < m_variance.size(); i++ ) {
      m_variance[i].resize( number_y );
    }
  }

  reset ();

  m_num_bins = numberOfBins ( Axes::X ) * numberOfBins ( Axes::Y );
}

void Bins2DProfile::reset()
{
  unsigned int i = 0; // For Visual Studio for scope bug.
  for ( ; i < m_data.size(); i++ ) {
    fill ( m_data[i].begin(), m_data[i].end(), 0.0 );
    fill ( m_sumZ[i].begin(), m_sumZ[i].end(), 0.0 );
  }

  for ( i = 0; i < m_variance.size(); i++ ) {
    fill ( m_variance[i].begin(), m_variance[i].end(), 0.0 );
  }
  
  m_empty = true;
}

void Bins2DProfile::accumulate( double x, double y, double z, double wt )
{
  int i = binNumberX( x );
  int j = binNumberY( y );
  
  if( i > 0 && i <= numberOfBins ( Axes::X ) &&
      j > 0 && j <= numberOfBins ( Axes::Y ) ) {
    m_variance[i-1][j-1] += wt * wt;
  }
  m_data[i][j] += wt;
  m_sumZ[i][j] += z;

  m_empty = false;
}

double Bins2DProfile::getZValue ( double x, double y ) const
{

  int i = binNumberX( x );
  int j = binNumberY( y );
  
  /* Avoid nan, is this the fix? */
  if (m_data[i][j] == 0) return 0.0;

  double v = m_sumZ[i][j] / m_data[i][j];
  return v;
}

NTuple *
Bins2DProfile::
createNTuple () const
{
  unsigned int size = numberOfBins ( Axes::X ) * numberOfBins ( Axes::Y );
  NTuple * ntuple = prepareNTuple ( size );

  fillDataSource ( ntuple );

  return ntuple;
}

namespace dp = hippodraw::DataPoint3DTuple;

/** @todo The error on the value has needs to be tested.
    @todo The scaling needs to be tested.
 */
void
Bins2DProfile::
fillDataSource ( DataSource * ntuple ) const
{
  ntuple -> clear ();
  vector < double > row ( dp::SIZE );

  int v_inc = 0;
  vector<double>::const_iterator vity;
  double next_x = getLow ( Axes::X );

  int num_x = numberOfBins ( Axes::X );
  int num_y = numberOfBins ( Axes::Y );
  
  for ( int x_inc = 1; x_inc <= num_x; x_inc++ ) {
    vity = m_variance[v_inc++].begin();
    double widthX = binWidthX ( x_inc - 1 );
    double half_widthX = 0.5 * widthX;
    double x = next_x +  half_widthX;
    next_x += widthX;
    double next_y = getLow ( Axes::Y );

    for ( int y_inc = 1; y_inc <= num_y; y_inc++ ) {
      double widthY = binWidthY ( y_inc - 1 );
      double half_widthY = 0.5 * widthY;
      double y = next_y;
      y += half_widthY;
      next_y += widthY;

      double var2 = *vity++;
      double v = 0.0;
      double verr = 0.0;
      if ( m_data[x_inc][y_inc] != 0 ) {
	double num = m_data[x_inc][y_inc];
	v = m_sumZ[x_inc][y_inc] / num;
	verr = sqrt ( ( var2 / ( num - 1. ) - v * v ) );
      }
      row[dp::X] = x;
      row[dp::Y] = y;
      row[dp::Z] = v;
      row [dp::XERR] = half_widthX;
      row [dp::YERR] = half_widthY;
      row [dp::ZERR] = verr;

      ntuple -> addRow ( row );
    }
  }
  vector < unsigned int > shape ( 3 );
  shape[0] = num_x;
  shape[1] = num_y;
  shape[2] = dp::SIZE;

  ntuple -> setShape ( shape );
}

/** @todo Needs implementation when static projector is
    implemented.
*/
void
Bins2DProfile::
setBinContents ( const DataSource * )
{
}
