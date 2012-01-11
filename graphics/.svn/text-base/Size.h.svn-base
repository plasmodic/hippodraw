/* -*- mode:c++ -*-
 *
 * HippoPlot Size  class interface
 *
 * Copyright (C) 2000, 2003   The Board of Trustees of The Leland
 * Stanford Junior University.  All Rights Reserved.
 *
 * $Id: Size.h,v 1.12 2005/10/30 00:10:12 pfkeb Exp $
 */

#ifndef _SIZE_H_
#define _SIZE_H_

#include "pattern/libhippo.h"

namespace hippodraw {

/** Class representing a two or three dimension size.  @attention the
    inline functions were measured to have high profile when plotting
    large image.
*/
class MDL_HIPPOPLOT_API Size
{
 private:
  double m_width;
  double m_height;
  double m_depth;


 public:
  Size();
  Size( double width, double height );
  Size( double width, double height, double depth );

  void setSize( double width, double height );
  void setSize( double width, double height, double depth );

  /** The following function returns the height. */
  inline double getHeight() const;

  /** The following function returns the width. */
  inline double getWidth() const;

  /** The following function returns the depth. */
  inline double getDepth() const;

  /** Sets the depth dimenision of the size. */
  void setDepth ( double );
 

};

inline
double
Size::
getWidth() const
{
  return m_width;
}

inline
double
Size::
getHeight() const
{
  return m_height;
}

inline
double
Size::
getDepth() const
{
  return m_depth;
}

} // namespace hippodraw

#endif // _SIZE_H_
