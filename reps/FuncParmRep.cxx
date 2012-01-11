/** @file

hippodraw::FuncParmRep class implementation

Copyright (C) 2001-2003, 2005, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FuncParmRep.cxx,v 1.29 2006/05/29 14:27:49 pfkeb Exp $

*/

#include "FuncParmRep.h"

#include "functions/FunctionBase.h"
#include "graphics/ViewBase.h"
#include "pattern/string_convert.h"
#include "projectors/FunctionProjector.h"

using std::string;
using std::vector;

using namespace hippodraw;

FuncParmRep::FuncParmRep ( )
  : TextRepBase ( "Function Parameters" )
{
}

FuncParmRep::FuncParmRep ( const FuncParmRep & rep )
  : TextRepBase( rep )
{
}

RepBase * FuncParmRep::clone ()
{
  return new FuncParmRep( *this );
}

void
FuncParmRep::
drawProjectedValues ( ProjectorBase & proj,
		      ViewBase & view )
{
  const FunctionProjector & projector 
    = dynamic_cast< const FunctionProjector & > ( proj );

  FunctionBase * function = projector.function ();

  string text = function -> name ();

  const vector < string > & parm_names = function->parmNames ();
  const vector < double > & parm_values =function->getParameters ();

  for ( unsigned int i = 0; i < parm_names.size (); i++ ) {
    text += "\n" + parm_names[i];
    text += " = ";
    text += String::convert ( parm_values[i] );
  }

  view.drawText ( text, 5.0, 0., m_size, 0, 'l', 't', true, 0, & m_color );
}
