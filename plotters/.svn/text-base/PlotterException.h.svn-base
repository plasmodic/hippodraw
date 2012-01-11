/* -*- mode: c++ -*- */

/** @file

PlotterException class interface

Copyright (C) 2003   The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: PlotterException.h,v 1.3 2005/10/30 00:10:13 pfkeb Exp $

*/

#ifndef _PlotterException_H_
#define _PlotterException_H_

#include "pattern/libhippo.h"

#include <exception>
#include <string>

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

namespace hippodraw {

/** An exception class that is thrown when attempting an illegal
    operation on a PlotterBase object.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API PlotterException : public std::exception
{
private:

  /** The error message. */
  std::string m_message;

public: 

  /** Constructor taking the error message as argument. 
   */
  PlotterException ( const std::string & message );

  /** The copy constructor.  @attention Although the implementation is
      the same as the default copy constructor, GCC 2.95.3 needs an
      implementation else the exception is not caught.  It is not
      needed by GCC 3.2.1.
  */
  PlotterException ( const PlotterException & );

  /** The virtual destructor.  Appears that the required declaration
      has to be the same as the base class, according to GCC 3.2.1. 
  */
  virtual ~PlotterException () throw ();

  /** The standard method to return a string giving message about the
      cause of the exception. 
  */
  virtual const char * what () const throw ();

};

} // namespace hippodraw

#endif // _PlotterException_H_
