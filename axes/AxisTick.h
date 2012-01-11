/* -*- mode:c++ -*- */

/** @file

AxisTick  class interface

Copyright (C) 2000, 2003   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: AxisTick.h,v 1.8 2006/10/10 22:39:47 pfkeb Exp $
*/

#ifndef _AxisTick_H_
#define _AxisTick_H_

#include <string>

#include "pattern/libhippo.h"


namespace hippodraw {

/** A class to maintain tick coordinates and string values.  

    @author Paul_Kunz@slac.stanford.edu 
*/


class MDL_HIPPOPLOT_API AxisTick
{

 private:

  /** The value at the coordinate. */
  double m_v;

  /** The content at the coordinate. */
  std::string m_c;

public:

  /** The default constructor. It is needed for initialization when it is
      included in the STL container. */
  AxisTick ( );

  /** The copy constructor. */
  AxisTick ( const AxisTick & );

  /** The normal constructor. */
  AxisTick( double v, const std::string & s );

  /** Returns the value of the coordinate. */
  double value ( ) const;

  /** Sets the value of the coordinate. */
  void setValue ( double );

  /** Returns the content at the coordinate. */
  const std::string & content ( ) const;

  /** Sets the content at the coordinate. */
  void setContent ( const std::string & );
};

} // namespace hippodraw

#endif // _AxisTick_H_
