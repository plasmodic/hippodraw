/* -*- c++ -*- */

/** @file

hippodraw::DataPointTuple namespace interface

Copyright (C) 2003-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DataPointTuple.h,v 1.11 2006/09/13 21:16:06 pfkeb Exp $

*/

#ifndef DataPointTuple_H
#define DataPointTuple_H

#include <string>

/** Namespace for HippoDraw. 
 */
namespace hippodraw {

  /** A namespace to set the standard for indexing into 2 dimension
      data point tuple.  This namespace sets the standard to indexing
      into columns of an DataSource representing 2 dimensional data
      points.  Each row of the %DataSource is data point with it attribute
      represented in columns.  The enumeration specified in the
      namespace gives the column indices.

      @note In the case that the tuple is being filled from the
      contents of a histogram, the Y coordinate should be the contents
      of the bin divided by the bin width and the YERR should be half
      the bin width.
  */
  namespace DataPoint2DTuple {
    /** The column indices for 2 dimension data point tuple.  
         */
    enum column { X, ///< X coordinate
		  Y, ///< Y coordinate
		  XERR, ///< error on X or half bin width
		  YERR, ///< error on Y
		  SIZE = 4 ///< number of columns 
    };
    
    extern std::string WIDTH; ///< XERR column label
    extern std::string ERROR; ///< YERR column label
  }

  /** A namespace to set the standard for indexing into 3 dimension
      data point tuple.  This namespace sets the standard to indexing
      into columns of an DataSource representing 3 dimensional data
      points.  Each row of the %DataSource is data point with it attribute
      represented in columns.  The enumeration specified in the
      namespace gives the column indices.
   */
  namespace DataPoint3DTuple {
    /** The column indices for 3 dimension data point tuple.
     */
    enum column { X, ///< X coordinate
		  Y, ///< Y coordinate
		  Z, ///< Z coordinate
		  XERR, ///< error on X or width
		  YERR, ///< error on Y or width
		  ZERR, ///< error on Z
		  SIZE = 6 ///< number of columns 
    };
    extern std::string XWIDTH; ///< X width label    
    extern std::string YWIDTH; ///< Y width label    

  }

} // namespace HippoDraw

#endif // DataPointTuple_H
