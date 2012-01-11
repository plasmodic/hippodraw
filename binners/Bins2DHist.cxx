/** @file

hippodraw::Bins2DHist class implementation

Originally from C code written at SLAC by Jonas Karlsson in August
1990. Modified substantially since August 1990 by Gravina, Kunz,
Pavel, and Rensing.

Copyright (C) 2000-2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: Bins2DHist.cxx,v 1.107 2007/04/23 21:57:20 pfkeb Exp $

*/

#ifdef _MSC_VER
// Include max() and min() missing from MicroSoft Visual C++.
#include "msdevstudio/MSconfig.h"
#endif

#include "Bins2DHist.h"

#include "BinnerAxis.h"

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

Bins2DHist::Bins2DHist ( )
  : Bins2DBase ( "Bins2DHist" )
{
}

Bins2DHist::Bins2DHist ( const Bins2DHist & binner )
  : Bins2DBase( binner ),
    m_variance( binner.m_variance )
{
  m_num_bins = binner.m_num_bins;

  for ( int i = 0; i < 3; i++ ) {
    m_x_moments[i] = binner.m_x_moments[i];
    m_y_moments[i] = binner.m_y_moments[i];
  }
}

BinsBase *
Bins2DHist::clone () const
{
  return new Bins2DHist ( *this );
}

Bins2DHist::~Bins2DHist ()
{
}

void
Bins2DHist::
setNumberOfBins ( hippodraw::Axes::Type axis, int nb ) 
{
  assert ( axis == Axes::X || axis == Axes::Y );

  if ( axis == Axes::X ) {
    m_data.resize ( nb + 2 ); //GB
    m_variance.resize ( nb ); //GB
    Bins2DBase::setNumberOfBins ( axis, nb );
  }
  else { // Y
    Bins2DBase::setNumberOfBins ( axis, nb );  

    int number_y = numberOfBins ( Axes::Y );
    
    unsigned int i = 0; // For Visual Studio C++ for scope bug
    for( ; i < m_data.size(); i++ ) {
      m_data[i].resize( number_y + 2 );
    }

    for( i = 0; i < m_variance.size(); i++ ) {
      m_variance[i].resize( number_y );
    }
  }

  reset ();
}

void Bins2DHist::reset ()
{
  unsigned int i = 0; // For Visual Studio for scope bug.
  for ( ; i < m_data.size(); i++ ) {
    fill ( m_data[i].begin(), m_data[i].end(), 0.0 );
  }

  for ( i = 0; i < m_variance.size(); i++ ) {
    fill ( m_variance[i].begin(), m_variance[i].end(), 0.0 );
  }

  fill ( m_x_moments, m_x_moments + 3, 0.0 );
  fill ( m_y_moments, m_y_moments + 3, 0.0 );

  m_empty = true;
}

void Bins2DHist::accumulate( double x, double y, double wt, double )
{
  int i = binNumberX ( x );
  int j = binNumberY ( y );

  if( i > 0 && i <= numberOfBins ( Axes::X ) && 
      j > 0 && j <= numberOfBins ( Axes::Y ) ) {
    m_variance[i-1][j-1] += wt * wt;
    m_x_moments[0] += wt;
    m_x_moments[1] += wt * x;
    m_x_moments[2] += wt * x * x;
    m_y_moments[0] += wt;
    m_y_moments[1] += wt * y;
    m_y_moments[2] += wt * y * y;
  }
  m_data[i][j] += wt;

  m_empty = false;
}

double Bins2DHist::getZValue ( double x, double y ) const
{
  int i = binNumberX( x );
  int j = binNumberY( y );
  double widthX = binWidthX ( i-1 );
  double widthY = binWidthY ( j-1 );
  double v = m_data[i][j] / ( widthX * widthY );
  return v;
}

NTuple *
Bins2DHist::
createNTuple () const
{
  unsigned int size = numberOfBins ( Axes::X ) * numberOfBins ( Axes::Y );
  NTuple * ntuple = prepareNTuple ( size );

  fillDataSource ( ntuple );

  return ntuple;
}

namespace dp = hippodraw::DataPoint3DTuple;

void
Bins2DHist::
fillDataSource ( DataSource * ntuple ) const
{
  ntuple -> clear ();

  double total = getNumberOfEntries ();
  double factor = m_is_scaling ? m_scale_factor / total : 1.0;

  vector < double > row ( dp::SIZE );

  double x = getLow ( Axes::X );

  unsigned int num_x = numberOfBins ( Axes::X );
  unsigned int num_y = numberOfBins ( Axes::Y );

  for ( unsigned int i = 0; i < num_x; i++ ) {

    double widthX = binWidthX ( i );
    double half_widthX = 0.5 * widthX;
    x += half_widthX;

    double y = getLow ( Axes::Y );

    for ( unsigned int j = 0; j < num_y; j++ ) {
      double widthY = binWidthY ( j ); 
      double half_widthY = 0.5 * widthY;
      y += half_widthY;

      double v = factor * ( m_data[i+1][j+1] / ( widthX * widthY ) );
      double verr = factor * ( sqrt ( m_variance[i][j] ) );

      row[dp::X] = x;
      row[dp::Y] = y;
      row[dp::Z] = v;
      row [dp::XERR] = half_widthX;
      row [dp::YERR] = half_widthY;
      row [dp::ZERR] = verr;

      ntuple -> addRow ( row );

      y += half_widthY;
    }
    x += half_widthX;
  }

  vector < unsigned int > shape ( 3 );
  shape[0] = num_x;
  shape[1] = num_y;
  shape[2] = dp::SIZE;

  ntuple -> setShape ( shape );
}

void
Bins2DHist::
setBinContents ( const DataSource * ntuple )
{
  unsigned int num_x = numberOfBins ( Axes::X );
  unsigned int num_y = numberOfBins ( Axes::Y );
  unsigned int r = 0;
  for ( unsigned int i = 0; i < num_x; i++ ) {
    double widthX = binWidthX ( i );
    for ( unsigned int j = 0; j < num_y; j++ ) {
      double widthY = binWidthY ( j );
      const vector < double > & row = ntuple -> getRow ( r++ );
      double value = row [ dp::Z ];
      double verr = row [ dp::ZERR ];
      m_data[i+1][j+1] = value * ( widthX * widthY );
      m_variance[i][j] = verr * verr;
    }
  }
}
