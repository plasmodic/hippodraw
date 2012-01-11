/* -*- mode: c++ -*- */

/** @file

FactoryException class interface

Copyright (C)  2002, 2003  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: FactoryException.h,v 1.15 2005/10/30 00:10:12 pfkeb Exp $

*/

#ifndef _FactoryException_H_
#define _FactoryException_H_

#include "libhippo.h"

#include <exception>
#include <string>

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

namespace hippodraw {

/** An exception class that is thrown when the factory fails to find
    the request class by its name.  The class contains the error
    message with the failed name. 

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API FactoryException : public std::exception
{
private:

  /** The type name that could not be found in the factory. */
  std::string m_type;

public: 

  /** The copy constructor.  @attention GCC 2.95.3 needs it else
      exception doesn't appear to be caught correctly.  GCC 3.2.1
      doesn't need it.
  */
  FactoryException ( const FactoryException & );

  /** The constructor.  Takes as argument the type name that could not
      be found in the factory. */
  FactoryException ( const std::string & type );

  /** The destructor.  @attention Althought implementation does
      nothing GCC 3.2.1 insists that it declared like the base
      class. */
  virtual ~FactoryException () throw ();
  
  /** The error message that can be retreived. */
  char const * what () const throw ();

};

} // namespace hippodraw

#endif // _FactoryException_H_
