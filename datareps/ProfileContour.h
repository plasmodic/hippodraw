/* -*- mode: c++ -*- */

/** @file

ProfileContour class interface

Copyright (C)  2000-2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: ProfileContour.h,v 1.10 2005/10/30 00:10:10 pfkeb Exp $

*/

#ifndef _ProfileContour_H_
#define _ProfileContour_H_

#include "DataRep.h"

namespace hippodraw {

/** A DataRep for two dimensional profile.  This data representations
    uses slices of two columns on the X and Y axis and shows the
    average value of a third column in color.  An example is shown
    below.

    @image html profile2d.png "ProfileContour data representation
    @image latex profile2d.eps "ProfileContour data representation

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Matan Shacham <matan@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API ProfileContour
  : public hippodraw::DataRep
{

public:

  /** The default constructor. */
  ProfileContour ( );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  DataRep * clone();

  virtual bool hasAxis ( hippodraw::Axes::Type ) const;


};

} // namespace hippodraw

#endif // _ProfileContour
