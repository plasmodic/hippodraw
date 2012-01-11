/** @file

hippodraw::Bins1DProfile class implementation

Copyright (C) 2000-2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: Bins1DProfile.cxx,v 1.86 2007/04/23 21:57:20 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "binners/Bins1DProfile.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/NTuple.h"

#include <algorithm>
#include <numeric>

#include <cmath>

#include <cassert>


#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::fill;
using std::list;
using std::sqrt;
using std::string;
using std::vector;
using std::min_element;
using std::max_element;
#endif

using namespace hippodraw;

Bins1DProfile::Bins1DProfile ( )
  : Bins1DBase ( "Bins1DProfile" )
{
}

Bins1DProfile::Bins1DProfile ( const Bins1DProfile & binner )
  : Bins1DBase ( binner ),
    m_sum( binner.m_sum ), 
    m_sumsq( binner.m_sumsq ),
    m_num( binner.m_num ),
    m_sumwtsq (binner.m_sumwtsq )
{
}

BinsBase *
Bins1DProfile::clone () const
{
  return new Bins1DProfile(*this);
}

Bins1DProfile::~Bins1DProfile ()
{
}

double Bins1DProfile::minBin()
{
  return *min_element ( m_sum.begin() + 1, m_sum.end ( ) - 1 );
}

double Bins1DProfile::maxBin()
{
  return *max_element ( m_sum.begin () + 1, m_sum.end () - 1 );
}

void Bins1DProfile::resize ( int number )
{
  m_sum.resize( number + 2 );
  m_num.resize( number + 2 );
  m_sumsq.resize( number + 2 );
  m_sumwtsq.resize( number + 2);

  reset();

  m_values_dirty = true;
}

void Bins1DProfile::reset()
{
  fill( m_sum.begin(), m_sum.end(), 0.0 );
  fill( m_num.begin(), m_num.end(), 0 ); // This is an int array
  fill( m_sumsq.begin(), m_sumsq.end(), 0.0 );
  fill( m_sumwtsq.begin(), m_sumwtsq.end(), 0.0 );

  m_empty = true;
}

int Bins1DProfile::getNumberOfEntries () const
{
  return static_cast < int > ( std::accumulate ( m_num.begin() + 1, 
						 m_num.end() - 2, 0.0 ) );
}

int Bins1DProfile::getNumberOfEntries ( int i ) const
{
//   return *( m_num.begin() + i + 1 );
  return static_cast < int > (m_num [ i+1] );
}

int Bins1DProfile::getUnderflow () const
{
  return -1;
}

int Bins1DProfile::getOverflow () const
{
  return -1;
}


void Bins1DProfile::accumulate( double x, double y, double wt, double )
{
  int i = binNumber ( x );

  m_sum[i] += y * wt;           // sum(w*y)
  m_sumsq[i] += y * y * wt;     // sum(w*y*y)
  m_num[i] += wt;               // sum(w), actually m_sumwt

  m_sumwtsq[i] += wt * wt;      // sum(w*w)

  m_empty = false;
}


NTuple *
Bins1DProfile::
createNTuple () const
{
  unsigned int size = m_sum.size ();
  NTuple * ntuple = prepareNTuple ( size );

  fillDataSource ( ntuple );

  return ntuple;
}

namespace dp = hippodraw::DataPoint2DTuple;

void
Bins1DProfile::
fillDataSource ( DataSource * ntuple ) const
{
  ntuple -> clear();
  vector < double > row ( dp::SIZE );

  double width;
  double x = getLow ( Axes::X );
  int x_number = numberOfBins ( Axes::X );
  for ( int i = 0; i < x_number; i++ ) {
    width = binWidth ( i );
    double half_width = 0.5 * width;
    x += half_width;
    double y = 0;
    double num = m_num [ i+1 ];
    if ( num <= 0 ) {
      x += half_width;
      continue;
    }
    if ( m_sum[i+1] != 0 ) {
      y = m_sum[i+1] / m_num[i+1];
    }
    else y = 0.;

    double yerr = 0.0;

    if ( m_num[i+1] > 1. ) {
      double num = m_num[i+1];
      /* Weighted function from
	 http://pygsl.sourceforge.net/reference/pygsl/node36.html

	 Special case: when all wt==1;
	 yerr = sqrt ( m_sumsq[i+1] / (num - 1 ) - y * y * ( num/ (num-1) ) );

	 When num >> 1, same as unweighed case. But what if num == 2?
      */
      yerr = sqrt ( (num/((num*num)-m_sumwtsq[i+1]))*
		    (m_sumsq[i+1]-2.0*y*m_sum[i+1]+y*y*num) );

      /* Unweight RMS function
	 yerr = sqrt ( ( m_sumsq[i+1] / ( num - 1. )  - y * y ) );
      */
    }
    else {
      yerr = 0.0;
    }

    row[dp::X] = x;
    row[dp::Y] = y;
    row[dp::XERR] = half_width;
    row[dp::YERR] = yerr;

    ntuple -> addRow ( row );

    x += half_width;
  }
}

/** @todo Needs implementation when static projectors are
    implemented.
*/
void
Bins1DProfile::
setBinContents ( const DataSource * ) //  ntuple )
{
}
