/** @file

hippodraw::Observable class implementaton

Copyright (C) 2001, 2003-2005, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Observable.cxx,v 1.43 2007/07/02 18:17:15 pfkeb Exp $

*/

// for memfun1
#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif
#endif

#include "Observable.h"

#include "Observer.h"

#include <algorithm>
#include <functional>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::bind2nd;
using std::for_each;
using std::list;
using std::mem_fun;
#endif

using namespace hippodraw;

Observable::Observable ()
{
//   m_list.clear();
}

/* virtual */
Observable::~ Observable ()
{
  // Don't need to do anything because the list will clear itself when
  // it is destroyed
}

/* virtual */
void Observable::addObserver ( hippodraw::Observer * observer )
{
  m_list.push_back ( observer );
}

/* virtual */
const Observable::ObserverList_t &
Observable::
getObservers ( ) const
{
  return m_list;
}

/* virtual */
void Observable::removeObserver ( hippodraw::Observer * observer )
{
  m_list.remove ( observer );
}

/* virtual */
void Observable::notifyObservers ( Action action ) const
{
#ifdef BIND2ND_DEFECT
//   list < Observer * >::const_iterator first = m_list.begin ();
  ObserverList_t::const_iterator first = m_list.begin ();

  for ( ; first != m_list.end (); ++first ) {
    ( (*first)->*action ) ( this );
  }
#else
#ifdef MEMFUN1_DEFECT
  for_each ( m_list.begin (), m_list.end (),
	     bind2nd ( mem_fun1 ( action ), this ) );
#else
  for_each ( m_list.begin (), m_list.end (),
	     bind2nd ( mem_fun ( action ), this ) );
#endif
#endif
}

/* virtual */
void Observable::notifyObservers ( ) const
{
  notifyObservers ( &hippodraw::Observer::update );
}
