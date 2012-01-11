/** @file

XYPlot classes implementation.

Copyright (C) 2000, 2001, 2003-2007   The Board of Trustees of The
Leland Stanford Junior University.  All Rights Reserved.

$Id: XYPlot.cxx,v 1.77 2007/07/26 21:44:34 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "XYPlot.h"

#include "projectors/Map2Projector.h"
#include "reps/SymbolPointRep.h"

using namespace hippodraw;

using std::string;
using std::vector;


XYPlot::XYPlot ( )
{
  m_name = "XY Plot";

  Map2Projector * projector = new Map2Projector();
  projector->setXErrorOption ( true ); // method not in ProjectorBase
  projector->setYErrorOption ( true );
  m_projector = projector;

  m_rep = new SymbolPointRep ( Symbol::SOLIDSQUARE, 4.0 );
}

DataRep * XYPlot::clone ()
{
  return new XYPlot( *this );
}

/** @todo In this and two other members, the implementation has
    knowledge of axis binding via string "nil".  Should probably have
    method in projector that gives the answer if error axis is
    bound. 
*/
void
XYPlot::
setRepresentation ( RepBase * rep )
{
  DataRep::setPointRep ( rep );
  if ( rep -> name () == "Symbol" ) {
    rep -> setSize ( 4.0 );
    const vector < string > & bindings = m_projector -> getAxisBindings ();

    bool yes = bindings[2] != "nil";
    setErrorDisplay ( Axes::X, yes );

    yes = bindings[3] != "nil";
    setErrorDisplay ( Axes::Y, yes );
  }
}

bool XYPlot::acceptFunction ( int num )
{
  return num == 1;
}

bool
XYPlot::
hasErrorDisplay () const
{
  bool yes = true;

  if ( m_rep -> name () == "Symbol" ) {
    const vector < string > & bindings = m_projector -> getAxisBindings ();
    
    yes = bindings[2] != "nil" || bindings[3] != "nil";
  }

  return yes;
}


void
XYPlot::
setAxisBinding ( const std::string & axis, const std::string & label )
{
  DataRep::setAxisBinding ( axis, label );

  NTupleProjector * projector 
    = dynamic_cast < NTupleProjector * > ( m_projector );
  unsigned int index = projector -> indexOfBindingOption ( axis );

  bool yes = label != "nil";
  if ( index == 2 ) {
    setErrorDisplay ( Axes::X, yes );
  }

  if ( index == 3 ) {
    setErrorDisplay ( Axes::Y, yes );
  }
}

void
XYPlot::
setAxisBindings ( const std::vector < std::string > & new_bindings )
{
  DataRep::setAxisBindings ( new_bindings );

  const vector < string > & bindings = m_projector ->getAxisBindings ();
  bool yes = bindings[2] != "nil";
  setErrorDisplay ( Axes::X, yes );

  yes = bindings[3] != "nil";
  setErrorDisplay ( Axes::Y, yes );
}

bool
XYPlot::
hasAxis ( hippodraw::Axes::Type axis ) const
{
  return axis == Axes::X || axis == Axes::Y;
}

bool
XYPlot::
hasZoomY ( ) const
{
  return true;
}
