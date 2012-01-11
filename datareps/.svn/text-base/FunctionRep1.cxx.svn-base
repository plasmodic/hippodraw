/** @file

hippodraw::FunctionRep1 class implementation

Copyright (C) 2005, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FunctionRep1.cxx,v 1.91 2006/05/28 19:35:26 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "FunctionRep1.h"

#include "functions/FunctionBase.h"
#include "projectors/ProjectorBase.h"

#include "reps/LineFunctionRep.h"

#include <cassert>

using namespace hippodraw;

FunctionRep1::
FunctionRep1 ( FunctionBase * function, DataRep * rep )
  : FunctionRep ( function, rep )
{
  m_rep = new LineFunctionRep(); 
  // The above deleted in ~DataRep()

    const Color red ( Color::red );
    m_rep->setColor ( red );
}

FunctionRep1::
FunctionRep1 ( const FunctionRep1 & rep )
  : FunctionRep ( rep )
{
}

FunctionRep1::
~FunctionRep1 ()
{
}

DataRep * FunctionRep1::clone ()
{
  return new FunctionRep1 ( *this );
}

bool
FunctionRep1::
hasAxis ( hippodraw::Axes::Type axis ) const
{
  return axis == Axes::X || axis == Axes::Y;
}

void
FunctionRep1::
drawProjectedValues ( TransformBase * transform,
		      ViewBase * view )
{
  drawCuts ( transform, view );

  const Range & range = m_projector -> getRange ( Axes::X );
  FunctionBase * function = getFunction ();
  LineFunctionRep * rep = dynamic_cast < LineFunctionRep * > ( m_rep );
  assert ( rep != 0 );

  rep -> drawProjectedValues ( range, function, transform, view );

  setDirty ( false );
}
