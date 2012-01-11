/** @file

hippodraw::FuncChiRep class implementation

Copyright (C) 2001-2003, 2005, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FuncChiRep.cxx,v 1.28 2006/07/16 14:53:38 jchiang Exp $

*/

#include "FuncChiRep.h"

#include "graphics/ViewBase.h"
#include "pattern/string_convert.h"
#include "projectors/FunctionProjector.h"

#include <iomanip>
#include <sstream>

using std::string;

using namespace hippodraw;

FuncChiRep::FuncChiRep ( )
  : TextRepBase ( "Chi-squared" )
{
}

FuncChiRep::FuncChiRep ( const FuncChiRep & rep )
  : TextRepBase( rep )
{
}

RepBase * FuncChiRep::clone ()
{
  return new FuncChiRep ( *this );
}

void
FuncChiRep::
drawProjectedValues ( ProjectorBase & proj,
		      ViewBase & view )
{
  const FunctionProjector & projector 
    = dynamic_cast< const FunctionProjector & > ( proj );

  int dof = projector.degreesOfFreedom ();

  std::ostringstream message;
  message << "chi^2 / dof = "
          << std::setprecision(3)
          << projector.objectiveValue()
          << " / " << dof;
  
  view.drawText ( message.str(), 5.0, 0.0, m_size, 0, 'l', 't', true,
		  0, &m_color );
}
