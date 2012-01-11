/** @file

The namespace for axes.

Copyright (C) 2000, 2003, 2004 The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: AxesType.h,v 1.21 2006/07/13 22:55:08 pfkeb Exp $ 

*/

#ifndef _AxesType_H_
#define _AxesType_H_

#include "pattern/libhippo.h"

#include <string>

namespace hippodraw {

  /** The namespace for axes constants. 
      @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
  */
  namespace Axes {

    /** Axes constants.  The axes constants are used to access and set
	attributes of PlotterBase objects.

	@author Stephane Bonneaud <gandalf@slac.stanford.edu> 
    */
    enum Type
      {
	X = 0,  ///< X axis.
	Y = 1,  ///< Y axis.
	Z = 2,  ///< Z axis.
	W = 3,  ///< weight
	XE = 4, ///< X error or width
	YE = 5, ///< Y error or width
	T = 10, ///< title 
	UNKNOWN = 1000
      };

    /** Converts from string representation ot Axes::Type
	representation.  
     */
    Type  MDL_HIPPOPLOT_API convert ( const std::string & axis );

    /** Converts from integer represenation to Axes::Tyoe
	representation.
    */
    Type  MDL_HIPPOPLOT_API convert ( unsigned int i );

  } // end namespace Axes
} // end namespace hippodraw

#endif // _AxesType_H_
