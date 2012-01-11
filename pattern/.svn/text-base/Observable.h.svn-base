/* -*- mode: c++ -*- */

/** @file

Observable class interface

Copyright (C) 2001, 2003, 2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Observable.h,v 1.29 2005/11/02 00:52:05 jchiang Exp $
*
*/

#ifndef _Observable_H_
#define _Observable_H_

#include "libhippo.h"

#ifdef _MSC_VER
#include <msdevstudio/MSconfig.h>
#endif

#include <list>

namespace hippodraw {

  class Observer;

/** Part of an implementation of the Observable-Observer pattern based
    on the example in the GOF Patterns text book and the Java
    Observable class. 

    @todo We don't have a copy constructor.  Determine if that a
    problem?

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API Observable
{
public:

  /** The type of STL container to maintain list of Observer objects.
   */
  typedef std::list < Observer * > ObserverList_t;

private:

  /** The list of objects observing this Observable.
   */
  ObserverList_t m_list;

//protected:

public:
  /** The default constructor. */
  Observable();

public:

  /** The destructor.  Does nothing but keep compiler warning messages
      away.  

      @note needs to be public because of SIP.
  */
  virtual ~ Observable ();

  /** Adds an Observer to the Observer list. */
  void addObserver ( Observer * );

  /** Returns the list of observers. */
  const ObserverList_t & getObservers () const;

  /** Removes an Observer from the Observer list. */
  void removeObserver ( Observer * );

  /** Notifies Observer objects of a change.  If interval counting is
      disabled, then Observer::Update is sent immediately.  If
      interval counting is enabled, then the current count is
      incremented.  If the current count is equal to the interval
      count, then the Observer::update message is sent and the current
      count reset to 0.  */
  virtual void notifyObservers () const;

  /** The member function of the Observer that is called. */
  typedef void ( Observer::*Action ) ( const Observable * );

protected:

  /** An internal method to iterate over all observers and 
      to send the message @a action. */
  void notifyObservers ( Action action ) const;

};

} // namespace hippodraw

#endif // _Observable_H_
