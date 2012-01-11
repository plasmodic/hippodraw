/* -*- mode: c++ -*- */

/** @file

SymbolType enumeration

Copyright (C) 2003, 2004   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: SymbolType.h,v 1.7 2005/10/12 21:28:19 pfkeb Exp $

*/

#ifndef _SymbolType_H_
#define _SymbolType_H_

#include "pattern/libhippo.h"

namespace hippodraw {

  /** A namespace for plotting symbols. 
   */
  namespace Symbol {
    /** Enumeration of the type of symbols supported by this class. 

        @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    */
    enum Type { 
      SQUARE,
      SOLIDSQUARE, /**< @image html symbolpointrepsolidsquare.png 
                        @image latex symbolpointrepsolidsquare.eps */
      PLUS, 
      TIMES, 
      TRIANGLE, 
      FILLED_TRIANGLE, 
      CIRCLE,
      FILLED_CIRCLE,
      INVISIBLE
    };

    Type MDL_HIPPOPLOT_API convert ( unsigned int type );

  }
}

#endif // _SymbolType_H_
