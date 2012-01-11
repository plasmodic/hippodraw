/* -*- mode:c++ -*- */

/** @file

This is the SIP interface definition for Observer

Copyright (C)  2003, 2005  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

$Id: Observer.sip,v 1.9 2005/04/25 15:44:58 pfkeb Exp $

@author stuvi@slac.stanford.edu

*/

class Observer
{
%TypeHeaderCode
#include "pattern/Observer.h"
using namespace hippodraw;
%End


protected:

  Observer ();

public:
  /** The update method.  This member function is called by the
      observed Observable when its state has changed  The Command
      argument is optional and may be a null pointer.  */
  virtual void update ( const Observable * ) = 0;

 /** Notifies @c this Observer object that one of its Observable
      objects is about to be deleted.  Since most Observable objects
      are owned by its Observer, it is the Observer that issued the
      delete.  Thus the default implementation here does nothing.
      Derived classes that observe objects owned by another should
      implement this method to remove the Observable from the list
      they are observing. */
  virtual void willDelete ( const Observable * );

  /** The destructor. */ 
  virtual ~Observer ();

};
