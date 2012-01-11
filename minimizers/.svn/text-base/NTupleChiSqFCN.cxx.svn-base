/** @file

hippodraw::NTupleChiSqFCN class implemenation.

Copyright (C) 2003-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: NTupleChiSqFCN.cxx,v 1.23 2006/08/16 21:51:06 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "NTupleChiSqFCN.h"

#include "functions/FunctionBase.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/DataSource.h"

using std::vector;

using namespace hippodraw;

NTupleChiSqFCN::
NTupleChiSqFCN ( )
{
}

NTupleChiSqFCN::
NTupleChiSqFCN ( const NTupleChiSqFCN & fcn )
  : NTupleFCN ( fcn )
{
}

StatedFCN * 
NTupleChiSqFCN::
clone () const
{
  return new NTupleChiSqFCN ( *this );
}

double
NTupleChiSqFCN::
#ifdef HAVE_MINUIT2
Up () const
#else
up () const
#endif
{
  return 1.0;
}

namespace dp2 = hippodraw::DataPoint2DTuple;
namespace dp3 = hippodraw::DataPoint3DTuple;

double
NTupleChiSqFCN::
objectiveValue () const
{
  double result = 0.0;

  unsigned int dim = ( m_indices.size() - 2 ) / 2;

  if ( dim == 1 ) {
    int ix = m_indices [ dp2::X ];
    int iy = m_indices [ dp2::Y ];
    int ie = m_indices [ dp2::YERR ];

    unsigned int rows = m_ntuple -> rows ();

    for ( unsigned int i = 0; i < rows; i++ ) {
      if ( acceptRow ( i ) ) {
	const vector < double > & row = m_ntuple -> getRow ( i );
	double err = ie < 0 ? 0. : row [ ie ];
	if ( err == 0.0 && m_has_errors ) continue;
	if ( m_has_errors == false ) err = 1.0;
	
	double diff = m_function -> operator () ( row [ ix ] ) -  row [ iy ];
	result += diff * diff / ( err * err );
      }
    }
  }
  else {
    vector < double > coord ( dim );
    int iv = m_indices [ dim ];
    int ie = getErrorColumn ();

    unsigned int rows = m_ntuple -> rows ();

    for ( unsigned int i = 0; i < rows; i++ ) {
      if ( acceptRow ( i ) ) {
	const vector < double > & row = m_ntuple -> getRow ( i );

	double err = ie < 0 ? 0. : row [ ie ];
	if ( err == 0.0 && m_has_errors ) continue;
	if ( m_has_errors == false ) err = 1.0;

	for ( unsigned int j = 0; j < dim; j++ ) {
	  coord [ j ] = row [ m_indices [ j ] ];
	}

	double diff = m_function -> operator () ( coord ) -  row [ iv ];
	result += diff * diff / ( err * err );
      }
    }
  }

  return result;
}

bool
NTupleChiSqFCN::
needsIntegrated () const
{
  return false;
}
