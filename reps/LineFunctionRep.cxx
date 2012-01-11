/** @file

hippodraw::LineFunctionRep class implementation

Copyright (C) 2005, 2006  The Board of Trustees of The
Leland Stanford Junior University.  All Rights Reserved.

$Id: LineFunctionRep.cxx,v 1.80 2006/05/28 19:35:26 pfkeb Exp $

*/

#ifdef _MSC_VER
// include max() and min() missing from MicroSoft Visual C++
#include "msdevstudio/MSconfig.h"
#endif //_MSC_VER

#include "LineFunctionRep.h"

#include "axes/Range.h"
#include "functions/FunctionBase.h"
#include "graphics/ViewBase.h"
#include "transforms/BinaryTransform.h"

using namespace hippodraw;

LineFunctionRep::
LineFunctionRep ()
  : LinePointRep ( "FunctionLine", 1.0 )
{
}

LineFunctionRep::
LineFunctionRep ( float size )
  : LinePointRep ( "FunctionLine", size )
{
}

LineFunctionRep::
LineFunctionRep ( const LineFunctionRep & point_rep )
  : LinePointRep ( point_rep )
{
}

LineFunctionRep::~LineFunctionRep ()
{
}

RepBase * LineFunctionRep::clone()
{
  return new LineFunctionRep( *this );
}

void
LineFunctionRep::
drawProjectedValues ( const Range & range,
		      const FunctionBase * function,
		      TransformBase * transform,
		      ViewBase * view )
{
  smoothCurve ( range, transform, function );
  drawValues ( view );
}

void
LineFunctionRep::
smoothCurve ( const Range & range,
	      TransformBase * transform,
	      const FunctionBase * function )
{
  const BinaryTransform * t
    = dynamic_cast < const BinaryTransform * > ( transform );

  unsigned int size = 50;
  m_x.resize ( size );
  m_y.resize ( size );

  double tx = range.low ();
  double ty = function -> operator () ( tx );
  t -> transform ( tx, ty ); // changes them.
  double txx = range.high ();
  double tyy = function -> operator () ( txx );
  t -> transform ( txx, tyy ); // changes them.

  double x = tx;
  double delta = ( txx - tx ) / ( size - 1 );

  for ( unsigned int i = 0; i < size; i++ ) {
    tx = x + 0.1 * delta;
    t -> inverseTransform ( tx, ty ); // changes them.
    ty = function -> operator () ( tx );
    t -> transform ( tx, ty ); // transform them back
    m_x [ i ] = tx;
    m_y [ i ] = ty;

    x += delta;
  }
}
