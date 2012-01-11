/* -*- mode:c++ -*- */

/** @file

This is the SIP interface definition for Observable

Copyright (C)  2003, 2005  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

$Id: Observable.sip,v 1.9 2007/02/24 22:24:38 pfkeb Exp $

@author Selim Tuvi <stuvi@slac.stanford.edu>

*/

class Observable
{
%TypeHeaderCode
#include "pattern/Observable.h"
#include "pattern/Observer.h"
using hippodraw::Observable;
using hippodraw::Observer;
%End

public:

 /** The type of STL container to maintain list of Observer objects.
   */
  typedef std::list < Observer * > ObserverList_t;

 /** The destructor.  Does nothing but keep compiler warning messages
      away. */
  virtual ~ Observable ();

  /** Adds an Observer to the Observer list. */
  virtual void addObserver ( Observer * );

//   /** Returns the list of observers. */
//   const ObserverList_t & getObservers () const;

  /** Removes an Observer from the Observer list. */
  void removeObserver ( Observer * );

  /** Notifies Observer objects of a change.  If interval counting is
      disabled, then Observer::Update is sent immediately.  If
      interval counting is enabled, then the current count is
      incremented.  If the current count is equal to the interval
      count, then the Observer::update message is sent and the current
      count reset to 0.  */
  virtual void notifyObservers () const;

//   /** The member function of the Observer that is called. */
//   typedef void ( Observer::*Action ) ( const Observable * );



protected:

  Observable ();
  //virtual ~Observable ();

//  /** An internal method to iterate over all observers and 
//       to send the message @a action. */
//   void notifyObservers ( Action action ) const;
};
