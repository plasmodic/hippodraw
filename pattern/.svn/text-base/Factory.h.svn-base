/* -*- mode: c++ -*- */

/** @file

hippodraw::Factory class interface and template implementation

Copyright (C) 2000-2003, 2005, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Factory.h,v 1.40 2006/06/23 22:09:33 pfkeb Exp $

*/

#ifndef _Factory_H_
#define _Factory_H_

#include "FactoryException.h"

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include <map>
#include <vector>

namespace hippodraw {

/** The base class for singleton factory classes. The factory
    maintains an instance of each type that can be produced by the
    factory.  This instance is called the @em prototype. There are two
    requirements on the type that can be handled by the factory.
    First that it has a member function @c name() that returns a @c
    std::string for its name.  The name does not have to be the class
    name.  And second that it has a member function @c clone () to
    make a copy of itself. 

    In most cases, a derived class of Factory need only supply the a
    way to initialize list of prototypes.  Most of the implementation
    is in this template class.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
template < class Type > class Factory
{
private:

  /** A private copy constructor to prevent copying. */
  Factory ( const Factory< Type > & );

protected:

  /** A list of available types. */
  std::map < std::string, Type * > m_types;

  /** The list of available types by names.  This list should be
      identical to the list of @c m_types keys.  It is updated
      when the member function names is called. */
  mutable std::vector< std::string > m_names;

  /** The default constructor.  Since this constructor is protected,
      only derived classes of Factory can be created. */
  Factory ( );

  /** The destructor.  It is responsible for deleting the
      prototype objects. */
  virtual ~Factory();

 public:

  /** Adds a prototype object to the list of prototypes. */
  void add ( Type * );

  /** Removes the named prototype object from the list of prototypes.
   */
  void remove ( const std::string & name );

  /** Returns @a true if prototype with name @a name exists in the
      factory, otherwise returns false.
  */
  bool exists ( const std::string & name ) const;

  /** Returns a pointer to a prototype with name @a name.  If a
      prototype with @a name doesn't exit, throws a FactoryException
      object with the name not found. */
  Type * prototype ( const std::string & name ) const;

  /** Creates a new object from a prototype named @a name.  If found,
      returns a new object by invoking the clone() member function of
      the prototype.  If not found, throws a FactoryException object
      with the name not found. */
  Type * create ( const std::string & name );

  /** Returns the list of available prototype names. */
  const std::vector< std::string > & names () const;

};

template < class Type >
Factory < Type >::Factory ( )
{
}

template< class Type >
Factory<Type>::~Factory ()
{
  typename std::map < std::string, Type * > ::iterator first
	  = m_types.begin();
  for ( ; first != m_types.end(); ++first ) {
    delete first->second;
  }

  m_types.clear ();
}

template< class Type >
void Factory<Type>::add ( Type * obj )
{
  const std::string & name = obj->name ();
  m_types[name] = obj;
}

template < class Type >
void
Factory < Type >::
remove ( const std::string & name )
{
  typename std::map < std::string, Type * > ::iterator it
    = m_types.find ( name );
  if ( it != m_types.end () ) {
    m_types.erase ( it );
  }
}

template < class Type >
bool Factory < Type>::
exists ( const std::string & name ) const
{
  // Don't use map::operator[]() to find the name, as it will create
  // one if it doesn't exist.
  typename std::map< std::string, Type * >::const_iterator it 
    = m_types.find ( name );

  return it != m_types.end ();
}


template< class Type >
Type * Factory<Type>::prototype ( const std::string & name ) const
{
  // Don't use map::operator[]() to find the name, as it will create
  // one if it doesn't exist.
  typename std::map< std::string, Type * >::const_iterator it 
    = m_types.find ( name );
  if ( it == m_types.end () ) throw FactoryException ( name );

  return it->second;
}

template< class Type >
Type * Factory<Type>::create ( const std::string & name )
{
  Type * obj = prototype ( name );
  if ( obj == 0 ) {
    return 0;
  }
  return obj->clone ();
}

template< class Type >
const std::vector< std::string > & Factory<Type>::names () const
{
  m_names.clear ();
  typename std::map <std::string, Type *>::const_iterator it 
    = m_types.begin ();
  for ( ; it != m_types.end (); ++it ) {
    m_names.push_back ( it->first );
  }

  return m_names;
}

} // namespace hippodraw

#endif // _Factory_H_
