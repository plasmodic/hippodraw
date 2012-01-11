/* -*- mode:c++ -*- */

/** @file

String conversion functions

Copyright (C) 2005 The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: string_convert.cxx,v 1.3 2007/03/12 21:33:45 pfkeb Exp $ 

*/

#include "string_convert.h"

#ifdef SSTREAM_DEFECT
#include <strstream>
using std::ostrstream;
#else
#include <sstream>
using std::ostringstream;
#endif

#include <algorithm>
#include <cassert>

using std::string;

namespace hippodraw {

  namespace String {

    string  convert ( int i )
    {
#ifdef SSTREAM_DEFECT
      ostrstream strm_text;
#else
      std::ostringstream strm_text;
#endif
      strm_text << i;
#ifdef SSTREAM_DEFECT
      strm_text << std::ends; // not needed for std class.
#endif

      return string ( strm_text.str() );
    }

    string  convert ( unsigned int i )
    {
#ifdef SSTREAM_DEFECT
      ostrstream strm_text;
#else
      std::ostringstream strm_text;
#endif
      strm_text << i;
#ifdef SSTREAM_DEFECT
      strm_text << std::ends; // not needed for std class.
#endif

      return string ( strm_text.str() );
    }

    string convert ( double v )
    {
#ifdef SSTREAM_DEFECT
      ostrstream strm_text;
#else
      std::ostringstream strm_text;
#endif
      strm_text << v;
#ifdef SSTREAM_DEFECT
      strm_text << std::ends; // not needed for std class.
#endif

      return string ( strm_text.str() );
    }

    bool ci_equal(char ch1, char ch2)
    {
      return toupper((unsigned char)ch1) == toupper((unsigned char)ch2);
    }
    
    size_t ci_find(const string& str1, const string& str2)
    {
      string::const_iterator pos = 
          std::search(str1. begin ( ), str1. end ( ), str2.
	       begin ( ), str2. end ( ), ci_equal);
      if (pos == str1. end ( ))
	return string::npos;
      else
	return pos - str1. begin ( );
    }

  } // end namespace String
} // end namespace hippodraw
