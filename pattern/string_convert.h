/* -*- mode:c++ -*- */

/** @file

The namespace for conversion to string

Copyright (C) 2005 The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: string_convert.h,v 1.4 2007/03/12 21:33:45 pfkeb Exp $ 

*/

#ifndef _String_H_
#define _String_H_

#include "libhippo.h"

#include <string>

namespace hippodraw {

  /** The namespace for conversion to string and search of string.
      @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
      @author Xie Fang <xiefang@stanford.edu>
  */
  namespace String {

    /** Converts an integer to a string.
     */
    std::string MDL_HIPPOPLOT_API convert ( int i );

    /** Converts an unsigned integer to a string.
     */
    std::string MDL_HIPPOPLOT_API convert ( unsigned int i );

    /** Converts a double to a string
     */
    std::string MDL_HIPPOPLOT_API convert ( double value );

    /** Case insensitive equal
     */
    bool MDL_HIPPOPLOT_API ci_equal(char ch1, char ch2);

    /** Case insensitive find
     */
    size_t MDL_HIPPOPLOT_API ci_find(const std::string& str1, const std::string& str2);



  } // end namespace String
} // end namespace hippodraw

#endif // _String_H_
