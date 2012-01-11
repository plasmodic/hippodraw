/* -*- mode: c++ -*- */

/** @file

hippodraw::Image class interface

Copyright (C) 2003-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: Image.h,v 1.39 2006/10/10 19:22:49 pfkeb Exp $

*/

#ifndef _Image_H_
#define _Image_H_

#include "datareps/DataRep.h"

namespace hippodraw {

/** Displays on the Z axis the values of a single column treated as a
    matrix of values. The matrix is stored column wise i.e.
    value[i][j] should be set in z[ i * ny + j ].

    By default the X and Y coordinates are generated from the matrix's
    column and row indexes, respectively. This is not to mean that one
    is tied to using these indexes along the axis. There are functions
    using which data can be distributed along any uniform grid (aka matrix). 
    Please take a look at functions setBinWidth, setOffsets, setRange.
    
    An example of this plot is shown below.

    @image html zplot.png "Image data representation"
    @image latex zplot.eps "Image data representation"

    @bug @@@@ Z axis is not autoscaling like problem with mesh used to be.

    @bug @@@@ Labels of Z values, and perhaps others if range is
    under two orders of magnitude and values are very small.  Try
    sip/z_label.py to see it.  Might convert this script to one that
    can be put in testsuite

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API Image : public DataRep
{
 public:

  /** The default constructor. */
  Image ( );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual DataRep * clone();

  bool acceptFunction ( int num );

  virtual bool hasAxis ( hippodraw::Axes::Type ) const;

  /** Returns @c true as the Image needs to have the XY matrix set in
      order to display itself.
  */
  virtual bool needsMatrixSet () const;

};

} // end namespace hippodraw
#endif // _Image
