/** @file

TextRepFactory class implementation

Copyright (C) 2001-2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: TextRepFactory.cxx,v 1.20 2006/04/12 20:45:54 xiefang Exp $

*/

#ifdef _MSC_VER
// A long identifier warning.
#include "msdevstudio/MSconfig.h"
#endif

#include "reps/TextRepFactory.h"

// A list of default TextReps.
#include "reps/TotalTextRep.h"
#include "reps/FuncChiRep.h"
#include "reps/FuncParmRep.h"
#include "reps/AverageTextRep.h"
#include "reps/BoxTextRep.h"
#include "reps/UnderflowTextRep.h"
#include "reps/OverflowTextRep.h"

namespace hippodraw {

TextRepFactory * TextRepFactory::s_instance = 0;

TextRepFactory::TextRepFactory ()
{
}

TextRepFactory * TextRepFactory::instance ()
{
  if ( s_instance == 0 ) {
    s_instance = new TextRepFactory ();
    s_instance->initialize ();
  }

  return s_instance;
}

void TextRepFactory::initialize ()
{
  add ( new TotalTextRep () );
  add ( new FuncChiRep () );
  add ( new FuncParmRep () );
  add ( new AverageTextRep ( Axes::X ) );
  add ( new AverageTextRep ( Axes::Y ) );
  add ( new BoxTextRep () );
  add ( new UnderflowTextRep () );
  add ( new OverflowTextRep () );
}

} // namespace hippodraw
