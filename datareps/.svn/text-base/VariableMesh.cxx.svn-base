/** @file

hippodraw::VariableMesh class implementation.

Copyright (C) 2006, 2007   The Board of Trustees of The
Leland Stanford Junior University.  All Rights Reserved.

$Id: VariableMesh.cxx,v 1.79 2007/07/20 19:29:56 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "VariableMesh.h"

#include "projectors/MeshProjector.h"
#include "reps/ColorBoxPointRep.h"

using namespace hippodraw;

using std::string;
using std::vector;


VariableMesh::VariableMesh ( )
  : DataRep ()
{
  m_name = "Variable Mesh";

  MeshProjector * projector = new MeshProjector();
  projector->setXErrorOption ( false ); // method not in ProjectorBase
  projector->setYErrorOption ( false );
  m_projector = projector;

  m_rep = new ColorBoxPointRep ( );
}

DataRep * VariableMesh::clone ()
{
  return new VariableMesh( *this );
}

/** @todo In this and two other members, the implementation has
    knowledge of axis binding via string "nil".  Should probably have
    method in projector that gives the answer if error axis is
    bound. 
*/
void
VariableMesh::
setRepresentation ( RepBase * rep )
{
  DataRep::setPointRep ( rep );

  const vector < string > & bindings = m_projector -> getAxisBindings ();

  bool yes = bindings[3] != "nil";
  setErrorDisplay ( Axes::X, yes );

  yes = bindings[4] != "nil";
  setErrorDisplay ( Axes::Y, yes );
}

bool VariableMesh::acceptFunction ( int num )
{
  return num == 2;
}

bool
VariableMesh::
hasErrorDisplay () const
{
  bool yes = true;

  const vector < string > & bindings = m_projector -> getAxisBindings ();
   
  // Was 2 and 3, should be a bug 
  yes = bindings[3] != "nil" || bindings[4] != "nil";    

  return yes;
}


void
VariableMesh::
setAxisBinding ( const std::string & axis, const std::string & label )
{
  DataRep::setAxisBinding ( axis, label );

  NTupleProjector * projector 
    = dynamic_cast < NTupleProjector * > ( m_projector );
  unsigned int index = projector -> indexOfBindingOption ( axis );

  bool yes = label != "nil";
  if ( index == 3 ) {
    setErrorDisplay ( Axes::X, yes );
  }

  if ( index == 4 ) {
    setErrorDisplay ( Axes::Y, yes );
  }
}

void
VariableMesh::
setAxisBindings ( const std::vector < std::string > & new_bindings )
{
  DataRep::setAxisBindings ( new_bindings );

  const vector < string > & bindings = m_projector ->getAxisBindings ();
  bool yes = bindings[3] != "nil";
  setErrorDisplay ( Axes::X, yes );

  yes = bindings[4] != "nil";
  setErrorDisplay ( Axes::Y, yes );
}

bool
VariableMesh::
hasAxis ( hippodraw::Axes::Type axis ) const
{
  return axis == Axes::X || axis == Axes::Y || axis == Axes::Z;
}
