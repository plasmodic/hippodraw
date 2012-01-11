/* -*- mode:c++ - */

/** @file 

Observer class interface

Copyright (C) 2001, 2003-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Observer.h,v 1.21 2006/09/21 22:55:29 pfkeb Exp $

*/

#ifndef _Observer_H_
#define _Observer_H_

#include "libhippo.h"


namespace hippodraw {

class Observable;

/** Part of an implementation of the Observable Observer 
    pattern based on the example implementation in the GOF 
    Patterns text. 

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

    @note Giulio Eulisse <giulio.eulisse@cern.ch> put this class in
    namespace hippodraw so that it wouldn't clash with a Corbra
    Observer and contributed all the code that needed changing.
*/
class MDL_HIPPOPLOT_API Observer 
{
public:

  /** The default constructor.
      @note Needs to be public because of SIP.
 */
  Observer();

  /** The destructor.  Does nothing but keep compiler warning messages
      away.

      @note Needs to be public because of SIP.
  */
  virtual ~Observer ();

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

};

} // namespace hippodraw
#endif // _Observer_H_
