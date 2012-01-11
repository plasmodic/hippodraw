/** @file

hippodraw::ObserverWrap class implementation

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: ObserverWrap.cxx,v 1.15 2007/07/02 18:17:15 pfkeb Exp $

*/

#ifdef HAVE_CONFIG_H
// for old python
#include "config.h"
#endif

// include first to avoid _POSIX_C_SOURCE warning.
#include <boost/python.hpp>

#include "ObserverWrap.h"

#include "pattern/Observable.h"

#include <iostream>

using std::string;

using namespace boost::python;


namespace hippodraw {
  namespace Python {
    void
    export_Observer ()
    {
		class_ < ObserverWrap,
			     std::auto_ptr<ObserverWrap>,
				 boost::noncopyable 		>
	 ( "Observer",
	  "The base class for classes that can be targets of Observable\n"
	  "update messages.",
	  no_init )
	;

      class_ < Observable >
	( "Observable",
	  "The base class for classes that can be target of an Observer\n"
	  "object.",
	  init <> () )
	;
    }
  } // namespace Python
} // namespace hippodraw

using namespace hippodraw;

  /** @todo This method and others may not be needed.
   */
ObserverWrap::
ObserverWrap ( PyObject * self )
  : hippodraw::Observer (),
    m_self ( self )
{
}

ObserverWrap::
ObserverWrap ( )
  : hippodraw::Observer (),
    m_self ( 0 )
{
}
ObserverWrap::
ObserverWrap ( const Observer & ow )
  : hippodraw::Observer ( ow ),
    m_self ( 0 )
{
}

template <class T>
object
ObserverWrap::
get_owner(T* ) const // me
{
      // Use secret interface to get the Python object
      // that owns *this.  I guess I will have to make that
      // interface public. -- Dave Abrahams
      return
object ( handle<> ( borrowed ( detail::wrapper_base_::get_owner(*this))));
}

/**@note This function is only available with Python 2.3 or later.
 */
void
ObserverWrap::
update ( const Observable * obs )
{
#ifndef HAVE_OLD_PYTHON
  PyGILState_STATE state = PyGILState_Ensure ();

  try {
     object self = get_owner ( this );
     call_method < void, const Observable * > ( self.ptr(), "update", obs );
  } catch ( error_already_set & ) {
     std::cout << "ObserverWrap::update: caught error_already_set" 
               << std::endl;
     PyErr_Print();
     handle_exception();
  }

  PyGILState_Release ( state );
#endif
}
