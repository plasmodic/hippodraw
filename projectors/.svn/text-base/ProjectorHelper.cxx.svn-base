/** @file

hippodraw::ProjectorHelper  class implementation

Copyright (C) 2003, 2004, 2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: ProjectorHelper.cxx,v 1.19 2007/04/23 21:57:21 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "ProjectorHelper.h"
#include "datasrcs/DataPointTuple.h"
#include "datasrcs/DataSource.h"

#include <algorithm>
#include <numeric>
#include <iterator>

#include <cmath>

using std::sqrt;
using std::vector;

namespace dp = hippodraw::DataPoint2DTuple;

namespace hippodraw {

ProjectorHelper::
ProjectorHelper ( const DataSource *ntuple )
  : m_ntuple ( ntuple )
{
}

int 
ProjectorHelper::
size () const
{
  return m_ntuple -> rows ();
}

double 
ProjectorHelper::
getTotal () const
{
  return m_ntuple -> sum ( dp::Y );
}

double 
ProjectorHelper::
minCoord () const
{
  const vector< double >& colCoord = m_ntuple -> getColumn( dp::X );

  return *std::min_element( colCoord.begin(), colCoord.end() );
}

double 
ProjectorHelper::
maxCoord () const
{
  const vector< double >& colCoord = m_ntuple -> getColumn( dp::X );

  return *std::max_element( colCoord.begin(), colCoord.end() );
}

double 
ProjectorHelper::
minValue () const
{
  const vector< double >& colValue = m_ntuple -> getColumn( dp::Y );

  return *std::min_element( colValue.begin(), colValue.end() );
}

double 
ProjectorHelper::
maxValue () const
{
  const vector< double >& colValue = m_ntuple -> getColumn( dp::Y );

  return *std::max_element( colValue.begin(), colValue.end() );
}

double 
ProjectorHelper::
meanValue () const
{
  const vector< double >& colValue = m_ntuple -> getColumn( dp::Y );
  double sum = 0.0;
  sum = std::accumulate( colValue.begin(), colValue.end(), sum );

  return sum / colValue.size();
}


double 
ProjectorHelper::
meanCoord () const
{
  const vector<double>& colCoord = m_ntuple -> getColumn( dp::X );
  const vector<double>& colValue = m_ntuple -> getColumn( dp::Y );
  
  double sumXY = 0.0;
  sumXY = std::inner_product( colCoord.begin(), colCoord.end(),
			      colValue.begin(), sumXY );
  double sumY = 0.0;
  sumY = std::accumulate( colValue.begin(), colValue.end(), sumY );
    
  return sumXY / sumY;
}

double 
ProjectorHelper::
stdCoord () const
{
  const vector<double>& x = m_ntuple -> getColumn( dp::X );
  const vector<double>& y = m_ntuple -> getColumn( dp::Y );
  
  double sumY = getTotal();
  double mean = meanCoord();
  double sumXXY = 0.0;
  
  for( unsigned int i = 0; i < x.size(); i++)
    sumXXY  += ( x[i] * x[i] * y[i] );
  
  return sqrt ( sumXXY / sumY - mean * mean );
}

double
ProjectorHelper::
valueAt( double x ) const
{
  const vector<double>& colValue = m_ntuple -> getColumn( dp::Y );
  const vector<double>& colCoord = m_ntuple -> getColumn( dp::X );

  double value = 0.0;
  bool is_set = false;

  for ( unsigned int i = 1; i < colValue.size(); i++) {
    if ( colCoord[i-1] <= x  && x <= colCoord[i] ) {
      value = colValue[i-1];   // Eschew linear interpolation in favor of
      is_set = true;         // returning a valid value, which is more robust.
    }
  }
  if ( is_set == false ) {
    throw std::string("valueAt: x lies outside valid range.");
  }
  
  return value;
}

} // namespace hippodraw

