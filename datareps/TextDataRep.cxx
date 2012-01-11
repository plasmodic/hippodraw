/** @file

hippodraw::TextDataRep class implementation

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: TextDataRep.cxx,v 1.28 2006/05/25 20:53:22 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "TextDataRep.h"

#include "projectors/Map1Projector.h"
#include "reps/TextRepBase.h"

#include <cassert>

using namespace hippodraw;

TextDataRep::TextDataRep ( RepBase * rep )
  : DataRep()
{
  m_name = "TextDataRep";
  m_rep = rep;
}

TextDataRep::TextDataRep ( )
  : DataRep()
{
  m_name = "TextDataRep";
  m_rep = 0;
}

TextDataRep::
TextDataRep ( const TextDataRep & rep )
  : DataRep ( rep )
{
}

DataRep * TextDataRep::clone ()
{
  return new TextDataRep ( *this );
}

bool TextDataRep::hasNTupleBindings () const
{
  return false;
}

ProjectorBase *
TextDataRep::
getTargetProjector ( ) const
{
  ProjectorBase * projector = 0;
  DataRep * target = getParentDataRep();

  if ( target != 0 ) {
    projector = target -> getProjector ();
  }

  return projector;
}

void
TextDataRep::
drawProjectedValues ( TransformBase *, ViewBase * view )
{
  assert ( m_rep );  
  TextRepBase * t_rep = dynamic_cast < TextRepBase * > ( m_rep );
  assert ( t_rep != 0 );

  ProjectorBase * projector = getTargetProjector ();
  if ( projector != 0 ) {
    t_rep -> drawProjectedValues ( *projector, *view );
  }
  else {
    t_rep -> displayError ( *view );
  }
}

bool
TextDataRep::
isTargetable () const
{
  return false;
}

using namespace hippodraw;

bool
TextDataRep::
hasAxis ( hippodraw::Axes::Type ) const
{
  return false;
}
