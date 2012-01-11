/** @file

hippodraw::ErrorBarRep class implementation

Copyright (C) 2000-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: ErrorBarRep.cxx,v 1.56 2006/10/24 19:48:56 pfkeb Exp $

*/

#ifdef _MSC_VER
// include max() and min() missing from MicroSoft Visual C++
#include "msdevstudio/MSconfig.h"
#endif //_MSC_VER

#include "ErrorBarRep.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/DataSource.h"
#include "graphics/ViewBase.h"
#include "transforms/BinaryTransform.h"

#include <cassert>
#include <math.h>

using std::string;
using std::vector;

using namespace hippodraw;

ErrorBarRep::ErrorBarRep ( )
  : RepBase ( "ErrorBar", 0 ),
    m_x_flag ( false ),
    m_y_flag ( false )
{
}

ErrorBarRep::~ErrorBarRep()
{
}

RepBase * ErrorBarRep::clone()
{
  return new ErrorBarRep( *this );
}

void ErrorBarRep::setXError ( bool flag )
{
  m_x_flag = flag;
}

void ErrorBarRep::setYError ( bool flag )
{
  m_y_flag = flag;
}

void
ErrorBarRep::
drawXError ( double x, double y, double error,
	     const TransformBase & tb )
{
  try {
    const BinaryTransform & t 
      = dynamic_cast < const BinaryTransform & > ( tb );

    double bull;

    double xhi = x + error;
    double xlo = x - error;

    t.transform( x, y );
    t.transform( xlo, bull );
    t.transform( xhi, bull );
    // Deal with errors that go to NaN or infinity
    if (!isfinite(xlo)) {
	xlo = m_x_range.low();
	t.transform( xlo, bull );
    }
    if (!isfinite(xhi)) {
	xhi = m_x_range.high();
	t.transform( xhi, bull );
    }

    // The line.
    m_x_err.push_back ( xhi );
    m_y_err.push_back ( y );
    m_x_err.push_back ( xlo );
    m_y_err.push_back ( y );

  }
  catch ( ... ) {
    assert ( false );
  }
}

void
ErrorBarRep::
drawYError ( double x, double y, double error,
	     const TransformBase & tb )
{
  try {
    const BinaryTransform & t 
      = dynamic_cast < const BinaryTransform & > ( tb );

    double bull = 0.0;

    double yhi = y + error; //0.5 * error;
    double ylo = y - error; //0.5 * error;

    t.transform( x, y );
    t.transform( bull, ylo );
    t.transform( bull, yhi );
    // Deal with errors that go to NaN or infinity
    if (!isfinite(ylo)) {
	ylo = m_y_range.low();
	t.transform( bull, ylo );
    }
    if (!isfinite(yhi)) {
	yhi = m_y_range.high();
	t.transform( bull, yhi );
    }

    // The line.
    m_x_err.push_back ( x );
    m_y_err.push_back ( yhi );
    m_x_err.push_back ( x );
    m_y_err.push_back ( ylo );

  }
  catch ( ... ) {
    assert ( false );
  }
}
namespace dp = hippodraw::DataPoint2DTuple;

void
ErrorBarRep::
drawProjectedValues ( const DataSource * ntuple,
		      TransformBase * transform,
		      ViewBase * view )
{
  if ( m_x_flag == false &&
       m_y_flag == false ) return;

  unsigned int size = ntuple -> rows ();
  if ( size == 0 ) return;

  m_x_err.clear ();
  m_y_err.clear ();

  unsigned int new_size = 0;
  if ( m_x_flag ) {
    new_size += 6 * size;
  }
  if ( m_y_flag ) {
    new_size += 6 * size;
  }

  m_x_err.reserve ( new_size );
  m_y_err.reserve ( new_size );

  getRanges ( view );

  for ( unsigned int i = 0; i < size; i++ ) {
    const vector < double > & row = ntuple -> getRow ( i );
    double x = row [ dp::X ];
    double y = row [ dp::Y ];

    if ( m_x_flag == true ) {
      double err = row [ dp::XERR ];
      drawXError ( x, y, err, *transform );
    }

    if ( m_y_flag == true ) {
      double err = row [ dp::YERR ];
      drawYError ( x, y, err, *transform );
    }
  }

  const Rect & user_rect = view -> getUserRect ();
  user_rect.makeInBounds ( m_x_err, m_y_err );
  const Color & cur_color = color ();

  view -> drawLines ( m_x_err, m_y_err, Line::Solid, cur_color, m_size );
}

void
ErrorBarRep::
getRanges ( const ViewBase * view )
{
  m_x_range = view -> getRange ( Axes::X );
  m_y_range = view -> getRange ( Axes::Y );
}
