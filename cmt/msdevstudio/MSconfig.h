// Use "#pragma once" instead of "ifndef MSCONFIG_H, define MSCONFIG_H,
// #endif" if you know you are using a Microsoft compiler, which we are
// if this file is being included. Using "pragma once" is much faster
// and less error prone.
#pragma once


/*
  G.Barrand : the below had been used to reconstruct
   the core with CMT and with VisualC++-7.1 (.NET 2003).
*/

#define VERSION "1.5.0"

/* Turn off annoying warning. */
# pragma warning(disable:4244)  // conversion from double to float
# pragma warning(disable:4250)  // inherits via dominance
# pragma warning(disable:4251)  // exporting from dll
# pragma warning(disable:4273)  // incosistent dll linkage
# pragma warning(disable:4275)  // non dll-interface class
# pragma warning(disable:4786)  // '255' characters in the debug information

/** Define the value of pi which appears to be missing from Dev Studio. */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define	M_E 2.7182818284590452354
#endif
