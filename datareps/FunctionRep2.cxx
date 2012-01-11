/** @file

hippodraw::FunctionRep2 class implementation

Copyright (C) 2005, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FunctionRep2.cxx,v 1.3 2006/05/28 19:35:26 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "FunctionRep2.h"

#include "functions/FunctionBase.h"
#include "projectors/ProjectorBase.h"

#include "reps/ContourFunctionRep.h"

#include <cassert>

using namespace hippodraw;

FunctionRep2::
FunctionRep2 ( FunctionBase * function, DataRep * rep )
  : FunctionRep ( function, rep )
{
  m_rep = new ContourFunctionRep(); 
  // The above deleted in ~DataRep()
}

FunctionRep2::
FunctionRep2 ( const FunctionRep2 & rep )
  : FunctionRep ( rep )
{
}

FunctionRep2::
~FunctionRep2 ()
{
}

DataRep * FunctionRep2::clone ()
{
  return new FunctionRep2 ( *this );
}

bool
FunctionRep2::
hasAxis ( hippodraw::Axes::Type axis ) const
{
  return axis == Axes::X || axis == Axes::Y || axis == Axes::Z;
}

void
FunctionRep2::
drawProjectedValues ( TransformBase * transform,
		      ViewBase * view )
{
  drawCuts ( transform, view );

  const Range & x_range = m_projector -> getRange ( Axes::X );
  const Range & y_range = m_projector -> getRange ( Axes::Y );
  FunctionBase * function = getFunction ();
  ContourFunctionRep * rep = dynamic_cast < ContourFunctionRep * > ( m_rep );
  assert ( rep != 0 );

  rep -> drawProjectedValues ( x_range, y_range, function, transform, view );

  setDirty ( false );
}
