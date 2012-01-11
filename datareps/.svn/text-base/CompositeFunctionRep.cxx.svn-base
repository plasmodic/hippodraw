/* -*- mode: c++ -*- */

/** @file

hippodraw::CompositeFunctionRep class implementation

Copyright (C) 2005, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: CompositeFunctionRep.cxx,v 1.14 2006/06/12 18:23:21 pfkeb Exp $

*/

#include "CompositeFunctionRep.h"

#include "FunctionParameter.h"

#include "functions/FunctionBase.h"
#include "projectors/ProjectorBase.h"

#include "reps/LineFunctionRep.h"

#include <algorithm>

#include <cassert>

using std::find;
using std::string;
using std::vector;

using namespace hippodraw;

CompositeFunctionRep::
CompositeFunctionRep ( FunctionBase * function, DataRep * target )
  : FunctionRep ( function, target )
{
  m_rep = new LineFunctionRep ();
  // The above deleted in ~DataRep()

  const Color blue ( Color::blue );
  m_rep -> setColor ( blue );
  m_rep -> setSize ( 2 );
}

/** @note The object may not have its Fitter set yet, so
    implementation should not try to repair the fixed flags if none
    exist.
 */
void
CompositeFunctionRep::
addToComposite ( FunctionRep * frep )
{
  frep -> setInComposite ( true );
  m_func_reps.push_back ( frep );

  FunctionBase * composite = getFunction ();
  FunctionBase * function = frep -> getFunction ();

  composite -> addToComposite ( function );

  Fitter * fitter = getFitter ();
  if ( fitter != 0 ) {
    vector < int > flags = getFixedFlags (); // a copy
    int size = function -> size ();
    while ( size-- != 0 ) {
      flags.push_back ( 0 );
    }

    setFixedFlags ( flags );
  }
}

bool
CompositeFunctionRep::
isMember ( const FunctionRep * rep )
{
  FunctionRepList_t::iterator first
    = find ( m_func_reps.begin(), m_func_reps.end(), rep );

  return first != m_func_reps.end();
}

void
CompositeFunctionRep::
removeFromComposite ( FunctionRep * frep )
{
  FunctionBase * function = frep -> getFunction ();
  FunctionBase * composite = getFunction ();
  composite -> removeFromComposite ( function );

  FunctionRepList_t::iterator first
    = find ( m_func_reps.begin(), m_func_reps.end(), frep );
  if ( first != m_func_reps.end () ) {
    m_func_reps.erase ( first );
  }

  frep -> setInComposite ( false );
}

unsigned int
CompositeFunctionRep::
count () const
{
  return m_func_reps.size ();
}

const CompositeFunctionRep::FunctionRepList_t &
CompositeFunctionRep::
getFunctionReps () const
{
  return m_func_reps;
}

void
CompositeFunctionRep::
drawProjectedValues ( TransformBase * transform, ViewBase * view )
{

  drawCuts ( transform, view );

  const Range & range = m_projector -> getRange ( Axes::X );
  FunctionBase * function = getFunction ();
  LineFunctionRep * rep = dynamic_cast < LineFunctionRep * > ( m_rep );
  assert ( rep != 0 );

  rep -> drawProjectedValues ( range, function, transform, view );

  setDirty ( false );
}

void
CompositeFunctionRep::
setFixedFlags ( const std::vector < int > & flags )
{
  unsigned int size = m_func_reps.size ();
  vector<int>::const_iterator first = flags.begin();

  for ( unsigned int i = 0; i < size; i++ ) {
    FunctionRep * frep = m_func_reps[i];
    vector < int > tmp = frep -> getFixedFlags ();// a copy
    unsigned int number = tmp.size ();
    vector<int>::const_iterator end = first + number;
    copy ( first, end, tmp.begin() );
    frep -> setFixedFlags ( tmp );
    first = end;
  }

  FunctionRep::setFixedFlags ( flags );
}
