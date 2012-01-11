/* -*- c++ -*- */

/** @file

hippodraw::ObserverWrap class interface.

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: ObserverWrap.h,v 1.7 2006/05/14 17:48:21 pfkeb Exp $

*/

#ifndef ObserverWrap_H
#define ObserverWrap_H

#include "pattern/Observer.h"

namespace hippodraw {

/** A wrapper class for Observer to allow its pure virtual function to
    be implemented in Python.

    @note This class needs Python 2.3 or later to be fully functional.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

 */
class ObserverWrap : public hippodraw::Observer,
                     public boost::python::wrapper < hippodraw::Observer >
{
private:

  /** The held Python object.
   */
  PyObject * m_self;

  /** Find the Python object that owns this object.
   */
  template < class T >
  boost::python::object get_owner ( T * me ) const;

public:

  /** The constructor.
   */
  ObserverWrap ( PyObject * self );

   ObserverWrap();
   ObserverWrap ( const Observer & );

  void update ( const Observable * );

};

} // namespace hippodraw

#endif // ObserverWrap_H
