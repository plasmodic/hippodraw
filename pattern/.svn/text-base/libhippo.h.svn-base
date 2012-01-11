#ifndef _libhippo_H_
#define _libhippo_H_

// Matthew D. Langston <langston@SLAC.Stanford.EDU>
//
// Every header file in our library that wants to export symbols
// (e.g. class definitions) from a DLL on Windows must include this
// header. It works on all platforms, both Windows and Linux/Unix.
//
// The following ifdef block is the standard way of exporting symbols
// (e.g. class definitions) from a DLL on Windows. The macro
// MDL_HIPPOPLOT_EXPORTS is defined when source code is compiled to
// create the DLL, which causes MDL_HIPPOPLOT_API to expand to
// __declspec(dllexport) so that symbols decorated with
// MDL_HIPPOPLOT_API are exported from the DLL.
//
// Clients of our DLL don't have to do anything special to use it, since
// they won't define MDL_HIPPOPLOT_EXPORTS, which means
// MDL_HIPPOPLOT_API will expand to __declspec(dllimport) so that
// symbols decorated with MDL_HIPPOPLOT_API are imported from the DLL.

#if defined(_WIN32) && defined(_MSC_VER) && defined( MDL_HIPPOPLOT_EXPORTS )
  #undef  MDL_HIPPOPLOT_API
  #define MDL_HIPPOPLOT_API __declspec(dllexport)
#elif defined(_WIN32) && defined(_MSC_VER) && defined( MDL_HIPPOPLOT_IMPORTS )
  #undef  MDL_HIPPOPLOT_API
  #define MDL_HIPPOPLOT_API __declspec(dllimport)
#else
  // The gcc compiler (i.e. the Linux/Unix compiler) exports the Universe
  // of symbols from a shared library, meaning that we can't control the
  // API of our shared libraries. We therefore just define the Symbol
  // Export Macro to expand to nothing.
  // Or if on Windows but using a static library (neither IMPORTS nor EXPORTS)
  // then we don't need this goofy declspec stuff.
  #undef  MDL_HIPPOPLOT_API
  #define MDL_HIPPOPLOT_API
#endif

#endif // _libhippo_H_
