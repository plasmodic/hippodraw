/** @file

hippodraw::NTupleLikeliHoodFCN class implemenation.

Copyright (C) 2003-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: NTupleLikeliHoodFCN.cxx,v 1.19 2006/01/08 19:21:48 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "NTupleLikeliHoodFCN.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/DataSource.h"
#include "functions/FunctionBase.h"

#include <cmath>
#include <cassert>

using std::vector;
using std::log;
using std::pow;
using std::exp;

using namespace hippodraw;

NTupleLikeliHoodFCN::
NTupleLikeliHoodFCN ()
{
}

NTupleLikeliHoodFCN::
NTupleLikeliHoodFCN ( const NTupleLikeliHoodFCN & fcn )
  : NTupleFCN ( fcn )
{
}

StatedFCN *
NTupleLikeliHoodFCN::
clone () const
{
  return new NTupleLikeliHoodFCN ( *this );
}

namespace dp = hippodraw::DataPoint2DTuple;

double
NTupleLikeliHoodFCN::
objectiveValue () const
{
  double result = 0.0;
  double sum1 = 0.0;
  double sum2 = 0.0;

  int ix = m_indices [ dp::X ];
  int iy = m_indices [ dp::Y ];
  int ie = m_indices [ dp::XERR ];
  
  int num_bins = m_ntuple -> rows ();
  for( int i = 0; i < num_bins; i++ )
    {
      if ( acceptRow ( i )  ) {
	const vector < double > & row = m_ntuple -> getRow ( i );
      
	// Get the edges of the bin.
	double x = row [ ix ];
	double hwidth = 0.5 * row [ ie ];
	double edgeL = x - hwidth;
	double edgeR = x + hwidth;
      
	// Integrate function over the entire i-th bin
	double theta = m_function-> integrate( edgeL, edgeR );
            
	// Get number of enteries in the ith bin
	double n = 2.0 * hwidth * row [ iy ];
      
	// Computing the factorial of n and taking log of it.
	// Exact computation is done when N is small ( < 100) else
	// we use sterliing's approximations for it.
	// CAUTION: Exactly calculating factorial itself usually leads to
	// a very large and unrepresentable number so we avoid it.
	//double logfactn = 0.0;
	//if( n < 100 )
	//{
	//  
	//  for( int j = 2; j <= n; j++)
	//    logfactn += log( (double) j );
	//  
	//  result += ( n * log ( theta ) - theta - logfactn   );
	//}
	//else
	//{
	//  logfactn = n * log( (double) n ) - n;
	//  result +=( n * ( log ( theta ) - log( (double) n ) + 1 ) - theta );
	//}

	//result += ( n * log ( theta ) - theta );
      
	sum1 += n * log ( theta );
	sum2 += theta;
      
      }
    }
  result = sum1 - sum2;
  // The quantity that is (asymptotically) equivalent to
  // chi-square is -2 log( L ) i.e we return  -2 * result
  
  return ( -2 * result );  
}

bool
NTupleLikeliHoodFCN::
needsIntegrated () const
{
  return true;
}
