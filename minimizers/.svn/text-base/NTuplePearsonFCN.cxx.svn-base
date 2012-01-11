/** @file

hippodraw::NTuplePearsonFCN class implemenation.

Copyright (C) 2003-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: NTuplePearsonFCN.cxx,v 1.8 2006/08/16 21:51:06 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "NTuplePearsonFCN.h"

#include "functions/FunctionBase.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/DataSource.h"

using std::vector;

using namespace hippodraw;

NTuplePearsonFCN::
NTuplePearsonFCN ( )
{
}

NTuplePearsonFCN::
NTuplePearsonFCN ( const NTuplePearsonFCN & fcn )
  : NTupleFCN ( fcn )
{
}

StatedFCN * 
NTuplePearsonFCN::
clone () const
{
  return new NTuplePearsonFCN ( *this );
}

double
NTuplePearsonFCN::
#ifdef HAVE_MINUIT2
Up () const
#else
up () const
#endif
{
  return 1.0;
}

namespace dp = hippodraw::DataPoint2DTuple;

double
NTuplePearsonFCN::
objectiveValue () const
{
  double result = 0.0;

  unsigned int size = m_indices.size ();

  if ( size == dp::SIZE ) { // 1D coordinate
    int ix = m_indices [ dp::X ];
    int iy = m_indices [ dp::Y ];

    unsigned int rows = m_ntuple -> rows ();

    for ( unsigned int i = 0; i < rows; i++ ) {
      if ( acceptRow ( i ) ) {
	const vector < double > & row = m_ntuple -> getRow ( i );
	double function_value = m_function -> operator () ( row [ ix ] );
	double diff = function_value -  row [ iy ];
	result += diff * diff / (function_value + 10e-6);
      }
    }
  }
  else { 
    unsigned int dimen = size - 2;
    vector < double > coord ( dimen );

    int iy = m_indices [ size / 2 - 1 ];
    int ie = m_indices [ size - 1 ];

    unsigned int rows = m_ntuple -> rows ();

    for ( unsigned int i = 0; i < rows; i++ ) {
      if ( acceptRow ( i ) ) {
	const vector < double > & row = m_ntuple -> getRow ( i );

	double err = ie < 0 ? 0. : row [ ie ];
	if ( err == 0.0 && m_has_errors ) continue;
	if ( m_has_errors == false ) err = 1.0;

	for ( unsigned int j = 0; j < dimen; j++ ) {
	  coord [ j ] = row [ m_indices [ j ] ];
	}
	double value = m_function -> operator () ( coord );
	double diff = value - row [ iy ];
	result += diff * diff / ( value * 10e-6 );
      }
    }
  }

  return result;
}

bool
NTuplePearsonFCN::
needsIntegrated () const
{
  return false;
}
