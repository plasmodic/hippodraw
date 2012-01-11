/** @file

hippodraw::CutPlotter class implementation

Copyright (C) 2000-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: CutPlotter.cxx,v 1.114 2007/07/02 18:17:15 pfkeb Exp $

*/


// for bind2 defect
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// for truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "CutPlotter.h"

#include "CompositePlotter.h"

#include "datareps/DataRep.h"

#include "datasrcs/DataSource.h"
#include "datasrcs/TupleCut.h"

#include "projectors/NTupleProjector.h"

#include "reps/CutRangeRep.h"
#include "reps/ColorBoxPointRep.h"

#include <algorithm>
#include <functional>

#include <cassert>

using std::list;
using std::mem_fun;
using std::swap;
using std::string;
using std::vector;

using namespace hippodraw;

CutPlotter::
CutPlotter ( const std::string & name )
  : XyPlotter( name )//,
{
}

CutPlotter::CutPlotter ( const CutPlotter & plotter )
  : XyPlotter ( plotter )
{
}

CutPlotter::~CutPlotter ()
{
  removeFromTargets ();
}

/** @todo Shouldn't call autoScale() here.  Setting autoscaled to
    false should be sufficient, but it doesn't work. Need to solve
    that problem.
*/
void
CutPlotter::
setAxisBinding ( const std::vector < std::string > & bindings )
{
  DataRep * datarep = selectedDataRep ();
  ProjectorBase * pbase = datarep->getProjector ();
  NTupleProjector * projector = dynamic_cast < NTupleProjector * > ( pbase );

  projector->setAxisBindings ( bindings );

  autoScale ();
  
  const vector < TupleCut > & cuts = m_datarep -> getCuts ();
  vector < TupleCut > & cut_list 
    = const_cast < vector < TupleCut > & > ( cuts ); 
  
  unsigned int size = bindings.size ();
  for ( unsigned int i = 0; i < size; i++ ) {
    const string & label = bindings[i];
    unsigned int index = projector -> indexOf ( label );
    TupleCut & cut = cut_list[i]; // so Doxygen can find it
    cut.setColumn ( index );
    cut.setLabel ( label );
  }

  updateTargets();
}

void
CutPlotter::
setCutRangeAt ( const Range & range, unsigned int index )
{
  m_datarep -> setCutRangeAt ( range, index );
  
  updateTargets ();
}

void
CutPlotter::
setCutRange ( double low, double high )
{
  const Range range ( low, high );
  setCutRangeAt ( range, 0 );
}

void
CutPlotter::
removeFromTarget ( DataRep * rep )
{
  // Does not remove rep from m_targets because that would invalidate
  // iterators in the function removeFromTargets().
  ProjectorBase * proj = rep->getProjector ();
  NTupleProjector * projector = 
    dynamic_cast < NTupleProjector * > ( proj );
  assert( projector );

  const vector < TupleCut > & cuts = m_datarep -> getCuts ();
  for ( unsigned int i = 0; i < cuts.size(); i++ ) {
    projector -> removeCut( & cuts[i] );
  }
  rep -> removeObserver( this );

  TargetList_t::iterator first
    = find ( m_targets.begin(), m_targets.end(), rep );
  assert ( first != m_targets.end() );
  m_targets.erase ( first );

  rep -> setDirty();
}

void 
CutPlotter::
removeFromTargets()
{
  TargetList_t::const_iterator first = m_targets.begin();
  while ( first != m_targets.end() ) {
    DataRep * rep = *first++;
    removeFromTarget ( rep );
  }

  assert ( m_targets.empty () );
}

void
CutPlotter::
willDelete ( const Observable * obs )
{
  const DataRep * rep = dynamic_cast < const DataRep * > ( obs );
  assert ( rep ); // only observes DataRep objects

  TargetList_t::iterator first
    = find ( m_targets.begin(), m_targets.end(),  rep );
  
  if ( first == m_targets.end () ) return; // not a target
  
  m_targets.erase ( first );
}

void CutPlotter::updateTargets ()
{
#ifdef BIND2ND_DEFECT
  TargetList_t::iterator first = m_targets.begin();
  while ( first != m_targets.end() )
    {
      DataRep * rep = *first++;
      rep->setDirty ( true );
    }
#else
#ifdef MEMFUN1_DEFECT
  for_each ( m_targets.begin(), m_targets.end(),
	     bind2nd ( mem_fun1 ( &DataRep::setDirty ), true ) );
#else
  for_each ( m_targets.begin(), m_targets.end(),
	     bind2nd ( mem_fun ( &DataRep::setDirty ), true ) );
#endif
#endif
  notifyObservers ();
}

const vector < TupleCut > &
CutPlotter::
getCuts () const
{
  return m_datarep -> getCuts ();
}

void
CutPlotter::setCuts ( const std::vector < const TupleCut * > & new_cuts )
{
  unsigned int size = new_cuts.size ();

  if ( m_datarep ) {
    const vector < TupleCut > & cuts = m_datarep -> getCuts ();
    vector < TupleCut > & cut_list 
      = const_cast < vector < TupleCut > & > ( cuts );
    cut_list.resize ( size, TupleCut () );
    for ( unsigned int i = 0; i < cut_list.size (); i++ ) {
      cut_list[i] = *new_cuts[i];
    }
  }
}

/* virtual */
bool CutPlotter::hasNTupleBindings () const
{
  return true;
}

void CutPlotter::clearCutTargets ()
{
  m_targets.clear();
}

void CutPlotter::addCutTarget ( DataRep * rep )
{
  if ( rep != m_datarep ) {
    m_targets.push_back ( rep );
    rep->addObserver ( this );
  }
}

const list < DataRep * > & CutPlotter::getCutTargets () const
{
  return m_targets;
}

bool CutPlotter::
isTarget ( DataRep * rep ) const
{
  return find ( m_targets.begin(), m_targets.end(), rep ) != m_targets.end();
}

void CutPlotter::setCutColor ( const Color & color )
{
  RepBase * rep = m_datarep -> getCutRep ();
  rep -> setColor ( color );

  XyPlotter::update ();

  notifyObservers ();
}

const Color & CutPlotter::getCutColor () const
{
  RepBase * rep = m_datarep -> getCutRep ();

  return rep -> getColor ();
}

void CutPlotter::setNTuple ( const DataSource * ntuple )
{
  DataRep * datarep = selectedDataRep ();
  ProjectorBase * pbase = datarep->getProjector ();
  NTupleProjector * projector = dynamic_cast < NTupleProjector * > ( pbase );

  projector->setNTuple ( ntuple );
  DataSource * nt = const_cast < DataSource * > ( ntuple );
  nt->addObserver ( projector );
}

void
CutPlotter::
setEnabled ( bool yes )
{
  bool no = false;

  const vector < TupleCut > & cuts = m_datarep -> getCuts ();
  for ( unsigned int i = 0; i < cuts.size(); i++ ) {
    no |= cuts[i].isEnabled ();
    m_datarep -> setEnabled ( i, yes );
  }

  if ( yes != no ) {
    updateTargets ();
  }
}

void
CutPlotter::
update ( const Observable * object )
{
  TargetList_t::const_iterator first 
    = find ( m_targets.begin(), m_targets.end(), object );

  if ( first == m_targets.end () ) {
    notifyObservers ();
  }
}

void
CutPlotter::
setActive ( bool yes )
{
  RepBase * rep = m_datarep -> getCutRep ();
  rep -> setSelected ( yes );
  notifyObservers ();
}

void
CutPlotter::
addTupleCut ( DataRep * )
{
}

bool
CutPlotter::
isImageConvertable () const
{
  return false;
}
