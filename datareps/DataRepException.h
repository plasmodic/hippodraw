/* -*- mode: c++ -*-
 *
 * HippoPlot DataRepException class interface
 *
 * Copyright (C)  2002, 2003  The Board of Trustees of The Leland Stanford
 * Junior University.  All Rights Reserved.
 *
 * $Id: DataRepException.h,v 1.9 2005/10/30 00:10:10 pfkeb Exp $
 *
 */

#ifndef _DataRepException_H_
#define _DataRepException_H_

#include "pattern/libhippo.h"

#include <exception>
#include <string>

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

namespace hippodraw {

/** An xception class that is thrown when attempting to bind a
    DataRep to a NTuple with a column name that doesn't exist.  The
    class contains the error message with the name of the bad column
    label.

    @todo Only QtDisplay uses this class, maybe it could be
    eliminated.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

*/
class MDL_HIPPOPLOT_API DataRepException : public std::exception
{
private:

  /** The binding label that could not be found in NTuple. */
  std::string m_type;

public: 

  /** Constructor taking the bad binding lable @a type as argument.*/
  DataRepException ( const std::string & type );

  /** The copy constructor.  @attention Although the implementation is
      the same as the default copy constructor, GCC 2.95.3 needs an
      implementation else the exception is not caught.  It is not
      needed by GCC 3.2.1.
  */
  DataRepException ( const DataRepException & );

  /** The virtual destructor.  Appears that the required declaration
      has to be the same as the base class, according to GCC 3.2.1. */
  virtual ~DataRepException () throw ();

  /** The standard method to return a string giving message about the
      cause of the exception. */
  virtual const char * what () const throw ();

};

} // namespace hippodraw

#endif // _DataRepException_H_
