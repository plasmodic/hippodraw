/* -*- mode: c++ -*- */

/** @file

hippodraw::HammerAito class interface

Copyright (C) 2002-2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

The library is distributed under the terms of the GNU Lesser General
Public License.

$Id: HammerAito.h,v 1.54 2007/03/23 22:41:33 xiefang Exp $

*/

#ifndef _HammerAito_H_
#define _HammerAito_H_

#include "WCSTransform.h"


namespace hippodraw {

/** A transform that performs the HammerAito projection.  A transform
    that performs the Hammer-Aitoff map projection. This map
    projection is an equal-area projection which displays the world on
    an ellipse. "Equal-area" means equal-areas on the globe are equal
    areas on the two dimension projection. If this transforms were
    used with an XY Plot containing the boundaries of the continents,
    it might look like image below.

    @bug @@@@ The right hand side of the axis box is not drawn. ( To
    remove: probably not a bug, since -180 and +180 is the same edge
    and should not be drawn twice. )

    @image html hammer.gif A Hammer-Aitoff projection.
    @image latex hammer.eps A Hammer-Aitoff projection
*/
class MDL_HIPPOPLOT_API HammerAito : public WCSTransform
{

private:

  
public:

  /** The constructor. The constructor taking a UnaryTransform for its
      Z axis.  */
  HammerAito ( UnaryTransform * z );

  /** The copy constructor. */
  HammerAito ( const HammerAito & );

  /** The virtual destructor. */
  virtual ~HammerAito();

  /** Creates a new Transform object by copying an existing one. */
#ifdef CLONE_DEFECT
  virtual TransformBase * clone () const;
#else
  virtual HammerAito    * clone () const;
#endif


  virtual double aspectRatio () const;


};

} // namespace hippodraw

#endif // _HammerAito_H_
