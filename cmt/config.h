/* config.h.  Generated automatically by configure.  */
/* config.h.in.  Generated automatically from configure.in by autoheader.  */
/* 
 * Macross defining non-supported C++ language features and compiler
 * specific pragmas.  
 *
 * $Id: config.h,v 1.4 2004/10/12 17:24:38 pfkeb Exp $
 *
 * Author: Paul_Kunz@slac.stanford.edu
 * */

#ifndef CONFIG_H
#define CONFIG_H

#ifdef _MSC_VER
// include MicroSoft Visual C++ specific stuff.
#include "MSconfig.h"
#endif //_MSC_VER


/* Define if the X Window System is missing or not being used.  */
/* #undef X_DISPLAY_MISSING */

/* Define if you have the <sstream> header file.  */
/* #undef HAVE_SSTREAM */

/* Define if you have the <strstream> header file.  */
#define HAVE_STRSTREAM 1

/* Name of package */
#define PACKAGE "hippodraw"

/* Version number of package */
#define VERSION "1.5.0"

/* Define if bind2nd() does not work with mem_func1 and member functions
returning void. */
/* #undef BIND2ND_DEFECT */

/* Define if vector<T>::iterator does not work declaring using namespace std  */
/* #undef ITERATOR_MEMBER_DEFECT */

/* define if std::distance does not work */
/* #undef DISTANCE_DEFECT */

/* define if obsolete mem_fun1 must be used. */
/* #define MEMFUN1_DEFECT 1 */


#endif /* CONFIG_H */

