/** @file

hippodraw::AxisRep2D class implementation

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: AxisRep2D.cxx,v 1.161 2006/02/12 20:51:35 pfkeb Exp $

*/

#ifdef _MSC_VER
// for min()
#include "msdevstudio/MSconfig.h"
#endif

#include "AxisRep2D.h"

#include "graphics/ViewBase.h"
#include <algorithm>

#include <cmath>
#include <cassert>

using std::min;
using std::string;
using std::vector;

using namespace hippodraw;

AxisRep2D::AxisRep2D ()
  : AxisRepBase ()
{
}

AxisRep2D::AxisRep2D( const AxisRep2D & axis_rep )
  : AxisRepBase( axis_rep )
{
}

AxisRepBase * AxisRep2D::clone()
{
  return new AxisRep2D( *this );
}

void
AxisRep2D::
drawZLabels( const AxisModelBase &,
	     ViewBase &, const std::string & )
{
  assert( false );
  // Should never be called.
}

void
AxisRep2D::
drawAllZTicks ( const AxisModelBase &,
		const TransformBase &,
		ViewBase & )
{
  // Should never be called;
  assert( false );
}
